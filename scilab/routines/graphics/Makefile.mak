# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/graphics.lib

OBJSC = periWin.obj periPos.obj periFig.obj periGif.obj Xcall.obj Xcall1.obj \
	GetProperty.obj BuildObjects.obj CloneObjects.obj DestroyObjects.obj \
	DrawObjects.obj SetProperty.obj HandleManagement.obj InitObjects.obj \
	Interaction.obj Interaction.obj SetProperty.obj \
	Contour.obj Plo3d.obj Math.obj Axes.obj Champ.obj Plo2d.obj \
	Plo2d1.obj Plo2d2.obj Plo2d3.obj Plo2d4.obj Plo2dEch.obj Rec.obj Gray.obj \
	Alloc.obj FeC.obj RecLoad.obj RecSave.obj Tests.obj Actions.obj \
	gsort.obj qsort.obj nues1.obj  Format.obj \
	dr1Call.obj sciCall.obj Plo2dn.obj Plo3dn.obj

OBJSF = 

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(XFLAGS) 

include ../Make.lib.mak

#--- dependencies generated with gcc ----
Actions.obj: Actions.c Math.h ../machine.h Graphics.h  GetProperty.h SetProperty.h DrawObjects.h
Alloc.obj: Alloc.c Math.h ../machine.h Graphics.h
Axes.obj: Axes.c Math.h ../machine.h Graphics.h PloEch.h GetProperty.h
Champ.obj: Champ.c Math.h ../machine.h Graphics.h PloEch.h GetProperty.h \
  SetProperty.h DrawObjects.h BuildObjects.h
Contour.obj: Contour.c Math.h ../machine.h Graphics.h PloEch.h GetProperty.h
FeC.obj: FeC.c Math.h ../machine.h Graphics.h \
  GetProperty.h SetProperty.h BuildObjects.h DrawObjects.h
Format.obj: Format.c Math.h ../machine.h Graphics.h
GetProperty.obj: GetProperty.c GetProperty.h PloEch.h bcg.h BuildObjects.h SetProperty.h
BuildObjects.obj: BuildObjects.c BuildObjects.h GetProperty.h \
  InitObjects.h DestroyObjects.h bcg.h BuildObjects.h SetProperty.h
CloneObjects.obj: CloneObjects.c CloneObjects.h GetProperty.h SetProperty.h \
  BuildObjects.h bcg.h
DestroyObjects.obj: DestroyObjects.c DestroyObjects.h GetProperty.h SetProperty.h \
  DrawObjects.h Interaction.h
DrawObjects.obj: DrawObjects.c DrawObjects.h GetProperty.h SetProperty.h \
  bcg.h BuildObjects.h DestroyObjects.h
SetProperty.obj: SetProperty.c SetProperty.h GetProperty.h InitObjects.h \
  bcg.h DrawObjects.h BuildObjects.h
HandleManagement.obj: HandleManagement.c HandleManagement.h GetProperty.h bcg.h
InitObjects.obj: InitObjects.c InitObjects.h GetProperty.h SetProperty.h \
  bcg.h PloEch.h
Interaction.obj: Interaction.c Interaction.h GetProperty.h SetProperty.h \
  DrawObjects.h
Gray.obj: Gray.c Math.h ../machine.h Graphics.h PloEch.h \
  GetProperty.h SetProperty.h DrawObjects.h BuildObjects.h
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
Plo2d2.obj: Plo2d2.c Math.h ../machine.h Graphics.h PloEch.h  GetProperty.h
Plo2d3.obj: Plo2d3.c Math.h ../machine.h Graphics.h PloEch.h
Plo2d4.obj: Plo2d4.c Math.h ../machine.h Graphics.h PloEch.h
Plo2d.obj: Plo2d.c Math.h ../machine.h Graphics.h PloEch.h GetProperty.h DrawObjects.h
Plo2dEch.obj: Plo2dEch.c Math.h ../machine.h Graphics.h PloEch.h \
GetProperty.h SetProperty.h BuildObjects.h
Plo3d.obj: Plo3d.c Math.h ../machine.h Graphics.h PloEch.h \
GetProperty.h DrawObjects.h
Plo3dn.obj : Plo3dn.c Math.h ../machine.h Graphics.h PloEch.h ../stack-c.h ../graphics/Math.h \
GetProperty.h SetProperty.h InitObjects.h DrawObjects.h BuildObjects.h
qsort.obj: qsort.c
Rec.obj: Rec.c Math.h ../machine.h Graphics.h Rec.h PloEch.h
RecLoad.obj: RecLoad.c Math.h ../machine.h Graphics.h Rec.h
RecSave.obj: RecSave.c Math.h ../machine.h Graphics.h Rec.h
Tests.obj: Tests.c Math.h ../machine.h Graphics.h
Xcall1.obj: Xcall1.c Math.h ../machine.h Graphics.h PloEch.h
Xcall.obj: Xcall.c Math.h ../machine.h Graphics.h periX11.h periPos.h \
 periFig.h periGif.h
sciCall.obj: sciCall.c ../stack-c.h ../graphics/Math.h \
  SetProperty.h GetProperty.h DrawObjects.h BuildObjects.h InitObjects.h \
  ../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h PloEch.h
