#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

SCIDIR=../..
SCIDIR1=..\..

include ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = tkscilib
NAM = SCI/macros/tksci

MACROS = tk_getfile.sci tk_getdir.sci tk_savefile.sci \
	buttondialog.sci demoplay.sci editvar_get.sci winclose.sci browsevar.sci \
	config.sci editvar.sci progressionbar.sci waitbar.sci winlist.sci sciGUIhelp.sci\
	sciGUI_init.sci TK_EvalFile.sci TK_EvalStr.sci TK_GetVar.sci TK_SetVar.sci\
	figure.sci GetUiControlPropertyName.sci IsAScalar.sci GetUiControlValueType.sci\
	uimenu.sci GetUiControlStyleName.sci uicontrol.sci

include ../Make.lib.mak
