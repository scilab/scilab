// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- MPI TEST 4 -->
//
// This test sends 42 from the master with a broadcast.
// All the nodes (including the server) check if we get the right value
MPI_Init();
rnk =    MPI_Comm_rank();
sizeNodes =    MPI_Comm_size();

assert_checkequal(MPI_Comm_size(), 4);

SLV = rnk;                // handy shortcuts, master is rank 0
Master = ~ SLV;            // slaves are all other
a=-1
if Master
   // Declare a
   a=42
end
b=MPI_Bcast(a, 0);
assert_checkequal(b,42);
MPI_Finalize();

exit

