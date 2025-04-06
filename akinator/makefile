OBJDIR ?= ../obj

FLAGS = -DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CC = g++
CMD = $(OBJDIR)/akinator.exe
INCLUDE = -I../include
CPPSRC = akinator_game.cpp ../file_data/file_data.cpp ../tree/tree.cpp main.cpp errors.cpp ../logger/logger.cpp

CPPOBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(CPPSRC)))

$(shell mkdir -p $(OBJDIR))

vpath %.cpp .:../file_data:../tree:../logger

$(OBJDIR)/%.o: %.cpp
	@$(CC) $(FLAGS) $(INCLUDE) -c $< -g -o $@

build: $(CPPOBJ)
	@$(CC) $^ $(FLAGS) -g $(INCLUDE) -o $(CMD) -lsfml-graphics -lsfml-window -lsfml-system

run: build
	LIBGL_ALWAYS_SOFTWARE=1 $(CMD) ../data/data_base.txt

clean:
	rm -rf $(OBJDIR)