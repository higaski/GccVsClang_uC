ifeq ($(MAKECMDGOALS), release)

ifndef CLANG
	include mk/gcc-arm.mk
else
	include mk/clang-arm.mk
endif 

ifndef DEBUG
	CPPFLAGS += -DNDEBUG
endif

INC_DIRS += ./driver
INC_DIRS += ./driver/CMSIS/Core/Include
INC_DIRS += ./driver/newlib
INC_DIRS += ./driver/STM32F4xx_HAL_Driver/Inc
INC_DIRS += ./inc

SRC_DIRS = ./driver 
SRC_DIRS += ./src

LDFLAGS += -T"STM32F407VGTx_FLASH.ld"

endif	

release: $(HEX)
	@echo "Build took $$(($$(date +%s)-$(DATE))) seconds"