CC=gcc -Wall
HEADER_FILES_DIR=.
SOURCES_DIR=.
INCLUDES=-I $(HEADER_FILES_DIR)
OUTPUT=ejecutable
LIB_HEADERS=$(HEADER_FILES_DIR)/$(wildcard *.h)
SRCS := $(SOURCES_DIR)/$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LIBS)

%.o: %.c $(LIB_HEADERS)
	$(CC) -c -o $@ $< $(INCLUDES)
	
cleanall: clean
	rm -f $(OUTPUT)

clean:
	rm -f *.o *~