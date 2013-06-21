ver = release
ifeq ($(ver), debug)
DEBUG = -g -Ddebug
BIN = main.d
else
BIN = main.r
DEBUG =
endif
CC := gcc
SUBDIRS := lib src obj
#BIN := main
PWD := $(shell pwd)
SRC_DIR := $(PWD)/src
OBJ_DIR := $(PWD)/obj
BIN_DIR := $(PWD)/bin
LIB_DIR := $(PWD)/lib
INCLUDE_DIR := $(PWD)/include 
export CC PWD BIN OBJ_DIR BIN_DIR LIB_DIR INCLUDE_DIR DEBUG

.PHONY : all
all : CHECK_DIR $(SUBDIRS)
CHECK_DIR : 
	mkdir -p $(BIN_DIR)
	#mkdir -p $(OBJ_DIR)

$(SUBDIRS) : ECHO
	make -C $@

ECHO :
	@echo $(SUBDIR)
	@echo begin complie

clean :
	-rm -rf $(OBJ_DIR)/*.a
	-rm -rf $(OBJ_DIR)/*.o
	-rm -rf $(BIN_DIR)
	-rm -rf $(LIB_DIR)/*.a
	-rm -rf $(LIB_DIR)/*.o
	-rm -rf $(PWD)/main.*
