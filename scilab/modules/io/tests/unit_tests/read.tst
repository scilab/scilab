//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//

path_size = TMPDIR + "/save_size";
path_double = TMPDIR + "/save_double";
path_int = TMPDIR + "/save_int";
path_string = TMPDIR + "/save_string";

rand("seed",0);

double = rand(5,7);
write(path_double,double);
result = read(path_double, 5, 7);
assert_checkalmostequal(result, double,[],%eps);

mdelete(path_double);

double = rand(5,5);
write(path_double,double);

result = read(path_double,-1,4);
assert_checkalmostequal(result, double(:,1:4),[],%eps);

mdelete(path_double);

double = rand(5,3);

write(path_double,double,"("" | "",3(f10.3,"" | ""))");
result = read(path_double,5,3,"(3x, 3(f10.3, 3x))");
assert_checkalmostequal(result, round(double*1000)/1000);

mdelete(path_double);

test_size=rand(2,2);

write(path_size,test_size);
result = read(path_size,1,1);
assert_checkalmostequal(result, test_size(1),[],%eps);
result = read(path_size,1,2);
assert_checkalmostequal(result, test_size(1,1:2),[],%eps);
result = read(path_size,2,1);
assert_checkalmostequal(result, test_size(1:2,1),[],%eps);
result = read(path_size,2,2);
assert_checkalmostequal(result, test_size,[],%eps);


refMsg = msprintf(_("Incorrect file or format.\n"));
assert_checkerror("read(path_size,3,2);", refMsg);
assert_checkerror("read(path_size,2,3);", refMsg);
assert_checkerror("read(path_size,3,3);", refMsg);

mdelete(path_size);

write(path_int,1:10,"(10(i3))");
result = read(path_int,1,10,"(10(i3))");
assert_checkequal(result, int32(1:10));
assert_checkerror("read(path_int,10,1,""(10(i3))"");",refMsg);

result = read(path_int,-1,1,"(10(i3))");
assert_checkequal(result, int32(1));

mdelete(path_int);

text = "Scilab";
write(path_string,text,"(a)");
result = read(path_string,1,1,"(a)");
assert_checkequal(result, text);

mdelete(path_string);
