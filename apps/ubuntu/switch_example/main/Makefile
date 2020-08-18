CC = gcc
RM = rm -rf
CFLAGS = -std=c99
LIB_NAME = iotcore
INC = -I${CORE_PATH}/src/include
LIB_DIR = -L${CORE_PATH}/output
LIBS = -l$(LIB_NAME) -lssl -lpthread -lrt -lcrypto $(shell pkg-config --libs gio-2.0 glib-2.0)
SRCS = switch_example.c
JSONS = device_info.json onboarding_config.json
OBJS = $(JSONS:%.json=%.o)
TARGET = switch_example
LD_CMD = ld

.PHONY: all clean
all: $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS)

%.o : %.json
	@echo $@
	$(LD_CMD) -r -b binary -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(SRCS) $(OBJS) $(LIB_DIR) $(INC) $(LIBS)
