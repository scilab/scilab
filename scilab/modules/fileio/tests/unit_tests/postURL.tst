// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Kartik Gupta
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// ============================================================================
// Unitary tests for postURL function
// ============================================================================

filename = postURL("http://researchweb.iiit.ac.in/~kartik.gupta/index.php",["var1","var2"],["test1","test2"],"test.txt")
assert_checktrue(isfile("test.txt"));
fd = mopen("test.txt");
assert_checktrue(fd>0);
line = mgetl(fd);
words = strsplit(line," ");
assert_checkequal(length(length(words)),2);
assert_checkequal(words(1),"test1");
assert_checkequal(words(2),"test2");
deletefile("test.txt");

assert_checkerror("postURL(''http://researchweb.iiit.ac.in/~kartik.gupta/index.php'',[''var1'',''var2''],[''test1'']);",[],999);

assert_checkerror("postURL(''http://researchweb.iiit.ac.in/~kartik.gupta/index.php'',[''var1'',''var2''],''test1'');",[],999);

assert_checkerror("postURL(''http://researchweb.iiit.ac.in/~kartik.gupta/index.php'',[''var2''],[''test1'',''test2'']);",[],999);
