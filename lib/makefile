AR := ar rc
CFLAGS := -c 
LOCAL_SRC := $(wildcard *.c)
LOCAL_OBJ := $(patsubst %.c,%.o,$(LOCAL_SRC))
LIB := libmain.a

.PHONY : all
all : ECHO $(LIB) MV
ECHO : 
	@echo $(LOCAL_OBJ)

$(LIB) : $(LOCAL_OBJ)
	$(AR) $(LIB) $^

$(LOCAL_OBJ) : $(LOCAL_SRC)
	$(CC) $(CFLAGS) $^ $(DEBUG) -I$(INCLUDE_DIR)

MV :
	-mv $(LIB) $(OBJ_DIR)
