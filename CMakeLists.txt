cmake_minimum_required(VERSION 3.13)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(rp2350_hacking_challenge_debug_version)

pico_sdk_init()

# First executable
add_executable(rp2350_hacking_challenge_debug_version)

target_sources(rp2350_hacking_challenge_debug_version PRIVATE
        main.c)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../lib")

target_link_libraries(rp2350_hacking_challenge_debug_version PRIVATE
        pico_stdlib
        hardware_pio
        hardware_i2c
        hardware_powman
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(rp2350_hacking_challenge_debug_version 1)
pico_enable_stdio_uart(rp2350_hacking_challenge_debug_version 0)

# Signing and hashing
pico_set_binary_type(rp2350_hacking_challenge_debug_version no_flash)
pico_sign_binary(rp2350_hacking_challenge_debug_version ${CMAKE_CURRENT_SOURCE_DIR}/ec_private_key.pem)
pico_hash_binary(rp2350_hacking_challenge_debug_version)
pico_package_uf2_output(rp2350_hacking_challenge_debug_version 0x10000000)
pico_set_otp_key_output_file(rp2350_hacking_challenge_debug_version ${CMAKE_CURRENT_LIST_DIR}/otp.json)
pico_add_extra_outputs(rp2350_hacking_challenge_debug_version)



# Second executable with GLITCH_DETECTOR_ON defined
add_executable(rp2350_hacking_challenge_secure_version)

target_sources(rp2350_hacking_challenge_secure_version PRIVATE
        main.c)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../lib")

target_compile_definitions(rp2350_hacking_challenge_secure_version PRIVATE SECURE_VERSION)

target_link_libraries(rp2350_hacking_challenge_secure_version PRIVATE
        pico_stdlib
        hardware_pio
        hardware_i2c
        )

pico_add_extra_outputs(rp2350_hacking_challenge_secure_version)

# disable usb output, disable uart output
pico_enable_stdio_usb(rp2350_hacking_challenge_secure_version 0)
pico_enable_stdio_uart(rp2350_hacking_challenge_secure_version 0)

# Signing and hashing
pico_set_binary_type(rp2350_hacking_challenge_secure_version no_flash)
pico_sign_binary(rp2350_hacking_challenge_secure_version ${CMAKE_CURRENT_SOURCE_DIR}/ec_private_key.pem)
pico_hash_binary(rp2350_hacking_challenge_secure_version)
pico_package_uf2_output(rp2350_hacking_challenge_secure_version 0x10000000)
pico_set_otp_key_output_file(rp2350_hacking_challenge_secure_version ${CMAKE_CURRENT_LIST_DIR}/otp.json)
pico_add_extra_outputs(rp2350_hacking_challenge_secure_version)

