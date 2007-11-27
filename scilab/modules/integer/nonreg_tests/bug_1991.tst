// <-- Non-regression test for bug 1991 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1991
//
// <-- Short Description -->
//    [a,b]=(3,int32(5))
//                      !--error 78 
//    convert: wrong number of lhs arguments

// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 15 mai 2006

[a,b]  = (3,int32(5));
if a<>3        then pause,end
if b<>int32(5) then pause,end

a=(3 + int32(5));
if a<>int32(8) then pause,end

[a,b,c]=(3,(2),sin(5));
if a<>3        then pause,end
if b<>2        then pause,end
if c<>sin(5)   then pause,end
