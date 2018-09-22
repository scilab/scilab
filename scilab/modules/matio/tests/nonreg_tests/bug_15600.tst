// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

//
// <-- Non-regression test for bug 15600 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15600
//
// <-- Short Description -->
// savematfile(filename) included many environment variables and variables
//  of unsupported types.

// Preparing data of various types
// -------------------------------
clear
b = %t;                         // Skipped : http://bugzilla.scilab.org/15568
i = int8((rand(2,3)-0.5)*200);
k = uint16(rand(1,4)*100);
d = %pi;
c = 1-%i;
h = rand(1,3,2);
t = "text";
L = list(%t, "abc");            // Ignored : http://bugzilla.scilab.org/15729
p = %z;                         // Ignored
rational = %z/(1-%z);           // Ignored
Sparse = sprand(10,10,0.05);
SparseB = Sparse<0.5;           // Ignored
Struct = struct("age",30, "type","software");
Cell = {1-%i, int8(-5); rand(2,3), "Abc"};

// Saving the environment
// ----------------------
savematfile(TMPDIR+"/tmp.mat");
savematfile(TMPDIR+"/tmp_73.mat","-v7.3");

// Testing the actually saved set of variables
// -------------------------------------------
// version 7
File = TMPDIR+"/tmp.mat";
fd = matfile_open(File, "r");
[names, classes, types] = matfile_listvar(fd);
matfile_close(fd);
ref = ["Cell" "Sparse" "Struct" "c" "d" "h" "i" "k" "t"]';
assert_checkequal(names, ref);
mdelete(File);

// version 7.3
File = TMPDIR+"/tmp_73.mat";
fd = matfile_open(File, "r");
[names, classes, types] = matfile_listvar(fd);
matfile_close(fd);
ref = ["Cell" "Sparse" "Struct" "c" "d" "h" "i" "k" "t"]';
assert_checkequal(names, ref);
mdelete(File);

// ==========================================
File = TMPDIR+"/tmp.mat";

// When the output file is actually empty, it is now not created
// -------------------------------------------------------------
savematfile(File, "%z", "-v7")
assert_checkfalse(isfile(File));

// Some input variables were corrupted by savematfile() internal values before being saved
// ---------------------------------------------------------------------------------------
// -v4: Saving the variables flag, head, it, it1, mn, v, x, x1, M, O, P, T, MOPT
V = rand(1);
[flag, head, it, x, x1, M, O, P, T, MOPT] = (V,V,V,V,V,V,V,V,V,V);
// Uncomment after fixing http://bugzilla.scilab.org/15731
//it1 = sparse(V);
//[mn, v, it10, mn0, v0] = (it1, it1, it1, it1, it1);
[it1, mn, v] = (V,V,V);

vnames = list("flag","head","it","it1","mn","v","x","x1","M","O","P","T","MOPT");
savematfile(File,"-v4", vnames(:));
clear(vnames(:))
loadmatfile(File);
for n = vnames
    assert_checktrue(isdef(n,"l"));
    y = evstr(n);
    yref = V;
    if issparse(y), yref = sparse(V), end
    assert_checkequal(y, yref);
end
mdelete(File);

// -v6 | 7|7.3: Saving the "vars" variable
vars = rand(2,3);
varsRef = vars;
for v = ["-v6" "-v7" "-v7.3"]
    vars = varsRef;
    savematfile(File,"vars", v);
    sleep(1000)
    clear vars
    loadmatfile(File);
    assert_checktrue(isdef("vars","l"));
    assert_checkequal(vars, varsRef);
    mdelete(File);
end

// Encoded integers were not saved in v4 format
// --------------------------------------------
T = ["int8" "uint8" "int16" "uint16" "int32" "uint32" "int64" "uint64"];
bm = list(int8(-%inf), 0, int16(-%inf), 0, -1e9, 0, -1e9, 0);
bp = list(int8(%inf), uint8(%inf), int16(%inf), uint16(%inf), -1e9, 1e9, -1e9, 1e9);
S.s = "1,1";
S.v = "1,3";
S.m = "2,3";
for i = 1:8
    for f = ["s" "v" "m"]
        tmp = T(i)+f+" = "+T(i)+"(grand("+S(f)+",""uin"",double(bm(i)),double(bp(i)))); "+..
              T(i)+f+"0 = "+T(i)+f+";"; // Reference copy
        execstr(tmp);
    end
end
vnames = ["int8s" "uint8s" "int8v" "uint8v" "int8m" "uint8m" ..
          "int16s" "uint16s" "int16v" "uint16v" "int16m" "uint16m" ..
          "int32s" "uint32s" "int32v" "uint32v" "int32m" "uint32m" ..
          "int64s" "uint64s" "int64v" "uint64v" "int64m" "uint64m"
          ];
savematfile(File,"-v4", vnames);
clear(vnames)
loadmatfile(File);  // variables are well stored as the given inttype,
                    // but are reloaded as doubles!
for v = vnames
    assert_checktrue(isdef(v,"l"));
    assert_checkequal(evstr(v), double(evstr(v+"0")));
end
