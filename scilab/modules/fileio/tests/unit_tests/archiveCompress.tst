// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Kartik Gupta
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// ============================================================================
// Unitary tests for archiveCompress function
// ============================================================================

createdir("archiveCompress1");
createdir("archiveCompress2");

x = archiveCompress('example.tar',["archiveCompress1","archiveCompress2"],"");
assert_checkequal(x,int32(0));
assert_checktrue(isfile("example.tar"));
files = archiveList("example.tar");
assert_checkequal(length(length(files)),2);
assert_checkequal(files(1),"archiveCompress1/");
assert_checkequal(files(2),"archiveCompress2/");
deletefile("example.tar");

x = archiveCompress('example.zip',["archiveCompress1","archiveCompress2"],"Z");
assert_checkequal(x,int32(0));
assert_checktrue(isfile("example.zip"));
files = archiveList("example.zip");
assert_checkequal(length(length(files)),2);
assert_checkequal(files(1),"archiveCompress1/");
assert_checkequal(files(2),"archiveCompress2/");
deletefile("example.zip");

x = archiveCompress('example.tar.bz2',["archiveCompress1","archiveCompress2"],"y");
assert_checkequal(x,int32(0));
assert_checktrue(isfile("example.tar.bz2"));
files = archiveList("example.tar.bz2");
assert_checkequal(length(length(files)),2);
assert_checkequal(files(1),"archiveCompress1/");
assert_checkequal(files(2),"archiveCompress2/");
deletefile("example.tar.bz2");

x = archiveCompress('example.tar.gz',["archiveCompress1","archiveCompress2"],"z");
assert_checkequal(x,int32(0));
assert_checktrue(isfile("example.tar.gz"));
files = archiveList("example.tar.gz");
assert_checkequal(length(length(files)),2);
assert_checkequal(files(1),"archiveCompress1/");
assert_checkequal(files(2),"archiveCompress2/");
deletefile("example.tar.gz");

rmdir("archiveCompress1");
rmdir("archiveCompress2");


