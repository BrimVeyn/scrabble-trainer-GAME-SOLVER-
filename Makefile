BIN             := demo
LIB_VECTOR		:= lib_vector/lib/libvector.a

CC              := cc
CFLAGS			:= -Wall -Wextra -Werror -g3 -mtune=native -march=native -Ofast
LDFLAGS			:= -lm -Ilib/raylib/include -Ilib_vector/include -Llib/raylib/lib -Lvector_lib/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
INC_DIR         := include
DEPS            := $(OBJ:.o=.d)

SRC             := $(wildcard src/*.c) $(wildcard src/hashTable/*.c) \
				   $(wildcard src/tree/*.c) $(wildcard src/game/*.c) \
				   $(wildcard src/event/*.c) $(wildcard src/draw/*.c) \
				   $(wildcard src/utils/*.c) main.c

OBJ             := $(SRC:src/%.c=objects/%.o)

OBJDIR          := objects objects/src objects/hashTable \
				   objects/tree objects/game \
				   objects/event objects/draw \
				   objects/utils

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

$(BIN): $(LIB_VECTOR) $(OBJ)
	@echo "$(GREEN)Making binary: $(BIN)"
	@printf "$(MAGENTA)"
	@$(CC) $(OBJ) $(CFLAGS) $(LIB_VECTOR) $(LDFLAGS) -I $(INC_DIR) -o $(BIN)
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

clean:
	@rm -rf $(OBJDIR)
	@printf "$(RED)Objects and library deleted !$(DEF_COLOR)\n"

fclean: clean
	@rm -rf $(BIN)
	@make -C lib_vector/ fclean
	@printf "$(RED)Binary deleted !$(DEF_COLOR)\n"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

$(LIB_VECTOR):
	@make -C lib_vector/

swap-lib:
	@mv ./lib/raylib/lib/libraylib.a tmp
	@mv ./lib/raylib/lib/.tmplib ./lib/raylib/lib/libraylib.a
	@mv tmp ./lib/raylib/lib/.tmplib

re: fclean all

.PHONY: all clean fclean re compile_commands.json
