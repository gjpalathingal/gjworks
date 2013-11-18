####################################
# File Name: Makefile
# Purpose: To build the GRS shell
# Author: Rekha Balasubramanian
# CWID: A20274931
####################################

CC = cc
APP = grsShell
PRG = grsApplicationProgram

C_OBJS = grsMain.o \
         grsShellUtils.o \
         grsProcess.o \
         grsCmdExec.o \
         grsAlias.o \
         grsPipes.o \
         grsParser.o \
         grsRedirection.o \
         grsIfThenElse.o

C_FILES = grsMain.c \
          grsShellUtils.c \
          grsProcess.c \
          grsCmdExec.c \
          grsAlias.c \
          grsPipes.c \
          grsParser.c \
          grsRedirection.c \
          grsIfThenElse.c

.c.o:
	${CC} -g -Wall -c ${C_FILES}

all: ${APP} ${PRG}
${APP}: ${C_OBJS}
	${CC} -g -Wall -o ${APP} ${C_OBJS}

${PRG}: grsApplicationProgram.c
	${CC} -g -Wall -DDEBUG_UNIT -o ${PRG} grsApplicationProgram.c

clean:
	@rm -f ${C_OBJS} ${APP} ${PRG} grsApplicationProgram.o
