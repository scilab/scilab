# This rule creates a `.def' file, which lists the symbols that are exported
# from the DLL.  We use `nm' to get a list of all the exported text (`T')
# symbols.  Note that exporting data symbols -- including uninitialized
# data (`B'), initialized data (`D'), read-only data (`R'), and
# common blocks (`C') -- does not work, so we only grep for symbols whose
# type is `T'.

DEFS= blas.def \
calelm.def \
control.def \
dcd.def \
graphics.def \
integ.def \
interf.def \
intersci.def \
lapack.def \
slicot.def \
libf2c.def \
menusX.def \
metanet.def \
optim.def \
poly.def \
rand.def \
scicos.def \
signal.def \
sound.def \
sparse.def \
sun.def \
system.def \
system2.def \
wsci.def \
xdr.def \
slicot.def \
wintools.def

all : $(DEFS)

%.def: %.lib 
	@echo "	Generation of $@"
	@echo EXPORTS > $@
	@nm $< | grep '^........ [T] _' | sed 's/[^_]*_//' >> $@

poo.def : poo
	cat $< | grep '^........ [T] _' | sed 's/[^_]*_//' >> $@
