// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5588 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5588
//
// <-- Short Description -->
// null() of a tlist() field shifts all remaining values with respect to
// field names.

// tlist null assignment indexed by a name
clear  %0_i_a %0_i_A
t=tlist(["a","b","c"],10,"foo");
if execstr("t.b=null()","errcatch")==0 then pause,end
function t=%0_i_a(i,void,t)
    f=getfield(1,t);
    k=find(f==i);
    if k<>[] then
        f(k)=[];
        t = setfield(k,null(),t);
        t = setfield(1,f,t);
    end
endfunction
if execstr("t.b=null()","errcatch")<>0 then pause,end
if or(t<>tlist(["a","c"],"foo"))  then pause,end

// tlist null assignment indexed by a number
t=tlist(["a","b","c"],10,"foo");
if execstr("t(2)=null()","errcatch")<>0 then pause,end
if or(t<>tlist(["a","b","c"],"foo"))  then pause,end

//struct null assignment indexed by a name
T.x="foo";T.y=33;
if execstr("size(T)=null()","errcatch")==0 then pause,end
if execstr("T(1,1)=null()","errcatch")==0 then pause,end

if execstr("T.x=null()","errcatch")<>0 then pause,end
T1.y=33;
if or(T<>T1) then pause,end
T1.x="foo";
if execstr("T(1)=null()","errcatch")==0 then pause,end


//cell null assignment
c=makecell([1,2],1,"xyz");
if execstr("c(1,1)=null()","errcatch")==0 then pause,end
if execstr("size(c)=null()","errcatch")==0 then pause,end

//sublist null assigment indexed by a name
L=tlist(["foo","x","y"],mlist(["A","b","c"],10,"zoo"),1:5);
if execstr("L.x.b=null()","errcatch")==0 then pause,end
%0_i_A=%0_i_a;
if execstr("L.x.b=null()","errcatch")<>0 then pause,end
if or(L<>tlist(["foo","x","y"],mlist(["A","c"],"zoo"),1:5)) then pause,end

//sublist null assigment indexed by a number
clear %0_i_A
L=mlist(["foo","x","y"],tlist(["A","b","c"],10,"zoo"),1:5);
if execstr("L.x(2)=null()","errcatch")<>0 then pause,end
if or(L<>mlist(["foo","x","y"],tlist(["A","b","c"],"zoo"),1:5)) then pause,end

//try a more complex context
L=tlist(["foo","x","y"],list(33,mlist(["A","b","c"],10,"zoo")),1:5);
%0_i_A=%0_i_a;
if execstr("L.x(2).b=null()","errcatch")<>0 then pause,end

if or(L<>tlist(["foo","x","y"],list(33,mlist(["A","c"],"zoo")),1:5)) then pause,end
