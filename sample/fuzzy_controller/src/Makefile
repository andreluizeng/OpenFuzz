DESTDIR		= ../bin
SRCDIR			= ../src
DEL_FILE		= rm -rf
CP_FILE		= cp -rf    

ROOTFS_DIR = $(SDKTARGETSYSROOT)

TARGET_PATH_LIB 	= $(ROOTFS_DIR)/usr/lib
TARGET_PATH_INCLUDE 	= $(ROOTFS_DIR)/usr/include

CFLAGS			= -DLINUX -DUSE_SOC_MX6 -Wall -O2 -fsigned-char -std=c++11 -Wno-attributes -Wno-strict-aliasing -Wno-comment \
			  -DEGL_API_FB -DEGL_API_WL -DGPU_TYPE_VIV -DGL_GLEXT_PROTOTYPES -DENABLE_GPU_RENDER_20 \
			  -I../include -I$(TARGET_PATH_INCLUDE) -I$(COMMON_DIR)/inc -I./glm/glm \
                          -I$(TARGET_PATH_INCLUDE)/glib-2.0 -I$(TARGET_PATH_LIB)/glib-2.0/include \
                          -I$(TARGET_PATH_INCLUDE)/libxml2 \


LFLAGS			= -std=c++14 -Wl,--library-path=$(TARGET_PATH_LIB),-rpath-link=$(TARGET_PATH_LIB) -lm -Wno-attributes \
			   -lCLC -lOpenCL -lpthread \
			   -lwayland-client -lwayland-cursor 

OBJECTS			= fuzzy_controller.o  defuzzy.o fisutils.o implications.o
first: all

all: $(APPNAME)

$(APPNAME): $(OBJECTS) 
	$(CXX) $(LFLAGS) -o $(DESTDIR)/$(APPNAME) $(OBJECTS)  

fuzzy_controller.o: fuzzy_controller.c
	$(CXX) $(CFLAGS) -c -o fuzzy_controller.o fuzzy_controller.c

defuzzy.o: defuzzy.c
	$(CXX) $(CFLAGS) -c -o defuzzy.o defuzzy.c

fisutils.o: fisutils.c
	$(CXX) $(CFLAGS) -c -o fisutils.o fisutils.c

implications.o: implications.c
	$(CXX) $(CFLAGS) -c -o implications.o implications.c




clean:
	$(DEL_FILE) $(SRCDIR)/$(OBJECTS)
	$(DEL_FILE) $(DESTDIR)/$(OBJECTS)
	$(DEL_FILE) $(DESTDIR)/*~ $(DESTDIR)/*.core
	$(DEL_FILE) $(DESTDIR)/$(APPNAME)
	
distclean: clean
	$(DEL_FILE) $(DESTDIR)/$(APPNAME)

install: all
	#${CP_FILE} $(SRCDIR)/$(APPNAME) $(DESTDIR)/$(APPNAME)
	#${CP_FILE} $(DESTDIR)/$(APPNAME) $(ROOTFS_DIR)/home/opencv

