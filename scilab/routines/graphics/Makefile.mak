# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/graphics.lib

OBJSC = periWin.obj periPos.obj periFig.obj periGif.obj Xcall.obj Xcall1.obj \
	Contour.obj Plo3d.obj Math.obj Axes.obj Champ.obj Plo2d.obj \
	Plo2d1.obj Plo2d2.obj Plo2d3.obj Plo2d4.obj Plo2dEch.obj Rec.obj Gray.obj \
	Alloc.obj FeC.obj RecLoad.obj RecSave.obj Actions.obj  Events.obj\
	gsort.obj qsort.obj nues1.obj  Format.obj \
	dr1Call.obj sciCall.obj Plo2dn.obj Plo3dn.obj \
	GetProperty.obj BuildObjects.obj CloneObjects.obj DestroyObjects.obj \
	DrawObjects.obj SetProperty.obj HandleManagement.obj InitObjects.obj \
	Interaction.obj Interaction.obj SetProperty.obj \

OBJSF = 

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

include ../Make.lib.mak

#--- dependencies generated with gcc ----
Actions.obj: Actions.c Math.h ../machine.h Graphics.h
Alloc.obj: Alloc.c Math.h ../machine.h Graphics.h
Axes.obj: Axes.c Math.h ../machine.h Graphics.h PloEch.h
Champ.obj: Champ.c Math.h ../machine.h Graphics.h PloEch.h
Contour.obj: Contour.c Math.h ../machine.h Graphics.h PloEch.h
Events.obj : Events.c bcg.h
FeC.obj: FeC.c Math.h ../machine.h Graphics.h
Format.obj: Format.c Math.h ../machine.h Graphics.h
Gray.obj: Gray.c Math.h ../machine.h Graphics.h PloEch.h
gsort.obj: gsort.c Math.h ../machine.h Graphics.h ../sun/men_Sutils.h \
 gsort-int.h gsort-double.h gsort-string.h
Math.obj: Math.c Math.h ../machine.h Graphics.h ../sparse/spConfig.h
nues1.obj: nues1.c ../machine.h Math.h Graphics.h
periFig.obj: periFig.c Math.h ../machine.h Graphics.h periFig.h color.h bcg.h
periGif.obj: periGif.c Math.h ../machine.h Graphics.h periGif.h color.h  bcg.h\
 ../gd/gd.h ../gd/../machine.h
periPos.obj: periPos.c Math.h ../machine.h Graphics.h periPos.h color.h bcg.h
periX11.o: periX11.c Math.h ../machine.h Graphics.h periX11.h \
 bcg.h ../version.h color.h ../intersci/cerro.h \
 ../intersci/../machine.h
Plo2d1.obj: Plo2d1.c Math.h ../machine.h Graphics.h PloEch.h
Plo2d2.obj: Plo2d2.c Math.h ../machine.h Graphics.h PloEch.h
Plo2d3.obj: Plo2d3.c Math.h ../machine.h Graphics.h PloEch.h
Plo2d4.obj: Plo2d4.c Math.h ../machine.h Graphics.h PloEch.h
Plo2d.obj: Plo2d.c Math.h ../machine.h Graphics.h PloEch.h
Plo2dEch.obj: Plo2dEch.c Math.h ../machine.h Graphics.h PloEch.h
Plo3d.obj: Plo3d.c Math.h ../machine.h Graphics.h PloEch.h
Plo3dn.obj : Plo3dn.c Math.h ../machine.h Graphics.h PloEch.h  ../stack-c.h ../graphics/Math.h
qsort.obj: qsort.c
Rec.obj: Rec.c Math.h ../machine.h Graphics.h Rec.h PloEch.h
RecLoad.obj: RecLoad.c Math.h ../machine.h Graphics.h Rec.h
RecSave.obj: RecSave.c Math.h ../machine.h Graphics.h Rec.h
Xcall1.obj: Xcall1.c Math.h ../machine.h Graphics.h PloEch.h
Xcall.obj: Xcall.c Math.h ../machine.h Graphics.h periX11.h periPos.h \
 periFig.h periGif.h

