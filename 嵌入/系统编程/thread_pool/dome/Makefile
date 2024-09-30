CC = gcc 
TARGET = bin/main
C_SOURCE = $(wildcard c_source/*.c)
INCLUDE_PATH = -I ./include

$(TARGET):$(C_SOURCE)
	$(CC) $^  -o  $@ $(INCLUDE_PATH)

.PHONY:clean	
	
clean:
	$(RM) $(TARGET)