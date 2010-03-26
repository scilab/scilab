
a=[23, 23; 42, 42];
file=TMPDIR+"/test"
MPI_serialize(file,a);
a=MPI_unserialize(file);

