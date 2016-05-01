//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//define tools
function A=testmat1(a,n)
	//eigen values are given by a dilation of nth roots of 1
	A=diag(a*ones(1,n-1),1)+diag((1/a)*ones(1,n-1),-1)
	A(1,n)=1/a;A(n,1)=a
endfunction
function r=Err(x)
	r=norm(x,1)
endfunction
rand('normal')

//==========================================================================
//==============================    \         ============================== 
//==========================================================================
function x=leftmatrixdiv(A,B)
  x=A\B
endfunction
//scalar division
  
//-----Square matrix-----
n=5;  A=rand(n,n);b=rand(2,n+1);
if leftmatrixdiv(A,[]) <>[] then pause,end
if execstr('leftmatrixdiv(A,B)','errcatch')==0 then pause,end
//Small dimensions real
n=5;
b=rand(n,2);A=rand(n,n);
x=leftmatrixdiv(A,b);
if Err(A*x-b)>200*%eps then pause,end
//Small dimensions complex
b=rand(n,2)+%i;A=rand(n,n);
x=leftmatrixdiv(A,b);
if Err(A*x-b)>500*%eps then pause,end

b=rand(n,2);A=rand(n,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A*x-b)>200*%eps then pause,end


b=rand(n,2)+%i;A=rand(n,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A*x-b)>200*%eps then pause,end


//Large dimensions real
n=50;
b=rand(n,2);A=rand(n,n);
x=leftmatrixdiv(A,b);
if Err(A*x-b)>10^6*%eps then pause,end
//Small dimensions complex
b=rand(n,2)+%i;A=rand(n,n);
x=leftmatrixdiv(A,b);
if Err(A*x-b)>50000*%eps then pause,end

b=rand(n,2);A=rand(n,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A*x-b)>50000*%eps then pause,end

b=rand(n,2)+%i;A=rand(n,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A*x-b)>50000*%eps then pause,end

//-----Rectangular matrix-----
n=5;m=3; A=rand(m,n);b=rand(n+1,2);
if leftmatrixdiv(A,[]) <>[] then pause,end
if execstr('leftmatrixdiv(A,b)','errcatch')==0 then pause,end

//Small dimensions real
n=5;m=3;
b=rand(m,2);A=rand(m,n);
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>200*%eps then pause,end

n=3;m=5;
b=rand(m,2);A=rand(m,n);
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end
//Small dimensions complex
n=5;m=3;
b=rand(m,2)+%i;A=rand(m,n);
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end
n=5;m=3;
b=rand(m,2);A=rand(m,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end
b=rand(m,2)+%i;A=rand(m,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end

n=3;m=5;
b=rand(m,2)+%i;A=rand(m,n);
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end

n=3;m=5;
b=rand(m,2);A=rand(m,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end

n=3;m=5;
b=rand(m,2)+%i;A=rand(m,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end

//LArge dimension real

n=40;m=20;
b=rand(m,2);A=rand(m,n);
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end

b=rand(m,2);A=rand(m,n);
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end
//Large dimensions complex

b=rand(m,2)+%i;A=rand(m,n);
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end


b=rand(m,2);A=rand(m,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end

b=rand(m,2)+%i;A=rand(m,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end


b=rand(m,2)+%i;A=rand(m,n);
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end

b=rand(m,2);A=rand(m,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end

b=rand(m,2)+%i;A=rand(m,n)+%i;
x=leftmatrixdiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end


