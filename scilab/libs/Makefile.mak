
include ../Makefile.incl.mak 

all :: ..\bin\LibScilab.def 


FILES = AAbase.def+blas.def+calelm.def+comm.def+control.def+dcd.def+default.def+fraclab.def+graphics.def+integ.def+interf.def+intersci.def+lapack.def+libcomm.def+libf2c.def+menusX.def+metanet.def+optim.def+poly.def+rand.def+scicos.def+signal.def+sound.def+sparse.def+sun.def+system.def+system2.def+wsci.def+xdr.def+gd.def+int.def

!IF "$(DTK)" == "-DWITH_TK"
FILES1 = $(FILES)+tksci.def 
!ELSE 
FILES1 = $(FILES)
!ENDIF

!IF "$(DPVM)" == "-DWITH_PVM"
FILES2 = $(FILES1) +pvm.def 
!ELSE 
FILES2 = $(FILES1)
!ENDIF


..\bin\LibScilab.def :  *.def 
	@echo "Creation of $*.def"
	@copy $(FILES1)  $*.def  > null


