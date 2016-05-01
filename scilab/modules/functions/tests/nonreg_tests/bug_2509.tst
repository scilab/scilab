// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 2509 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2509
//
// <-- Short Description -->
// execstr forgets to store the function name and line number when in the errcatch mode

prot=funcprot();funcprot(0);


//uncompiled macros
deff("foo","a=aaaa")
if execstr("foo()","errcatch")==0 then pause,end
[str,n,l,f]=lasterror(%t);
if l<>2|f<>"foo" then pause,end

try
    foo();
catch
    [str,n,l,f]=lasterror(%t);
    if l<>2|f<>"foo" then pause,end;
end

if exec(foo,"errcatch")==0 then pause,end
[str,n,l,f]=lasterror(%t);
if l<>2|f<>"foo" then pause,end

//compiled macros
deff("foo","a=aaaa")
if execstr("foo()","errcatch")==0 then pause,end
[str,n,l,f]=lasterror(%t);
if l<>2|f<>"foo" then pause,end

try
    foo();
catch
    [str,n,l,f]=lasterror(%t);
    if l<>2|f<>"foo" then pause,end;
end

if exec(foo,"errcatch")==0 then pause,end
[str,n,l,f]=lasterror(%t);
if l<>2|f<>"foo" then pause,end

//scripts
foo=["a=1";"a=aaaa"];
if execstr(foo,"errcatch")==0 then pause,end
[str,n,l,f]=lasterror(%t);
if l<>2|f<>"" then pause,end

//complex situations
deff("foo","execstr(''a=aaaa'')")
if execstr("foo()","errcatch")==0 then pause,end
[str,n,l,f]=lasterror(%t);
if l<>1|f<>"foo" then pause,end


funcprot(prot);
