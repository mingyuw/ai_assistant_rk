
PROJ_PATH:=$(shell pwd)
APP_PATH:=$(PROJ_PATH)/../../app/src/
APP:= libnuAlgo.so

TARGET_DEVICE = $(shell gcc -dumpmachine | cut -f1 -d -)

RNKPU_DIR?=/home/linaro/projects/RKNPUTools/rknn-api/Linux/rknn_api_sdk/
LIB_INSTALL_DIR?=/usr/lib/aarch64-linux-gnu/

ifeq ($(TARGET_DEVICE), aarch64)
  CFLAGS:= -DPLATFORM_TEGRA
  #-std=c99
endif
CFLAGS+= -O2 -fpermissive -Wwrite-strings -include stddef.h -fPIC

INCS:= $(wildcard *.h)

CFLAGS+= -I../inc \
		-I../export \
		-I/usr/include/
		

LIBS:= -lz -lpthread -lrt -lstdc++ 
LIBS+= -L$(LIB_INSTALL_DIR) -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc

C_SRCS:= $(wildcard *.c)
CPP_SRCS:= $(wildcard *.cpp)


C_OBJS:= $(C_SRCS:.c=.o)
CPP_OBJS:= $(CPP_SRCS:.cpp=.o)

all: $(APP)

%.o: %.c
	$(CXX) -c -o $@ $(CFLAGS) $<

%.o: %.cpp
	$(CXX) -c -o $@ $(CFLAGS) $<

$(APP): $(C_OBJS) $(CPP_OBJS)
	$(CXX) -shared -o $(APP) $(C_OBJS) $(CPP_OBJS) $(LIBS)
	

install: $(APP)
	cp -rv $(APP) $(APP_PATH)

clean:
	rm -rf $(C_OBJS) $(CPP_OBJS) $(APP)


