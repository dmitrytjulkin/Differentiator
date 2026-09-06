FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat 			 \
		-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts				 \
		-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral	 \
		-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor	 \
		-Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls		 \
		-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override	 \
		-Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused				 \
		-Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing				 \
		-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new					 \
		-fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer \
		-Wlarger-than=8192 -fPIE -Werror=vla \
		-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CC = g++

MAIN = main.cpp

FUNC_OP_DIR = func_and_op
FUNC_OP_FILE = $(FUNC_OP_DIR)/list_of_func_and_op.cpp

DIFF_DIR = differentiator
DIFFERENTIATE = $(DIFF_DIR)/differentiator.cpp $(DIFF_DIR)/optimize_tree.cpp

DUMP_DIR = build_dumps
TO_DUMP = $(DUMP_DIR)/build_graph_dump.cpp $(DUMP_DIR)/build_tex_dump.cpp \
		$(DUMP_DIR)/clear_dump.cpp

TREE_DIR = parse_tree
PARSE_TREE = $(TREE_DIR)/free_tree.cpp $(TREE_DIR)/get_tree_size.cpp \
		$(TREE_DIR)/init_tree.cpp $(TREE_DIR)/make_or_delete_node.cpp \
		$(TREE_DIR)/print_tree.cpp

INPUT_DIR = input_to_tree
PARSE_INPUT = $(INPUT_DIR)/read_tree_from_file.cpp $(INPUT_DIR)/read_input.cpp  \
		 $(INPUT_DIR)/input_tokenization.cpp

TOKEN_ARR_DIR = work_with_token_array
PARSE_TOKEN_ARRAY = $(TOKEN_ARR_DIR)/parse_token_array.cpp

NAMETABLE_DIR = work_with_nametable
PARSE_NAMETABLE = $(NAMETABLE_DIR)/parse_nametable.cpp

BINARY = differentiate

all:
	$(CC) $(MAIN) $(TO_DUMP) $(PARSE_TREE) $(PARSE_INPUT) $(PARSE_TOKEN_ARRAY) $(DIFFERENTIATE) \
	$(PARSE_NAMETABLE) $(FUNC_OP_FILE) $(FLAGS) -o ./$(BINARY)

run: all
	./$(BINARY)

noise:

clean:
	rm -rf $(BINARY).dSYM \
	rm -f BINARY


.PHONY: all run clean
