OBJ = main.o sensor.o helper.o
CFLAGS = -Wall -Werror -Wextra -g


all : tempmon
tempmon : $(OBJ)
	cc $(CFLAGS) -o tempmon $(OBJ)
main.o : main.c sensor.o helper.o
	cc $(CFLAGS) -c main.c
sensor.o : sensor.c sensor.h
	cc $(CFLAGS) -c sensor.c
helper.o : helper.c helper.h
	cc $(CFLAGS) -c helper.c
.PHONY : clean format remake debug all
clean :
	rm -f tempmon $(OBJ)
format :
	clang-format -i *.c *.h
debug :
	$(MAKE) clean
	$(MAKE) tempmon CFLAGS="$(CFLAGS) -O0"
	gdb --args ./tempmon 3
remake :
	$(MAKE) clean
	$(MAKE) tempmon
