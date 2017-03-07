CC=g++
CFLAGS=-I/usr/include/opencv -I/usr/include/opencv2 -I. -I include
LIBS=`pkg-config --libs opencv`
LIBDIR=-L/usr/local/include

BINDIR=bin

LRDIR=lr
LRSRC=		$(LRDIR)/ChromacityShadRem.cpp $(LRDIR)/ChromacityShadRemParams.cpp \
						$(LRDIR)/GeometryShadRem.cpp  $(LRDIR)/GeometryShadRemParams.cpp \
						$(LRDIR)/LrTextureShadRem.cpp  $(LRDIR)/LrTextureShadRemParams.cpp \
						$(LRDIR)/PhysicalShadRem.cpp  $(LRDIR)/PhysicalShadRemParams.cpp \
						$(LRDIR)/SrTextureShadRem.cpp  $(LRDIR)/SrTextureShadRemParams.cpp

UTILDIR=utils
UTILS=			$(UTILDIR)/GaborFilter.cpp \
						$(UTILDIR)/ConnComp.cpp $(UTILDIR)/ConnCompGroup.cpp \
						$(UTILDIR)/GaussianMixtureModel.cpp \
						$(UTILDIR)/VideoStats.cpp \
						$(UTILDIR)/Metrics.cpp

SRCDIR=src
SRC=				$(SRCDIR)/stream_gui.cpp \
						$(SRCDIR)/quantitative_gui.cpp \
						$(SRCDIR)/quantitative.cpp \
						$(SRCDIR)/make_bg.cpp \
						$(SRCDIR)/make_fg.cpp \
						$(SRCDIR)/avgattenmodel.cpp

OBJS=$(LRSRC:.cpp=.o)
OBJS+=$(UTILS:.cpp=.o)

TARGETS=$(BINDIR)/stream_gui $(BINDIR)/quantitative_gui $(BINDIR)/quantitative $(BINDIR)/make_bg $(BINDIR)/make_fg $(BINDIR)/avgattenmodel

$(UTILDIR)/%.o: $(UTILDIR)/%.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

$(LRDIR)/%.o: $(LRDIR)/%.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CC) -c -o $@ $^ $(CFLAGS)

.PHONY: all
all: $(TARGETS)

$(BINDIR)/stream_gui: $(BINDIR)/stream_gui.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/stream_gui $(BINDIR)/stream_gui.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/quantitative_gui: $(BINDIR)/quantitative_gui.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/quantitative_gui $(BINDIR)/quantitative_gui.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/quantitative: $(BINDIR)/quantitative.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/quantitative $(BINDIR)/quantitative.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/make_bg: $(BINDIR)/make_bg.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/make_bg $(BINDIR)/make_bg.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/make_fg: $(BINDIR)/make_fg.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/make_fg $(BINDIR)/make_fg.o $(OBJS) $(LIBS) $(LIBDIR)

$(BINDIR)/avgattenmodel: $(BINDIR)/avgattenmodel.o $(OBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/avgattenmodel $(BINDIR)/avgattenmodel.o $(OBJS) $(LIBS) $(LIBDIR)

.PHONY: clean
clean: ; rm -rf $(OBJS) $(TARGETS) $(BINDIR)
