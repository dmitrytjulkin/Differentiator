FLAGS="-D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat
-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion
-Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral
-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor
-Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls
-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override
-Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused
-Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing
-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new
-fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer
-Wlarger-than=8192 -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,\
enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-\
attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,\
undefined,unreachable,vla-bound,vptr"


g++ tree_main.cpp differentiator.cpp optimize_tree.cpp read_tree_from_file.cpp \
    list_of_func_and_op.cpp work_with_dumps/clear_dump.cpp \
    work_with_dumps/build_graph_dump.cpp work_with_dumps/build_tex_dump.cpp \
    operations_with_tree/free_tree.cpp operations_with_tree/get_tree_size.cpp \
    operations_with_tree/init_tree.cpp operations_with_tree/make_or_delete_node.cpp \
    operations_with_tree/print_tree.cpp -o ./tree $FLAGS && ./tree

# TODO: think about grouping files from same directory
