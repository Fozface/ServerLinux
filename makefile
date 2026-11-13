B = build/
OBJ = $(B)main.o $(B)sensor.o $(B)helper.o
CFLAGS = -Wall -Werror -Wextra -g -Iinc

vpath %.h ./inc/
vpath %.c ./src/


all : $(B)tempmon
$(B)tempmon : $(OBJ)
	cc $(CFLAGS) -o $(B)tempmon $(OBJ)
$(B)main.o : main.c sensor.h helper.h | $(B)
	cc $(CFLAGS) -c $< -o $@
$(B)sensor.o : sensor.c sensor.h helper.h | $(B)
	cc $(CFLAGS) -c $< -o $@
$(B)helper.o : helper.c helper.h | $(B)
	cc $(CFLAGS) -c $< -o $@
.PHONY : clean format remake debug all mkdir
mkdir $(B) : 
	mkdir -p build
clean :
	rm -rf ./build
format :
	clang-format -i src/*.c inc/*.h
debug :
	$(MAKE) clean
	$(MAKE) $(B)tempmon CFLAGS="$(CFLAGS) -O0"
	gdb --args ./$(B)tempmon 3
remake :
	$(MAKE) clean
	$(MAKE) $(B)tempmon
