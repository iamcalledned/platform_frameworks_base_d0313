<<<<<<< HEAD
# 
# This configure file is just for Linux projects against Android
#

VOPRJ := 
=======
#
# This configure file is just for Linux projects against Android
#

VOPRJ :=
>>>>>>> upstream/master
VONJ :=

# WARNING:
# Using v7 breaks generic build
ifeq ($(TARGET_ARCH),arm)
VOTT := v5
else
VOTT := pc
endif

# Do we also need to check on ARCH_ARM_HAVE_ARMV7A? - probably not
ifeq ($(ARCH_ARM_HAVE_NEON),true)
VOTT := v7
endif

VOTEST := 0

<<<<<<< HEAD
VO_CFLAGS:=-DLINUX 
=======
VO_CFLAGS:=-DLINUX
>>>>>>> upstream/master

