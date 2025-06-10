CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g -fsanitize=address `pkg-config --cflags sdl3 sdl3-ttf sdl3-image` -DSDL_MAIN_USE_CALLBACKS
LDFLAGS =  -fsanitize=address `pkg-config --libs sdl3 sdl3-ttf sdl3-image`

BUILD_DIR=build/
SRC = $(wildcard src/*.c src/*/*.c)
INC = 
EXEC = EUI
#下方代码最好不要更改
CFLAGS += $(foreach i,$(INC),-I$(i))
OBJ = $(addprefix $(BUILD_DIR),$(SRC:.c=.o))
DEF = $(addprefix $(BUILD_DIR),$(SRC:.c=.d))

all: $(BUILD_DIR) $(foreach i,$(sort $(dir $(OBJ))),$(i)) $(EXEC)

-include $(DEF)#引入依赖文件

$(BUILD_DIR) $(foreach i,$(sort $(dir $(OBJ))),$(i)):#创建目录
	mkdir $@

$(BUILD_DIR)%.d: %.c #生成依赖文件
	$(CC) $(CFLAGS) -MM -MF $@ $<

$(BUILD_DIR)%.o: %.c #生成目标文件
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXEC): $(OBJ) #生成可执行文件
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)

clean: #删除文件
	rm -rf $(OBJ) $(BUILD_DIR)$(EXEC)

run: all #运行可执行文件
	./$(EXEC)

log:
	@echo SRC: $(SRC)
	@echo OBJ: $(OBJ)
	@echo EXEC:$(EXEC)