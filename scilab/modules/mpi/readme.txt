src/c/mappingScilabMPI.c => mettre ‡ jour la gestion des autres types de donnÈes

mpitest_scilab_pack => uses two messages. A first communication to send the meta data (size & type), the second to send / recv the actual data.
=> 

mpitest_scilab_data => use MPI_Type_contiguous + probe + get_element
probably the most interesting stuff


src/c/mappingScilabMPI.c => la serialisation essaye d'etre generique
c'est un peu trop ambitieux

probleme de seg fault: verifier que l'on a pas mpich2 qui se mixe avec openmpi


lancer un test unitaire:
mpirun -c 2 ./bin/scilab -nwni -f modules/mpi/tests/unit_tests/mpi_init.tst 


Serialisation

1) Donn√©es homogenes
* matrices:
	- double
	- boolean
	- int
	- string
type
dimension
contenu

Utilisation de MPI_Type_contiguous

2) Heterogenes



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
