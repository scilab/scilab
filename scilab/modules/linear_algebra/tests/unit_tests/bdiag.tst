// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function r=Err(x),r=norm(x,1),endfunction
rand('normal')


//==========================================================================
//==============================   bdiag      ============================== 
//==========================================================================
if bdiag([])<>[] then pause,end
[ab,x]=bdiag([]);
if ab<>[]|x<>[] then pause,end
[ab,x,bs]=bdiag([]);
if ab<>[]|x<>[]|bs<>[] then pause,end
if execstr('bdiag([1 2;3 4;5 6])','errcatch')==0 then pause,end

//Small dimension
//---------------
//Real case
e=1.d-1;
A=[1 1  2 3 4 5
   0 1  6 7 8 9
   0 0  1 e 3 1
   0 0 -e 1 5 9
   0 0  0 0 2 e
   0 0  0 0 0 3];
X1=[0.5,0.3,0,0.3,0.3,0.2;
   1,0.6,0.5,0.1,0.7,0.4;
   0.7,0.1,0.4,0.6,0.1,1;
   0,0.6,0.2,0.3,0.4,0.5;
   0.6,0.7,0.5,0.7,0.7,0.5;
   0.3,0.3,0.4,0.5,0.9,0.6]
A=inv(X1)*A*X1;

Ab1=bdiag(A);
if or(triu(Ab1,-1)<>Ab1) then pause,end
[Ab2,X]=bdiag(A);
if Err(Ab2-Ab1)>>10*%eps then pause,end 
if Err(Ab2-inv(X)*A*X )>1d6*%eps then pause,end 

[Ab2,X,bs]=bdiag(A);
if Err(Ab2-Ab1)>>10*%eps then pause,end 
if Err(Ab2-inv(X)*A*X )>2.d-10 then pause,end 
if or(size(bs)<>[3,1]) then pause,end
if sum(bs)<>size(A,1) then pause,end
if or(bs<=0) then pause,end

[Ab2,X,bs]=bdiag(A,1);
if Err(Ab2-Ab1)>>10*%eps then pause,end 
if Err(Ab2-inv(X)*A*X )>2d-7 then pause,end 
if or(size(bs)<>[1,1]) then pause,end
if sum(bs)<>size(A,1) then pause,end
if or(bs<=0) then pause,end



//Complex case
e=1.d-1;
A=[1 1  2 3 4 5
   0 1  6 7 8 9
   0 0  1 e 3 1
   0 0 -e 1 5 9
   0 0  0 0 2 e
   0 0  0 0 0 3];
X1=[0.5,0.3,0,0.3,0.3,0.2;
   1,0.6,0.5,0.1,0.7,0.4;
   0.7,0.1,0.4,0.6,0.1,1;
   0,0.6,0.2,0.3,0.4,0.5;
   0.6,0.7,0.5,0.7,0.7,0.5;
   0.3,0.3,0.4,0.5,0.9,0.6]+%i*eye(A);
A=inv(X1)*A*X1;

Ab1=bdiag(A);
if or(triu(Ab1)<>Ab1) then pause,end
[Ab2,X]=bdiag(A);
if Err(Ab2-Ab1)>>10*%eps then pause,end 
if Err(Ab2-inv(X)*A*X )>1.d-8 then pause,end 

[Ab2,X,bs]=bdiag(A);
if Err(Ab2-Ab1)>>10*%eps then pause,end 
if Err(Ab2-inv(X)*A*X )>1.d-8 then pause,end 
if size(bs,2)<>1 then pause,end
if sum(bs)<>size(A,1) then pause,end
if or(bs<=0) then pause,end
//Large dimension
//---------------
//Real case
A=rand(25,25);
Ab1=bdiag(A);
if or(triu(Ab1,-1)<>Ab1) then pause,end
[Ab2,X]=bdiag(A);
if Err(Ab2-Ab1)>>10*%eps then pause,end 
if Err(Ab2-inv(X)*A*X )>10000*%eps then pause,end 

[Ab2,X,bs]=bdiag(A);
if Err(Ab2-Ab1)>>10*%eps then pause,end 
if Err(Ab2-inv(X)*A*X )>10000*%eps then pause,end 
if size(bs,2)<>1 then pause,end
if sum(bs)<>size(A,1) then pause,end
if or(bs<=0) then pause,end

//Complex case
A=rand(25,25)+%i*rand(25,25);
Ab1=bdiag(A);
if or(triu(Ab1)<>Ab1) then pause,end
[Ab2,X]=bdiag(A);
if Err(Ab2-Ab1)>>10*%eps then pause,end 
if Err(Ab2-inv(X)*A*X )>10000*%eps then pause,end 

[Ab2,X,bs]=bdiag(A);
if Err(Ab2-Ab1)>>10*%eps then pause,end 
if Err(Ab2-inv(X)*A*X )>10000*%eps then pause,end 
if size(bs,2)<>1 then pause,end
if sum(bs)<>size(A,1) then pause,end
if or(bs<=0) then pause,end


