//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2016 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
K=0.001;      // Minimum relative prediction error in the nyquist plan
Epss=0.002;   // minimum frequency distance with a singularity
nptmax=5000;  //maximum number of discretization points
pas=100/(2*%pi);
s=%s;

h=syslin("c",1/%s);n=1;


[f,bnds,split]=calfrq(h,0.01,100);
assert_checktrue(split==1);
rf=freq(h.num,h.den,2*%pi*%i*f);
//finite difference derivative estimate
ddf=diff(f)/pas;
drf=(freq(h.num,h.den,2*%pi*%i*(f(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
assert_checktrue(max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])<=K);

[f1,bnds1,split1]=calfrq(tf2ss(h),0.01,100);
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);

[f1,bnds1,split1]=calfrq(zpk(h),0.01,100);
save TMPDIR/fii f1 bnds1 split1 f bnds split
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);
return

h=syslin("c",[1/(%s+0.5);1/(%s+0.3)]);n=2;
[f,bnds,split]=calfrq(h,0.01,100);
if split<>1 then pause,end
rf=freq(h.num,h.den,2*%pi*%i*f);
//finite difference derivative estimate
ddf=diff(f)/pas;
drf=(freq(h.num,h.den,2*%pi*%i*(f(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
assert_checktrue(max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])<=K);

[f1,bnds1,split1]=calfrq(tf2ss(h),0.01,100);
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);
[f1,bnds1,split1]=calfrq(zpk(h),0.01,100);
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);




h=syslin("c",(%s^2+2*0.9*10*%s+100)/(%s^2+2*0.3*10.1*%s+102.01));
//h=h*syslin('c',(%s)/(%s^2+81)) ;
n=1;
[f,bnds,split]=calfrq(h,0.01,100);
assert_checktrue(size(split,1)==1);

rf=freq(h.num,h.den,2*%pi*%i*f);
//finite difference derivative estimate
ddf=diff(f)/100;
drf=(freq(h.num,h.den,2*%pi*%i*(f(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*100);
assert_checktrue(max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])<=K);

[f1,bnds1,split1]=calfrq(tf2ss(h),0.01,100);
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);
[f1,bnds1,split1]=calfrq(zpk(h),0.01,100);
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);


//case with singularity
h=syslin("c",(%s)/(%s^2+81)) ;
sing=9/(2*%pi);
n=1;
[f,bnds,split]=calfrq(h,0.01,100);
assert_checktrue(size(split,"*")==2);
ks=split(2);
assert_checktrue(abs(f(ks-1)-f(ks))*2*%pi>=Epss);
assert_checktrue(f(ks-1)<=sing&f(ks)>=sing);

//finite difference derivative estimate
f1=f(1:ks-15);//remove points near singularity for which pasmin constraint may be active
rf=freq(h.num,h.den,2*%pi*%i*f1);
ddf=diff(f1)/pas;
drf=(freq(h.num,h.den,2*%pi*%i*(f1(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
assert_checktrue(max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])<=K);

f1=f(ks+15:$);//remove points near singularity for which pasmin constraint may be active
rf=freq(h.num,h.den,2*%pi*%i*f1);
ddf=diff(f1)/pas;
drf=(freq(h.num,h.den,2*%pi*%i*(f1(1:$-1)+ddf))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
assert_checktrue(max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])<=K);


//state space case
sl=tf2ss(h);
n=1;
[f1,bnds1,split1]=calfrq(sl,0.01,100);
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);
// zpk case
sl=zpk(h);
n=1;
[f1,bnds1,split1]=calfrq(sl,0.01,100);
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);



//discrete case
h=syslin("c",(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01));
hd=ss2tf(dscr(h,0.01));
[f,bnds,split]=calfrq(hd,0.00001,3);
assert_checktrue(size(split,"*")==1);

rf=freq(h.num,h.den,exp(2*%pi*%i*f));
//finite difference derivative estimate
ddf=diff(f)/pas;
drf=(freq(h.num,h.den,exp(2*%pi*%i*(f(1:$-1)+ddf)))-rf(:,1:$-1));
//error between computed and extrapolated value
e=rf(:,2:$)-(rf(:,1:$-1)+drf*pas);
assert_checktrue(max([abs(real(e))/max(bnds(2)-bnds(1),1); abs(imag(e))/max(bnds(4)- bnds(3),1)])<=K);

[f1,bnds1,split1]=calfrq(tf2ss(hd),0.00001,3);
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);
[f1,bnds1,split1]=calfrq(zpk(hd),0.00001,3);
assert_checkalmostequal(f,f1);
assert_checkalmostequal(bnds,bnds1);
assert_checkequal(split,split1);
