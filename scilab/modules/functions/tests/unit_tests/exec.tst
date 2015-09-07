//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"תוכנית"];

str_exec = "disp(''OK'');";

for i = 1 : size(tab_ref,"*")
    mkdir(TMPDIR + filesep() + tab_ref(i));
    fd = mopen(TMPDIR + filesep() + tab_ref(i) + filesep() + tab_ref(i) + ".sce","wt");
    mputl(str_exec,fd);
    mclose(fd);
    exec(TMPDIR + filesep() + tab_ref(i) + filesep() + tab_ref(i) + ".sce");
    mdelete(TMPDIR + filesep() + tab_ref(i) + filesep() + tab_ref(i) + ".sce");
    rmdir(TMPDIR + filesep() + tab_ref(i));
end

// create a script file
mputl("a=1;b=2",TMPDIR+"/myscript");
// execute it
exec(TMPDIR+"/myscript")
if isdef("a")<> %t then pause,end
if isdef("b")<> %t then pause,end
assert_checkequal(a, 1);
assert_checkequal(b, 2);


// create a function
deff("y=foo(x)","a=x+1;y=a^2");
clear a b
// call the function
k = foo(1);
if isdef("k")<> %t then pause,end
if isdef("a")<> %f then pause,end
assert_checkequal(k, 4);
clear k y

// create a function and call this body as a script
deff("script()","a=x+1;y=a^2");
x=4; //create x to make it known by script
exec(script);
if isdef("y")<> %t then pause,end
if isdef("a")<> %t then pause,end
assert_checkequal(y, 25);

// no warning must be displayed when
// exec a file with mclose("all") inside
filename = TMPDIR + filesep() + "testmclose.sce";
mputl("mclose(""all"");",filename);
exec(filename);
