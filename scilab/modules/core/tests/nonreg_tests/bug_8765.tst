// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 -INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8765 -->
//
// <-- CLI SHELL MODE -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8765
//
// <-- Short Description -->
// A(1:N,j)=[] returns bullshit if N>size(A,1)

//Array of doubles
Ar=[1 2 3;4 5 6];
A=Ar;A(1:3,1)=[];
if or(A<>Ar(:,[2 3])) then pause,end

//Array of integers
Ar=int32([1 2 3;4 5 6]);
A=Ar;A(1:3,1)=[];
if or(A<>Ar(:,[2 3])) then pause,end

//Array of polynomials
Ar=[1 2 3;4 5 6];Ar(1,2)=%s;
A=Ar;A(1:3,1)=[];
if or(A<>Ar(:,[2 3])) then pause,end

//Array of strings
Ar=['123' '4567' '89';'A','BCDEF','GHIJHKL'];
A=Ar;A(1:3,1)=[];
if or(A<>Ar(:,[2 3])) then pause,end

//Array of booleans
Ar=[%t %f %f;%f %t %t];
A=Ar;A(1:3,1)=[];
if or(A<>Ar(:,[2 3])) then pause,end

//sparse matrices
Ar=sparse([1 2 3;4 5 6]);
A=Ar;A(1:3,1)=[];
if or(A<>Ar(:,[2 3])) then pause,end
