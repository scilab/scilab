//Copyright INRIA

//-----------------------------------------------------------------------
// Second problem (Van Der Pol oscillator).
// The initial value problem is..
//   DY1/DT = Y2,  DY2/DT = 100*(1 - Y1**2)*Y2 - Y1,
//   Y1(0) = 2,  Y2(0) = 0,  0 .LE. T .LE. 200
//   Y1PRIME(0) = 0, Y2PRIME(0) = -2
// The root function is  G = Y1.
// An analytic solution is not known, but the zeros of Y1 are known
// to 15 figures for purposes of checking the accuracy.
//-----------------------------------------------------------------------
Eps=1.e-5
rtol=[1.d-6;1.d-6];atol=[1.d-6;1.d-4];
t0=0;y0=[2;0];y0d=[0;-2];t=[20:20:200];ng=1;
info=list([],0,[],[],[],0,0);
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,'res2','jac2',ng,'gr2',info);
if abs(nn(1)-81.163512)  > Eps then pause,end

deff('[delta,ires]=res2(t,y,ydot)',...
'ires=0;y1=y(1),y2=y(2),delta=[ydot-[y2;100*(1-y1*y1)*y2-y1]]')
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res2,'jac2',ng,'gr2',info);
deff('J=jac2(t,y,ydot,c)','y1=y(1);y2=y(2);J=[c,-1;200*y1*y2+1,c-100*(1-y1*y1)]')
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,'gr2',info);
deff('s=gr2(t,y,yd)','s=y(1)')
[yy,nn]=dasrt([y0,y0d],t0,t,atol,rtol,res2,jac2,ng,gr2,info);

//           Hot Restart

[yy,nn,hot]=dasrt([y0,y0d],t0,t,atol,rtol,'res2','jac2',ng,'gr2',info);
t01=nn(1);t=100:20:200;[pp,qq]=size(yy);y01=yy(2:3,qq);y0d1=yy(3:4,qq);
[yy,nn,hot]=dasrt([y01,y0d1],t01,t,atol,rtol,'res2','jac2',ng,'gr2',info,hot);
if abs(nn(1)-162.57763) > Eps then pause,end
