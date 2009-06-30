// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

//funcprot

deff("foo()","a=1");

funcprot(0);
if funcprot() <> 0 then pause,end
if execstr("deff(""foo()"",""a=1"");","errcatch") <> 0 then pause,end
deff("foo()","a=2");

funcprot(1);
if funcprot() <> 1 then pause,end
if execstr("deff(""foo()"",""a=3"");","errcatch") <> 0 then pause,end
deff("foo()","a=4")

funcprot(2);
if funcprot() <> 2 then pause,end
if execstr("deff(""foo()"",""a=5"");","errcatch") == 0 then pause,end
