#!/bin/bash

picotool otp load otp.json
picotool otp set OTP_DATA_CRIT1.DEBUG_DISABLE 1 


# picotool otp set OTP_DATA_CRIT1.GLITCH_DETECTOR_ENABLE 1
# TODO sensitivity we want to use