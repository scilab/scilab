//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function r=Err(x),r=norm(x,1),endfunction
rand('normal')

//==========================================================================
//==============================      lu      ============================== 
//==========================================================================
//Empty matrix
A=[];
[L,U]=lu(A);
if L<>[]|U<>[] then pause,end
[L,U,E]=lu(A);
if L<>[]|U<>[]|E<>[] then pause,end
//Non full rank
A=rand(5,2);A=A*A';Ac=rand(5,2)+%i*rand(5,2);Ac=Ac*Ac';
[L,U,E]=lu(A);
if Err(L*U-E*A) >200*%eps then pause,end
[L,U,E]=lu(Ac);
if Err(L*U-E*Ac) >200*%eps then pause,end

//Small dimension
//---------------
//Square
A=rand(5,5);Ac=A+%i*rand(A);
//Real case 
[L,U]=lu(A);
if Err(L*U-A) >200*%eps then pause,end
[L,U,E]=lu(A);
if Err(L*U-E*A) >200*%eps then pause,end
//Complex case
[L,U]=lu(Ac);
if Err(L*U-Ac) >200*%eps then pause,end
[L,U,E]=lu(Ac);
if Err(L*U-E*Ac) >200*%eps then pause,end
//Fat
A=rand(3,5);Ac=A+%i*rand(A);
//Real case
[L,U]=lu(A);
if Err(L*U-A) >200*%eps then pause,end
[L,U,E]=lu(A);
if Err(L*U-E*A) >200*%eps then pause,end
//Complex case
[L,U]=lu(Ac);
if Err(L*U-Ac) >200*%eps then pause,end
[L,U,E]=lu(Ac);
if Err(L*U-E*Ac) >200*%eps then pause,end
//Tall
A=rand(5,3);Ac=A+%i*rand(A);
//Real case
[L,U]=lu(A);
if Err(L*U-A) >200*%eps then pause,end
[L,U,E]=lu(A);
if Err(L*U-E*A) >200*%eps then pause,end
//Complex case
[L,U]=lu(Ac);
if Err(L*U-Ac) >200*%eps then pause,end
[L,U,E]=lu(Ac);
if Err(L*U-E*Ac) >200*%eps then pause,end

//large dimension
//---------------
//Square
A=rand(50,50);Ac=A+%i*rand(A);
//Real case
[L,U]=lu(A);
if Err(L*U-A) >1000*%eps then pause,end
[L,U,E]=lu(A);
if Err(L*U-E*A) >1000*%eps then pause,end
//Complex case
[L,U]=lu(Ac);
if Err(L*U-Ac) >1000*%eps then pause,end
[L,U,E]=lu(Ac);
if Err(L*U-E*Ac) >1000*%eps then pause,end
//Fat
A=rand(30,50);Ac=A+%i*rand(A);
//Real case
[L,U]=lu(A);
if Err(L*U-A) >1000*%eps then pause,end
[L,U,E]=lu(A);
if Err(L*U-E*A) >1000*%eps then pause,end
//Complex case
[L,U]=lu(Ac);
if Err(L*U-Ac) >1000*%eps then pause,end
[L,U,E]=lu(Ac);
if Err(L*U-E*Ac) >1000*%eps then pause,end
//Tall
A=rand(50,30);Ac=A+%i*rand(A);
//Real case
[L,U]=lu(A);
if Err(L*U-A) >1000*%eps then pause,end
[L,U,E]=lu(A);
if Err(L*U-E*A) >1000*%eps then pause,end
//Complex case
[L,U]=lu(Ac);
if Err(L*U-Ac) >1000*%eps then pause,end
[L,U,E]=lu(Ac);
if Err(L*U-E*Ac) >1000*%eps then pause,end



