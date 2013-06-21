LOCAL_SRC := $(wildcard *.c)
LOCAL_OBJ := $(patsubst %.c,%.o,$(LOCAL_SRC))
CFLAGS := -c

.PHONY : all
all : $(LOCAL_OBJ) MV
$(LOCAL_OBJ) : $(LOCAL_SRC)
	$(CC) $(CFLAGS) $(DEBUG) $^ -I$(INCLUDE_DIR)

MV :
	-mv $(LOCAL_OBJ) $(OBJ_DIR)

