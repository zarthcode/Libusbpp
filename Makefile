#
# Makefile for Libusbpp
#
#

NAME				 	=	libusbpp
INCLUDE_PATH 	= ./headers

#
#	Compiler
#
#	By default, the GNU C++ compiler is used.
#

COMPILER			=	g++
LINKER				=	g++

#
#	Compiler and linker flags
#
#	This variable holds the flags that are passed to the compiler. By default,
# 	we include the -O2 flag. This flag tells the compiler to optimize the code,
#	but it makes debugging more difficult. So if you're debugging your application,
#	you probably want to remove this -O2 flag. At the same time, you can then
#	add the -g flag to instruct the compiler to include debug information in
#	the library (but this will make the final file much bigger, so
#	you want to leave that flag out on production servers).
#
#COMPILER_FLAGS = -Wall -c -ggdb3 -fpic -I${INCLUDE_PATH} -o
COMPILER_FLAGS		=	-Wall -c -O2 -fpic -I${INCLUDE_PATH} -o
LINKER_FLAGS		=	-lusb-1.0 -lpthread

#
#	Command to remove files, copy files and create directories.
#

RM	= rm -f
CP	= cp -f
MKDIR	= mkdir -p
LS	= ls -1

#
#	All source files are simply all *.cpp files found in the current directory
#
#	A builtin Makefile macro is used to scan the current directory and find
#	all source files. The object files are all compiled versions of the source
#	file, with the .cpp extension being replaced by .o.
#

SOURCES	= $(wildcard ./src/*.cpp)
HEARDERS	= $(wildcard ./src/*.hpp)
EXAMPELS = $(wildcard ./examples/*.cpp)
OBJECTS	= $(SOURCES:%.cpp=%.o) $(EXAMPELS:%.cpp=%.o)
EXECUTABLES = $(EXAMPELS:%.cpp=%)

#
#	From here the build instructions start
#
all:	${EXECUTABLES} ${OBJECTS} ${SOURCES} ${HEARDERS} ${EXAMPELS}
		echo ${EXECUTABLES} ${OBJECTS} ${SOURCES} ${HEARDERS} ${EXAMPELS}

${OBJECTS}: ${SOURCES} ${HEARDERS} ${EXAMPELS}
		${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

${EXECUTABLES}: ${OBJECTS}
		${LINKER} -o $@ ${OBJECTS} ${LINKER_FLAGS}

test: .TEST

.TEST:
	./examples/LibusbTest

clean:
	echo ${RM} ${OBJECTS}
	${RM} ${OBJECTS}
