SHELL = /bin/sh
MKDIR_P = mkdir -p
RM = -rm -rf
CP = cp
DATE := $(shell date +%s)

BUILD_DIR = ./build
TARGET_NAME = $(MAKECMDGOALS)
TARGET_DIR = $(BUILD_DIR)/$(TARGET_NAME)
EXE = $(TARGET_DIR)/$(TARGET_NAME)
HEX = $(TARGET_DIR)/$(TARGET_NAME).hex
ELF = $(TARGET_DIR)/$(TARGET_NAME).elf
LIB = $(TARGET_DIR)/$(TARGET_NAME).a
LST = $(TARGET_DIR)/$(TARGET_NAME).lst
MAP = $(TARGET_DIR)/$(TARGET_NAME).map

all:
	$(MAKE) release	
	
.PHONY: all clean

clean:
	$(RM) $(BUILD_DIR)/*

# Flags
include mk/debugging.mk
include mk/optimizations.mk
include mk/verbose.mk
include mk/warnings.mk

# Targets
include mk/release.mk

# Generic build rules
include mk/build.mk
