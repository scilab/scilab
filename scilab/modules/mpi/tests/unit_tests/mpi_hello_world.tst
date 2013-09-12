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
assert_checktrue(MPI_Comm_rank() == 0 | MPI_Comm_rank() == 1);
assert_checkequal(MPI_Comm_size(), 2);

name =  MPI_Get_processor_name();
assert_checkequal(size(name), [1, 1]);
assert_checktrue(length(name)>0);

MPI_Finalize();
exit();