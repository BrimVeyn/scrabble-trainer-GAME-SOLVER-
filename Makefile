LIB_NAME        := libhash.a
LIB_DIR         := lib
BIN             := demo

CC              := cc
CFLAGS			:= -Wall -Wextra -Werror -g3 -mtune=native -march=native -Ofast
LDFLAGS			:= -lm -Ilib/raylib/include -Llib/raylib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
SILENT_FLAGS 	:= -D SILENT=1
INC_DIR         := include
DEPS            := $(OBJ:.o=.d)

SRC             := $(wildcard src/*.c) $(wildcard src/hashTable/*.c)

OBJ             := $(SRC:src/%.c=objects/%.o)

OBJDIR          := objects objects/src objects/hashTable

DEF_COLOR       := \033[0;39m
GRAY            := \033[0;90m
RED             := \033[0;91m
GREEN           := \033[0;92m
YELLOW          := \033[0;93m
BLUE            := \033[0;94m
MAGENTA         := \033[0;95m
CYAN            := \033[0;96m
WHITE           := \033[0;97m

all: $(BIN)

$(LIB_DIR)/$(LIB_NAME): $(LIB_DIR) $(OBJ)
	@echo "$(GREEN)Creating library: $(LIB_NAME)"
	@ar rcs $(LIB_DIR)/$(LIB_NAME) $(OBJ)
	@printf "Done !$(DEF_COLOR)\n"

$(BIN): $(LIB_DIR)/$(LIB_NAME) objects/main.o
	@echo "$(GREEN)Making binary: $(BIN)"
	@printf "$(MAGENTA)"
	@$(CC) objects/main.o $(LIB_DIR)/$(LIB_NAME) $(CFLAGS) $(LDFLAGS) -I $(INC_DIR) -o $(BIN)
	@printf "Done !$(DEF_COLOR)\n"

objects/%.o: src/%.c | $(OBJDIR)
	@printf '$(YELLOW)Compiling : %-45s $(CYAN)--> $(YELLOW)%-30s\n' "$<" "$@";
	@printf "$(BLUE)"
	@$(CC) $(CFLAGS) -I $(INC_DIR) -MMD -MP -c $< -o $@
	@printf "$(DEF_COLOR)"

objects/main.o: main.c | $(OBJDIR)
	@printf '$(YELLOW)Compiling : %-45s $(CYAN)--> $(YELLOW)%-30s\n' "$<" "$@";
	@printf "$(BLUE)"
	@$(CC) $(CFLAGS) -I $(INC_DIR) -MMD -MP -c $< -o $@
	@printf "$(DEF_COLOR)"

-include $(DEPS)

compile_commands.json:
	bear -- make

silent: CFLAGS += $(SILENT_FLAGS)
silent: $(BIN)

clean:
	@rm -rf $(OBJDIR) $(LIB_DIR)/$(LIB_NAME)
	@printf "$(RED)Objects and library deleted !$(DEF_COLOR)\n"

fclean: clean
	@rm -rf $(BIN)
	@printf "$(RED)Binary deleted !$(DEF_COLOR)\n"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

re: fclean all

.PHONY: all clean fclean re compile_commands.json
