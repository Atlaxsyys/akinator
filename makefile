.PHONY: all build run

build:
	make -C akinator build
	
run:
	make -C akinator run

tree.o:
	make -C akinator tree.o

main.o:
	make -C akinator main.o

file_data.o:
	make -C akinator file_data.o

clean:
	make -C akinator clean