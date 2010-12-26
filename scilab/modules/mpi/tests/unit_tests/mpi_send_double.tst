// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- MPI TEST -->
// This test sends a matrix of double [42,41] and, on each slave,
// it adds +1 to each element
// and send it back to the master
// 
MPI_Init();
rnk =	MPI_Comm_rank();
sizeNodes =	MPI_Comm_size();

if MPI_Comm_size() <> 2 then pause, end

SLV = rnk;				// handy shortcuts, master is rank 0
Master = ~ SLV;			// slaves are all other

if Master

	disp("MASTER: We have "+string(sizeNodes) + " processors")
	for slaveId = 1:sizeNodes-1
		MPI_Send([42,41],slaveId)
		disp("MASTER: ICI after mpi_send");
	end

	for slaveId = 1:sizeNodes-1
		tag=0
		disp("MASTER: mpi_recv")
	    valueBack=MPI_Recv(slaveId, tag);
		if valueBack(1) <> 43 & valueBack(2) <> 42 then pause, end

	end
else
	disp("SLAVE: Processor "+string(rnk))
	rankSource=0;
	tag=0;
    value=MPI_Recv(rankSource, tag)
	disp("SLAVE:"+string(value));
	value=value+1;
	// Send back to the master
	MPI_Send(value,0)

end

MPI_Finalize()
