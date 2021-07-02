cmake_minimum_required(VERSION 3.12)

include("../CMakeShared.txt")

set(PROJ_NAME pr_audio_alure)

project(${PROJ_NAME} CXX)

add_include_dir(glm)
link_external_library(alsoundsystem)

link_generic_module_libraries()
register_pragma_module(${PROJ_NAME})

link_external_library(alure)
link_external_library(openal_soft)

add_dependencies(${PROJ_NAME} alsoundsystem)

set_target_properties(pr_audio_alure PROPERTIES FOLDER modules/audio)

set_property(GLOBAL PROPERTY PRAGMA_MODULE_SKIP_TARGET_PROPERTY_FOLDER 1)
