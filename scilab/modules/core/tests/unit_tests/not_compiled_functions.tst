// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//non compiled functions

deff("y=foo(a)","x=a+1000,stacksize(x),y=stacksize();y=y(1)","n");

a = stacksize();
a = a(1);
y = foo(a);

if y<>a+1000 then pause,end

a = y;
deff("y=toto()","a=stacksize();a=a(1);y=foo(a)","n");
y = toto();

if y<>a+1000 then pause,end
a=y;
exec(toto);
if y<>a+1000 then pause,end
a=y;
