//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
K=0.001;      // Minimum relative prediction error in the nyquist plan 
Epss=0.002;   // minimum frequency distance with a singularity
nptmax=5000;  //maximum number of discretization points
pas=100/(2*%pi);
s=%s;

h=syslin('c',1/%s);n=1;


[f,bnds,split]=calfrq(h,0.01,100);
if split<>1 then pause,end
rf=freq(h.num,h.den,2*%pi*%i*f);
//finite difference derivative estimate
ddf=diff(f)/pas;
drf=(freq(h.num,h.den,2*%pi*%i*(f(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
if max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])>K then pause,end

h=syslin('c',[1/(%s+0.5);1/(%s+0.3)]);n=2;
[f,bnds,split]=calfrq(h,0.01,100);
if split<>1 then pause,end
rf=freq(h.num,h.den,2*%pi*%i*f);
//finite difference derivative estimate
ddf=diff(f)/pas;
drf=(freq(h.num,h.den,2*%pi*%i*(f(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
if max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])>K then pause,end



h=syslin('c',(%s^2+2*0.9*10*%s+100)/(%s^2+2*0.3*10.1*%s+102.01));
//h=h*syslin('c',(%s)/(%s^2+81)) ;
n=1;
[f,bnds,split]=calfrq(h,0.01,100);
if size(split,1)<>1 then pause,end

rf=freq(h.num,h.den,2*%pi*%i*f);
//finite difference derivative estimate
ddf=diff(f)/100;
drf=(freq(h.num,h.den,2*%pi*%i*(f(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*100);
if max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])>K then pause,end






//case with singularity
h=syslin('c',(%s)/(%s^2+81)) ;
sing=9/(2*%pi);
n=1;
[f,bnds,split]=calfrq(h,0.01,100);
if size(split,2)<>2 then pause,end
ks=split(2);
if abs(f(ks-1)-f(ks))*2*%pi<Epss then pause,end
if f(ks-1)>sing|f(ks)<sing then pause,end

//finite difference derivative estimate
f1=f(1:ks-15);//remove points near singularity for which pasmin constraint may be active
rf=freq(h.num,h.den,2*%pi*%i*f1);
ddf=diff(f1)/pas;
drf=(freq(h.num,h.den,2*%pi*%i*(f1(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
if max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])>K then pause,end
f1=f(ks+15:$);//remove points near singularity for which pasmin constraint may be active
rf=freq(h.num,h.den,2*%pi*%i*f1);
ddf=diff(f1)/pas;
drf=(freq(h.num,h.den,2*%pi*%i*(f1(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
if max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])>K then pause,end


//state space case
sl=tf2ss(h);
n=1;
[f1,bnds1,split1]=calfrq(sl,0.01,100);
if norm(f-f1)>1d-13 then pause,end
if or(split<>split1) then pause,end

//discrete case
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01));
hd=ss2tf(dscr(h,0.01));
[f,bnds,split]=calfrq(hd,0.00001,3);
if size(split,1)<>1 then pause,end

rf=freq(h.num,h.den,exp(2*%pi*%i*f));
//finite difference derivative estimate
ddf=diff(f)/pas;
drf=(freq(h.num,h.den,exp(2*%pi*%i*(f(1:$-1)+ddf)))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
if max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])>K then pause,end
