// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//

a=[23, 23; 42, 42];
myFile=TMPDIR+"/test"
MPI_serialize(myFile,a);
a2=MPI_unserialize(myFile);

if a<>a2 then pause, end

clear a,a2;

a=23;
myFile=TMPDIR+"/test"
MPI_serialize(myFile,a);
a2=MPI_unserialize(myFile);

if a<>a2 then pause, end


clear a,a2;

a=rand(100,100);
myFile=TMPDIR+"/test"
MPI_serialize(myFile,a);
a2=MPI_unserialize(myFile);

if a<>a2 then pause, end
