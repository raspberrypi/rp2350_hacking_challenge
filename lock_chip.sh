#!/bin/bash

echo "Are you sure you want to fully lock down the chip?"
echo "Please type PLEASE LOCK to continue"
read -r user_input
if [ "$user_input" != "PLEASE LOCK" ]; then
    echo "Operation canceled."
    exit 1
fi


picotool otp set OTP_DATA_CRIT1.DEBUG_DISABLE 1 
picotool otp set OTP_DATA_CRIT1.GLITCH_DETECTOR_ENABLE 1
# Highest sensitivity
picotool otp set OTP_DATA_CRIT1.GLITCH_DETECTOR_SENS 3

set +e
set +v

echo -e "\n\n"
echo "Your chip is locked! Good luck!"