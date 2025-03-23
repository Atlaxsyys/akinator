.PHONY: all build run

build:
	make -C tree build
	
run:
	make -C tree run

tree.o:
	make -C tree tree.o

main.o:
	make -C tree main.o

file_data.o:
	make -C tree file_data.o

clean:
	make -C tree clean