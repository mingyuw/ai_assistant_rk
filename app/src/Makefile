
PROJ_PATH:=$(shell pwd)
APP_PATH:=$(PROJ_PATH)
APP:= similarity_test

TARGET_DEVICE = $(shell gcc -dumpmachine | cut -f1 -d -)

RNKPU_DIR?=/home/linaro/projects/RKNPUTools/rknn-api/Linux/rknn_api_sdk/
LIB_INSTALL_DIR?=/usr/lib/aarch64-linux-gnu/

CFLAGS+=  -O2 -g 

INCS:= $(wildcard *.h)

C_SRCS:= $(wildcard *.c)
CPP_SRCS:= $(wildcard *.cpp)


C_OBJS:= $(C_SRCS:.c=.o)
CPP_OBJS:= $(CPP_SRCS:.cpp=.o)

CFLAGS+= -I../inc \
		-I../export \
		-I/usr/include/

LIBS:= -lz -lpthread -lrt -lstdc++ -lnuAlgo -L.
LIBS+= -L$(LIB_INSTALL_DIR) -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio

GST_PKGS:= gstreamer-1.0
CFLAGS+= `pkg-config --cflags $(GST_PKGS)`
LIBS+= `pkg-config --libs $(GST_PKGS)`

# CV_PKGS:= opencv4
# CFLAGS+= `pkg-config --cflags $(CV_PKGS)`
# LIBS+= `pkg-config --libs $(CV_PKGS)`

all: $(APP)

%.o: %.c
	$(CXX) -c -o $@ $(CFLAGS) $<

%.o: %.cpp
	$(CXX) -c -o $@ $(CFLAGS) $<

$(APP): $(C_OBJS) $(CPP_OBJS)
	$(CXX) -o $(APP) $(C_OBJS) $(CPP_OBJS) $(LIBS)
	

install: $(APP)
	cp -rv $(APP) $(APP_PATH)

clean:
	rm -rf $(C_OBJS) $(CPP_OBJS) $(APP)

