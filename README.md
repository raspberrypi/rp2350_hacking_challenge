# RP2350 Hacking Challenge

Welcome to the Raspberry Pi RP2350 hacking challenge!

The goal is easy: Find an attack that lets you dump a secret hidden in OTP ROW 0xc08 - the secret is 128-bit long,
and protected by OTP_DATA_PAGE48_LOCK1 and RP2350's secure boot!

Good luck!

## Disclaimer

For this challenge we will do the following persistent & irreversible changes to your RP2350:
- Writing bootkey0 (with a public key - or you can generate your own & build your own firmware)
- Enabling secure-boot via `crit1.secure_boot_enable` (but with public keys)
- Disable debug via `crit1.debug_disable`
- Overwrite & lock data in OTP ROW 0xc08

## Setup

- Connect an RP2350 in bootloader mode to your computer
- The repository already contains signing keys: `ec_private_key.pem` and `ec_public_key.pem`. If you want to generate your own keys you can run `keygen.sh` to generate new ones using openSSL.
- Next we write the secret that we want to hide using: `./write_otp_secret.sh` - we will not be able to delete the secret!
- Next we build our project:
    - `mkdir build`
    - `cd build`
    - `cmake -DPICO_PLATFORM=rp2350 -DPICO_BOARD=pico2 ..`
    - `make`
    - `cd ..`
- Next we lock down the chip by running `lock_chip.sh` (This irreversibly enables secure-boot! Make sure you keep a backup of your keys!)
- And now we are ready to install the firmware:
    - Either copy rp2350_hacking_challenge_debug_version.uf2 or rp2350_hacking_challenge_secure_version.uf2
