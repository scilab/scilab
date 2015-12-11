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

if execstr("MPI_Init(1);", "errcatch") <> 77 then pause,end
if execstr("MPI_Comm_rank(1);", "errcatch") <> 77 then pause,end
// if execstr("MPI_Comm_size(1,2);", "errcatch") <> 77 then pause,end

MPI_Finalize();
exit()
