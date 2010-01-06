#
#   Translator from Modelica 2.x to flat Modelica
# 
#   Copyright (C) 2005 - 2007 Imagine S.A.
#   For more information or commercial use please contact us at www.amesim.com
# 
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation; either version 2
#   of the License, or (at your option) any later version.
# 
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
# 
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
# 
#

EXE=../../translator
OCAMLDEP=ocamldep
OCAMLC=ocamlc
OCAMLOPT=ocamlopt
CAMLP4=camlp4
INCLUDES=-I ../parsing -I ../compilation -I ../instantiation -I ../exceptionHandling

ifeq ($(RELEASE),yes)
OCAMLFLAGS=$(INCLUDES)
else
OCAMLFLAGS=-g $(INCLUDES)
endif

OCAMLOPTFLAGS=$(INCLUDES)
LIBS=../parsing/parsing.cma ../compilation/compilation.cma \
	../instantiation/instantiation.cma ../exceptionHandling/exceptionHandling.cma \
	unix.cma
LIBSOPT=$(LIBS:.cma=.cmxa)

SRCS=codeGeneration.ml libraryManager.ml versiondate.ml


OBJS=$(SRCS:.ml=.cmo)
OBJSOPT=$(SRCS:.ml=.cmx)

# Avoid problems with svn command
LANG:=C
export LANG

SVNLASTCHANGEDATESTR:=$(shell svn info .. | grep "Last Changed Date:")
SVNLASTCHANGEREVSTR:=$(shell svn info .. | grep "Last Changed Rev:")

.SUFFIXES: .ml .mli .cmo .cmi .cmx .mlp .mpi

%.cmo:	%mlp
	$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -impl $< > $(@:.cmo=.tmp)
	$(OCAMLC) $(OCAMLFLAGS) -c -impl $(@:.cmo=.tmp)
	rm -f $(@:.cmo=.tmp)

%.cmi:	%.mpi
	$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -intf $< > $(@:.cmi=.tmp)
	$(OCAMLC) $(OCAMLFLAGS) -c -intf $(@:.cmi=.tmp)
	rm -f $(@:.cmi=.tmp)

%.cmx:	%.mlp
	$(CAMLP4) pa_o.cmo pa_op.cmo pr_o.cmo -impl $< > $(@:.cmx=.tmp)
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -c -impl $(@:.cmx=.tmp)
	rm -f $(@:.cmx=.tmp)

%.cmo:	%.ml %.cmi
	$(OCAMLC) $(OCAMLFLAGS) -c $<

%.cmi:	%.mli
	$(OCAMLC) $(OCAMLFLAGS) -c $<

%.cmx:	%.ml
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -c $<


all: bytecode

opt: $(OBJSOPT)
	$(OCAMLOPT) -verbose $(OCAMLOPTFLAGS) -o $(EXE) $(LFLAGS) $(LIBSOPT) $(OBJSOPT)  translator.ml

bytecode: $(OBJS)
	$(OCAMLC) $(OCAMLFLAGS) -o $(EXE) $(LFLAGS) $(LIBS) $(OBJS)	translator.ml

custom: $(OBJS)
	$(OCAMLC) -verbose -custom $(OCAMLFLAGS) -o $(EXE) $(LFLAGS) $(LIBS) $(OBJS) translator.ml 

depend: version $(SRCS)
	$(OCAMLDEP) $(INCLUDES) $(SRCS:.ml=.mli) $(SRCS) > .depend

versiontarget : 
ifeq ("$(strip $(SVNLASTCHANGEDATESTR))", "")
ifeq ("$(strip $(wildcard versiondate.ml))", "")
	@echo "***********************************************"
	@echo "*** Could not expand the SVN version date *****"
	@echo "*** using the template file as it is      *****"
	@echo "*** The build date will not be correct    *****"
	@echo "***********************************************"
	cp -p versiondate.ml.tmpl versiondate.ml
else
	@echo "***********************************************"
	@echo "*** Reusing existing versiondate.ml      ******"
	@echo "***********************************************"
endif
else
	sed -e '/let/s/AUTO_UPDATE_DATE/$(SVNLASTCHANGEDATESTR)/' -e '/let/s/AUTO_UPDATE_REV/$(SVNLASTCHANGEREVSTR)/' < versiondate.ml.tmpl > versiondate.ml
endif

version:
	$(MAKE) versiontarget

versiondate.ml: $(filter-out versiondate.ml, $(SRCS))
	$(MAKE) versiontarget

-include .depend

clean:
	rm -f *.cmxa
	rm -f *.cm[aiox]
	rm -f *.o
	rm -f *.obj
	rm -f $(EXE)
	rm -f *~

