// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// =============================================================================
// Unitary tests for mkdir function
// =============================================================================

// TEST 1 : absolute path with one input argument

cd;
test_1_dir = TMPDIR+"/mkdir_test_1";
status_1   = mkdir(test_1_dir);

if status_1 <> 1      then pause,end
if ~isdir(test_1_dir) then pause,end

// TEST 2 : absolute path with 2 input arguments

cd;
test_2_dir = TMPDIR+"/mkdir_test_2";
status_2   = mkdir(TMPDIR,"mkdir_test_2");

if status_2 <> 1      then pause,end
if ~isdir(test_2_dir) then pause,end

// TEST 3 : relative path

cd TMPDIR;
test_3_dir = TMPDIR+"/mkdir_test_3";
status_3   = mkdir("mkdir_test_3");

if status_3 <> 1          then pause,end
if ~isdir(test_3_dir)     then pause,end
if ~isdir("mkdir_test_3") then pause,end

// TEST 4 : relative path with sub-directory

cd TMPDIR;
test_4_dir = TMPDIR+"/mkdir_test_3/mkdir_test_4";
status_4   = mkdir("mkdir_test_3/mkdir_test_4");

if status_4 <> 1                       then pause,end
if ~isdir(test_4_dir)                  then pause,end
if ~isdir("mkdir_test_3/mkdir_test_4") then pause,end

// TEST 5 : Bad Case - The directory already exists

cd;
test_5_dir  =  test_1_dir;
status_5    =  mkdir(test_5_dir);
if status_5 <> 2 then pause,end

// TEST 6 : Bad Case - The file already exists

cd;
test_6_dir  =  TMPDIR+"/mkdir_test_6";

[fd_6,err_6] = mopen(test_6_dir,"wb");
mclose(fd_6);

status_6    =  mkdir(test_6_dir);
if status_6 <> -2 then pause,end

// TEST 7 : mkdir creates in the same instruction one folder and one subfolder.

cd;
test_7_dir  =  TMPDIR+"/mkdir_test_7/mkdir_test_7";
status_7    =  mkdir(test_7_dir);
if status_7 <> 1 then pause,end


removedir(test_1_dir);
removedir(test_2_dir);
removedir(test_3_dir);
removedir(test_4_dir);
removedir(test_5_dir);
removedir(test_6_dir);
removedir(test_7_dir);

// TEST : error messages
try
  mkdir([])
catch
  [str,n]=lasterror();
  if n <> 999 then pause, end
  if str <> msprintf(_("%s: Wrong type for input argument #%d: String expected.\n"), "mkdir", 1) then pause, end
end

try
  mkdir(["a", "b"])
catch
  [str,n]=lasterror();
  if n <> 999 then pause, end
  if str <> msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), "mkdir", 1) then pause, end
end
