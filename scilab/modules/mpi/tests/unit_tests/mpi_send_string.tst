// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- MPI TEST -->
//

MPI_Init();
rnk =    MPI_Comm_rank();
sizeNodes =    MPI_Comm_size();

assert_checkequal(MPI_Comm_size(), 2);

SLV = rnk;                // handy shortcuts, master is rank 0
Master = ~ SLV;            // slaves are all other

if Master

    disp("MASTER: We have "+string(sizeNodes) + " processors")
    for slaveId = 1:sizeNodes-1
        value = ["test","of","MPI","!"]
        MPI_Send(value, slaveId)
    end

    for slaveId = 1:sizeNodes-1
        tag=0
        valueBack=MPI_Recv(slaveId, tag);
        assert_checkequal(size(valueBack), [1,1]);
        assert_checkequal(valueBack, strcat(value))
    end
else
    disp("SLAVE: Processor "+string(rnk))
    rankSource=0;
    tag=0;
    value=MPI_Recv(rankSource, tag)
    value = strcat(value);
    // Send back to the master
    MPI_Send(value,0)
end

MPI_Finalize()
exit();