//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
mode(7)
//To quit this demo, enter CTRL+C, then type "abort"

if haveacompiler() then
    //           CALLING EXTERNAL ROUTINE

    foo=["void foo(double *a,double *b,double *c)";
    "{ *c = *a + *b; }"  ];

    // we use TMPDIR for compilation

    if ~c_link("foo") then
        path = pwd();
        chdir(TMPDIR);
        mputl(foo,"foo.c");
        ilib_for_link(["foo"],"foo.c",[],"c");
        exec loader.sce
        chdir(path)
    end

    //5+7 by C function
    call("foo",5,1,"d",7,2,"d","out",[1,1],3,"d")
end
