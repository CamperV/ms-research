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

$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJS) $(LIBS) $(LIBDIR)

.PHONY: clean
clean: ; rm -rf $(OBJS) $(TARGETS)
