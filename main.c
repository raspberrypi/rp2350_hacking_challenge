#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/watchdog.h"
#include "pico/bootrom.h"

// #include "hardware/regs/glitch_detector.h"
#include "hardware/regs/powman.h"
#include "hardware/regs/otp_data.h"
#include "hardware/structs/powman.h"
#include "hardware/structs/otp.h"

// In the secure version we don't use dprintf, as an FI
// attack on dprintf could potentially be used to leak the OTP secret.
#ifdef SECURE_VERSION
    #define dprintf(fmt, ...) ((void)0)
#else
    #define dprintf printf
#endif

static inline bool is_locked() {
	volatile uint32_t *  otp_page48_lock1_ptr = ((uint32_t *)(OTP_DATA_BASE + (0xfe0*2)));
	if(otp_page48_lock1_ptr[0] != 0x3C3C0000) {
		return false;
	}
	if(otp_page48_lock1_ptr[1] != 0x3C) {
		return false;
	}
	return true;
}

static inline void lock_otp_secret() {
	dprintf("Locking OTP secret...\n");
	otp_cmd_t cmd;
	cmd.flags = OTP_DATA_PAGE48_LOCK1_ROW | OTP_CMD_ECC_BITS | OTP_CMD_WRITE_BITS;
	// 3 redundant copies
	uint32_t value = 0x3c3c3c;
	uint32_t ret = rom_func_otp_access(&value, sizeof(value), cmd);
	if (ret) {
		dprintf("\tLocking failed with error: %d\n", ret);
		while(1) {

		}
	} else {
		dprintf("\tLocking succeeded!ECC Write succeeded!\n");
	}
}

static inline void lock_sw_lock_48() {
	dprintf("Locking OTP secret via SW_LOCK\n");
	// Lock the OTP memory so it's not readable afterwards
	// - [3:2] - Non-Secure lock status
	// - [1:0] - Secure lock status
	// - 0 = read_write 1 = read_only 3 = inaccessible
	otp_hw->sw_lock[48] = 0b1111;
	dprintf("\tDone!\n\n");
}

int main()
{
#ifndef SECURE_VERSION
	stdio_init_all();
	// Wait 5 seconds to give the user a chance to connect to the USB serial console
	sleep_ms(5000);
	dprintf("Welcome to the Raspberry Pi RP2350 Hacking Challenge!\n\n");
	dprintf("The goal is easy: Find an attack that lets you dump a secret\n");
	dprintf("hidden in OTP ROW 0xc08 - the secret is 64-bit long, and\n");
	dprintf("protected by OTP_DATA_PAGE48_LOCK1 and RP2350's secure boot!\n");
	dprintf("\n");
	dprintf("\n");
	dprintf("Good luck!\n\n");
#endif

	// Before we do anything else we check whether our secret OTP pages are locked.
	// If they aren't locked yet we write OTP_DATA_PAGE48_LOCK1 to 0x3c3c3c.
	// This is persistent and prevents reading of the OTP pages via picotool and
	// from non-secure code. Secure code (i.e. if you manage to bypass secure-boot)
	// will still be able to access the secret. Good luck!
	if(!is_locked()) {
		dprintf("OTP area is locked!\n");
	} else {
		dprintf("OTP area is not locked!\n");
		lock_otp_secret();
	}
	puts("");

#ifndef SECURE_VERSION
	// This is how you could leak the secret:
	dprintf("Test access to the OTP before it's locked using SW_LOCK:\n");

	volatile uint32_t *  otp_data_ptr = ((uint32_t *)(OTP_DATA_GUARDED_BASE + (0xc00*2)));
	dprintf("%04X", otp_data_ptr[4] & 0xFFFF);
	dprintf("%04X\n\n", (otp_data_ptr[4] & 0xFFFF0000) >> 16);
#endif

	// Next, we lock the OTP area down even further using SW_LOCK48 - this ensures that
	// the secret can't be retrieved if you exploit the application/gain code-exec after
	// this point!
	lock_sw_lock_48();

#ifndef SECURE_VERSION
	// This is how you could leak the secret:
	dprintf("Test access to the OTP after it's locked using SW_LOCK:\n");
	dprintf("%04X", otp_data_ptr[4] & 0xFFFF);
	dprintf("%04X\n", (otp_data_ptr[4] & 0xFFFF0000) >> 16);
#endif
	while(1) {

	}
}
