ifile=SCIP_ROOT+'/lib/intmpi_'+PVM_ARCH+'.o';
ufiles=[SCIP_ROOT+'/lib/libintmpi_sci_'+PVM_ARCH+'.a', MPI_LIB]
files=[ifile,ufiles]
exec(SCIP_ROOT+'/src-intersci/intmpi.sce');
