// Copyright INRIA
// test conversion transfer <---> state-space
// 1- coff inversion 
 s=poly(0,'s'); a=[1 2 3;4 5 6;7 8 9];
 [num,den]=coff(a,'s');h1=num/den;h2=(s*eye()-a)^(-1);
 e=h1-h2;if norm(coeff(e(2)))>5000*%eps then pause,end
// 2-test de tf2ss et ss2tf
 n=[1+s   2+3*s+4*s^2        5; 0        1-s             s];
 d=[1+3*s   5-s^3           s+1;1+s     1+s+s^2      3*s-1];
 
 h=syslin('c',n./d);
 [n,d]=simp(n,d);if h<>rlist(n,d,'c') then pause,end
 sl=tf2ss(h); e=h-ss2tf(sl);
 if norm(coeff(e(2)))>100000*%eps then pause,end
 
 
//
  a = [0.8604043 , 0.0070020 , 0.6223373 , -1.356213 , -4.2301775
       0.159714 ,  0.0857791 , -0.2367751 , 0.4958580 , 0.6398817
      -4.3054931 , -0.0365878 , 2.1784911 , 0.0314793 , 2.3728994
      -3.7115779 , -0.5370809 , 2.4291716 , -0.6352663 , 2.6972781
       6.3580966 , 0.1377712 , -4.0461243 , -0.5192899 , -4.0394083];
  b = [-0.0532544 , -0.1494083 , -0.0098619
       -0.0355030 , -0.0162722 , 0.1045365
       -0.1301775 , 0.1736686 , -0.0611440
        0.1834320 , 0.1757396 , -0.5956607
        0.1775148 , -0.1186391 , 0.1439842];
  c = [2 , 7 , -2 , 5 , 1
       0 , -1 , 3 , 0 , 2];
  d = [1 , 0 , 0
       0 , 0 , 0];
 
sl=syslin('c',a,b,c);
if or(sl<>lsslist(a,b,c,0*ones(2,3),0*ones(5,1),'c')) then pause,end
//
eps=sqrt(%eps);
if contr(a,b,eps)<>4 then pause,end
if contr(a',c',eps)<>3 then pause,end
spec(a);
xbasc();xselect();
plzr(sl)
//
slc=contrss(sl,eps);//ssprint(slc)
slo=obsvss(sl,eps);//ssprint(slo)
slm=minss(sl,eps);//ssprint(slm)
//
hm=ss2tf(slm);
h=ss2tf(sl);
hh=c*(s*eye()-a)^(-1)*b + 0*ones(2,3);
hh=hh-h;
if norm(coeff(hh(2))) > 1.e-5 then pause,end
[num,den]=coff(a,'s');
hh=c*real(num)*b/real(den) + 0*ones(2,3);
hh=hh-h;
if norm(coeff(hh(2))) > 1.e-5 then pause,end
slh=tf2ss(hm);       //was tf2ss(h)
//
u=eye(3,60);
xbasc();
halt();
plot2d1("enn",1,flts(u,dscr(slh,0.3))');
plot2d1("enn",1,flts(u,dscr(sl,0.3))',[-3,-4],"101")
 
//csim  flts
//definition
ti=2.7;k=0.87;td=0.69;n=200;
 
a=[0 0 0 0 0 -1/ti
   0 -n/td 0 0 0 n/td
   k n -1 0 0 -k-n
   0 0 1 -1 0 0
   0 0 0 1 -1 0
   0 0 0 0 1 -1];
 
b=[1/ti;0;k;0;0;0];
 
c=[0 0 0 0 0 1];
tech=0.2;t=0:tech:15; //
deff('[y]=u(t)','if t==0 then y=0;else y=1,end') //step
 
// with csim
if type(csim)<>13 then comp(csim);end
sl=syslin('c',a,b,c);
//comparison 
// csim
xbasc(xget("window"));
plot2d(t',csim(u,t,sl)')
plot2d(t',csim('ech',t,sl)',-2,"001")
//exact discretization
sld=dscr(sl,tech);
plot2d(t',flts(ones(t),sld)',-3,"001")
//
//impulse responses
//
halt();xbasc();
plot2d(t',csim('imp',t,sl)')
//discretization
plot2d(t',flts(eye(t)/tech,sld)',-2,"001");
//fin
halt();xbasc();
//test bode - black et nyquist
 s=poly(0,'s')
// n=poly(1,'s','c'); d=real(poly([5+15*%i,5-15*%i],'s'))
 n=1+s;d=1+2*s;
 h=syslin('c',n,d)
 sl=tf2ss(h);
 sld=dscr(sl,0.01);
 hd=ss2tf(sld);
[w,rf]=repfreq(h,0.01,100);
//
//transfer
 bode(h,0.01,100);
halt();xbasc();
 bode(h,0.01,100,0.01)
halt();xbasc();
 bode(sl,0.01,100);
halt();xbasc();
 bode(sl,0.01,100,0.01)
halt();xbasc();
//
 bode(w,rf)
halt();xbasc();
 bode(w,20*log(abs(rf))/log(10),(180/%pi)*atan(imag(rf),real(rf)))
halt();xbasc();
//
//transfer
 bode(sld,0.001,1)
// bode(sld,0.001,1,0.01)
//
halt();xbasc();
// bode(hd,0.001,1)
 bode(hd,0.001,1,0.01)
//
//
//nyquist
//
halt();xbasc();
nyquist(h,0.01,100); nyquist(h,0.01,100,0.01);
halt();xbasc();
nyquist(sl,0.01,100); nyquist(sl,0.01,100,0.01);
halt();xbasc();
nyquist(w,rf);
nyquist(w,20*log(abs(rf))/log(10),(180/%pi)*atan(imag(rf),real(rf)));
//
//nyquist(sld,0.001,1);nyquist(sld,0.001,1,0.01);
halt();xbasc();
//nyquist(hd,0.001,1);nyquist(hd,0.001,1,0.01);
halt();xbasc();
//
//black
//

black(h,0.01,100); black(h,0.01,100,0.01);
halt();xbasc();
black(sl,0.01,100); black(sl,0.01,100,0.01);
halt();xbasc();
black(w,rf);
halt();xbasc();
black(w,20*log(abs(rf))/log(10),(180/%pi)*atan(imag(rf),real(rf)));
//
halt();xbasc();
black(sld,0.001,1);black(sld,0.001,1,0.01);
halt();xbasc();
black(hd,0.001,1);black(hd,0.001,1,0.01);
halt();xbasc();
//
//test  dscr
slc=syslin('c',[0 1;0 0],[0;0],[1,0]);qc=[0 0;0 0.1]
qd=ones(2,2)./[30000 2000;2000 100];
sld=syslin(0.1,[1 0.1;0 1],[0;0],[1 0]);
[s1]=dscr(slc,0.1);
if norm(s1(2)-sld(2))>10*%eps then pause,end
if norm(s1(3)-sld(3))>10*%eps then pause,end
[s1,r]=dscr(slc,0.1,qc);
if norm(s1(2)-sld(2))>10*%eps then pause,end
if norm(s1(3)-sld(3))>10*%eps then pause,end
if norm(r-qd)>10*%eps then pause,end

