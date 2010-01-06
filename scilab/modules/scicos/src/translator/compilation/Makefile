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

OCAMLDEP=ocamldep
OCAMLC=ocamlc
OCAMLOPT=ocamlopt
CAMLP4=camlp4
INCLUDES=-I ../parsing
ifeq ($(RELEASE),yes)
OCAMLFLAGS=$(INCLUDES)
else
OCAMLFLAGS=-g $(INCLUDES)
endif
OCAMLOPTFLAGS=$(INCLUDES)
LIBS=
LIBSOPT=$(LIBS:.cma=.cmxa)
SRCS=types.ml nameResolve.ml


OBJS=$(SRCS:.ml=.cmo)
OBJSOPT=$(SRCS:.ml=.cmx)
LIB=compilation.cma
LIBOPT=$(LIB:.cma=.cmxa)

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

%.cmo:	%.ml
	$(OCAMLC) $(OCAMLFLAGS) -c $<

%.cmi:	%.mli
	$(OCAMLC) $(OCAMLFLAGS) -c $<

%.cmx:	%.ml
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -c $<

all: bytecode

opt: $(OBJSOPT)
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -a -o $(LIBOPT) $(LFLAGS) $(LIBSOPT) $(OBJSOPT)

bytecode: $(OBJS)
	$(OCAMLC) $(OCAMLFLAGS) -a -o $(LIB) $(LFLAGS) $(LIBS) $(OBJS)


depend:
	$(OCAMLDEP) $(INCLUDES) $(SRCS:.ml=.mli) $(SRCS) > .depend

-include .depend

clean:
	rm -f *.cmxa
	rm -f *.cm[aiox]
	rm -f *.o
	rm -f *.obj
	rm -f *.a
	rm -f *.lib
	rm -f *~

