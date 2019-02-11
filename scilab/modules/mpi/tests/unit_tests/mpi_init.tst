// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- MPI TEST -->

MPI_Init();

if MPI_Comm_rank() <> 0 & MPI_Comm_rank() <> 1 then pause, end
assert_checkequal(MPI_Comm_size(), 2);

assert_checkerror("MPI_Init(1);", msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "MPI_Init", 0));
assert_checkerror("MPI_Comm_rank(1,2);", msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "MPI_Comm_rank", 0, 1));
assert_checkerror("MPI_Comm_size(1,2)", msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "MPI_Comm_size", 0, 1));

MPI_Finalize();
exit()
