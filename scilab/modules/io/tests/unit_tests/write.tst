// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//

path_double = TMPDIR + "/save_double";
path_int = TMPDIR + "/save_int";
path_int2 = TMPDIR + "/save_int2";
path_int3 = TMPDIR + "/save_int3";
path_int4 = TMPDIR + "/save_int4";
path_logic = TMPDIR + "/save_logic";
path_string = TMPDIR + "/save_string";

rand("seed",0);
A=rand(5,3);

write(6,A,"("" | "",3(f10.3,"" | ""))")
write(6,rand(5,7)*10)
write(6,round(rand(5,7)))
write(6,int(rand(5,7)*10))
write(6,"aaa")
write(6,["aaa","bbb";"ccccc","read me"])
write(6,int(rand(5,7)*10),"("" | "",7(f10.3,"" | ""))")
//pass from  scilab 6.0
write(6,int32(rand(5,7)*10))

write(path_int,int(rand(5,7)*10));
fd = mopen(path_int, "rt");
solution = [
"   1.00000000000000        8.00000000000000        2.00000000000000     ";
"   6.00000000000000        1.00000000000000       0.000000000000000E+000";
"   9.00000000000000     ";
"   9.00000000000000        7.00000000000000        9.00000000000000     ";
"   9.00000000000000        8.00000000000000        8.00000000000000     ";
"   3.00000000000000     ";
"   6.00000000000000        1.00000000000000        8.00000000000000     ";
"  0.000000000000000E+000   3.00000000000000        5.00000000000000     ";
"   1.00000000000000     ";
"   2.00000000000000        7.00000000000000        2.00000000000000     ";
"   2.00000000000000        5.00000000000000        2.00000000000000     ";
"   9.00000000000000     ";
"   5.00000000000000        3.00000000000000        8.00000000000000     ";
"   7.00000000000000        5.00000000000000        9.00000000000000     ";
"   7.00000000000000     "];
result = mgetl(fd);
assert_checkequal(result, solution);
mclose(fd);

mdelete(path_int);

write(path_double,A);
fd = mopen(path_double, "rt");
solution = [
"  0.211324865464121       0.628391788341105       0.560848606284708     ";
"  0.756043854169548       0.849745235871524       0.662356937304139     ";
"  2.211346291005611E-004  0.685731019824743       0.726350676734000     ";
"  0.330327091738582       0.878216481301934       0.198514384217560     ";
"  0.665381104219705       6.837403681129217E-002  0.544257316272706     "];
result = mgetl(fd);
assert_checkequal(result, solution);
mclose(fd);

refMsg = msprintf(_("File ""%s"" already exists or directory write access denied.\n"), path_double);
assert_checkerror("write(path_double,rand(5,7));", refMsg);

mdelete(path_double);

refMsg = msprintf(_("%s: Wrong type for input argument #%d : A string expected.\n"), "write", 2);
assert_checkerror("write(path_logic,[%t, %f; %f, %f]);", refMsg);

mdelete(path_logic);

write(path_string,"aaa");
fd = mopen(path_string, "rt");
solution = "aaa";
result = mgetl(fd);
assert_checkequal(result, solution);
mclose(fd);

mdelete(path_string);

write(path_string,["aaa","bbb";"cccc","scilab"]);
fd = mopen(path_string, "rt");
solution = ["aaa";"cccc";"bbb";"scilab"];
result = mgetl(fd);
assert_checkequal(result, solution);
mclose(fd);

mdelete(path_string);

write(path_int3,1:10,"(10(i))");
fd = mopen(path_int3, "rt");
solution = "           1           2           3           4           5           6           7           8           9          10";
result = mgetl(fd);
assert_checkequal(result, solution);
mclose(fd);

mdelete(path_int3);

write(path_int4,round(rand(5,7)*10),"(10(i))");
fd = mopen(path_int4, "rt");
solution = [
"           3           4           0           8           0           1           9";
"           6           1           3           1           9           1           1";
"           4           3           2           9           8           5           4";
"          10           1           2           5           4           4           9";
"           3           8           3           3           8           6           0"];
result = mgetl(fd);
assert_checkequal(result, solution);
mclose(fd);

mdelete(path_int4);
