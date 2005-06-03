function r=Err(x),r=norm(x,1),endfunction
rand('normal')
//==========================================================================
//==============================      qr      ============================== 
//==========================================================================
//Empty matrix

e=[];
if qr(e)<>[] then pause,end
if qr(e,"e")<>[] then pause,end

[Q,R]=qr(e);
if Q<>[]|R<>[] then pause,end
[Q,R]=qr(e,"e");
if Q<>[]|R<>[] then pause,end

[Q,R,x]=qr(e);
if Q<>[]|R<>[]|x<>[] then pause,end
[Q,R,x]=qr(e,"e");
if Q<>[]|R<>[]|x<>[] then pause,end

//Small dimension
//---------------
A=rand(3,2);Ac=A+rand(A)*%i;

//Real Case
Q=qr(A);
if Err(Q*Q'-eye())> 200*%eps then pause,end
Q=qr(A,"e");
if Err(Q'*Q-eye())> 200*%eps then pause,end

[Q,R]=qr(A);
if Err(Q*R-A)> 200*%eps then pause,end

[Q,R]=qr(A,"e");
if Err(Q*R-A)> 200*%eps then pause,end
if Err(Q'*Q-eye())> 200*%eps then pause,end

Q=qr(A');
if Err(Q*Q'-eye())> 200*%eps then pause,end
Q=qr(A',"e");
if Err(Q*Q'-eye())> 200*%eps then pause,end

[Q,R]=qr(A');
if Err(Q*R-A')> 200*%eps then pause,end

[Q,R]=qr(A',"e");
if Err(Q*R-A')> 200*%eps then pause,end

[Q,R,x]=qr(A);
if Err(Q*R*x'-A)> 200*%eps then pause,end

[Q,R,x]=qr(A,"e");
if Err(Q*R*x'-A)> 200*%eps then pause,end

//Complex case
Q=qr(Ac);
if Err(Q*Q'-eye())> 200*%eps then pause,end
Q=qr(Ac,"e");
if Err(Q'*Q-eye())> 200*%eps then pause,end

[Q,R]=qr(Ac);
if Err(Q*R-Ac)> 200*%eps then pause,end
[Q,R]=qr(Ac,"e");
if Err(Q*R-Ac)> 200*%eps then pause,end
if Err(Q'*Q-eye())> 200*%eps then pause,end

Q=qr(Ac');
if Err(Q*Q'-eye())> 200*%eps then pause,end
Q=qr(Ac',"e");
if Err(Q*Q'-eye())> 200*%eps then pause,end


[Q,R]=qr(Ac');
if Err(Q*R-Ac')> 200*%eps then pause,end
[Q,R]=qr(Ac',"e");
if Err(Q*R-Ac')> 200*%eps then pause,end

[Q,R,x]=qr(Ac);
if Err(Q*R-Ac*x)> 200*%eps then pause,end
[Q,R,x]=qr(Ac,"e");
if Err(Q*R-Ac*x)> 200*%eps then pause,end

[Q,R,x]=qr(Ac');
if Err(Q*R-Ac'*x)> 200*%eps then pause,end
[Q,R,x]=qr(Ac',"e");
if Err(Q*R-Ac'*x)> 200*%eps then pause,end

//Rank detection (obsolete)
[Q,R,rk,x]=qr(A);
if Err(Q*R*x'-A)> 200*%eps | rk<>2  then pause,end
[Q,R,rk,x]=qr(A,1.d-8);
if Err(Q*R*x'-A)> 200*%eps | rk<>2  then pause,end

[Q,R,rk,x]=qr(Ac);
if Err(Q*R*x'-Ac)> 200*%eps | rk<>2  then pause,end

[Q,R,rk,x]=qr(Ac,1.d-8);
if Err(Q*R*x'-Ac)> 200*%eps | rk<>2  then pause,end

//Large dimension
//---------------
A=rand(150,60);Ac=A+rand(A)*%i;
//Real Case
Q=qr(A);
if Err(Q*Q'-eye())> 1000*%eps then pause,end
Q=qr(A,"e");
if Err(Q'*Q-eye())> 1000*%eps then pause,end

[Q,R]=qr(A);
if Err(Q*R-A)> 1000*%eps then pause,end

[Q,R]=qr(A,"e");
if Err(Q*R-A)> 1000*%eps then pause,end
if Err(Q'*Q-eye())> 1000*%eps then pause,end

Q=qr(A');
if Err(Q*Q'-eye())> 1000*%eps then pause,end
Q=qr(A',"e");
if Err(Q*Q'-eye())> 1000*%eps then pause,end

[Q,R]=qr(A');
if Err(Q*R-A')> 1000*%eps then pause,end

[Q,R]=qr(A',"e");
if Err(Q*R-A')> 1000*%eps then pause,end

[Q,R,x]=qr(A);
if Err(Q*R*x'-A)> 1000*%eps then pause,end

[Q,R,x]=qr(A,"e");
if Err(Q*R*x'-A)> 1000*%eps then pause,end

//Complex case
Q=qr(Ac);
if Err(Q*Q'-eye())> 2000*%eps then pause,end
Q=qr(Ac,"e");
if Err(Q'*Q-eye())> 2000*%eps then pause,end

[Q,R]=qr(Ac);
if Err(Q*R-Ac)> 2000*%eps then pause,end
[Q,R]=qr(Ac,"e");
if Err(Q*R-Ac)> 2000*%eps then pause,end
if Err(Q'*Q-eye())> 2000*%eps then pause,end

Q=qr(Ac');
if Err(Q*Q'-eye())> 2000*%eps then pause,end
Q=qr(Ac',"e");
if Err(Q*Q'-eye())> 2000*%eps then pause,end


[Q,R]=qr(Ac');
if Err(Q*R-Ac')> 2000*%eps then pause,end
[Q,R]=qr(Ac',"e");
if Err(Q*R-Ac')> 2000*%eps then pause,end

[Q,R,x]=qr(Ac);
if Err(Q*R-Ac*x)> 2000*%eps then pause,end
[Q,R,x]=qr(Ac,"e");
if Err(Q*R-Ac*x)> 2000*%eps then pause,end

[Q,R,x]=qr(Ac');
if Err(Q*R-Ac'*x)> 2000*%eps then pause,end
[Q,R,x]=qr(Ac',"e");
if Err(Q*R-Ac'*x)> 2000*%eps then pause,end

//Rank detection (obsolete)
[Q,R,rk,x]=qr(A);
if Err(Q*R*x'-A)> 2000*%eps | rk<>60  then pause,end
[Q,R,rk,x]=qr(A,1.d-8);
if Err(Q*R*x'-A)> 2000*%eps | rk<>60  then pause,end

[Q,R,rk,x]=qr(Ac);
if Err(Q*R*x'-Ac)> 2000*%eps | rk<>60  then pause,end

[Q,R,rk,x]=qr(Ac,1.d-8);
if Err(Q*R*x'-Ac)> 2000*%eps | rk<>60  then pause,end
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
A=rand(5,2);A=A*A';;Ac=rand(5,2)+%i*rand(5,2);Ac=Ac*Ac';
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


//==========================================================================
//==============================     svd      ============================== 
//==========================================================================
//Empty matrix
A=[];
[U,S,V]=svd(A);
if U<>[]|V<>[]|S<>[] then pause,end
S=svd(A);
if S<>[] then pause,end
[U,S,V]=svd(A,"e");
if U<>[]|V<>[]|S<>[] then pause,end

S=svd(A,"e");
if S<>[] then pause,end

//Matrix with inf or nan
if execstr('svd([%inf 1;2 3])','errcatch')==0 then pause,end
if execstr('svd([1 %nan;2 3])','errcatch')==0 then pause,end

if execstr('svd([%inf %i;2 3])','errcatch')==0 then pause,end
if execstr('svd([%i %i;%nan 3])','errcatch')==0 then pause,end


//Small dimension
//---------------
A=rand(3,5);Ac=A+%i*rand(A);

//Real Case
[U,S,V]=svd(A);
if Err(U*S*V'-A)>200*%eps then pause,end
if Err(svd(A)-diag(S))> 200*%eps then pause,end

[U,S,V]=svd(A,"e");
if Err(U*S*V'-A)>200*%eps then pause,end

A=A';
[U,S,V]=svd(A);
if Err(U*S*V'-A)>200*%eps then pause,end
if Err(svd(A)-diag(S))> 200*%eps then pause,end

[U,S,V]=svd(A,"e");
if Err(U*S*V'-A)>200*%eps then pause,end


//Complex Case
[U,S,V]=svd(Ac);
if Err(U*S*V'-Ac)>200*%eps then pause,end
if Err(svd(Ac)-diag(S))> 200*%eps then pause,end

[U,S,V]=svd(Ac,"e");
if Err(U*S*V'-Ac)>200*%eps then pause,end

Ac=Ac';
[U,S,V]=svd(Ac);U*S*V'-Ac;
if Err(U*S*V'-Ac)>200*%eps then pause,end
if Err(svd(Ac)-diag(S))> 200*%eps then pause,end

[U,S,V]=svd(Ac,"e");
if Err(U*S*V'-Ac)>200*%eps then pause,end


//Large dimension
//---------------
A=rand(150,60);Ac=A+rand(A)*%i;
//Real Case
[U,S,V]=svd(A);
if Err(U*S*V'-A)>10000*%eps then pause,end
if Err(svd(A)-diag(S))> 10000*%eps then pause,end

[U,S,V]=svd(A,"e");
if Err(U*S*V'-A)>10000*%eps then pause,end

A=A';
[U,S,V]=svd(A);
if Err(U*S*V'-A)>10000*%eps then pause,end
if Err(svd(A)-diag(S))> 10000*%eps then pause,end

[U,S,V]=svd(A,"e");
if Err(U*S*V'-A)>10000*%eps then pause,end


//Complex Case
[U,S,V]=svd(Ac);
if Err(U*S*V'-Ac)>10000*%eps then pause,end
if Err(svd(Ac)-diag(S))> 10000*%eps then pause,end

[U,S,V]=svd(Ac,"e");
if Err(U*S*V'-Ac)>10000*%eps then pause,end

Ac=Ac';
[U,S,V]=svd(Ac);U*S*V'-Ac;
if Err(U*S*V'-Ac)>10000*%eps then pause,end
if Err(svd(Ac)-diag(S))> 10000*%eps then pause,end

[U,S,V]=svd(Ac,"e");
if Err(U*S*V'-Ac)>10000*%eps then pause,end
//==========================================================================
//==============================     spec     ============================== 
//==========================================================================
//define tools
function A=testmat1(a,n)
//eigen values are given by a dilation of nth roots of 1
A=diag(a*ones(1,n-1),1)+diag((1/a)*ones(1,n-1),-1)
A(1,n)=1/a;A(n,1)=a
endfunction
function r=Checktestmat1(a,n)
   A=testmat1(a,n);S=spec(A);
   SR=real(S);SI=imag(S);
   dt=2*%i*%pi/n;Z=exp(dt*(1:n)');ZR=real(Z*((1+a*a')/a));
   ZI=-imag(Z*((a*a'-1)/a));
   r=max(norm(sort(SR)-sort(ZR)),norm(sort(SI)-sort(ZI)))
endfunction
function A=testmat2(a,n)
//eigen values are given by a dilation of nth roots of 1
A=testmat1(a,n);A=A+A'
endfunction
function r=Checktestmat2(a,n)
   A=testmat2(a,n);S=spec(A);
   SR=real(S);SI=imag(S);
   dt=2*%i*%pi/n;Z=exp(dt*(1:n)');
   ZR=2*real(Z*((1+a*a')/a));ZI=0*ZR;
   r=max(norm(sort(SR)-sort(ZR)),norm(sort(SI)-sort(ZI)))
endfunction



//Empty matrix
A=[];
S=spec(A);
if S<>[] then pause,end
//Matrix with Inf or Nan (test de la detection d'erreur
if execstr('spec([%inf 1;2 3])','errcatch')==0 then pause,end
if execstr('spec([1 %nan;2 3])','errcatch')==0 then pause,end

if execstr('spec([%inf %i;2 3])','errcatch')==0 then pause,end
if execstr('spec([%i %i;%nan 3])','errcatch')==0 then pause,end

//Small dimension
//---------------
//Real Case
//Unsymetric
if Checktestmat1(3,5)>200*%eps then pause,end
[U,S]=spec(testmat1(3,5));
if Err(U*S/U-testmat1(3,5))>200*%eps then pause,end 
//Symmetric
if Checktestmat2(3,5)>200*%eps then pause,end
[U,S]=spec(testmat2(3,5));
if Err(U*S/U-testmat2(3,5))>200*%eps then pause,end 

//Complex Case
//Unsymetric
if Checktestmat1(3+2*%i,5)>200*%eps then pause,end
[U,S]=spec(testmat1(3+2*%i,5));
if Err(U*S/U-testmat1(3+2*%i,5))>200*%eps then pause,end 

//Symmetric
if Checktestmat2(3+2*%i,5)>200*%eps then pause,end
[U,S]=spec(testmat2(3+2*%i,5));
if Err(U*S/U-testmat2(3+2*%i,5))>200*%eps then pause,end 

//Large dimension
//---------------
//Real Case
//Unsymetric
if Checktestmat1(3,50)>1000*%eps then pause,end
[U,S]=spec(testmat1(3,50));
if Err(U*S/U-testmat1(3,50))>1000*%eps then pause,end 

//Symmetric
if Checktestmat2(3,50)>1000*%eps then pause,end
[U,S]=spec(testmat2(3,50));
if Err(U*S/U-testmat2(3,50))>1000*%eps then pause,end 

//Complex Case
//Unsymetric
if Checktestmat1(3+2*%i,50)>1000*%eps then pause,end
[U,S]=spec(testmat1(3+2*%i,50));
if Err(U*S/U-testmat1(3+2*%i,50))>1000*%eps then pause,end 

//Symmetric
if Checktestmat2(3+2*%i,50)>1000*%eps then pause,end
[U,S]=spec(testmat2(3+2*%i,50));
if Err(U*S/U-testmat2(3+2*%i,50))>1000*%eps then pause,end 

//==========================================================================
//==============================    gspec     ============================== 
//==========================================================================


//Empty matrix
S=spec([],[]);
if S<>[] then pause,end
[al,be]=spec([],[]);
if al<>[]|be<>[] then pause,end
[al,be,Z]=spec([],[]);
if al<>[]|be<>[]|Z<>[] then pause,end
[al,be,Z,Q]=spec([],[]);
if al<>[]|be<>[]|Z<>[]|Q<>[] then pause,end

//Matrix with Inf or Nan (test de la detection d'erreur
if execstr('spec([%inf 1;2 3],[1 2;3 4])','errcatch')==0 then pause,end
if execstr('spec([1 2;3 4],[1 %nan;2 3])','errcatch')==0 then pause,end

//Small dimension
//---------------
//Real Case
A=testmat1(3,5);E=testmat1(-2,5);
S=spec(A,E);
[Sa,Se]=spec(A,E);
if Err(S-Sa./Se)>10*%eps then pause,end
[Sa,Se,Z]=spec(A,E);
if Err(S-Sa./Se)>10*%eps then pause,end
if Err(A*Z-E*Z*diag(S))>200*%eps then pause,end
[Sa,Se,Q,Z]=spec(A,E);
if Err(S-Sa./Se)>10*%eps then pause,end
if Err(A*Z-E*Z*diag(S))>200*%eps then pause,end
if Err(Q'*A-diag(S)*Q'*E)>200*%eps then pause,end

//Complex Case

A=testmat1(3-%i,5);E=testmat1(-2+0.1*%i,5);
S=spec(A,E);
[Sa,Se]=spec(A,E);
if Err(S-Sa./Se)>10*%eps then pause,end
[Sa,Se,Z]=spec(A,E);
if Err(S-Sa./Se)>10*%eps then pause,end
if Err(A*Z-E*Z*diag(S))>200*%eps then pause,end
[Sa,Se,Q,Z]=spec(A,E);
if Err(S-Sa./Se)>10*%eps then pause,end
if Err(A*Z-E*Z*diag(S))>200*%eps then pause,end
if Err(Q'*A-diag(S)*Q'*E)>200*%eps then pause,end


//Large dimension
//---------------

//Real Case
A=testmat1(3,30);E=testmat1(-2,30);
S=spec(A,E);
[Sa,Se]=spec(A,E);
if Err(S-Sa./Se)>10*%eps then pause,end
[Sa,Se,Z]=spec(A,E);
if Err(S-Sa./Se)>10*%eps then pause,end
if Err(A*Z-E*Z*diag(S))>1000*%eps then pause,end
[Sa,Se,Q,Z]=spec(A,E);
if Err(S-Sa./Se)>10*%eps then pause,end
if Err(A*Z-E*Z*diag(S))>1000*%eps then pause,end
if Err(Q'*A-diag(S)*Q'*E)>1000*%eps then pause,end

//Complex Case

A=testmat1(3-%i,30);E=testmat1(-2+0.1*%i,30);
S=spec(A,E);
[Sa,Se]=spec(A,E);
if Err(S-Sa./Se)>20*%eps then pause,end
[Sa,Se,Z]=spec(A,E);
if Err(S-Sa./Se)>20*%eps then pause,end
if Err(A*Z-E*Z*diag(S))>1000*%eps then pause,end
[Sa,Se,Q,Z]=spec(A,E);
if Err(S-Sa./Se)>20*%eps then pause,end
if Err(A*Z-E*Z*diag(S))>1000*%eps then pause,end
if Err(Q'*A-diag(S)*Q'*E)>1000*%eps then pause,end


//==========================================================================
//==============================     inv      ============================== 
//==========================================================================
//Empty matrix
A=[];
if inv(A)<>[] then pause,end
//Singular matrix
if execstr('inv([0 0;2 3])','errcatch')==0 then pause,end
if execstr('inv([0 0;%i 3])','errcatch')==0 then pause,end
//Rectangular matrix
if execstr('inv(rand(2,3))','errcatch')==0 then pause,end
if execstr('inv(rand(2,3)+%i*eye())','errcatch')==0 then pause,end
//Small dimension
//---------------
//Unsymetric
A=testmat1(3,5);Ac=testmat1(3+%i,5);
//Real Case
if Err(A*inv(A)-eye(A)) >200*%eps then pause,end
//Complex Case
if Err(Ac*inv(Ac)-eye(A)) >200*%eps then pause,end
//Symetric
A=A*A';Ac=Ac*Ac';
//Real Case
if Err(A*inv(A)-eye(A)) >1000*%eps then pause,end
//Complex Case
if Err(Ac*inv(Ac)-eye(A)) >1000*%eps then pause,end
//Large dimension
//---------------
//Unsymetric
A=testmat1(3,50);Ac=testmat1(3+%i,50);
//Real Case
if Err(A*inv(A)-eye(A)) >1000*%eps then pause,end
//Complex Case
if Err(Ac*inv(Ac)-eye(A)) >2000*%eps then pause,end
//Symetric
A=A*A';Ac=Ac*Ac';
//Real Case
if Err(A*inv(A)-eye(A)) >1.d-10 then pause,end
//Complex Case
if Err(Ac*inv(Ac)-eye(A)) >4.d-10 then pause,end
//==========================================================================
//==============================    rcond     ============================== 
//==========================================================================
//Empty matrix
A=[];
if rcond(A)<>[] then pause,end

//Rectangular matrix
if execstr('rcond(rand(2,3))','errcatch')==0 then pause,end
if execstr('rcond(rand(2,3)+%i*eye())','errcatch')==0 then pause,end
//Small dimension
//---------------
//Real Case
if Err(rcond(eye(5,5))-1)>10*%eps then pause,end
//Complex Case
if  Err(rcond(eye(5,5)*(1+%i))-1)>10*%eps then pause,end


//Large dimension
//---------------
//Real Case
if Err(rcond(eye(50,50))-1)>10*%eps then pause,end
//Complex Case
if  Err(rcond(eye(50,50)*(1+%i))-1)>10*%eps then pause,end


//==========================================================================
//==============================    schur     ============================== 
//==========================================================================
function t=sel(R),t=real(R)<0 ,endfunction
//Empty matrix
A=[];
if schur(A)<>[] then pause,end
if schur(A,'real')<>[] then pause,end
if schur(A,'complex')<>[] then pause,end

if schur(A,'c')<>[] then pause,end
if schur(A,'d')<>[] then pause,end
if schur(A,sel)<>[] then pause,end

[U,S]=schur(A);
if U<>[]|S<>[] then pause,end
[U,S]=schur(A,'real');
if U<>[]|S<>[] then pause,end
[U,S]=schur(A,'complex');
if U<>[]|S<>[] then pause,end



[U,N]=schur(A,'c');
if U<>[]|N<>0 then pause,end
[U,N]=schur(A,'d');
if U<>[]|N<>0 then pause,end
[U,N]=schur(A,sel);
if U<>[]|N<>0 then pause,end

[U,N,S]=schur(A,'c');
if U<>[]|N<>0|S<>[] then pause,end
[U,N,S]=schur(A,'d');
if U<>[]|N<>0|S<>[] then pause,end
[U,N,S]=schur(A,sel);
if U<>[]|N<>0|S<>[] then pause,end

//Rectangular matrix
if execstr('schur(rand(2,3))','errcatch')==0 then pause,end
if execstr('[U,S]=schur(rand(2,3))','errcatch')==0 then pause,end

if execstr('schur(rand(2,3)+%i*eye())','errcatch')==0 then pause,end
if execstr('[U,S]=schur(rand(2,3)+%i*eye())','errcatch')==0 then pause,end

//Small dimension
A=testmat1(3,5);Ac=testmat1(3+%i,5);
//Real
[U,S]=schur(A);
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-A)>200*%eps then pause,end
if Err(schur(A)-S) >%eps then pause,end

[U,S]=schur(A,'real');
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-A)>200*%eps then pause,end
if Err(schur(A)-S) >%eps then pause,end

[U,S]=schur(A,'complex');
if Err(triu(S)-S)>%eps then pause,end
if Err(U*S*U'-A)>200*%eps then pause,end
if Err(schur(A,'complex')-S) >%eps then pause,end

[U,n]=schur(A,'c');S=U'*A*U;
if n<>2 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

[U,n]=schur(A,'d');S=U'*A*U;
if n<>0 then pause,end
if or(abs(spec(S(n+1:$,n+1:$)))<1) then pause,end

[U,n]=schur(A,sel);S=U'*A*U;
if n<>2 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end


//Complex
[U,S]=schur(Ac);
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-Ac)>200*%eps then pause,end
if Err(schur(Ac)-S) >%eps then pause,end

[U,S]=schur(Ac,'complex');
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-Ac)>200*%eps then pause,end
if Err(schur(Ac)-S) >%eps then pause,end


[U,n]=schur(Ac,'c');S=U'*Ac*U;
if n<>3 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

[U,n]=schur(Ac,'d');S=U'*A*U;
if n<>0 then pause,end
if or(abs(spec(S(n+1:$,n+1:$)))<1) then pause,end

[U,n]=schur(Ac,sel);S=U'*Ac*U;
if n<>3 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end


//Large dimension
A=testmat1(3,50);Ac=testmat1(3+%i,50);
//Real
[U,S]=schur(A);
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-A)>1000*%eps then pause,end
if Err(schur(A)-S) >%eps then pause,end

[U,S]=schur(A,'real');
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-A)>1000*%eps then pause,end
if Err(schur(A)-S) >%eps then pause,end

[U,S]=schur(A,'complex');
if Err(triu(S)-S)>%eps then pause,end
if Err(U*S*U'-A)>1000*%eps then pause,end
if Err(schur(A,'complex')-S) >%eps then pause,end


[U,n]=schur(A,'c');S=U'*A*U;
if n<>25 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

[U,n]=schur(A,'d');S=U'*A*U;
if n<>0 then pause,end
if or(abs(spec(S(n+1:$,n+1:$)))<1) then pause,end

[U,n]=schur(A,sel);S=U'*A*U;
if n<>25 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

//Complex
[U,S]=schur(Ac);
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-Ac)>1000*%eps then pause,end
if Err(schur(Ac)-S) >%eps then pause,end

[U,S]=schur(Ac,'complex');
if Err(triu(S,-1)-S)>%eps then pause,end
if Err(U*S*U'-Ac)>1000*%eps then pause,end
if Err(schur(Ac)-S) >%eps then pause,end

[U,n]=schur(Ac,'c');S=U'*Ac*U;
if n<>25 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

[U,n]=schur(Ac,'d');S=U'*Ac*U;
if n<>0 then pause,end
if or(abs(spec(S(n+1:$,n+1:$)))<1) then pause,end

[U,n]=schur(Ac,sel);S=U'*Ac*U;
if n<>25 then pause,end
if or(real(spec(S(1:n,1:n)))>=0) then pause,end
if or(real(spec(S(n+1:$,n+1:$)))<0) then pause,end

//==========================================================================
//==============================    gschur    ============================== 
//==========================================================================
//Empty matrix
[As,Es]=schur([],[]);
if As<>[]|Es<>[] then pause,end

[As,dim]=schur([],[],'c');
if As<>[]|dim<>0 then pause,end
[As,dim]=schur([],[],'d');
if As<>[]|dim<>0 then pause,end
[As,dim]=schur([],[],sel);
if As<>[]|dim<>0 then pause,end

[As,Es,Q,Z]=schur([],[]);
if As<>[]|Es<>[]|Q<>[]|Z<>[] then pause,end

[As,Es,dim]=schur([],[],'c');
if As<>[]|Es<>[]|dim<>0 then pause,end
[As,Es,dim]=schur([],[],'d');
if As<>[]|Es<>[]|dim<>0 then pause,end
[As,Es,dim]=schur([],[],sel);
if As<>[]|Es<>[]|dim<>0 then pause,end

[Z,dim]=schur([],[],'c');
if Z<>[]|dim<>0 then pause,end
[Z,dim]=schur([],[],'d');
if Z<>[]|dim<>0 then pause,end
[Z,dim]=schur([],[],sel);
if Z<>[]|dim<>0 then pause,end


//Rectangular matrix
if execstr('[As,Es]=schur(rand(2,3),rand(2,3))','errcatch')==0 then  pause,end
if execstr('[As,Es,Q,Z]=schur(rand(2,3),rand(2,3))','errcatch')==0 then  pause,end
if execstr('[As,Es,dim]=schur(rand(2,3),rand(2,3),''c'')','errcatch')==0 then  pause,end
if execstr('[Z,dim]=schur(rand(2,3),rand(2,3),sel)','errcatch')==0 then  pause,end

//Small dimension
//----Real------------
A=testmat1(1,5);E=testmat1(-2,5) ;
[As,Es,Q,Z]=schur(A,E);
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

[As1,Es1]=schur(A,E);
if Err(As1-As)>10*%eps then pause,end
if Err(Es1-Es)>10*%eps then pause,end

// Ordered 'c'
dim=schur(A,E,'c');
if dim<>5 then pause,end
[Z,dim]=schur(A,E,'c');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'c');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>5 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>5 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
// Ordered 'd'
dim=schur(A,E,'d');
if dim<>5 then pause,end
[Z,dim]=schur(A,E,'d');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'d');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>5 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>5 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

//ordered sel
function t=sel(Alpha,Beta),t=real(Alpha)>-0.2*real(Beta) ,endfunction

dim=schur(A,E,sel);
if dim<>2 then pause,end
[Z,dim]=schur(A,E,sel);
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,sel);
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>2 then pause,end

[As,Es,Z,dim]=schur(A,E,sel);
if dim<>2 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
//----Complex------------
A=testmat1(1+%i,5);E=testmat1(-2-3*%i,5) ;
[As,Es,Q,Z]=schur(A,E);
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

[As1,Es1]=schur(A,E);
if Err(As1-As)>10*%eps then pause,end
if Err(Es1-Es)>10*%eps then pause,end

// Ordered 'c'
dim=schur(A,E,'c');
if dim<>5 then pause,end
[Z,dim]=schur(A,E,'c');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'c');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>5 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>5 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
// Ordered 'd'
dim=schur(A,E,'d');
if dim<>5 then pause,end
[Z,dim]=schur(A,E,'d');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'d');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>5 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>5 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

//ordered sel
function t=sel(Alpha,Beta),t=imag(Alpha)>0 ,endfunction

dim=schur(A,E,sel);
if dim<>3 then pause,end
[Z,dim]=schur(A,E,sel);
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,sel);
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>3 then pause,end

[As,Es,Z,dim]=schur(A,E,sel);
if dim<>3 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

//Large dimension

//----Real------------
A=testmat1(1,50);E=testmat1(-2,50) ;
[As,Es,Q,Z]=schur(A,E);
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

[As1,Es1]=schur(A,E);
if Err(As1-As)>10*%eps then pause,end
if Err(Es1-Es)>10*%eps then pause,end

// Ordered 'c'
dim=schur(A,E,'c');
if dim<>50 then pause,end
[Z,dim]=schur(A,E,'c');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'c');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>50 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>50 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
// Ordered 'd'
dim=schur(A,E,'d');
if dim<>50 then pause,end
[Z,dim]=schur(A,E,'d');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'d');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>50 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>50 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end

//ordered sel
function t=sel(Alpha,Beta),t=real(Alpha)>-0.2*real(Beta) ,endfunction

dim=schur(A,E,sel);
if dim<>12 then pause,end
[Z,dim]=schur(A,E,sel);
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,sel);
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>12 then pause,end

[As,Es,Z,dim]=schur(A,E,sel);
if dim<>12 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >200*%eps then pause,end
if Err(Es-Q'*E*Z) >200*%eps then pause,end
//----Complex------------
A=testmat1(1+%i,50);E=testmat1(-2-3*%i,50) ;
[As,Es,Q,Z]=schur(A,E);
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >1000*%eps then pause,end
if Err(Es-Q'*E*Z) >1000*%eps then pause,end

[As1,Es1]=schur(A,E);
if Err(As1-As)>10*%eps then pause,end
if Err(Es1-Es)>10*%eps then pause,end

// Ordered 'c'
dim=schur(A,E,'c');
if dim<>50 then pause,end
[Z,dim]=schur(A,E,'c');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'c');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>50 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>50 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >1000*%eps then pause,end
if Err(Es-Q'*E*Z) >1000*%eps then pause,end
// Ordered 'd'
dim=schur(A,E,'d');
if dim<>50 then pause,end
[Z,dim]=schur(A,E,'d');
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,'d');
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>50 then pause,end

[As,Es,Z,dim]=schur(A,E,'d');
if dim<>50 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >1000*%eps then pause,end
if Err(Es-Q'*E*Z) >1000*%eps then pause,end

//ordered sel
function t=sel(Alpha,Beta),t=imag(Alpha)>0 ,endfunction

dim=schur(A,E,sel);
if dim<>32 then pause,end
[Z,dim]=schur(A,E,sel);
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end

[Q,Z1,dim]=schur(A,E,sel);
if Err(Z1-Z)>10*%eps then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if dim<>32 then pause,end

[As,Es,Z,dim]=schur(A,E,sel);
if dim<>32 then pause,end
if Err(Q*Q'-eye(Q)) >200*%eps then pause,end
if Err(Z*Z'-eye(Z)) >200*%eps then pause,end
if Err(As-Q'*A*Z) >1000*%eps then pause,end
if Err(Es-Q'*E*Z) >1000*%eps then pause,end

//==========================================================================
//==============================     svd      ============================== 
//==========================================================================
//Empty matrix
if svd([])<>[] then pause,end
if svd([],"e")<>[] then pause,end

[U,S]=svd([])
if U<>[]|S<>[]  then pause,end
[U,S,V]=svd([]);
if U<>[]|S<>[]|V<>[]  then pause,end
[U,S,V,rk]=svd([]);
if U<>[]|S<>[]|V<>[]|rk<>0  then pause,end
[U,S,V,rk]=svd([],%eps);
if U<>[]|S<>[]|V<>[]|rk<>0  then pause,end

if execstr('[U,S,V,rk]=svd([],'"e'")','errcatch') == 0 then pause,end

//Small dimension
//Real Case Fat
A=rand(3,5);
S=svd(A);
if or(S<0) then pause,end
if sort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>10*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U1'*U1-eye())>200*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end
if Err(U1*S1*V'-A) >200*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>10*%eps  then pause,end
if Err(U1*S1*V1'-A) >200*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(V-V1) >200*%eps  then pause,end
if rk<>3 then pause,end

//Real Case Tall
A=rand(5,3);

S=svd(A);
if or(S<0) then pause,end
if sort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U1'*U1-eye())>200*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end
if Err(U1*S1*V'-A) >200*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if size(U1,2)<>3 then pause,end
if Err(U1*S1*V1'-A) >200*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(V-V1) >200*%eps  then pause,end
if rk<>3 then pause,end

//Complex Case Fat
A=rand(3,5)+%i*rand(3,5);
S=svd(A);
if or(S<0) then pause,end
if sort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U1'*U1-eye())>200*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end
if Err(U1*S1*V'-A) >30*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(U1*S1*V1'-A) >200*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(V-V1) >200*%eps  then pause,end
if rk<>3 then pause,end

//Complex Case Tall
A=rand(5,3)+%i*rand(5,3);

S=svd(A);
if or(S<0) then pause,end
if sort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U1'*U1-eye())>200*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U'*U-eye())>200*%eps  then pause,end
if Err(U1*S1*V'-A) >200*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>200*%eps  then pause,end
if size(U1,2)<>3 then pause,end
if Err(U1*S1*V1'-A) >200*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>200*%eps  then pause,end
if Err(U-U1)>200*%eps  then pause,end
if Err(V-V1) >200*%eps  then pause,end
if rk<>3 then pause,end


//Large dimension
//Real Case Fat
A=rand(30,50);
S=svd(A);
if or(S<0) then pause,end
if sort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U'*U-eye())>1000*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U1'*U1-eye())>1000*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U'*U-eye())>1000*%eps  then pause,end
if Err(U1*S1*V'-A) >1000*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U-U1)>10*%eps  then pause,end
if Err(U1*S1*V1'-A) >1000*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>1000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(V-V1) >5000*%eps  then pause,end
if rk<>30 then pause,end

//Real Case Tall
A=rand(50,30);

S=svd(A);
if or(S<0) then pause,end
if sort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U1'*U1-eye())>5000*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end
if Err(U1*S1*V'-A) >5000*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if size(U1,2)<>30 then pause,end
if Err(U1*S1*V1'-A) >5000*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(V-V1) >5000*%eps  then pause,end
if rk<>30 then pause,end

//Complex Case Fat
A=rand(30,50)+%i*rand(30,50);
S=svd(A);
if or(S<0) then pause,end
if sort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U1'*U1-eye())>5000*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end
if Err(U1*S1*V'-A) >5000*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(U1*S1*V1'-A) >5000*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(V-V1) >5000*%eps  then pause,end
if rk<>30 then pause,end

//Complex Case Tall
A=rand(50,30)+%i*rand(50,30);

S=svd(A);
if or(S<0) then pause,end
if sort(S)<>S  then pause,end

[U,S1]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end

[U1,S1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U1'*U1-eye())>5000*%eps  then pause,end

[U1,S1,V]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U'*U-eye())>5000*%eps  then pause,end
if Err(U1*S1*V'-A) >5000*%eps  then pause,end

[U1,S1,V1]=svd(A,"e");
if Err(S-diag(S1))>5000*%eps  then pause,end
if size(U1,2)<>30 then pause,end
if Err(U1*S1*V1'-A) >5000*%eps  then pause,end

[U1,S1,V1,rk]=svd(A);
if Err(S-diag(S1))>5000*%eps  then pause,end
if Err(U-U1)>5000*%eps  then pause,end
if Err(V-V1) >5000*%eps  then pause,end
if rk<>30 then pause,end

function c=cond(A)
  if A==[] then c=1,else S=svd(A);c=S($)/S(1),end
endfunction

//==========================================================================
//==============================     hess     ============================== 
//==========================================================================

//Empty matrix
if hess([])<>[] then pause,end
[U,H]=hess([]);
if U<>[]|H<>[] then pause,end

if execstr('hess(rand(2,5))','errcatch')==0 then pause,end
if execstr('[U,H]=hess(rand(2,5))','errcatch')==0 then pause,end
if execstr('hess(rand(2,5)+%i)','errcatch')==0 then pause,end
if execstr('[U,H]=hess(rand(2,5)+%i)','errcatch')==0 then pause,end

//Small dimension
//Real case
A=rand(5,5);
H=hess(A);
[U,H1]=hess(A);
if Err(H-H1)>200*%eps then pause,end
if Err(U'*U-eye()) >200*%eps then pause,end
if Err(U'*A*U-H1)  >200*%eps then pause,end
//complex case
A=rand(5,5)+%i*rand(5,5);
H=hess(A);
[U,H1]=hess(A);
if Err(H-H1)>200*%eps then pause,end
if Err(U'*U-eye()) >200*%eps then pause,end
if Err(U'*A*U-H1)  >200*%eps then pause,end
//Large dimension
A=rand(20,20);
H=hess(A);
[U,H1]=hess(A);
if Err(H-H1)>200*%eps then pause,end
if Err(U'*U-eye()) >1000*%eps then pause,end
if Err(U'*A*U-H1)  >1000*%eps then pause,end
//complex case
A=rand(20,20)+%i*rand(20,20);
H=hess(A);
[U,H1]=hess(A);
if Err(H-H1)>1000*%eps then pause,end
if Err(U'*U-eye()) >1000*%eps then pause,end
if Err(U'*A*U-H1)  >1000*%eps then pause,end
//==========================================================================
//==============================     lsq      ============================== 
//==========================================================================

//Empty matrix
if lsq([],[])<>[] then pause,end
if execstr('lsq([],1)','errcatch')==0 then pause,end
if execstr('lsq(1,[])','errcatch')==0 then pause,end
if execstr('lsq(rand(3,2),rand(2,1))','errcatch')==0 then pause,end

//Small dimensions
//Real full rank fat
A=rand(3,5);b=rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
//Real rank deficient fat
A=[1 2 3;1 2 3];b=[4;5];
X=lsq(A,b);
if Err(A'*A*X-A'*b)> 200*%eps then pause,end
//Real  tall
A=[1 2;4 2;0 1];b=[1;1;1];
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:2,:))*b1(1:2);
if Err(X-X1)>200*%eps then pause,end
//Complex full rank fat
A=rand(3,5)+%i*rand(3,5);b=rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end

A=rand(3,5);b=rand(3,2)+%i*rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end

A=rand(3,5)+%i*rand(3,5);b=rand(3,2)+%i*rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
//Complex  rank deficient fat
A=[1 2 3;1 2 3]+%i;b=[4;5];
X=lsq(A,b);
A=[1 2 3;1 2 3];b=[4;5]+%i;
X=lsq(A,b);
if Err(A'*A*X-A'*b)>200*%eps then pause,end

if Err(A'*A*X-A'*b)>200*%eps then pause,end
A=[1 2 3;1 2 3]+%i;b=[4;5]+%i;
X=lsq(A,b);
if Err(A'*A*X-A'*b)>1000*%eps then pause,end

//Complex  full rank tall
A=[1 2;4 2;0 1]+%i;b=[1;1;1];
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:2,:))*b1(1:2);
if Err(X-X1)>200*%eps then pause,end

A=[1 2;4 2;0 1];b=[1;1;1]+%i;
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:2,:))*b1(1:2);
if Err(X-X1)>200*%eps then pause,end

A=[1 2;4 2;0 1]+%i;b=[1;1;1]+%i;
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:2,:))*b1(1:2);
if Err(X-X1)>200*%eps then pause,end



//LArge dimension
//Real full rank fat
A=rand(3,50);b=rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
//Real full rank tall
A=rand(50,3);b=rand(50,2);
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:3,:))*b1(1:3,:);
if Err(X-X1)>200*%eps then pause,end

//Complex full rank fat
A=rand(3,50)+%i;b=rand(3,2);
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
A=rand(3,50);b=rand(3,2)+%i;
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end

A=rand(3,50);b=rand(3,2)+%i;
X=lsq(A,b);
if Err(A*X-b)>200*%eps then pause,end
//Complex full rank tall
A=rand(50,3)+%i;b=rand(50,2);
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:3,:))*b1(1:3,:);
if Err(X-X1)>200*%eps then pause,end

A=rand(50,3);b=rand(50,2)+%i;
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:3,:))*b1(1:3,:);
if Err(X-X1)>200*%eps then pause,end
A=rand(50,3)+%i;b=rand(50,2)+%i;
X=lsq(A,b);
[Q,R]=qr(A);b1=Q'*b;X1=inv(R(1:3,:))*b1(1:3,:);
if Err(X-X1)>200*%eps then pause,end
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

//==========================================================================
//==============================    /         ============================== 
//==========================================================================
//function x=rdiv(A,B),x=slash(A,B),endfunction
function x=rdiv(A,B),x=A/B,endfunction
//scalar division
  
//-----Square matrix-----
n=5;  A=rand(n,n);b=rand(2,n+1);
if rdiv([],A) <>[] then pause,end
if execstr('rdiv(b,A)','errcatch')==0 then pause,end
//Small dimensions real
n=5;
b=rand(2,n);A=rand(n,n);
x=rdiv(b,A);
if Err(x*A-b)>200*%eps then pause,end
//Small dimensions complex
b=rand(2,n)+%i;A=rand(n,n);
x=rdiv(b,A);
if Err(x*A-b)>500*%eps then pause,end

b=rand(2,n);A=rand(n,n)+%i;
x=rdiv(b,A);
if Err(x*A-b)>500*%eps then pause,end

b=rand(2,n)+%i;A=rand(n,n)+%i;
x=rdiv(b,A);
if Err(x*A-b)>500*%eps then pause,end

//Large dimensions real
n=50;
b=rand(2,n);A=rand(n,n);
x=rdiv(b,A);
if Err(x*A-b)>10000*%eps then pause,end
//Small dimensions complex
b=rand(2,n)+%i;A=rand(n,n);
x=rdiv(b,A);
if Err(x*A-b)>10000*%eps then pause,end

b=rand(2,n);A=rand(n,n)+%i;
x=rdiv(b,A);
if Err(x*A-b)>10000*%eps then pause,end

b=rand(2,n)+%i;A=rand(n,n)+%i;
x=rdiv(b,A);
if Err(x*A-b)>10000*%eps then pause,end

//-----Rectangular matrix-----
n=5;m=3; A=rand(m,n);b=rand(2,n+1);
if rdiv([],A) <>[] then pause,end
if execstr('rdiv(b,A)','errcatch')==0 then pause,end

//Small dimensions real
n=5;m=3;
b=rand(2,n);A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end

n=3;m=5;
b=rand(2,n);A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end
//Small dimensions complex
n=5;m=3;
b=rand(2,n)+%i;A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end
n=5;m=3;
b=rand(2,n);A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end
b=rand(2,n)+%i;A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>200*%eps then pause,end

n=3;m=5;
b=rand(2,n)+%i;A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=3;m=5;
b=rand(2,n);A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=3;m=5;
b=rand(2,n)+%i;A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

//LArge dimension real

n=50;m=30;
b=rand(2,n);A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=30;m=50;
b=rand(2,n);A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end
//Large dimensions complex
n=50;m=30;
b=rand(2,n)+%i;A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end
n=50;m=30;
b=rand(2,n);A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end
b=rand(2,n)+%i;A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=30;m=50;
b=rand(2,n)+%i;A=rand(m,n);
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=30;m=50;
b=rand(2,n);A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

n=30;m=50;
b=rand(2,n)+%i;A=rand(m,n)+%i;
x=rdiv(b,A);
if Err(x*A*A'-b*A')>1000*%eps then pause,end

//==========================================================================
//==============================    \         ============================== 
//==========================================================================
function x=ldiv(A,B),x=A\B,endfunction
//scalar division
  
//-----Square matrix-----
n=5;  A=rand(n,n);b=rand(2,n+1);
if ldiv(A,[]) <>[] then pause,end
if execstr('ldiv(A,B)','errcatch')==0 then pause,end
//Small dimensions real
n=5;
b=rand(n,2);A=rand(n,n);
x=ldiv(A,b);
if Err(A*x-b)>200*%eps then pause,end
//Small dimensions complex
b=rand(n,2)+%i;A=rand(n,n);
x=ldiv(A,b);
if Err(A*x-b)>500*%eps then pause,end

b=rand(n,2);A=rand(n,n)+%i;
x=ldiv(A,b);
if Err(A*x-b)>200*%eps then pause,end


b=rand(n,2)+%i;A=rand(n,n)+%i;
x=ldiv(A,b);
if Err(A*x-b)>200*%eps then pause,end


//Large dimensions real
n=50;
b=rand(n,2);A=rand(n,n);
x=ldiv(A,b);
if Err(A*x-b)>10000*%eps then pause,end
//Small dimensions complex
b=rand(n,2)+%i;A=rand(n,n);
x=ldiv(A,b);
if Err(A*x-b)>50000*%eps then pause,end

b=rand(n,2);A=rand(n,n)+%i;
x=ldiv(A,b);
if Err(A*x-b)>50000*%eps then pause,end

b=rand(n,2)+%i;A=rand(n,n)+%i;
x=ldiv(A,b);
if Err(A*x-b)>50000*%eps then pause,end

//-----Rectangular matrix-----
n=5;m=3; A=rand(m,n);b=rand(n+1,2);
if ldiv(A,[]) <>[] then pause,end
if execstr('ldiv(A,b)','errcatch')==0 then pause,end

//Small dimensions real
n=5;m=3;
b=rand(m,2);A=rand(m,n);
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>200*%eps then pause,end

n=3;m=5;
b=rand(m,2);A=rand(m,n);
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end
//Small dimensions complex
n=5;m=3;
b=rand(m,2)+%i;A=rand(m,n);
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end
n=5;m=3;
b=rand(m,2);A=rand(m,n)+%i;
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end
b=rand(m,2)+%i;A=rand(m,n)+%i;
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end

n=3;m=5;
b=rand(m,2)+%i;A=rand(m,n);
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end

n=3;m=5;
b=rand(m,2);A=rand(m,n)+%i;
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end

n=3;m=5;
b=rand(m,2)+%i;A=rand(m,n)+%i;
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>1000*%eps then pause,end

//LArge dimension real

n=40;m=20;
b=rand(m,2);A=rand(m,n);
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end

b=rand(m,2);A=rand(m,n);
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end
//Large dimensions complex

b=rand(m,2)+%i;A=rand(m,n);
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end


b=rand(m,2);A=rand(m,n)+%i;
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end

b=rand(m,2)+%i;A=rand(m,n)+%i;
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end


b=rand(m,2)+%i;A=rand(m,n);
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end

b=rand(m,2);A=rand(m,n)+%i;
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end

b=rand(m,2)+%i;A=rand(m,n)+%i;
x=ldiv(A,b);
if Err(A'*A*x-A'*b)>10000*%eps then pause,end
//==========================================================================
//==============================   det        ============================== 
//==========================================================================
if execstr('det([1 2;3 4;5 6])','errcatch')==0 then pause,end
//Small dimension 
//Real
A=[1 1; 1 2];
if Err(det(A)-1)>10*%eps then pause,end
[e,m]=det(A);
if e<>0 |Err(m-1)>10*%eps then pause,end
//Complex
A=A+%i;
if Err(det(A)-1-%i)>10*%eps then pause,end
[e,m]=det(A);
if e<>0 |Err(m-1-%i)>10*%eps then pause,end
//Large dimension
//Real
v=rand(1,21);
A=rand(21,21); A=(triu(A,1)+diag(v))*(tril(A,-1)+diag(ones(1,21)));
if Err(det(A)-prod(v))>400000*%eps then pause,end
[e,m]=det(A);
if Err(m*(10^e)-prod(v))>400000*%eps then pause,end
//Complex
v=(v+rand(v)*%i)/2;
A=rand(21,21); A=(triu(A,1)+diag(v))*(tril(A,-1)+diag(ones(1,21)));
if Err(det(A)-prod(v))>10000*%eps then pause,end
[e,m]=det(A);
if Err(m*(10^e)-prod(v))>10000*%eps then pause,end
//==========================================================================
//==============================   balanc     ============================== 
//==========================================================================
function A=testmat3(a,n)
//eigen values are given by a dilation of nth roots of 1
A=diag(a*ones(1,n-1),1)+diag((1/a)*ones(1,n-1),-1)
A(1,n)=1/a;
endfunction
A=[];[Ab,X]=balanc(A);
if Ab<>[]|X<>[] then pause,end
// MATRIX
//Small dimension
//---------------
//Real Case
A=testmat3(10000,5);
[Ab,X]=balanc(A);
if Err(diag(diag(X))-X)>200*%eps then pause,end 
if Err(Ab-inv(X)*A*X)>200*%eps then pause,end 

//Complex Case
A=testmat3(10000+0.01*%i,5);
[Ab,X]=balanc(A);
if Err(diag(diag(X))-X)>200*%eps then pause,end 
if Err(Ab-inv(X)*A*X)>200*%eps then pause,end 

//LArge dimension
A=testmat3(10000,30);
[Ab,X]=balanc(A);
if Err(diag(diag(X))-X)>200*%eps then pause,end 
if Err(Ab-inv(X)*A*X)>1000*%eps then pause,end 

//Complex Case
A=testmat3(10000+0.01*%i,30);
[Ab,X]=balanc(A);
if Err(diag(diag(X))-X)>200*%eps then pause,end 
if Err(Ab-inv(X)*A*X)>1000*%eps then pause,end 

// PENCILS
//Small dimension
//---------------
//Real Case
A=testmat3(10000,5);B=testmat3(1000,5);
[Ab,Bb,X,Y]=balanc(A,B);
if Err(Bb-inv(X)*B*Y)>200*%eps then pause,end 
if Err(Ab-inv(X)*A*Y)>200*%eps then pause,end 

//complex case
A=testmat3(10000+0.001*%i,5);B=testmat3(1000+100*%i,5);
[Ab,Bb,X,Y]=balanc(A,B);
if Err(Bb-inv(X)*B*Y)>200*%eps then pause,end 
if Err(Ab-inv(X)*A*Y)>200*%eps then pause,end 


//Large dimension
//---------------
//Real Case
A=testmat3(10000,20);B=testmat3(1000,20);
[Ab,Bb,X,Y]=balanc(A,B);
if Err(Bb-inv(X)*B*Y)>1000*%eps then pause,end 
if Err(Ab-inv(X)*A*Y)>1000*%eps then pause,end 

//complex case
A=testmat3(10000+0.001*%i,20);B=testmat3(1000+100*%i,20);
[Ab,Bb,X,Y]=balanc(A,B);
if Err(Bb-inv(X)*B*Y)>1000*%eps then pause,end 
if Err(Ab-inv(X)*A*Y)>1000*%eps then pause,end 

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
