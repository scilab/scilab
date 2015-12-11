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
//==============================    chol      ============================== 
//==========================================================================

//Empty matrix
if chol([])<>[] then pause,end

if execstr('chol([1 2;3 4])','errcatch')==0 then pause,end
if execstr('chol([1 2;3 4]+%i)','errcatch')==0 then pause,end

//Small dimension
//REAL
A=rand(5,5);A=A*A';
U=chol(A);
if Err(triu(U)-U)>200*%eps then pause,end
if Err(U'*U-A)>200*%eps then pause,end

//Complex
A=rand(5,5)+%i*rand(5,5);A=A*A';
U=chol(A);
if Err(triu(U)-U)>200*%eps then pause,end
if Err(U'*U-A)>200*%eps then pause,end

//Large dimension
//REAL
A=rand(50,50);A=A*A';
U=chol(A);
if Err(triu(U)-U)>10*%eps then pause,end
if Err(U'*U-A)>1000*%eps then pause,end

//Complex
A=rand(5,5)+%i*rand(5,5);A=A*A';
U=chol(A);
if Err(triu(U)-U)>10*%eps then pause,end
if Err(U'*U-A)>1000*%eps then pause,end

