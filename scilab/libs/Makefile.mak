
include ../Makefile.incl.mak 

all :: ..\bin\LibScilab.def 


FILES = AAbase.def+arpack.def+blas.def+calelm.def+console.def+control.def+dcd.def+default.def+gd.def+graphics.def+int.def+integ.def+interf.def+intersci.def+lapack.def+libf2c.def+menusX.def+metanet.def+optim.def+poly.def+rand.def+scicos.def+signal.def+slicot.def+fileio.def+sparse.def+os_specific.def+system.def+system2.def+wsci.def+xdr.def+wintools.def


!IF "$(DTK)" == "-DWITH_TK"
FILES1 = $(FILES)+tclsci.def
!ELSE 
FILES1 = $(FILES)
!ENDIF

!IF "$(DPVM)" == "-DWITH_PVM"
FILES2 = $(FILES1)+pvm.def 
!ELSE 
FILES2 = $(FILES1)
!ENDIF


..\bin\LibScilab.def :  *.def 
	@echo "Creation of $*.def"
	@copy $(FILES1)  $*.def  > null

clean::
	-del arpack.def
	-del blas.def
	-del calelm.def
	-del console.def
	-del control.def
	-del dcd.def
	-del default.def
	-del gd.def
	-del graphics.def
	-del int.def
	-del integ.def
	-del interf.def
	-del intersci.def
	-del lapack.def
	-del libf2c.def
	-del menusX.def
	-del metanet.def
	-del optim.def
	-del poly.def
	-del rand.def
	-del scicos.def
	-del signal.def
	-del slicot.def
	-del fieio.def
	-del sparse.def
	-del os_specific.def
	-del system.def
	-del system2.def
	-del wsci.def
	-del xdr.def
	-del pvm.def 
	-del tclsci.def
	-del wintools.def 
	-del *.lib
	-del ..\bin\libscilab.def
	-del ..\bin\libscilab.dll
	-del ..\bin\libscilab.lib


distclean::
	-del arpack.def
	-del blas.def
	-del calelm.def
	-del console.def
	-del control.def
	-del dcd.def
	-del default.def
	-del gd.def
	-del graphics.def
	-del int.def
	-del integ.def
	-del interf.def
	-del intersci.def
	-del lapack.def
	-del libf2c.def
	-del menusX.def
	-del metanet.def
	-del optim.def
	-del poly.def
	-del rand.def
	-del scicos.def
	-del signal.def
	-del slicot.def
	-del fileio.def
	-del sparse.def
	-del os_specific.def
	-del system.def
	-del system2.def
	-del wsci.def
	-del xdr.def
	-del pvm.def 
	-del tclsci.def
	-del wintools.def 
	-del *.lib
	

