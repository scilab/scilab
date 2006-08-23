SHELL = /bin/sh

SCIDIR=../../..
include ../../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

LIBRARY = $(SCIDIR)/libs/libXmu.a

OBJSC = AllCmap.o \
	Atoms.o \
	ClientWin.o \
	CloseHook.o \
	CmapAlloc.o \
	CrCmap.o \
	CrPixFBit.o \
	CursorName.o \
	CvtCache.o \
	CvtStdSel.o \
	DefErrMsg.o \
	DelCmap.o \
	DisplayQue.o \
	Distinct.o \
	DrawLogo.o \
	DrRndRect.o \
	EditresCom.o \
	FToCback.o \
	GetHost.o \
	GrayPixmap.o \
	Initer.o \
	LocBitmap.o \
	Lookup.o \
	LookupCmap.o \
	Lower.o \
	RdBitF.o \
	ScrOfWin.o \
	ShapeWidg.o \
	StdCmap.o \
	StrToBS.o \
	StrToBmap.o \
	StrToCurs.o \
	StrToGrav.o \
	StrToJust.o \
	StrToLong.o \
	StrToOrnt.o \
	StrToShap.o \
	StrToWidg.o \
	UpdMapHint.o \
	VisCmap.o \
	WidgetNode.o \
	Xct.o \
	Clip.o ExtAgent.o

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
