//Copyright INRIA

//                 DASRT
// 
//-----------------------------------------------------------------------
// First problem.
// The initial value problem is..
//   DY/DT = ((2*LOG(Y) + 8)/T - 5)*Y,  Y(1) = 1,  1 .LE. T .LE. 6
// The solution is  Y(T) = EXP(-T**2 + 5*T - 4), YPRIME(1) = 3
// The two root functions are..
//   G1 = ((2*LOG(Y)+8)/T - 5)*Y (= DY/DT)  (with root at T = 2.5),
//   G2 = LOG(Y) - 2.2491  (with roots at T = 2.47 and 2.53)
//-----------------------------------------------------------------------
Eps=1.e-3
y0=1;t=2:6;t0=1;y0d=3;
info=list([],0,[],[],[],0,0);
atol=1.d-6;rtol=0;ng=2;
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res1',ng,'gr1',info);
if abs(nn(1)-2.47) > Eps then pause,end
y0=yy(2,2);y0d=yy(3,2);t0=nn(1);t=[3,4,5,6];
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res1',ng,'gr1',info);
if abs(nn(1)-2.5)  > Eps then pause,end
y0=yy(2,1);y0d=yy(3,1);t0=nn(1);t=[3,4,5,6];
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res1',ng,'gr1',info);
if abs(nn(1)-2.53) > Eps then pause,end

deff('[delta,ires]=res1(t,y,ydot)','ires=0;delta=ydot-((2*log(y)+8)/t-5)*y')
deff('[rts]=gr1(t,y,yd)','rts=[((2*log(y)+8)/t-5)*y;log(y)-2.2491]')

y0=1;t=2:6;t0=1;y0d=3;
info=list([],0,[],[],[],0,0);
atol=1.d-6;rtol=0;ng=2;
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res1,ng,gr1,info);
if abs(nn(1)-2.47)  > Eps then pause,end
y0=yy(2,2);y0d=yy(3,2);t0=nn(1);t=[3,4,5,6];
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res1,ng,gr1,info);
if abs(nn(1)-2.5)  > Eps then pause,end
y0=yy(2,1);y0d=yy(3,1);t0=nn(1);t=[3,4,5,6];
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res1,ng,gr1,info);
if abs(nn(1)-2.53)  > Eps then pause,end

