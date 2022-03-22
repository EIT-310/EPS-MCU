# General Setup
cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project("EPS_MCU" C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)


# Hardware info for compiler
set(MCPU cortex-m4)

#Uncomment for hardware floating point
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)


# Add header dirs
include_directories(
        ${CMAKE_SOURCE_DIR}/include
)

# Add Src Dir
file(
        GLOB_RECURSE SOURCES
        ${CMAKE_SOURCE_DIR}/src/*.*
)

add_executable(${PROJECT_NAME} ${SOURCES})


# OpenCM3 Configuration
add_custom_target(libopencm3 make TARGETS=stm32/l4 WORKING_DIRECTORY ${libopencm3_SOURCE_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include)
set_property(TARGET ${PROJECT_NAME} PROPERTY IMPORTED_LOCATION ${libopencm3_SOURCE_DIR}/lib/libopencm3_stm32l4.a)
add_dependencies(${PROJECT_NAME} libopencm3)
target_link_directories(${PROJECT_NAME} INTERFACE ${libopencm3_SOURCE_DIR}/lib)

target_compile_definitions(${PROJECT_NAME} INTERFACE -DSTM32L4)


#FreeRTOS Configuration
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_SOURCE_DIR}/include CACHE STRING "")
set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")

# Add FreeRTOS CMAKE file
add_subdirectory(${CMAKE_SOURCE_DIR}/external/FreeRTOS-Kernel)
target_link_libraries(${PROJECT_NAME} SHARED freertos_kernel)
target_include_directories(
        ${PROJECT_NAME} PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/external/FreeRTOS-Kernel/include"
)

