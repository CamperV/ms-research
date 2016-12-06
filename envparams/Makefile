CC=g++
CFLAGS=-I/usr/include/opencv -I/usr/include/opencv2 -I. -I include
LIBS=`pkg-config --libs opencv`
LIBDIR=-L/usr/local/include

UTILDIR=utils
UTILS=			$(UTILDIR)/GaborFilter.cpp \
						$(UTILDIR)/ConnComp.cpp $(UTILDIR)/ConnCompGroup.cpp \
						$(UTILDIR)/GaussianMixtureModel.cpp \
						$(UTILDIR)/VideoStats.cpp \
						$(UTILDIR)/Metrics.cpp

OBJS+=$(UTILS:.cpp=.o)

TARGETS=envparams seqparams ccompparams

$(UTILDIR)/%.o: $(UTILDIR)/%.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: all
all: $(TARGETS)

envparams: envparams.o
	$(CC) $(CFLAGS) -o envparams envparams.o $(LIBS) $(LIBDIR)

seqparams: seqparams.o
	$(CC) $(CFLAGS) -o seqparams seqparams.o $(LIBS) $(LIBDIR)

ccompparams: ccompparams.o $(OBJS)
	$(CC) $(CFLAGS) -o ccompparams ccompparams.o $(OBJS) $(LIBS) $(LIBDIR)

.PHONY: clean
clean: ; rm -rf $(OBJS) $(TARGETS)
