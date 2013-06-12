//Copyright INRIA
Eps=1.e-5
//        %ODEOPTIONS
//
rand("seed",0);rand("normal");
nx=20;A=rand(nx,nx);A=A-4.5*eye();
deff("y=f(t,x)","y=A*x")
deff("J=j(t,x)","J=A")
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
if norm(xf(:,nt)-eAt*x0)  > Eps then pause,end
xfj=ode(x0,t0,t,f,j);   //lsoda with jacobian
if norm(xfj(:,nt)-eAt*x0)  > Eps then pause,end


//itask=2;   --->  solution at mesh points  ---> t=tmax
//========================
%ODEOPTIONS(1)=2;tmax=t(5);
xft=ode(x0,t0,tmax,f);
[p,q]=size(xft);
xlast=xft(2:nx+1,q);
if xft(1,q)<tmax then pause,end
if norm(xlast-expm(A*xft(1,q))*x0) > Eps  then pause,end

//itask=3;   ---> solution at first mesh point beyond t=tmax
%ODEOPTIONS(1)=3;
x3=ode(x0,t0,tmax,f);
if norm(x3(2:nx+1)-xlast,1) > Eps then pause,end

//itask=4; test with %ODEOPTIONS(2)=tcrit
%ODEOPTIONS(1)=4; //---> computation at all t and t>tcrit are not called
tcrit=2.5;%ODEOPTIONS(2)=tcrit;
chk=0;
deff("y=fcrit(t,x)",["if t<=tcrit then"
" y=A*x;"
"else"
" y=A*x;chk=resume(1);end"])
x42=ode(x0,t0,t,fcrit);
if chk==1 then pause,end
[p,q]=size(x42);
if norm(x42(:,q)-ode(x0,t0,tcrit,f),1) > Eps then pause,end

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
if norm(x35-xf,1)  > Eps then pause,end

//test of %ODEOPTIONS(6)=jacflag
%ODEOPTIONS(6)=1;//Jacobian given
%ODEOPTIONS(3:5)=[0 0 0];
x61=ode("st",x0,t0,t,f,j);   //with Jacobian
if norm (x61-xf,1) > Eps then pause,end


%ODEOPTIONS(6)=0; // jacobian nor called nor estimated
x60=ode("st",x0,t0,t,f,j);   //Jacobian not used (warning)
x60=ode("st",x0,t0,t,f);    //Jacobian not used
if norm (x60-x61,1)  > Eps then pause,end


%ODEOPTIONS(6)=1;//Jacobian estimated
x60=ode("st",x0,t0,t,f)  ;
if norm (x60-x61,1) > Eps then pause,end

//test of %ODEOPTIONS(6)=jacflag   (adams)
%ODEOPTIONS(6)=1;//with given Jacobian
x60=ode("ad",x0,t0,t,f,j) ;
if norm (x60-x61,1)  > Eps then pause,end


%ODEOPTIONS(6)=0;// jacobian nor called nor estimated
x60=ode("ad",x0,t0,t,f,j);   //Jacobian not used (warning)
x60=ode("ad",x0,t0,t,f);    //Jacobian not used
if norm (x60-x61,1) > Eps then pause,end

// test lsoda
%ODEOPTIONS(6)=2;// jacobian  estimated
x60=ode(x0,t0,t,f);
if norm (x60-x61,1) > Eps then pause,end

%ODEOPTIONS(6)=1;//Jacobian estimated
x60=ode(x0,t0,t,f);
if norm (x60-x61,1) > Eps then pause,end


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
clear f;
deff("xd=f(t,x)","xd=A*x")
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
deff("jj=j1(t,x)","jj=A")
xnotband=ode("st",x0,t0,t,f,j1);


%ODEOPTIONS(6)=4;//banded jacobian external given
%ODEOPTIONS(11:12)=[3,2];
deff("jj=j(t,x)","jj=J")
xband=ode("st",x0,t0,t,f,j);
if norm (xnotband-xband,1)  > Eps then pause,end

%ODEOPTIONS(6)=5;//banded jacobian evaluated
%ODEOPTIONS(11:12)=[3,2];
deff("jj=j(t,x)","jj=J")
xband=ode("st",x0,t0,t,f,j);
if norm (xnotband-xband,1) > Eps then pause,end


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
ww=ode(x0,t0,t,f);
if norm (wref-ww,1) > Eps then pause,end

%ODEOPTIONS(8:9)=[12,5];
if norm (wref-ode(x0,t0,t,f),1) > Eps then pause,end

//using stiff method

%ODEOPTIONS(9)=0;
wref=ode("st",x0,t0,t,f);
%ODEOPTIONS(9)=5;
if norm (wref-ode("st",x0,t0,t,f),1)  > Eps then pause,end
%ODEOPTIONS(9)=4;
if norm (wref-ode("st",x0,t0,t,f),1) > Eps then pause,end


//using nonstiff method

%ODEOPTIONS(8)=0;
wref=ode("ad",x0,t0,t,f);
%ODEOPTIONS(8)=12;
if norm (wref-ode("ad",x0,t0,t,f),1)  > Eps then pause,end
%ODEOPTIONS(8)=5;
if norm (wref-ode("ad",x0,t0,t,f),1) > Eps then pause,end

//mixed
%ODEOPTIONS(8:9)=[5,12];
wref=ode(x0,t0,t,f);
%ODEOPTIONS(8:9)=[4,10];
if norm (ode(x0,t0,t,f)-wref,1) > Eps then pause,end


A=diag([-10,-0.01,-1]);
deff("uu=u(t)","uu=sin(t)");
B=rand(3,1);
deff("y=f(t,x)","y=A*x+B*u(t)")
%ODEOPTIONS(1)=2;
yy1=ode("stiff",[1;1;1],0,1,f);
yy2=ode("stiff",[1;1;1],0,2,f);
%ODEOPTIONS(1)=3;
yy1=ode("stiff",[1;1;1],0,1,f);
yy2=ode("stiff",[1;1;1],0,2,f);

clear %ODEOPTIONS;
rand("seed",0);rand("normal");
nx=20;A=rand(nx,nx);A=A-4.5*eye();
clear f;
deff("y=f(t,x)","y=A*x")
clear j;
deff("J=j(t,x)","J=A")
//%ODEOPTIONS(1)=1;
y2=ode("stiff",ones(nx,1),0,2,f,j);
[y1,w,iw]=ode("stiff",ones(nx,1),0,1,f,j);
y2p=ode("stiff",y1,1,2,f,j,w,iw);
y12=ode("stiff",ones(nx,1),0,[1,2],f,j);
if norm (y12(:,2)-y2p) > Eps then pause,end
yaf=ode("adams",ones(nx,1),0,2,f,j);
yaj=ode("adams",ones(nx,1),0,2,f,j);
ysf=ode("stiff",ones(nx,1),0,2,f,j);
ysj=ode("stiff",ones(nx,1),0,2,f,j);



