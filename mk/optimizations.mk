# GCC produces the smallest binaries with -Os
# Clang seems to to better with -O2
ifeq ($(CXX),$(filter $(CXX),g++ arm-none-eabi-g++))
ifndef DEBUG
	OPTFLAGS += -Os
else
	OPTFLAGS += -Og
endif
else
ifndef DEBUG
	OPTFLAGS += -O2
else	
	OPTFLAGS += -Og	
endif
endif

# Sadly we can't use LTO as long as we're mixing compilers
OPTFLAGS += -flto 
OPTFLAGS += -ffunction-sections
OPTFLAGS += -fdata-sections
OPTFLAGS += -fomit-frame-pointer
OPTFLAGS += -finline-small-functions