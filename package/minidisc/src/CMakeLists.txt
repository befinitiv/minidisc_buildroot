cmake_minimum_required(VERSION 3.1...3.29)

project(
  minidisc
  VERSION 1.0
  LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiodcxx)
pkg_check_modules(MPG123 REQUIRED libmpg123)
pkg_check_modules(OUT123 REQUIRED libout123)

add_executable(${PROJECT_NAME} minidisc.cpp Cd.cpp Mpg123.cpp Buttons.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC ${GPIOD_INCLUDE_DIRS} ${MPG123_INCLUDE_DIRS} ${OUT123_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GPIOD_LIBRARIES} ${MPG123_LIBRARIES} ${OUT123_LIBRARIES} gd) 



install (TARGETS ${PROJECT_NAME} DESTINATION bin)
