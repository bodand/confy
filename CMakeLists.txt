# CMakeLists.txt --
#   The main CMake file for building calligraphy

cmake_minimum_required(VERSION 3.20)

project(confy CXX)

## confy EXECUTABLE ##

add_executable(confy src/type_id.hpp src/type_id.cpp src/visitor.hpp src/visitor.cpp src/bad_key.cpp src/bad_key.hpp src/bad_syntax.cpp src/bad_syntax.hpp test/capture_stdio.hpp src/cachable.hpp src/cache_visitor_for.cpp src/cache_visitor_for.hpp src/caches.cpp src/caches.hpp src/cache_factory.cpp src/cache_factory.hpp test/test.bad_key.cpp test/gtest_lite.h src/memtrace.h src/memtrace.cpp
               test/test.bad_syntax.cpp test/test_main.cpp test/test.visitor.cpp test/test.type_id.cpp test/test.cache.cpp test/call_tuple.hpp test/test.uncached.cachefactory.cpp
               test/test.cached.cachefactory.cpp
               src/parser.hpp src/confy_parser.cpp src/confy_parser.hpp test/test.confy_parser.cpp src/config.cpp src/config.hpp src/config_set.cpp src/config_set.hpp src/user_modes.cpp src/user_modes.hpp src/main.cpp test/test.user_modes.cpp test/test.config_set.cpp)
target_compile_definitions(confy PRIVATE -DCPORTA)
target_compile_features(confy PRIVATE cxx_std_17)
target_include_directories(confy PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_options(confy PRIVATE
                       $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
                       $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->)
target_link_libraries(confy PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:stdc++fs>)

set(confy_inputs
    test/inputs/bare_words.confy
    test/inputs/broken1.confy
    test/inputs/broken2.confy
    test/inputs/broken3.confy
    test/inputs/broken4.confy
    test/inputs/broken5.confy
    test/inputs/broken6.confy
    test/inputs/double-strings.confy
    test/inputs/empty1.confy
    test/inputs/empty2.confy
    test/inputs/empty3.confy
    test/inputs/ints.confy
    test/inputs/key_clash.confy
    test/inputs/mixed.confy
    test/inputs/single-strings.confy
    test/inputs/xcolors.confy
    )
add_custom_command(TARGET confy POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E copy ${confy_inputs} "${CMAKE_CURRENT_BINARY_DIR}"
                   WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

## DOCUMENTATION ##

find_package(Perl 5.20)
find_package(Doxygen)
find_program(APDF asciidoctor-pdf)
if (APDF AND PERL_FOUND AND DOXYGEN_FOUND)
    if (NOT DEFINED HAVE_MATHEMATICAL)
        message(CHECK_START "Searching for Ruby's mathematical")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/ruby-test.rb" [[require 'asciidoctor-mathematical']])
        execute_process(
                COMMAND ruby "${CMAKE_CURRENT_BINARY_DIR}/ruby-test.rb"
                OUTPUT_QUIET
                ERROR_QUIET
                RESULT_VARIABLE HAVE_MATHEMATICAL)
        if (NOT HAVE_MATHEMATICAL)
            message(CHECK_PASS "found")
        else ()
            message(CHECK_FAIL "not found -- will not have fancy math symbols in PDF")
        endif ()
        set(HAVE_MATHEMATICAL "${HAVE_MATHEMATICAL}" CACHE INTERNAL "Have mathematical?" FORCE)
    endif ()

    if (NOT HAVE_MATHEMATICAL)
        set(_use_math -r asciidoctor-mathematical)
    endif ()

    configure_file(docs/Doxyfile.in Doxyfile @ONLY)

    get_target_property(confy_SOURCES confy SOURCES)
    list(FILTER confy_SOURCES EXCLUDE REGEX [[gtest_lite]])

    add_custom_command(OUTPUT
                       "${CMAKE_CURRENT_BINARY_DIR}/perlmod/DoxyDocs.pm"
                       BYPRODUCTS
                       "${CMAKE_CURRENT_BINARY_DIR}/perlmod/DoxyStructure.pm"
                       "${CMAKE_CURRENT_BINARY_DIR}/perlmod/doxyrules.make"
                       "${CMAKE_CURRENT_BINARY_DIR}/perlmod/Makefile"
                       DEPENDS
                       "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in"
                       ${confy_SOURCES}
                       VERBATIM COMMAND
                       Doxygen::doxygen "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
                       WORKING_DIRECTORY
                       "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_command(OUTPUT
                       "${CMAKE_CURRENT_BINARY_DIR}/doxy.adoc"
                       DEPENDS
                       "${CMAKE_CURRENT_SOURCE_DIR}/utils/doxml-process.pl"
                       "${CMAKE_CURRENT_BINARY_DIR}/perlmod/DoxyDocs.pm"
                       VERBATIM COMMAND
                       "${PERL_EXECUTABLE}" -I "${CMAKE_CURRENT_BINARY_DIR}/perlmod" "${CMAKE_CURRENT_SOURCE_DIR}/utils/doxml-process.pl"
                       WORKING_DIRECTORY
                       "${CMAKE_CURRENT_BINARY_DIR}")

    add_custom_command(OUTPUT
                       "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf"
                       DEPENDS
                       "${CMAKE_CURRENT_BINARY_DIR}/doxy.adoc"
                       "${CMAKE_CURRENT_SOURCE_DIR}/docs/doc.adoc"
                       "${CMAKE_CURRENT_SOURCE_DIR}/docs/plex.yml"
                       "${CMAKE_CURRENT_SOURCE_DIR}/docs/confy.puml"
                       VERBATIM COMMAND
                       "${APDF}" -b pdf -r asciidoctor-diagram ${_use_math} -o "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf" -a "doxydoc=${CMAKE_CURRENT_BINARY_DIR}/doxy.adoc" "${CMAKE_CURRENT_SOURCE_DIR}/docs/doc.adoc")
    add_custom_target(docs ALL
                      DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf")
    set(DOC_FILE "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf")
else ()
    message(WARNING "Couldn't find asciidoctor-pdf... no documentation will be built")
    add_custom_target(docs ALL COMMAND "[No docs being built]")
    set(DOC_FILE)
endif ()

## BEAD PACKER ##
set(STAGING "${CMAKE_CURRENT_BINARY_DIR}/staging")
get_target_property(CONFY_ALL_SRCS confy SOURCES)

if (NOT IS_DIRECTORY "${STAGING}")
    file(MAKE_DIRECTORY "${STAGING}")
endif ()

if (DOC_FILE)
    cmake_path(GET DOC_FILE FILENAME doc_fname)
    add_custom_command(OUTPUT "${STAGING}/${doc_fname}"
                       COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${DOC_FILE}" "${STAGING}/${doc_fname}"
                       DEPENDS "${DOC_FILE}")
    set(PACK_DOC_FILE "${STAGING}/${doc_fname}")
    set(PACK_DOC_DEP "${STAGING}/${doc_fname}")
endif ()

set(CONFY_OUT_SRCS)
set(CONFY_OUT_DEPS)
foreach (src IN LISTS CONFY_ALL_SRCS)
    cmake_path(GET src FILENAME src_name)
    add_custom_command(OUTPUT "${STAGING}/${src_name}"
                       COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${src}" "${STAGING}/${src_name}"
                       DEPENDS "${src}"
                       WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    list(APPEND CONFY_OUT_SRCS "${src_name}")
    list(APPEND CONFY_OUT_DEPS "${STAGING}/${src_name}")
endforeach ()

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/confy-src.zip"
                   COMMAND "${CMAKE_COMMAND}" -E tar cf "${CMAKE_CURRENT_BINARY_DIR}/confy-src.zip" --format=zip  ${CONFY_OUT_SRCS}
                   DEPENDS ${CONFY_OUT_DEPS} ${PACK_DOC_DEP}
                   WORKING_DIRECTORY "${STAGING}")

add_custom_target(pack ALL
                  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/confy-src.zip")
