# CMakeLists.txt --
#   The main CMake file for building calligraphy

cmake_minimum_required(VERSION 3.20)

project(confy CXX)

find_program(APDF asciidoctor-pdf)
if (APDF)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf"
                       DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/docs/doc.adoc"
                       "${CMAKE_CURRENT_SOURCE_DIR}/docs/plex.yml"
                       "${CMAKE_CURRENT_SOURCE_DIR}/docs/confy.puml"
                       COMMAND "${APDF}" -b pdf -r asciidoctor-diagram -o "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf" "${CMAKE_CURRENT_SOURCE_DIR}/docs/doc.adoc")
    add_custom_target(docs ALL
                      DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/confy.pdf")
else ()
    message(WARNING "Couldn't find asciidoctor-pdf... no documentation will be built")
    add_custom_target(docs ALL COMMAND "[No docs being built]")
endif ()
