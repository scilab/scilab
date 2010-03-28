README File

1. Description

The Message Passing Interface (MPI) is a language-independent computer communications descriptive application programming interface (API) for message-passing on a parallel computer.

See : http://en.wikipedia.org/wiki/Message_Passing_Interface




mpirun -c 5 ./bin/scilab -nwni -e "MPIHelloWorld()"


2. MPI Functions available into Scilab 
2.1  MPI_Init

2.2 MPI_Comm_size

2.3 MPI_Comm_rank

2.4 MPI_Send

2.5 MPI_Recv

2.6 MPI_Finalize


3. Example
$ mpirun -c 5 scilab -q --eval "MPIPi(2E7,'r')"	# parallel run, 5 ranks

$ mpirun -c 5 scilab -q -e "getf('SCI/modules/mpi/macros/MPIPi_seq.sci'); MPIPi_seq(2E7)"	# parallel run, 5 ranks



=== Serialization ===

The formated style is the following:
| Scilab type | nbrow | nbcol | complex | data |
