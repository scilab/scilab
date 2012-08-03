// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

// unit tests for fieldnames function
// =============================================================================

if fieldnames(1)<>[] then pause,end

my_struct = struct("field_1",123,"field_2",456);
if or(fieldnames(my_struct)<>["field_1";"field_2"]) then pause,end

clear T;
T.a=rand(2,2);
T.b='s';
if or(fieldnames(T)<>["a";"b"]) then pause,end


if or(fieldnames(1/%s)<>["num";"den";"dt"]) then pause,end

M=mlist(['foo','A','B','C'],[],[],[])
if or(fieldnames(M)<>['A';'B';'C']) then pause,end
