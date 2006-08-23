SHELL = /bin/sh

SCIDIR=../../..
include ../../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

LIBRARY = $(SCIDIR)/libs/libXaw3d.a

OBJSC =AllWidgets.o \
	AsciiSink.o \
	AsciiSrc.o \
	AsciiText.o \
	Box.o \
	Command.o \
	Dialog.o \
	Form.o \
	Grip.o \
	Label.o \
	Layout.o \
	List.o \
	MenuButton.o \
	MultiSink.o \
	MultiSrc.o \
	Paned.o \
	Panner.o \
	Porthole.o \
	Repeater.o \
	Scrollbar.o \
	sharedlib.o \
	Simple.o \
	SimpleMenu.o \
	SmeBSB.o \
	Sme.o \
	SmeLine.o \
	SmeThreeD.o \
	StripChart.o \
	Template.o \
	TextAction.o \
	Text.o \
	TextPop.o \
	TextSink.o \
	TextSrc.o \
	TextTr.o \
	ThreeD.o \
        Toggle.o \
	Tree.o \
	Vendor.o \
	Viewport.o \
	XawI18n.o \
	XawIm.o \
	XawInit.o

OBJSF =

OBJS = $(OBJSC) $(OBJSF)

all:: $(LIBRARY)

$(LIBRARY): $(OBJS)
	@echo Creation of $(LIBRARY)
	@$(RM) $@
	@$(AR) $@ $(OBJS)
	@$(RANLIB) $@

clean::
	$(RM) $(OBJS)
distclean::
	$(RM) $(OBJS) $(LIBRARY)
