// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- MPI TEST -->
//
// This test sends a rand matrix of double complex and, on each slave,
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
        value = ["test","of","MPI","!"]
        MPI_Isend(value, slaveId, 42);
    end

    for slaveId = 1:sizeNodes-1
        tag=0;
        MPI_Irecv(slaveId, tag, 43);
        valueBack=MPI_Wait(43);
        assert_checkequal(size(valueBack), [1,1]);
        assert_checkequal(valueBack, strcat(value))

    end
else
    rankSource=0;
    tag=0;
    MPI_Irecv(rankSource, tag, 42);
    value=MPI_Wait(42)
    value = strcat(value);
    // Isend back to the master
    MPI_Isend(value, 0, 43);

end

MPI_Finalize();
exit()
