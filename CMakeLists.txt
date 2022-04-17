# CMakeLists.txt --
#   The main CMake file for building calligraphy

cmake_minimum_required(VERSION 3.20)

project(confy CXX)

find_program(APDF asciidoctor-pdf)
if (APDF)
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

    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf"
                       DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/docs/doc.adoc"
                       "${CMAKE_CURRENT_SOURCE_DIR}/docs/plex.yml"
                       "${CMAKE_CURRENT_SOURCE_DIR}/docs/confy.puml"
                       COMMAND "${APDF}" -b pdf -r asciidoctor-diagram ${_use_math} -o "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf" "${CMAKE_CURRENT_SOURCE_DIR}/docs/doc.adoc")
    add_custom_target(docs ALL
                      DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf")
else ()
    message(WARNING "Couldn't find asciidoctor-pdf... no documentation will be built")
    add_custom_target(docs ALL COMMAND "[No docs being built]")
endif ()
