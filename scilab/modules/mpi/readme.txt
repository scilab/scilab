lancer un test unitaire:
mpirun -c 2 ./bin/scilab -nwni -f modules/mpi/tests/unit_tests/mpi_init.tst 

mpirun -c 2 -tag-output <command> will show which nodes display what

mpirun -c 5 ./bin/scilab -nwni -e "MPIHelloWorld()"

$ mpirun -c 5 scilab -q --eval "MPIPi(2E7,'r')"	# parallel run, 5 ranks

$ mpirun -c 5 scilab -q -e "getf('SCI/modules/mpi/macros/MPIPi_seq.sci'); MPIPi_seq(2E7)"	# parallel run, 5 ranks
