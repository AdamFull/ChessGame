cmake_minimum_required(VERSION 3.2)
project(CHESSGAME CXX)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCC)
	option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
	
	if(ENABLE_COVERAGE)
		add_compile_options(--coverage -O0)
	endif()
endif()

if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#Add dirrectory with screen emulator
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/IoT_Screen_Emulator/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/IoT_Screen_Emulator/)

file(GLOB CPP_FILES *.cpp *.c)
add_executable(CHESSGAME ${CPP_FILES})

target_link_libraries(CHESSGAME IOT_SCREEN_EMULATOR)