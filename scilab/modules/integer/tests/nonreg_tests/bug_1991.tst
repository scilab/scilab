//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1991 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1991
//
// <-- Short Description -->
//    [a,b]=(3,int32(5))
//                      !--error 78
//    convert: wrong number of lhs arguments

[a,b]  = (3,int32(5));
if a<>3        then pause,end
if b<>int32(5) then pause,end

a=(3 + int32(5));
if a<>int32(8) then pause,end

[a,b,c]=(3,(2),sin(5));
if a<>3        then pause,end
if b<>2        then pause,end
if c<>sin(5)   then pause,end
