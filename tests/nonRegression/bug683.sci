// Non-regression test file for bug 683
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear;

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

getf(SCIFILE1);
getf(SCIFILE2);

// Exec functions
[ce1,ce2] = test_cell1(23);
[ce3,ce4] = test_cell1([1 2]);
ce5=test_cell2();

// Verify results
correct1=and(ce1==mlist(["ce","dims","entries"],int32([1 3]),list(1,23,"c")))
affich_result(correct1,681.1);

correct2=and(ce2==mlist(["ce","dims","entries"],int32([1 3]),list(1,2,"c")))
affich_result(correct2,681.2);

correct3=and(ce3==mlist(["ce","dims","entries"],int32([1 3]),list(1,[1 2],"c")))
affich_result(correct3,681.3);

correct4=and(ce4==mlist(["ce","dims","entries"],int32([1 3]),list(1,2,"c")))
affich_result(correct4,681.4);

correct5=and(ce5==mlist(["ce","dims","entries"],int32([1 3]),list(1,2,"we")))
affich_result(correct5,681.5);

correct=correct1&correct2&correct3&correct4&correct5
affich_result(correct,681);

clear
