cmake_minimum_required(VERSION 3.13.1)

# Set Board
set(BOARD nucleo_l432kc)


# Find Zephyr. This also loads Zephyr's build system.
find_package(Zephyr)
project(EPS_MCU)

# Add your source file to the "app" target. This must come after
# find_package(Zephyr) which defines the target.
target_sources(app PRIVATE src/main.c)