// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//DASSL
// PROBLEM 1..   LINEAR DIFFERENTIAL/ALGEBRAIC SYSTEM
//
//X1DOT + 10.0*X1 = 0  
//X1 + X2 = 1
//X1(0) = 1.0, X2(0) = 0.0
//
t=1:10;t0=0;y0=[1;0];y0d=[-10;0];
info=list([],0,[],[],[],0,0);
//    Calling Scilab functions
deff('[r,ires]=dres1(t,y,ydot)','r=[ydot(1)+10*y(1);y(2)+y(1)-1];ires=0')
deff('pd=djac1(t,y,ydot,cj)','pd=[cj+10,0;1,1]')
//   scilab function, without jacobian
yy0=dassl([y0,y0d],t0,t,dres1,info);
//   scilab functions, with jacobian
yy1=dassl([y0,y0d],t0,t,dres1,djac1,info);
// fortran routine, without jacobian
yy2=dassl([y0,y0d],t0,t,'dres1',info);   //=yy0
assert_checkalmostequal(norm(yy2,1),norm(yy0,1),1E-5);
// fortran routines, with jacobian
yy3=dassl([y0,y0d],t0,t,'dres1','djac1',info);  //=yy1
assert_checkalmostequal(norm(yy3,1),norm(yy1,1),1E-5);
yy3bis=dassl([y0,y0d],t0,t,'dres1',djac1,info); 
// call fortran dres1 and scilab's djac1
yy3ter=dassl([y0,y0d],t0,t,dres1,'djac1',info);
//
// with specific atol and rtol parameters
atol=1.d-6;rtol=0;
yy4=dassl([y0,y0d],t0,t,atol,rtol,dres1,info);
yy5=dassl([y0,y0d],t0,t,atol,rtol,'dres1',info); //=yy4
assert_checkalmostequal(norm(yy5,1),norm(yy4,1),1E-9);
yy6=dassl([y0,y0d],t0,t,atol,rtol,dres1,djac1,info); 
yy7=dassl([y0,y0d],t0,t,atol,rtol,'dres1','djac1',info); //==yy6
assert_checkalmostequal(norm(yy7,1),norm(yy6,1),1E-12);
//    
//   Testing E xdot - A x=0
//   x(0)=x0;   xdot(0)=xd0
rand('seed',0);
nx=5;
E=rand(nx,1)*rand(1,nx);A=rand(nx,nx);
//         Index 1
[Si,Pi,Di,o]=penlaur(E,A);pp=Si*E;[q,m]=fullrf(pp);x0=q(:,1);x0d=pinv(E)*A*x0;
deff('[r,ires]=g(t,x,xdot)','r=E*xdot-A*x;ires=0');
t=[1,2,3];t0=0;info=list([],0,[],[],[],0,0);
x=dassl([x0,x0d],t0,t,g,info);x1=x(2:nx+1,:);
assert_checkalmostequal(norm(pp*x1,1),norm(x1,1),1.d-5);
//x(4) goes through 1 at  t=1.5409711;
t=1.5409711;ww=dassl([x0,x0d],t0,t,g,info);
assert_checkalmostequal(ww(5),1,0.001);
deff('[rt]=surface(t,y,yd)','rt=y(4)-1');nbsurf=1;
[yyy,nnn]=dasrt([x0,x0d],t0,t,g,nbsurf,surface,info);
deff('pd=j(t,y,ydot,cj)','pd=cj*E-A');
x=dassl([x0,x0d],t0,t,g,j,info);x2=x(2:nx+1,1);
assert_checkalmostequal(norm(x2,1),norm(ww(2:nx+1,1),1),0.0001);
[yyy1,nnn]=dasrt([x0,x0d],t0,t,g,j,nbsurf,surface,info);
//x0d is not known:
x0d=ones(x0);info(7)=1;
x=dassl([x0,x0d],t0,t,g,info);
xn=dassl([x0,x0d],t0,t,g,j,info);
assert_checkalmostequal(norm(x,1),norm(xn,1),0.00001);


//PROBLEM 2..

info=list([],0,[],[],[],0,0);
y0=zeros(25,1);y0(1)=1;
delta=0*y0;
//link('dres2.o','dres2');
//y0d=call('dres2',0,1,'d',y0,2,'d',delta,3,'d',0,5,'i',0,6,'d',0,7,'d','out',[25,1],4,'d');
y0d=zeros(y0);y0d(1)=-2;y0d(2)=1;y0d(6)=1;
t0=0;t=[0.01,0.1,1,10,100];
rtol=0;atol=1.d-6;
y=dassl([y0,y0d],t0,t,atol,rtol,'dres2',info);

//                 DASRT
// 
//C-----------------------------------------------------------------------
//C First problem.
//C The initial value problem is..
//C   DY/DT = ((2*LOG(Y) + 8)/T - 5)*Y,  Y(1) = 1,  1 .LE. T .LE. 6
//C The solution is  Y(T) = EXP(-T**2 + 5*T - 4), YPRIME(1) = 3
//C The two root functions are..
//C   G1 = ((2*LOG(Y)+8)/T - 5)*Y (= DY/DT)  (with root at T = 2.5),
//C   G2 = LOG(Y) - 2.2491  (with roots at T = 2.47 and 2.53)
//C-----------------------------------------------------------------------
y0=1;t=2:6;t0=1;y0d=3;
info=list([],0,[],[],[],0,0);
atol=1.d-6;rtol=0;ng=2;
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res1',ng,'gr1',info);
assert_checkalmostequal(nn(1),2.47,0.001);
y0=yy(2,2);y0d=yy(3,2);t0=nn(1);t=[3,4,5,6];
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res1',ng,'gr1',info);
assert_checkalmostequal(nn(1),2.5,0.001);
y0=yy(2,1);y0d=yy(3,1);t0=nn(1);t=[3,4,5,6];
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res1',ng,'gr1',info);
assert_checkalmostequal(nn(1),2.53,0.001);

deff('[delta,ires]=res1(t,y,ydot)','ires=0;delta=ydot-((2*log(y)+8)/t-5)*y')
deff('[rts]=gr1(t,y,yd)','rts=[((2*log(y)+8)/t-5)*y;log(y)-2.2491]')

y0=1;t=2:6;t0=1;y0d=3;
info=list([],0,[],[],[],0,0);
atol=1.d-6;rtol=0;ng=2;
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res1,ng,gr1,info);
assert_checkalmostequal(nn(1),2.47,0.001);
y0=yy(2,2);y0d=yy(3,2);t0=nn(1);t=[3,4,5,6];
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res1,ng,gr1,info);
assert_checkalmostequal(nn(1),2.5,0.001);
y0=yy(2,1);y0d=yy(3,1);t0=nn(1);t=[3,4,5,6];
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res1,ng,gr1,info);
assert_checkalmostequal(nn(1),2.53,0.001);

//C
//C-----------------------------------------------------------------------
//C Second problem (Van Der Pol oscillator).
//C The initial value problem is..
//C   DY1/DT = Y2,  DY2/DT = 100*(1 - Y1**2)*Y2 - Y1,
//C   Y1(0) = 2,  Y2(0) = 0,  0 .LE. T .LE. 200
//C   Y1PRIME(0) = 0, Y2PRIME(0) = -2
//C The root function is  G = Y1.
//C An analytic solution is not known, but the zeros of Y1 are known
//C to 15 figures for purposes of checking the accuracy.
//C-----------------------------------------------------------------------
rtol=[1.d-6;1.d-6];atol=[1.d-6;1.d-4];
t0=0;y0=[2;0];y0d=[0;-2];t=[20:20:200];ng=1;
info=list([],0,[],[],[],0,0);
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res2','jac2',ng,'gr2',info);
assert_checkalmostequal(nn(1),81.163512,0.001);

deff('[delta,ires]=res2(t,y,ydot)',...
'ires=0;y1=y(1),y2=y(2),delta=[ydot-[y2;100*(1-y1*y1)*y2-y1]]')
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res2,'jac2',ng,'gr2',info);
deff('J=jac2(t,y,ydot,c)','y1=y(1);y2=y(2);J=[c,-1;200*y1*y2+1,c-100*(1-y1*y1)]')
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,'gr2',info);
deff('s=gr2(t,y,yd)','s=y(1)')
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,gr2,info);

//           Hot Restart

[yy,nn,hotd]=dasrt([y0,y0d],t0,t,atol,rtol,'res2','jac2',ng,'gr2',info);
t01=nn(1);t=100:20:200;[pp,qq]=size(yy);y01=yy(2:3,qq);y0d1=yy(3:4,qq);
[yy,nn,hotd]=dasrt([y01,y0d1],t01,t,atol,rtol,'res2','jac2',ng,'gr2',info,hotd);
assert_checkalmostequal(nn(1),162.57763,0.004);

//Test of Dynamic link (Require f77!)
//         1 making the routines
res22=[...
'      SUBROUTINE RES22(T,Y,YDOT,DELTA,IRES,RPAR,IPAR)';
'      IMPLICIT DOUBLE PRECISION (A-H,O-Z)';
'      INTEGER NEQ';
'      DIMENSION Y(*), YDOT(*), DELTA(*)';
'      NEQ=2';
'      CALL F2(NEQ,T,Y,DELTA)';
'      DO 10 I = 1,NEQ';
'         DELTA(I) = YDOT(I) - DELTA(I)';
' 10   CONTINUE';
'      RETURN';
'      END';
'      SUBROUTINE F2 (NEQ, T, Y, YDOT)';
'      IMPLICIT DOUBLE PRECISION (A-H,O-Z)';
'      INTEGER NEQ';
'      DOUBLE PRECISION T, Y, YDOT';
'      DIMENSION Y(*), YDOT(*)';
'      YDOT(1) = Y(2)';
'      YDOT(2) = 100.0D0*(1.0D0 - Y(1)*Y(1))*Y(2) - Y(1)';
'      RETURN';
'      END';]

jac22=[...
'      SUBROUTINE JAC22 (T, Y, ydot, PD, CJ, RPAR, IPAR)';
' ';
'      IMPLICIT DOUBLE PRECISION (A-H,O-Z)';
'      INTEGER  NROWPD';
'      DOUBLE PRECISION T, Y, PD';
'      PARAMETER (NROWPD=2)';
'      DIMENSION Y(2), PD(NROWPD,2)';
'      PD(1,1) = 0.0D0';
'      PD(1,2) = 1.0D0';
'      PD(2,1) = -200.0D0*Y(1)*Y(2) - 1.0D0';
'      PD(2,2) = 100.0D0*(1.0D0 - Y(1)*Y(1))';
'      PD(1,1) = CJ - PD(1,1)';
'      PD(1,2) =    - PD(1,2)';
'      PD(2,1) =    - PD(2,1)';
'      PD(2,2) = CJ - PD(2,2)';
'      RETURN';
'      END';]


gr22=[...
'      SUBROUTINE GR22 (NEQ, T, Y, NG, GROOT, RPAR, IPAR)';
'      IMPLICIT DOUBLE PRECISION (A-H,O-Z)';
'      INTEGER NEQ, NG';
'      DOUBLE PRECISION T, Y, GROOT';
'      DIMENSION Y(*), GROOT(*)';
'      GROOT(1) = Y(1)';
'      RETURN';
'      END';]

//Uncomment lines below: link may be machine dependent if some f77 libs are 
//needed for linking
//unix_g('cd /tmp;rm -f /tmp/res22.f');unix_g('cd /tmp;rm -f /tmp/gr22.f');
//unix_g('cd /tmp;rm -f /tmp/jac22.f');
//write('/tmp/res22.f',res22);write('/tmp/gr22.f',gr22);write('/tmp/jac22.f',jac22)
//unix_g("cd /tmp;make /tmp/res22.o");unix_g('cd /tmp;make /tmp/gr22.o');
//unix_g('cd /tmp;make /tmp/jac22.o');
//          2  Linking the routines
//link('/tmp/res22.o','res22');link('/tmp/jac22.o','jac22');link('/tmp/gr22.o','gr22')
//rtol=[1.d-6;1.d-6];atol=[1.d-6;1.d-4];
//t0=0;y0=[2;0];y0d=[0;-2];t=[20:20:200];ng=1;
//info=list([],0,[],[],[],0,0);
//          3 Calling the routines by dasrt
//[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res22','jac22',ng,'gr22',info);
// hot restart
//[yy,nn,hotd]=dasrt([y0,y0d],t0,t,atol,rtol,'res22','jac22',ng,'gr22',info);
//t01=nn(1);t=100:20:200;[pp,qq]=size(yy);y01=yy(2:3,qq);y0d1=yy(3:4,qq);
//[yy,nn,hotd]=dasrt([y01,y0d1],t01,t,atol,rtol,'res22','jac22',ng,'gr22',info,hotd);
