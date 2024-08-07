# RP2350 Hacking Challenge

Welcome to the Raspberry Pi RP2350 hacking challenge!

The goal is easy: Find an attack that lets you dump a secret hidden in OTP ROW 0xc08 - the secret is 128-bit long, and protected by OTP_DATA_PAGE48_LOCK1 and RP2350's secure boot!

If you think you have found a break then come to our DEF CON booth at the Embedded Systems Village - we will give you a Pico2 with a custom secret hidden in it. If you manage to extract it, you win $10,000!

If you can't come to our DEF CON booth, or are participating in the challenge after the DEF CON conference has concluded, please email us with details of your break at [doh@raspberrypi.com](mailto:doh@raspberrypi.com) and we'll be in touch.

This challenge will run from 4PM UK time on 8th August 2024 to 4PM UK time on 7th September 2024.

Good luck!

## Disclaimer

For this challenge we will do the following persistent & irreversible changes to your RP2350:
- Writing bootkey0 (with a public key - or you can generate your own & build your own firmware)
- Enabling secure-boot via `crit1.secure_boot_enable` (but with public keys)
- Disable debug via `crit1.debug_disable`
- Overwrite & lock data in OTP ROW 0xc08
- *Enabling security will permanently disable both Hazard3 RISC-V cores (M33 cores will still be operable)*

## Setup

- Connect an RP2350 in BOOTSEL mode to your computer via USB
- The repository already contains signing keys: `ec_private_key.pem` and `ec_public_key.pem`. If you want to generate your own keys you can run `keygen.sh` to generate new ones using openSSL.
- Next we write the secret that we want to hide using: `./write_otp_secret.sh` - this is irreversible, as we can't "erase" OTP.
- You can check whether this write was successful by running `./read_otp_secret.sh`
- Next we build our project:
    - `mkdir build`
    - `cd build`
    - `cmake -DPICO_PLATFORM=rp2350 -DPICO_BOARD=pico2 ..`
    - `make`
    - `cd ..`
- Next we enable secure-boot on the chip by running `enable_secureboot.sh` (This irreversibly enables secure-boot! Make sure you keep a backup of your keys!)
    - To fully lock down the chip including disabling debugging and enabling the glitch detectors, please run `lock_chip.sh`
- And now we are ready to install the firmware:
    - Either copy rp2350_hacking_challenge_debug_version.uf2 or rp2350_hacking_challenge_secure_version.uf2

## What's the difference between the debug and the secure version?

The debug version shows how to read the OTP secret that you need to extract, and also gives
some debug output on what's going on in the chip.

As the printfs etc. might be susceptible to fault-injection attacks we have disabled them in
the secure version.

Our challenge Pico2 will run the secure-version of the firmware.

## Using the chip in the future

By participating in this challenge you are permanently enabling secure-boot on your device.
Any firmware you want to install in the firmware you need to sign yourself. You can enable
signing for other projects by simply adding this to the CMakeLists.txt and copying the
ec_private_key.pem to your source directoy.

```
# Signing and hashing
pico_sign_binary(project_name ${CMAKE_CURRENT_SOURCE_DIR}/ec_private_key.pem)
pico_hash_binary(project_name)
pico_set_otp_key_output_file(project_name ${CMAKE_CURRENT_LIST_DIR}/otp.json)
```
## Rules, Terms and Conditions

Please see [here](terms_conditions.md) for rules as well as the Terms and Conditions of this challenge.

