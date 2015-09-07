// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 6440 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6440
//
// <-- Short Description -->
// xcos demos did not work and returned an error "Unable to open file".
//

dataDirP1 = TMPDIR + "/世界您好";
dataDirP2 = TMPDIR + "/世界您好/JOSÉDO~1/";
dataFile = dataDirP2 + "/my_data.sod";

mkdir(dataDirP1);
mkdir(dataDirP2);
if ~isdir(dataDirP2) then pause,end

A = [1 2 3];
B = [4 5 6];

data = [A B];
c1 = pwd();
ierr = execstr("save(dataFile, ''data'');","errcatch");
c2 = pwd();

if ierr <> 0 then pause,end
if c1 <> c2 then pause,end
if ~isfile(dataFile) then pause, end

clear data;

c3 = pwd();
ierr = execstr("load(dataFile);","errcatch");
c4 = pwd();

if ierr <> 0 then pause,end
if c3 <> c4 then pause,end

if ~and(data == [A B]) then pause,end
