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
rnk =    MPI_Comm_rank();
sizeNodes =    MPI_Comm_size();

assert_checkequal(MPI_Comm_size(), 2);

SLV = rnk;                // handy shortcuts, master is rank 0
Master = ~ SLV;            // slaves are all other

if Master

    for slaveId = 1:sizeNodes-1
        dense=[ %T, %F, %T, %F
        %F, %T, %F, %T
        %T, %F, %T, %F
        %F, %T, %F, %T];
        value = sparse(dense);
        MPI_Send(value, slaveId)
    end

    for slaveId = 1:sizeNodes-1
        tag=0
        valueBack=MPI_Recv(slaveId, tag);
        assert_checkequal(valueBack, ~value);
    end
else
    rankSource=0;
    tag=0;
    value=MPI_Recv(rankSource, tag)
    value = ~value;

    // Send back to the master
    MPI_Send(value,0)

end

MPI_Finalize()
exit();
