OPTFLAGS += -O2

# Sadly we can't use LTO as long as we're mixing compilers
#OPTFLAGS += -flto 
OPTFLAGS += -ffunction-sections
OPTFLAGS += -fdata-sections
OPTFLAGS += -fomit-frame-pointer
OPTFLAGS += -finline-small-functions