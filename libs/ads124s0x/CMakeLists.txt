file(GLOB ads124s0x_SOURCES src/*.cpp)

add_library(ads124s0x STATIC ${ads124s0x_SOURCES})

target_include_directories(
  ads124s0x
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/ads124s0x)

target_link_libraries(ads124s0x PUBLIC pico_stdlib hardware_spi)
