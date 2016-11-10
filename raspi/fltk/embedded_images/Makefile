CC=gcc
CXX=g++
LINKS:=$(shell fltk-config --use-images --ldflags)
OBJS=main.o BadSmiley.o
#RESOURCES=BadSmiley.png blargle.bin
EXENAME=embedded_image_test
FLAGS+=
CXXFLAGS+=-std=c++14 $(shell fltk-config --use-images --cxxflags)

ifeq ($(OS),Windows_NT)
	#CCFLAGS += -D WIN32
	BINARY_ARCH=i386
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		#CCFLAGS += -D AMD64
		OBJCOPY_FORMAT=pe-x86-64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			#CCFLAGS += -D AMD64
			OBJCOPY_FORMAT=pe-x86-64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			#CCFLAGS += -D IA32
			OBJCOPY_FORMAT=pe-i386
		endif
	endif
	EXENAME:=$(EXENAME).exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OBJCOPY_FORMAT=elf
		#CCFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OBJCOPY_FORMAT=elf
		#CCFLAGS += -D OSX
	endif
	UNAME_M := $(shell uname -m)
	ifeq ($(UNAME_M),x86_64)
		OBJCOPY_FORMAT:=$(OBJCOPY_FORMAT)64-x86-64
		BINARY_ARCH=i386
		#CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_M)),)
		OBJCOPY_FORMAT:=$(OBJCOPY_FORMAT)32-i386
		BINARY_ARCH=i386
		#CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_M)),)
		#CCFLAGS += -D ARM
		OBJCOPY_FORMAT:=$(OBJCOPY_FORMAT)32-little
		BINARY_ARCH=arm
	endif
endif

#You may need -lfltk_forms -lfltk_images -lfltk_gl

.PHONY: all clean debug all_impl

all:
	$(eval CXXFLAGS+= -O2 -DNDEBUG)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	+$(MAKE) all_impl
	strip $(EXENAME)

#before make debug, MUST make clean first!
debug:
	$(eval CXXFLAGS+= -g -DDEBUG)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	+$(MAKE) all_impl

all_impl: $(EXENAME)

#WHEN ADDING A NEW CLASS
#ClassName.o: precompiled_headers.h.gch common.h ClassName.cpp ClassName.h
#	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) ClassName.cpp -o ClassName.o
#Note that each .o file must not only depend on its .cpp file, but also on all .h files it uses

$(EXENAME): $(OBJS)
	$(CXX) $(FLAGS) $(CXXFLAGS) -o $(EXENAME) $(OBJS) $(LINKS)

clean:
	rm -f ./*.o ./$(EXENAME) ./*.d ./*.gch

main.o: main.cpp
	$(CXX) -c $(FLAGS) $(CXXFLAGS) main.cpp -o main.o

BadSmiley.o: BadSmiley.png
	objcopy -I binary -O $(OBJCOPY_FORMAT) --binary-architecture $(BINARY_ARCH) BadSmiley.png BadSmiley.o

#$(RESOURCES).o: $(RESOURCES)
#	@echo Converting $(*)
#	objcopy -I binary -O $(OBJCOPY_FORMAT) -B $(BINARY_ARCH) \
#	--redefine-sym _binary_$*_bin_start=$* \
#	--redefine-sym _binary_$*_bin_end=$*_end \
#	--redefine-sym _binary_$*_bin_size=$*_size_sym \
#	$(*) $(@)
#	@echo "extern const char" $(*)"[];" > $(*).h
#	@echo "extern const char" $(*)_end"[];" >> $(*).h
#	echo "extern const char" $(*)_size_sym"[];" >> $(*).h
#	@echo "#define $(*)_size ((int)$(*)_size_sym)" >> $(*).h
