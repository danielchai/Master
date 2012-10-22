# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
#CC = /home/sampler/Downloads/arm-2008q3/bin/arm-none-linux-gnueabi-gcc
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -g

# define any directories containing header files other than /usr/include
#
INCLUDES = -I. -I/home/sampler/Master/include -I/usr/include/python2.6/

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:

LFLAGS = -L. -L../lib -L/usr/lib/python2.6/config/

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -Wl -Bstatic libtsctl.a -Wl -Bstatic libjson.a  -Wl,-Bdynamic libsqlite3.so.0
LIBS = -lmodbus -ltsctl -ljson -lsqlite3 -lm -lpython2.6

EXEDIR = /home/sampler/Master/src/
DIR = /home/sampler/Master/obj/
# define the C source files
SRCS =Check_time.c Current_time.c Get_ADC.c Get_calibration.c Get_configure.c Init_bus.c Init_db.c Json_convert.c Json_convert_array.c Json_convert_float.c Json_convert_profile.c Json_convert_string.c Json_convert_sdl.c Log.c master.c  Muxbus.c Modbus.c Parse.c Process_killer.c Reset_db.c Sdl.c SDLdb.c Set_calibration.c Set_configure.c Set_profile.c Set_time.c slave.c Sleep_all.c systemget.c systemset.c Time_diff.c profile_db.c Wakeup.c Temp.c
#SRCS = ../obj/get.c ../obj/dbinit.c ../obj/set.c ../obj/getall.c ../obj/parse.c ../obj/reset.c ../obj/checkvalue.c ../obj/set_time.c ../obj/check_time.c ../obj/get_adc.c ../obj/json_convert.c ../obj/flowbotics.c
# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)
OBJ = $(SRCS:%.c=$(DIR)%.o)
# define the executable file 
MAIN = master

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
	@echo  Simple compiler named mycc has been compiled

$(MAIN): $(OBJ) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJ) $(LFLAGS) $(LIBS)
# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
$(DIR)%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) $(OBJ) $(EXEDIR)$(MAIN)

depend: $(SRCS) $(OBJS) $(OBJ)
	 makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

