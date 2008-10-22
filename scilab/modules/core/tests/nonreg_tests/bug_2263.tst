// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2263 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2263
//
// <-- Short Description -->
//    Suppose you want to call a arbitrary scilab function "test". But you want the
//    outputs from this function be stored in a matlab-like struct, for example,
//    [out.t1,out.t2,out.t3,out.t4]=test(1)
//    This works fine, but if you have a nested structure (out.a.XXX) than scilab is
//    not able to provide the outputs at the correct places, for example
//    [out.a.t1,out.a.t2,out.a.t3,out.a.t4]=test(1).
//
//
//    The function "test" could be:
//    function [a,b,c,d,e]=teste(in)
//      a=in;
//      b=in+1;
//      c=in+2;
//      d=in+3;
//      e=in+4;
//    endfunction

function [a,b,c,d,e]=test()
  a=1;b=2;c=3;d=4;e=5;
endfunction

[out.a.t1,out.a.t2,out.a.t3,out.a.t4]=test();

if or(out<>mlist(["st","dims","a"],int32([1,1]),..
		   mlist(["st","dims","t4","t3","t2","t1"],int32([1,1]),4,3,2,1))) then pause,end

clear out
[out.a.t1,t2,out.a.t3,t4]=test();
if or(out<>mlist(["st","dims","a"],int32([1,1]),..
		   mlist(["st","dims","t3","t1"],int32([1,1]),3,1))) then pause,end
if t2<>2 then pause,end
if t4<>4 then pause,end

clear a
[a.t1,a.t2,a.t3,a.t4]=test();
if or(a<>mlist(["st","dims","t4","t3","t2","t1"],int32([1,1]),4,3,2,1)) then pause,end

clear a
[t1,a.t2,t3,a.t4]=test();
if or(a<>mlist(["st","dims","t4","t2"],int32([1,1]),4,2)) then pause,end
if t1<>1 then pause,end
if t3<>3 then pause,end

[t1,t2,t3,t4]=test();
if or([t1 t2 t3 t4]<>[1 2 3 4]) then pause,end
