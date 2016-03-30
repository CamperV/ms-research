CC=g++
CFLAGS=-I/usr/include/opencv -I/usr/include/opencv2 -I. -I./include
LIBS=`pkg-config --libs opencv`
LIBDIR=-L/usr/local/include
UTILDIR=utils

OBJS=ChromacityShadRem.o ChromacityShadRemParams.o GeometryShadRem.o GeometryShadRemParams.o LrTextureShadRem.o LrTextureShadRemParams.o PhysicalShadRem.o PhysicalShadRemParams.o SrTextureShadRem.o SrTextureShadRemParams.o
UTILS=$(UTILDIR)/GaborFilter.o $(UTILDIR)/ConnComp.o $(UTILDIR)/ConnCompGroup.o $(UTILDIR)/GaussianMixtureModel.o $(UTILDIR)/VideoStats.o
TARGETS=shadowdetect

utils/%.o: utils/%.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: all
all: $(TARGETS)

shadowdetect: shadowdetect.o $(OBJS) $(UTILS)
	$(CC) $(CFLAGS) -o shadowdetect shadowdetect.o $(OBJS) $(UTILS) $(LIBS) $(LIBDIR)

.PHONY: clean
clean: ; rm -rf *.o $(UTILDIR)/*.o $(TARGETS)
