CC = gcc

INCLUDES = -I/usr/include/glib-2.0 -I/usr/include/json-glib-1.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include

LD_FLAGS = -lglib-2.0 -ljson-glib-1.0 -lgio-2.0 -lgobject-2.0

COMMON_CFLAGS = $(INCLUDES) -O3 -pedantic -pedantic-errors -Wall -Wextra -Werror
SHARED_LIBRARY_CFLAGS = $(COMMON_CFLAGS) -fPIC -shared 
RUN_EXAMPLE_CFLAGS = $(COMMON_CFLAGS)

SHARED_LIBRARY_SOURCE = $(wildcard src/data_tier/*.c) \
	$(wildcard src/logic_tier/*.c) \
	$(wildcard src/presentation_tier/*.c) 

RUN_EXAMPLE_SOURCE = $(wildcard src/data_tier/*.c) \
	$(wildcard src/logic_tier/*.c) \
	$(wildcard src/presentation_tier/*.c) \
	$(wildcard src/examples/*.c) \
	$(wildcard src/*.c)
	      
SHARED_LIBRARY_OBJECTS = $(SHARED_LIBRARY_SOURCE:.c=.o)
RUN_EXAMPLE_OBJECTS = $(RUN_EXAMPLE_SOURCE:.c=.o)

SHARED_LIBRARY_TARGET = libT-Rex.so
RUN_EXAMPLE_TARGET = runExample

ifeq ($(library),true)
	CFLAGS = $(SHARED_LIBRARY_CFLAGS)
	OBJECTS = $(SHARED_LIBRARY_OBJECTS)
	TARGET = $(SHARED_LIBRARY_TARGET)
else
	CFLAGS = $(RUN_EXAMPLE_CFLAGS)
	OBJECTS = $(RUN_EXAMPLE_OBJECTS)
	TARGET = $(RUN_EXAMPLE_TARGET)
endif

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LD_FLAGS)

clean:
	$(RM) $(SHARED_LIBRARY_TARGET) $(RUN_EXAMPLE_TARGET) $(SHARED_LIBRARY_OBJECTS) $(RUN_EXAMPLE_OBJECTS)
