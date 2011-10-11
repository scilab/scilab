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
        value = ["test","of","MPI","!"]
		MPI_Send(value, slaveId)
	end

	for slaveId = 1:sizeNodes-1
		tag=0
		valueBack=MPI_Recv(slaveId, tag);
		if size(valueBack) <> [1,1] | valueBack <> strcat(value) then disp("Failed (expected scalar string): " + valueBack);
           pause
        else
            disp("Node " + string(slaveId) + ": OK")
        end
	end
else
	disp("SLAVE: Processor "+string(rnk))
	rankSource=0;
	tag=0;
    value=MPI_Recv(rankSource, tag)
    value = strcat(value);
    disp(value)
	// Send back to the master
	MPI_Send(value,0)

end

MPI_Finalize()
exit;
