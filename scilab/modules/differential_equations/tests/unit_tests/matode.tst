// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

warning("off");

funcprot(0);

// Copyright INRIA
Leps=1.e-6;
//     dy1/dt = -.04*y1 + 1.e4*y2*y3
//     dy2/dt = .04*y1 - 1.e4*y2*y3 - 3.e7*y2**2
//     dy3/dt = 3.e7*y2**2
// on the interval from t = 0.0 to t = 4.e10, with initial conditions
// y1 = 1.0, y2 = y3 = 0.  the problem is stiff.
//     definition of rhs
function [ydot]=f(t,y)
    f1=-0.04*y(1)+1e4*y(2)*y(3)
    f3=3e7*y(2)*y(2)
    ydot=[f1;-f1-f3;f3]
endfunction

//     jacobian
function [jac]=j(t,y)
    jac(1,1)=-0.04;jac(1,2)=1.e4*y(3);jac(1,3)=1.e4*y(2)
    jac(3,1)=0
    jac(3,2)=6.e7*y(2)
    jac(3,3)=0
    jac(2,1)=-jac(1,1)
    jac(2,2)=-jac(1,2)-jac(3,2)
    jac(2,3)=-jac(1,3)
endfunction
//
y0=[1;0;0];
t0=0;
t1=[0.4,4];
nt=size(t1,"*");
//    solution
yref=[0.9851721 0.9055180;0.0000339 0.0000224;0.0147940 0.0944596];
//
//  1. fortran called by fydot, without jacobian
y1=ode(y0,t0,t1,"fex");
if max(y1-yref) > Leps then pause,end
//  2. fortran called by fydot, type given (stiff), no jacobian
y2=ode("stiff",y0,t0,t1,"fex");
if max(y2-yref) > Leps then pause,end
//  3. fortran called by fydot , fjac, type given
y3=ode("stiff",y0,t0,t1,"fex","jex");
if max(y3-yref) > Leps then pause,end
//   hot restart
[z,w,iw]=ode("stiff",y0,0,0.4,"fex","jex");
z=ode("stiff",z,0.4,4,"fex","jex",w,iw);
if max(z-y3(:,2)) > %eps then pause,end

[y1,w,iw]=ode(y0,t0,t1(1),"fex");
y2=ode(y0,t1(1),t1(2:nt),"fex",w,iw);
if max([y1 y2]-yref) > Leps then pause,end

[y1,w,iw]=ode(y0,t0,t1(1),"fex","jex");
y2=ode(y0,t1(1),t1(2:nt),"fex","jex",w,iw);
if max([y1 y2]-yref) > Leps then pause,end

//   variation of tolerances
atol=[0.001,0.0001,0.001];
rtol=atol;
//    externals
//  4. type given , scilab lhs ,jacobian not passed
y4=ode("stiff",y0,t0,t1(1),atol,rtol,f);
if max(y4(:,1)-yref(:,1)) > 0.01 then pause,end
//  5. type non given, rhs and scilab jacobian
y5=ode(y0,t0,t1,f,j);
if max(y5-yref) > Leps then pause,end
//  6. type given (stiff),rhs and jacobian  by scilab
y6=ode("stiff",y0,t0,t1,0.00001,0.00001,f,j);
if (y6-yref) > 2*0.00001 then pause,end
//  7. matrix rhs, type given(adams),jacobian not passed
//
a=rand(3,3);ea=expm(a);
function [ydot]=f(t,y)
    ydot=a*y
endfunction
t1=1;y=ode("adams",eye(a),t0,t1,f);
if max(ea-y) > Leps then pause,end
//
//   DAE's
//     dy1/dt = -.04*y1 + 1.e4*y2*y3
//     dy2/dt = .04*y1 - 1.e4*y2*y3 - 3.e7*y2**2
//       0.   = y1 + y2 + y3 - 1.
//  y1(0) = 1.0, y2(0) = y3(0) = 0.
// scilab macros
function [r]=resid(t,y,s)
    r(1)=-0.04*y(1)+1.d4*y(2)*y(3)-s(1)
    r(2)=0.04*y(1)-1.d4*y(2)*y(3)-3.d7*y(2)*y(2)-s(2)
    r(3)=y(1)+y(2)+y(3)-1
endfunction

function [p]=aplusp(t,y,p)
    p(1,1)=p(1,1)+1
    p(2,2)=p(2,2)+1
endfunction

//        %ODEOPTIONS tests
//
rand("seed",0);rand("normal");
nx=20;A=rand(nx,nx);A=A-4.5*eye();
function y=f(t,x)
    y=A*x
endfunction

function J=j(t,x)
    J=A
endfunction

x0=ones(nx,1);t0=0;t=[1,2,3,4,5];
nt=size(t,"*");
eAt=expm(A*t(nt));

//        Test itask=%ODEOPTIONS(1)

//itask=1  --->  usual call (t=vector of instants, solution at all t)
//========================
itask=1;tcrit=0;h0=0;hmax=0;hmin=0;ixpr=0;mxstep=0;maxordn=0;maxords=0;
jacflag=2;ml=-1;mu=-1;
%ODEOPTIONS=[itask,tcrit,h0,hmax,hmin,jacflag,mxstep,maxordn,maxords,ixpr,ml,mu];
xf=ode(x0,t0,t,f);   //lsoda
if norm(xf(:,nt)-eAt*x0)>Leps then pause,end
xfj=ode(x0,t0,t,f,j);   //lsoda with jacobian
if norm(xfj(:,nt)-eAt*x0)>Leps then pause,end


//itask=2;   --->  solution at mesh points  ---> t=tmax
//========================
%ODEOPTIONS(1)=2;tmax=t(5);
xft=ode(x0,t0,tmax,f);
[p,q]=size(xft);
xlast=xft(2:nx+1,q);
if xft(1,q)<tmax then pause,end
if norm(xlast-expm(A*xft(1,q))*x0)>Leps then pause,end

//itask=3;   ---> solution at first mesh point beyond t=tmax
%ODEOPTIONS(1)=3;
x3=ode(x0,t0,tmax,f);
if norm(x3(2:nx+1)-xlast,1)>Leps then pause,end

//itask=4; test with %ODEOPTIONS(2)=tcrit
%ODEOPTIONS(1)=4; //---> computation at all t and t>tcrit are not called
tcrit=2.5;%ODEOPTIONS(2)=tcrit;
chk=0;
function y=fcrit(t,x)
    if t<=tcrit then
        y=A*x;
    else
        y=A*x;
        chk=resume(1);
    end
endfunction

x42=ode(x0,t0,t,fcrit);
if chk==1 then pause,end
[p,q]=size(x42);
if norm(x42(:,q)-ode(x0,t0,tcrit,f),1)>Leps then pause,end

//itask=5; test with %ODEOPTIONS(2)=tcrit
%ODEOPTIONS(1)=5;  //---> computation at mesh points and t>tcrit are not called
%ODEOPTIONS(6)=2;  // Estimated jacobian
chk=0;
x52=ode(x0,t0,2.3,fcrit);
if chk==1 then pause,end
[p,q]=size(x52);
if x52(1,q)>tcrit then pause,end

//test of %ODEOPTIONS(3:5)=[h0,hmax,hmin]
%ODEOPTIONS(1)=1;
h0=0.0;hmax=0.1;hmin=0.0001;
%ODEOPTIONS(3:5)=[h0,hmax,hmin];
x35=ode(x0,t0,t,f);
if norm(x35-xf,1)>Leps then pause,end

//test of %ODEOPTIONS(6)=jacflag
%ODEOPTIONS(6)=1;//Jacobian given
%ODEOPTIONS(3:5)=[0 0 0];
x61=ode("stiff",x0,t0,t,f,j);   //with Jacobian
if norm(x61-xf,1)>10*Leps then pause,end


%ODEOPTIONS(6)=0; // jacobian nor called nor estimated
x60=ode("stiff",x0,t0,t,f,j);   //Jacobian not used (warning)
x60=ode("stiff",x0,t0,t,f);    //Jacobian not used
if norm(x60-x61,1)>10*Leps then pause,end


%ODEOPTIONS(6)=1;//Jacobian estimated
x60=ode("stiff",x0,t0,t,f);
if norm(x60-x61,1)>10*Leps then pause,end

//test of %ODEOPTIONS(6)=jacflag   (adams)
%ODEOPTIONS(6)=1;//with given Jacobian
x60=ode("adams",x0,t0,t,f,j) ;
if norm(x60-x61,1)>10*Leps then pause,end


%ODEOPTIONS(6)=0;// jacobian nor called nor estimated
x60=ode("adams",x0,t0,t,f,j);   //Jacobian not used (warning)
x60=ode("adams",x0,t0,t,f);    //Jacobian not used
if norm(x60-x61,1)>10*Leps then pause,end

// test lsoda
%ODEOPTIONS(6)=2;// jacobian  estimated
x60=ode(x0,t0,t,f);
if norm(x60-x61,1)>10*Leps then pause,end

%ODEOPTIONS(6)=1;//Jacobian estimated
x60=ode(x0,t0,t,f);
if norm(x60-x61,1)>10*Leps then pause,end


//   Banded Jacobian
itask=1;tcrit=0;h0=0;hmax=0;hmin=0;ixpr=0;mxstep=0;maxordn=0;maxords=0;
//provisional values as default
jacflag=2;ml=-1;mu=-1;

%ODEOPTIONS=[itask,tcrit,h0,hmax,hmin,jacflag,mxstep,maxordn,maxords,ixpr,ml,mu];
jacflag=2;%ODEOPTIONS(6)=jacflag;   //Banded Jacobian, given
nx=20;A=diag(-[1:nx]);x0=ones(nx,1);
t0=0;t=[1,2,3,4,5];
for k=1:nx-1, A(k,k+1)=1;end
for k=1:nx-2, A(k,k+2)=-2;end
for k=1:nx-1, A(k+1,k)=-1;end
for k=1:nx-2, A(k+2,k)=2;end
for k=1:nx-3, A(k+3,k)=-3;end
function xd=f(t,x)
    xd=A*x
endfunction

ml=3;mu=2;
%ODEOPTIONS(11:12)=[ml,mu];
for i=1:nx;
    for j=1:nx;
        if A(i,j)<>0 then J(i-j+mu+1,j)=A(i,j);end
end;end;
// J is a ml+mu+1 x ny matrix.
// Column 1 of J is made of mu zeros followed by df1/dx1, df2/dx1, df3/dx1,
// i.e. 1 + ml possibly nonzeros entries.
// Column 2 of J is made of mu-1 zeros followed by df1/dx2, df2/dx2, ...
// etc...
%ODEOPTIONS(6)=1;%ODEOPTIONS(11:12)=[-1,-1];
function jj=j1(t,x)
    jj=A
endfunction

xnotband=ode("stiff",x0,t0,t,f,j1);


%ODEOPTIONS(6)=4;//banded jacobian external given
%ODEOPTIONS(11:12)=[3,2];
function jj=j(t,x)
    jj=J
endfunction

xband=ode("stiff",x0,t0,t,f,j);
if norm(xnotband-xband,1)>Leps then pause,end

%ODEOPTIONS(6)=5;//banded jacobian evaluated
%ODEOPTIONS(11:12)=[3,2];
function jj=j(t,x)
    jj=J
endfunction
xband=ode("stiff",x0,t0,t,f,j);
if norm(xnotband-xband,1)>Leps then pause,end


//            Test of %ODEOPTIONS(7)
//%ODEOPTIONS(7)=mxstep  ---> maximum number od steps allowed
itask=1;tcrit=0;h0=0;hmax=0;hmin=0;ixpr=0;mxstep=0;maxordn=0;maxords=0;
//provisional values as default
jacflag=2;ml=-1;mu=-1;
%ODEOPTIONS=[itask,tcrit,h0,hmax,hmin,jacflag,mxstep,maxordn,maxords,ixpr,ml,mu];
%ODEOPTIONS(7)=10;
//ode(x0,t0,t,f);  // ---> Non convergence

//            Test of %ODEOPTIONS(8:9)
//%ODEOPTIONS(8:9)=[maxordn,maxords] ---> maximum order for nonstiff and stiff
itask=1;tcrit=0;h0=0;hmax=0;hmin=0;ixpr=0;mxstep=0;maxordn=0;maxords=0;
//provisional values as default
jacflag=2;ml=-1;mu=-1;
%ODEOPTIONS=[itask,tcrit,h0,hmax,hmin,jacflag,mxstep,maxordn,maxords,ixpr,ml,mu];
%ODEOPTIONS(8:9)=[0,0]; //---> default values
wref=ode(x0,t0,t,f); //just for computing reference

%ODEOPTIONS(8:9)=[4,3];
ww=ode(x0,t0,t,f);norm(wref-ww,1);

%ODEOPTIONS(8:9)=[12,5];
if norm(wref-ode(x0,t0,t,f),1)>Leps then pause,end

//using stiff method

%ODEOPTIONS(9)=0;
wref=ode("stiff",x0,t0,t,f);
%ODEOPTIONS(9)=5;
if norm(wref-ode("stiff",x0,t0,t,f),1) >Leps then pause,end //=0
%ODEOPTIONS(9)=4;
if norm(wref-ode("stiff",x0,t0,t,f),1)  >Leps then pause,end  //small


//using nonstiff method

%ODEOPTIONS(8)=0;
wref=ode("adams",x0,t0,t,f);
%ODEOPTIONS(8)=12;
if norm(wref-ode("adams",x0,t0,t,f),1) >Leps then pause,end  //=0
%ODEOPTIONS(8)=5;
if norm(wref-ode("adams",x0,t0,t,f),1) >Leps then pause,end   //small

//mixed
%ODEOPTIONS(8:9)=[5,12];
wref=ode(x0,t0,t,f);
%ODEOPTIONS(8:9)=[4,10];
if norm(ode(x0,t0,t,f)-wref,1)>Leps then pause,end   //small


A=diag([-10,-0.01,-1]);
function uu=u(t)
    uu=sin(t)
endfunction
B=rand(3,1);
function y=f(t,x)
    y=A*x+B*u(t)
endfunction

%ODEOPTIONS(1)=2;
yy1=ode("stiff",[1;1;1],0,1,f);
yy2=ode("stiff",[1;1;1],0,2,f);
%ODEOPTIONS(1)=3;
yy1=ode("stiff",[1;1;1],0,1,f);
yy2=ode("stiff",[1;1;1],0,2,f);

clear %ODEOPTIONS;
rand("seed",0);rand("normal");
nx=20;A=rand(nx,nx);A=A-4.5*eye();
function y=f(t,x)
    y=A*x
endfunction
function J=j(t,x)
    J=A
endfunction
//%ODEOPTIONS(1)=1;
y2=ode("stiff",ones(nx,1),0,2,f,j);
[y1,w,iw]=ode("stiff",ones(nx,1),0,1,f,j);
y2p=ode("stiff",y1,1,2,f,j,w,iw);
y12=ode("stiff",ones(nx,1),0,[1,2],f,j);
norm(y12(:,2)-y2p);
yaf=ode("adams",ones(nx,1),0,2,f,j);
yaj=ode("adams",ones(nx,1),0,2,f,j);
ysf=ode("stiff",ones(nx,1),0,2,f,j);
ysj=ode("stiff",ones(nx,1),0,2,f,j);

function xd=f(t,x)
    xd=A*x+B*sin(3*t)
endfunction

A=rand(10,10)-4.5*eye();B=rand(10,1);
x=ode(ones(10,1),0,[1,2,3],f);
//link('fexab.o','fexab')
if norm(x-ode(ones(10,1),0,[1,2,3],"fexab"),1) > 1.d-9 then pause,end

