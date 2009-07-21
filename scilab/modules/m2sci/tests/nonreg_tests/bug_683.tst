// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 683 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=683
//
// <-- Short Description -->
//    problem with makecell : when a makecell input is an input of
//    the main function, makecell doesn't give this input value to
//    the created cell. Strange behavior...

// Create 2 M-files
MFUN1TXT=["function [a,c] = test_cell1(b)";
"a = {1,b,''c''};";
"c = {1,2,''c''};"]

MFUN2TXT=["function a = test_cell2()";
"e = ''we'';";
"a = {1,2,e};"]

MFILE1=TMPDIR+"/test_cell1.m";
MFILE2=TMPDIR+"/test_cell2.m";

fd=mopen(MFILE1,"w");
mputl(MFUN1TXT,fd);
mclose(fd);

fd=mopen(MFILE2,"w");
mputl(MFUN2TXT,fd);
mclose(fd);

// Convert M-files to Scilab
mfile2sci(MFILE1,TMPDIR);
mfile2sci(MFILE2,TMPDIR);

// Load functions into Scilab
SCIFILE1=TMPDIR+"/test_cell1.sci";
SCIFILE2=TMPDIR+"/test_cell2.sci";

exec(SCIFILE1);
exec(SCIFILE2);

// Exec functions
[ce1,ce2] = test_cell1(23);
[ce3,ce4] = test_cell1([1 2]);
ce5=test_cell2();

// Verify results

if or(ce1<>mlist(["ce","dims","entries"],int32([1 3]),list(1,23,"c")))    then pause,end
if or(ce2<>mlist(["ce","dims","entries"],int32([1 3]),list(1,2,"c")))     then pause,end
if or(ce3<>mlist(["ce","dims","entries"],int32([1 3]),list(1,[1 2],"c"))) then pause,end
if or(ce4<>mlist(["ce","dims","entries"],int32([1 3]),list(1,2,"c")))     then pause,end
if or(ce5<>mlist(["ce","dims","entries"],int32([1 3]),list(1,2,"we")))    then pause,end
