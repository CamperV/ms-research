CC=g++
CFLAGS=-I/usr/include/opencv -I/usr/include/opencv2 -I. -I include
LIBS=`pkg-config --libs opencv`
LIBDIR=-L/usr/local/include

BINDIR=bin

UTILDIR=utils
UTILS=			$(UTILDIR)/GaborFilter.cpp \
						$(UTILDIR)/ConnComp.cpp $(UTILDIR)/ConnCompGroup.cpp \
						$(UTILDIR)/GaussianMixtureModel.cpp \
						$(UTILDIR)/VideoStats.cpp \
						$(UTILDIR)/Metrics.cpp
OBJS+=$(UTILS:.cpp=.o)

SRCDIR=src
SRC=				$(SRCDIR)/seqparams.cpp \
						$(SRCDIR)/ccompparams.cpp \
						$(SRCDIR)/siftparams.cpp \
						$(SRCDIR)/bgsiftparams.cpp \
						$(SRCDIR)/colorsiftparams.cpp \
						$(SRCDIR)/hsvobjparams.cpp \
						$(SRCDIR)/avgattenparams.cpp \
						$(SRCDIR)/bowparams.cpp

TARGETS=$(BINDIR)/hsvparams \
				$(BINDIR)/ccompparams \
				$(BINDIR)/siftparams \
				$(BINDIR)/bgsiftparams \
				$(BINDIR)/colorsiftparams \
				$(BINDIR)/hsvobjparams \
				$(BINDIR)/avgattenparams \
				$(BINDIR)/bowparams

$(UTILDIR)/%.o: $(UTILDIR)/%.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: all
all: $(TARGETS)

$(BINDIR)/hsvparams: $(BINDIR)/hsvparams.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/hsvparams $(BINDIR)/hsvparams.o $(LIBS) $(LIBDIR)

$(BINDIR)/ccompparams: $(BINDIR)/ccompparams.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/ccompparams $(BINDIR)/ccompparams.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/siftparams: $(BINDIR)/siftparams.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/siftparams $(BINDIR)/siftparams.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/bgsiftparams: $(BINDIR)/bgsiftparams.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/bgsiftparams $(BINDIR)/bgsiftparams.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/hsvobjparams: $(BINDIR)/hsvobjparams.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/hsvobjparams $(BINDIR)/hsvobjparams.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/avgattenparams: $(BINDIR)/avgattenparams.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/avgattenparams $(BINDIR)/avgattenparams.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/bowparams: $(BINDIR)/bowparams.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/bowparams $(BINDIR)/bowparams.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/colorsiftparams: $(BINDIR)/colorsiftparams.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/colorsiftparams $(BINDIR)/colorsiftparams.o $(OBJS) $(LIBS) $(LIBDIR)

.PHONY: clean
clean: ; rm -rf $(OBJS) $(TARGETS) $(BINDIR)
