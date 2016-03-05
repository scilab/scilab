// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1041 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1041
//
// <-- Short Description -->
// An error inside a loop is reported as if it occurred on the "end" line
clear bxx

txt=['for i=1:10'
     ' a=1; '
     ' a=2;'
     ' a=3;'
     ' a=bxx;'
     ' a=5'
     ' a=6;'
     'end'];

ierr=execstr(txt,'errcatch');
if ierr==0 then pause,end
[str,n,l,func]=lasterror(%t);
if l<>5 then pause,end


deff('foo',txt);
ierr=execstr('foo()','errcatch');
if ierr==0 then pause,end
[str,n,l,func]=lasterror(%t);
if l<>6 then pause,end

deff('foo1',txt)
ierr=execstr('foo1()','errcatch');
if ierr==0 then pause,end
[str,n,l,func]=lasterror(%t);
if l<>6 then pause,end


t1=['if %t then'
    '  '+txt
    'end'];
ierr=execstr(t1,'errcatch');
if ierr==0 then pause,end
[str,n,l,func]=lasterror(%t);
if l<>6 then pause,end

clear foo foo1
deff('foo',t1);
ierr=execstr('foo()','errcatch');
if ierr==0 then pause,end
[str,n,l,func]=lasterror(%t);
if l<>7 then pause,end

deff('foo1',t1);
ierr=execstr('foo1()','errcatch');
if ierr==0 then pause,end
[str,n,l,func]=lasterror(%t);
if l<>7 then pause,end

clear foo foo1
deff('foo','execstr(txt)');
ierr=execstr('foo()','errcatch');
if ierr==0 then pause,end
[str,n,l,func]=lasterror(%t);
if l<>5 then pause,end

deff('foo1','execstr(txt)');
ierr=execstr('foo1()','errcatch');
if ierr==0 then pause,end
[str,n,l,func]=lasterror(%t);
if l<>5 then pause,end
