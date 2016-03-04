CC=g++
CFLAGS=-I/usr/include/opencv -I/usr/include/opencv2 -std=c++0x
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

shadowdetect: main.o $(OBJS) $(UTILS)
	$(CC) $(CFLAGS) -o shadowdetect main.o $(OBJS) $(UTILS) $(LIBS) $(LIBDIR)

.PHONY: clean
clean: ; rm -rf *.o $(UTILDIR)/*.o $(TARGETS)
