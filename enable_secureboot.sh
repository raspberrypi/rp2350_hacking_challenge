#!/bin/bash

echo "Are you sure you want to enable secure-boot?"
echo "Any future firmware will have to be signed by the keys"
echo "in this repo (or the ones you generated)."
echo ""
echo "Please type ENABLE to continue"
read -r user_input
if [ "$user_input" != "ENABLE" ]; then
    echo "Operation canceled."
    exit 1
fi

echo "Loading otp.json onto Pico2!"
picotool otp load otp.json
