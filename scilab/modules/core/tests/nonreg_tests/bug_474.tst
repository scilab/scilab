//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 474 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=474
//
// <-- Short Description -->
//    Bug Report Id: 120442003624174827
//    k should be a 2 element vector!! like
//
//    -->a=matrix(1:9,3,3); [m,k]=min(a)       
//     k  =
//
//    !   1.    1. !
//     m  =
//
//        1.  
//
//    -->a=matrix(1:9,3,3); [m,k]=max(a)
//     k  =
//
//    !   3.    3. !
//     m  =
//
//        9.
//
//
// ...

//correct=%F
a=matrix(1:9,3,3)
[mia,kia]=min(a)
[maa,kaa]=max(a)

b=int16(matrix(1:9,3,3))
[mib,kib]=min(b)
[mab,kab]=max(b)

if mia<>mib     then pause,end
if or(kia<>kib) then pause,end
if maa<>mab     then pause,end
if or(kaa<>kab) then pause,end
