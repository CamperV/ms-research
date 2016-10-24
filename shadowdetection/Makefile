CC=g++
CFLAGS=-I/usr/include/opencv -I/usr/include/opencv2 -I. -I include
LIBS=`pkg-config --libs opencv`
LIBDIR=-L/usr/local/include

SOURCES=		ChromacityShadRem.cpp ChromacityShadRemParams.cpp \
						GeometryShadRem.cpp GeometryShadRemParams.cpp \
						LrTextureShadRem.cpp LrTextureShadRemParams.cpp \
						PhysicalShadRem.cpp PhysicalShadRemParams.cpp \
						SrTextureShadRem.cpp SrTextureShadRemParams.cpp

UTILDIR=utils
UTILS=			$(UTILDIR)/GaborFilter.cpp \
						$(UTILDIR)/ConnComp.cpp $(UTILDIR)/ConnCompGroup.cpp \
						$(UTILDIR)/GaussianMixtureModel.cpp \
						$(UTILDIR)/VideoStats.cpp \
						$(UTILDIR)/Metrics.cpp

OBJS=$(SOURCES:.cpp=.o)
OBJS+=$(UTILS:.cpp=.o)

TARGETS=shadowdetect singletest quantitative quantitative2 stream_quantitative

$(UTILDIR)/%.o: $(UTILDIR)/%.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: all
all: $(TARGETS)

shadowdetect: shadowdetect.o $(OBJS)
	$(CC) $(CFLAGS) -o shadowdetect shadowdetect.o $(OBJS) $(LIBS) $(LIBDIR)

singletest: singletest.o $(OBJS)
	$(CC) $(CFLAGS) -o singletest singletest.o $(OBJS) $(LIBS) $(LIBDIR)

quantitative: quantitative.o $(OBJS)
	$(CC) $(CFLAGS) -o quantitative quantitative.o $(OBJS) $(LIBS) $(LIBDIR)

quantitative2: quantitative2.o $(OBJS)
	$(CC) $(CFLAGS) -o quantitative2 quantitative2.o $(OBJS) $(LIBS) $(LIBDIR)

stream_quantitative: stream_quantitative.o $(OBJS)
	$(CC) $(CFLAGS) -o stream_quantitative stream_quantitative.o $(OBJS) $(LIBS) $(LIBDIR)

.PHONY: clean
clean: ; rm -rf $(OBJS) $(TARGETS)
