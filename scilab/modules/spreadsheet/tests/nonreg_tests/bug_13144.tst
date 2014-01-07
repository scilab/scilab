// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13146 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13146
//
// <-- Short Description -->
// csvRead could not ignore header comments.
// It is now possible thanks to a eighth input argument 'header'.

comments = [
"// Copyright (C) INRIA"
"// This file must be used under the terms of the CeCILL." ];
filename = fullfile(TMPDIR , "foo.csv");
rand("seed", 0);
csvWrite(rand(2,3), filename, ascii(9), ",", [], comments);

header = 2;
[M, c] = csvRead(filename, ascii(9), ",", "double", [], [], [], header); // Ignore the two first lines (the header)

refM = [
0.211324865464121 0.000221134629101 0.665381104219705
0.756043854169548 0.330327091738582 0.628391788341105 ];
refC = "";

assert_checkalmostequal(M, refM);
assert_checkequal(c, refC);


// Case when csvRead has two output arguments but no 'regexpcomments' argument.
// This test case comes from the help page:
M = 1:50;
filename = fullfile(TMPDIR, "data.csv");
csvWrite(M, filename, ascii(9), ".");

// Read csv file
[M1, c] = csvRead(filename,ascii(9), [], "string");

refM1 = string(1:50);
refC = "";

assert_checkequal(M1, refM1);
assert_checkequal(c, refC);

