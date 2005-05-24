SHELL = /bin/sh
SCIDIR=../..
include ../../Version.incl

LIBRARY = $(SCIDIR)/libs/graphics.a

OBJSC = periX11.o periPos.o periFig.o  periGif.o Xcall.o Xcall1.o \
	GetProperty.o BuildObjects.o CloneObjects.o DestroyObjects.o \
	DrawObjects.o SetProperty.o HandleManagement.o InitObjects.o \
	Interaction.o Interaction.o SetProperty.o \
	Contour.o Plo3d.o Math.o Axes.o Champ.o Plo2d.o \
	Plo2d1.o Plo2d2.o Plo2d3.o Plo2d4.o Plo2dEch.o Rec.o Gray.o \
	Alloc.o FeC.o RecLoad.o RecSave.o Tests.o Actions.o Events.o\
	gsort.o qsort.o nues1.o Format.o w_rottext.o\
	dr1Call.o sciCall.o Plo2dn.o Plo3dn.o

OBJSF = 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS) $(TK_INC_PATH) $(TCL_INC_PATH) 

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

# with gcc compiler, do not inline "floor" function in "Format.c"

Format.o: Format.c
	$(CC) $(CFLAGS)  -c $< -o $@

# do not optimize periX11.c on darwin 

periX11darwin.o: periX11darwin.c
	$(CC) `echo $(CFLAGS) | sed -e "s/-O2//"`  -c $< -o $@

#--- dependencies generated with gcc -MM  ----
Actions.o: Actions.c Math.h ../machine.h Graphics.h \
  ../stack-c.h ../graphics/Math.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h \
  GetProperty.h SetProperty.h DrawObjects.h
Alloc.o: Alloc.c Math.h ../machine.h Graphics.h
Axes.o: Axes.c Math.h ../machine.h Graphics.h PloEch.h GetProperty.h \
  ../stack-c.h ../graphics/Math.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
Champ.o: Champ.c Math.h ../machine.h Graphics.h PloEch.h GetProperty.h \
  SetProperty.h DrawObjects.h BuildObjects.h \
  ../stack-c.h ../graphics/Math.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
Contour.o: Contour.c Math.h ../machine.h Graphics.h PloEch.h GetProperty.h
GetProperty.o: GetProperty.c GetProperty.h PloEch.h bcg.h BuildObjects.h SetProperty.h
BuildObjects.o: BuildObjects.c BuildObjects.h GetProperty.h \
  InitObjects.h DestroyObjects.h bcg.h BuildObjects.h SetProperty.h
CloneObjects.o: CloneObjects.c CloneObjects.h GetProperty.h SetProperty.h \
  BuildObjects.h bcg.h
DestroyObjects.o: DestroyObjects.c DestroyObjects.h GetProperty.h SetProperty.h \
  DrawObjects.h Interaction.h
DrawObjects.o: DrawObjects.c DrawObjects.h GetProperty.h SetProperty.h \
  bcg.h BuildObjects.h DestroyObjects.h
SetProperty.o: SetProperty.c SetProperty.h GetProperty.h InitObjects.h \
  bcg.h DrawObjects.h BuildObjects.h
HandleManagement.o: HandleManagement.c HandleManagement.h GetProperty.h bcg.h
InitObjects.o: InitObjects.c InitObjects.h GetProperty.h SetProperty.h \
  bcg.h PloEch.h
Interaction.o: Interaction.c Interaction.h GetProperty.h SetProperty.h \
  DrawObjects.h
Events.o : Events.c bcg.h
FeC.o: FeC.c Math.h ../machine.h Graphics.h ../stack-c.h \
  GetProperty.h SetProperty.h BuildObjects.h DrawObjects.h \
  ../graphics/Math.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h
Format.o: Format.c Math.h ../machine.h Graphics.h
Gray.o: Gray.c Math.h ../machine.h Graphics.h PloEch.h \
  GetProperty.h SetProperty.h DrawObjects.h BuildObjects.h \
  ../stack-c.h ../graphics/Math.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
Math.o: Math.c Math.h ../machine.h Graphics.h ../sparse/spConfig.h
Plo2d.o: Plo2d.c Math.h ../machine.h Graphics.h PloEch.h \
  ../sparse/spConfig.h GetProperty.h DrawObjects.h ../stack-c.h ../graphics/Math.h \
  ../stack-def.h ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
Plo2d1.o: Plo2d1.c Math.h ../machine.h Graphics.h PloEch.h
Plo2d2.o: Plo2d2.c Math.h ../machine.h Graphics.h PloEch.h GetProperty.h
Plo2d3.o: Plo2d3.c Math.h ../machine.h Graphics.h PloEch.h
Plo2d4.o: Plo2d4.c Math.h ../machine.h Graphics.h PloEch.h
Plo2dEch.o: Plo2dEch.c Math.h ../machine.h Graphics.h PloEch.h \
  GetProperty.h SetProperty.h BuildObjects.h \
  ../stack-c.h ../graphics/Math.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h Interaction.h DrawObjects.h
Plo2dn.o: Plo2dn.c Math.h ../machine.h Graphics.h PloEch.h \
  ../sparse/spConfig.h ../stack-c.h ../graphics/Math.h \
  ../stack-def.h ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h \
  GetProperty.h InitObjects.h DrawObjects.h BuildObjects.h
Plo3dn.o: Plo3dn.c Math.h ../machine.h Graphics.h PloEch.h \
  ../sparse/spConfig.h ../stack-c.h ../graphics/Math.h \
  ../stack-def.h ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h \
  GetProperty.h SetProperty.h InitObjects.h DrawObjects.h BuildObjects.h
Plo3d.o: Plo3d.c Math.h ../machine.h Graphics.h PloEch.h GetProperty.h \
  DrawObjects.h ../stack-c.h ../graphics/Math.h ../stack-def.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
Rec.o: Rec.c Math.h ../machine.h Graphics.h Rec.h PloEch.h
RecLoad.o: RecLoad.c Math.h ../machine.h Graphics.h Rec.h
RecSave.o: RecSave.c Math.h ../machine.h Graphics.h Rec.h
Tests.o: Tests.c Math.h ../machine.h Graphics.h
Xcall.o: Xcall.c Math.h ../machine.h Graphics.h periX11.h periPos.h \
  periFig.h periGif.h
Xcall1.o: Xcall1.c Math.h ../machine.h Graphics.h PloEch.h
dr1Call.o: dr1Call.c ../stack-c.h ../graphics/Math.h ../machine.h \
  ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h
gsort.o: gsort.c Math.h ../machine.h Graphics.h ../sun/men_Sutils.h \
  gsort-int.h gsort-double.h gsort-string.h
nues1.o: nues1.c ../machine.h Math.h Graphics.h
periFig.o: periFig.c Math.h ../machine.h Graphics.h periFig.h color.h \
  bcg.h ../stack-c.h ../graphics/Math.h ../stack-def.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
periGif.o: periGif.c Math.h ../machine.h Graphics.h periGif.h color.h \
  ../gd/gd.h bcg.h ../stack-c.h ../graphics/Math.h \
  ../stack-def.h ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
periGtk.o: periGtk.c Math.h ../machine.h Graphics.h bcg.h \
  ../stack-c.h ../graphics/Math.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h ../version.h color.h
periPos.o: periPos.c Math.h ../machine.h Graphics.h periPos.h color.h \
  bcg.h ../stack-c.h ../graphics/Math.h ../stack-def.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h
periWin.o: periWin.c ../wsci/wresource.h ../wsci/wcommon.h \
  ../graphics/Math.h ../machine.h ../graphics/Graphics.h \
  ../graphics/bcg.h ../stack-c.h ../stack-def.h \
  ../interf/stack1.h ../interf/stack2.h ../interf/stack3.h ../wsci/wadd.h \
  ../wsci/common.h ../wsci/wgraph.h periWin.h ../version.h color.h \
  Graphics.h scigraphic.h
periX11.o: periX11.c Math.h ../machine.h Graphics.h periX11.h \
  ../version.h color.h ../intersci/cerro.h bcg.h ../stack-c.h \
  ../graphics/Math.h ../stack-def.h ../interf/stack1.h ../interf/stack2.h \
  ../interf/stack3.h
qsort.o: qsort.c
sciCall.o: sciCall.c ../stack-c.h ../graphics/Math.h \
  SetProperty.h GetProperty.h DrawObjects.h BuildObjects.h InitObjects.h \
  ../machine.h ../graphics/Graphics.h ../stack-def.h ../interf/stack1.h \
  ../interf/stack2.h ../interf/stack3.h PloEch.h
w_rottext.o: w_rottext.h resources.h
