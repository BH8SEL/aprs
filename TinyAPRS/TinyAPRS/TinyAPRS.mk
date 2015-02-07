#
# Wizard autogenerated makefile.
# DO NOT EDIT, use the TinyAPRS_user.mk file instead.
#

# Constants automatically defined by the selected modules
TinyAPRS_DEBUG = 0

# Our target application
TRG += TinyAPRS

TinyAPRS_PREFIX = "/usr/bin/avr-"

TinyAPRS_SUFFIX = ""

TinyAPRS_SRC_PATH = TinyAPRS

TinyAPRS_HW_PATH = TinyAPRS

# Files automatically generated by the wizard. DO NOT EDIT, USE TinyAPRS_USER_CSRC INSTEAD!
TinyAPRS_WIZARD_CSRC = \
	bertos/algo/crc_ccitt.c \
	bertos/cpu/avr/drv/ser_avr.c \
	bertos/cpu/avr/drv/ser_mega.c \
	bertos/cpu/avr/drv/timer_avr.c \
	bertos/cpu/avr/drv/timer_mega.c \
	bertos/drv/ser.c \
	bertos/drv/timer.c \
	bertos/io/kfile.c \
	bertos/mware/event.c \
	bertos/mware/formatwr.c \
	bertos/mware/hex.c \
	bertos/net/afsk.c \
	bertos/net/ax25.c \
	bertos/net/kiss.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE TinyAPRS_USER_PCSRC INSTEAD!
TinyAPRS_WIZARD_PCSRC = \
	bertos/mware/formatwr.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE TinyAPRS_USER_CPPASRC INSTEAD!
TinyAPRS_WIZARD_CPPASRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE TinyAPRS_USER_CXXSRC INSTEAD!
TinyAPRS_WIZARD_CXXSRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE TinyAPRS_USER_ASRC INSTEAD!
TinyAPRS_WIZARD_ASRC = \
	 \
	#

TinyAPRS_CPPFLAGS = -D'CPU_FREQ=(16000000UL)' -D'ARCH=(ARCH_DEFAULT)' -D'WIZ_AUTOGEN' -I$(TinyAPRS_HW_PATH) -I$(TinyAPRS_SRC_PATH) $(TinyAPRS_CPU_CPPFLAGS) $(TinyAPRS_USER_CPPFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
TinyAPRS_LDFLAGS = $(TinyAPRS_CPU_LDFLAGS) $(TinyAPRS_WIZARD_LDFLAGS) $(TinyAPRS_USER_LDFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
TinyAPRS_CPPAFLAGS = $(TinyAPRS_CPU_CPPAFLAGS) $(TinyAPRS_WIZARD_CPPAFLAGS) $(TinyAPRS_USER_CPPAFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
TinyAPRS_CSRC = $(TinyAPRS_CPU_CSRC) $(TinyAPRS_WIZARD_CSRC) $(TinyAPRS_USER_CSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
TinyAPRS_PCSRC = $(TinyAPRS_CPU_PCSRC) $(TinyAPRS_WIZARD_PCSRC) $(TinyAPRS_USER_PCSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
TinyAPRS_CPPASRC = $(TinyAPRS_CPU_CPPASRC) $(TinyAPRS_WIZARD_CPPASRC) $(TinyAPRS_USER_CPPASRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
TinyAPRS_CXXSRC = $(TinyAPRS_CPU_CXXSRC) $(TinyAPRS_WIZARD_CXXSRC) $(TinyAPRS_USER_CXXSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
TinyAPRS_ASRC = $(TinyAPRS_CPU_ASRC) $(TinyAPRS_WIZARD_ASRC) $(TinyAPRS_USER_ASRC)

# CPU specific flags and options, defined in the CPU definition files.
# Automatically generated by the wizard. PLEASE DO NOT EDIT!
TinyAPRS_MCU = atmega328p
TinyAPRS_PROGRAMMER_CPU = atmega328p
#TinyAPRS_MCU = atmega644pa
#TinyAPRS_PROGRAMMER_CPU = atmega644pa
TinyAPRS_CPU_CPPFLAGS = -Os -Ibertos/cpu/avr/
TinyAPRS_STOPFLASH_SCRIPT = bertos/prg_scripts/avr/stopflash.sh
TinyAPRS_STOPDEBUG_SCRIPT = bertos/prg_scripts/none.sh
TinyAPRS_DEBUG_SCRIPT = bertos/prg_scripts/nodebug.sh
TinyAPRS_FLASH_SCRIPT = bertos/prg_scripts/avr/flash.sh

include $(TinyAPRS_SRC_PATH)/TinyAPRS_user.mk
