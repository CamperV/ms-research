CC=g++
CFLAGS=-I/usr/include/opencv -I/usr/include/opencv2 -I. -I include
LIBS=`pkg-config --libs opencv`
LIBDIR=-L/usr/local/include

SOURCES=		shadowdetect.cpp \
						ChromacityShadRem.cpp ChromacityShadRemParams.cpp \
						GeometryShadRem.cpp GeometryShadRemParams.cpp \
						LrTextureShadRem.cpp LrTextureShadRemParams.cpp \
						PhysicalShadRem.cpp PhysicalShadRemParams.cpp \
						SrTextureShadRem.cpp SrTextureShadRemParams.cpp

UTILDIR=utils
UTILS=			$(UTILDIR)/GaborFilter.cpp \
						$(UTILDIR)/ConnComp.cpp $(UTILDIR)/ConnCompGroup.cpp \
						$(UTILDIR)/GaussianMixtureModel.cpp \
						$(UTILDIR)/VideoStats.cpp

OBJS=$(SOURCES:.cpp=.o)
OBJS+=$(UTILS:.cpp=.o)

TARGETS=shadowdetect

$(UTILDIR)/%.o: $(UTILDIR)/%.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: all
all: $(TARGETS)

$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGETS) $(OBJS) $(LIBS) $(LIBDIR)

.PHONY: clean
clean: ; rm -rf $(OBJS) $(TARGETS)
