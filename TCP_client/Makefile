CC = gcc
TARGET = CLIENT
LDFLAGS = -lwsock32
RM = rm -f

SOURCE_DIR = src
INCLUDE_DIR = include

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard $(SOURCE_DIR)/*.c))
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

$(TARGET): $(OBJECTS)
	$(CC) -g -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) -g -c -o $@ $< -I$(INCLUDE_DIR)

clean:
	$(RM) $(SOURCE_DIR)/*.o