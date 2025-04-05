.PHONY: all build run clean

OBJDIR = obj

all: build

build:
	make -C akinator build OBJDIR=$(abspath $(OBJDIR))

run:
	make -C akinator run OBJDIR=$(abspath $(OBJDIR))

tree.o:
	make -C akinator $(OBJDIR)/tree.o OBJDIR=$(abspath $(OBJDIR))

main.o:
	make -C akinator $(OBJDIR)/main.o OBJDIR=$(abspath $(OBJDIR))

file_data.o:
	make -C akinator $(OBJDIR)/file_data.o OBJDIR=$(abspath $(OBJDIR))

clean:
	make -C akinator clean OBJDIR=$(abspath $(OBJDIR))