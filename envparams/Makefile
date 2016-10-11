CC=g++
CFLAGS=-I/usr/include/opencv -I/usr/include/opencv2 -I. -I include
LIBS=`pkg-config --libs opencv`
LIBDIR=-L/usr/local/include

SOURCES=		envparams.cpp \
						seqparams.cpp

OBJS=$(SOURCES:.cpp=.o)

TARGETS=envparams seqparams

%.o: %.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: all
all: $(TARGETS)

envparams: envparams.o
	$(CC) $(CFLAGS) -o envparams envparams.o $(LIBS) $(LIBDIR)

seqparams: seqparams.o
	$(CC) $(CFLAGS) -o seqparams seqparams.o $(LIBS) $(LIBDIR)

.PHONY: clean
clean: ; rm -rf $(OBJS) $(TARGETS)
