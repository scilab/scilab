SHELL = /bin/sh

SCIDIR=../../..
SCIDIR1=..\..\..

include ../../../Makefile.incl

.SUFFIXES: .sci .bin $(SUFFIXES)

NAME = m2spercentlib
NAM = SCI/macros/m2sci/percent

OP = %02sci.sci %52sci.sci %a2sci.sci %cc2sci.sci %d2sci.sci %e2sci.sci %g2sci.sci \
     %h2sci.sci %i2sci_c.sci %i2sci_g.sci %i2sci_r.sci %i2sci.sci %i2sci_s.sci \
     %imp2sci.sci %j2sci.sci %l2sci.sci %log2sci.sci %m2sci.sci %p2sci.sci \
     %q2sci.sci %r2sci.sci %rc2sci.sci %s2sci.sci %t2sci.sci %x2sci.sci

STRUCT =  %i_st2sci.sci %e_st2sci.sci

CELL = %cc_cell2sci.sci %rc_cell2sci.sci %i_ce2sci.sci %e_ce2sci.sci

UTIL = get_contents_infer.sci

MACROS = $(OP) $(CELL) $(UTIL) $(STRUCT) 


include ../../Make.lib
