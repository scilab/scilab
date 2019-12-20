// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
Q=diag([2,5]);R=2;S=[1 1];
//full state LQ controller Continuous time plant
//----------------------------------------------
//S=0
dcmotor=syslin("c",[-10,1;-0.02,-2],[0;2],[]);

//long syntax
[K1,X1]=lqr(dcmotor,Q,R);
//check stability
assert_checktrue(real(spec(dcmotor.A+dcmotor.B*K1))<0);
//Check Riccati equation
assert_checktrue(norm(dcmotor.A'*X1+X1*dcmotor.A-X1*(dcmotor.B/R)*dcmotor.B'*X1+Q,1)<1e-10);

//Augmented plant syntax
[w,wp]=fullrf(blockdiag(Q,R));  //[C1,D12]'*[C1,D12]=Big
dcmotor.C=wp(:,1:2);
dcmotor.D=wp(:,3:$);
[K2,X2]=lqr(dcmotor);
assert_checktrue(norm(X2-X1,1)<1e-12);
assert_checktrue(norm(K2-K1,1)<1e-12);

//Non zero S term
dcmotor=syslin("c",[-10,1;-0.02,-2],[0;2],[]);

//long syntax
[K1,X1]=lqr(dcmotor,Q,R,S);
//check stability
assert_checktrue(real(spec(dcmotor.A+dcmotor.B*K1))<0);
//Check Riccati equation
Ric=(dcmotor.A-dcmotor.B/R*S)'*X1+X1*(dcmotor.A-dcmotor.B/R*S)-X1*(dcmotor.B/R)*dcmotor.B'*X1+Q-S'/R*S;
assert_checktrue(norm(Ric,1)<1e-10);

//Augmented plant syntax
[w,wp]=fullrf([Q S';S R]);  //[C1,D12]'*[C1,D12]=Big
dcmotor.C=wp(:,1:2);
dcmotor.D=wp(:,3:$);
[K2,X2]=lqr(dcmotor);
assert_checktrue(norm(X2-X1,1)<1e-12);
assert_checktrue(norm(K2-K1,1)<1e-12);



//full state LQ controller discrete time plant
//----------------------------------------------
dcmotor=dscr(syslin("c",[-10,1;-0.02,-2],[0;2],[]),0.01); 
//long syntax
[K1,X1]=lqr(dcmotor,Q,R);
//check stability
assert_checktrue(abs(spec(dcmotor.A+dcmotor.B*K1))<1);
//Check Riccati equation
Ric=dcmotor.A'*X1*dcmotor.A-(dcmotor.A'*X1*dcmotor.B)*pinv(dcmotor.B'*X1*dcmotor.B+R)*(dcmotor.B'*X1*dcmotor.A)+Q-X1;
assert_checktrue(norm(Ric,1)<1e-10);

//Augmented plant syntax
[w,wp]=fullrf(blockdiag(Q,R));  //[C1,D12]'*[C1,D12]=Big
dcmotor.C=wp(:,1:2);
dcmotor.D=wp(:,3:$);
[K2,X2]=lqr(dcmotor);
assert_checktrue(norm(X2-X1,1)<1e-10);
assert_checktrue(norm(K2-K1,1)<1e-10);


//non zero S term
dcmotor=dscr(syslin("c",[-10,1;-0.02,-2],[0;2],[]),0.01); 
//long syntax
[K1,X1]=lqr(dcmotor,Q,R,S);
//check stability
assert_checktrue(abs(spec(dcmotor.A+dcmotor.B*K1))<1);
//Check Riccati equation
Ric=dcmotor.A'*X1*dcmotor.A-(dcmotor.A'*X1*dcmotor.B+S')*pinv(dcmotor.B'*X1*dcmotor.B+R)*(dcmotor.B'*X1*dcmotor.A+S)+Q-X1;
assert_checktrue(norm(Ric,1)<1e-10);

//Augmented plant syntax
[w,wp]=fullrf([Q S';S R]);  //[C1,D12]'*[C1,D12]=Big
dcmotor.C=wp(:,1:2);
dcmotor.D=wp(:,3:$);
[K2,X2]=lqr(dcmotor);
assert_checktrue(norm(X2-X1,1)<1e-10);
assert_checktrue(norm(K2-K1,1)<1e-10);
