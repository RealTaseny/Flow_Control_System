cmake_minimum_required(VERSION 3.22)

enable_language(C)

aux_source_directory(./src Hardware_Drivers_Srcs)

target_sources(
        ${CMAKE_PROJECT_NAME} PRIVATE ${Hardware_Drivers_Srcs}
)

target_include_directories(
        ${CMAKE_PROJECT_NAME} PRIVATE ./inc
)