function [ad,att,rp,flag,itrans,m,zz]=optfir(typ,n,fc,tw,eps);
//<ad,flag,itrans,m,b>=optfir(typ,n,fc,eps);
//   type   : 'lp' for lowpass filters
//          : 'free' when all the samples are specified by the user.
//   n      : filter length
//   fc     : 2-vector of cut-off frequencies (in [0 0.5]) for 'bp' and 'bs'
//          : if 'lp' and 'hp' only the first component is used.
//   tw     : transition width in normalized frequency (0<tw<0.5).
//   eps    : 2-vector giving (in dB) either the maximum allowed ripple
//          : in the passbands or the minimum required
//          : attenuation in the stopbands.
//          :  In the first case, eps must be given as:
//          :              <ripple;0>
//          : the maximum attenuation in the stopbands being then minimized.
//          :  In the second case, eps must be given as:
//          :               <0;attenuation>
//          : the maximum ripple in the passbands being then minimized.
//!
// Copyright INRIA

if eps(1)==0, crit='att';else crit='rip';end;
p=prod(size(fc));
p1=prod(size(tw));
ch1='Transition width is less than the achievable resolution';
ch2='First transition width is less than the achievable resolution';
ch3='Second transition width is less than the achievable resolution';
ch4='Stopband is less than the sum of transition widths';
if p1==1,nbech=int(tw*n);
if tw<1/n then error(ch1);return;end;
else
if tw(1)<1/n then error(ch2);return;end;
if tw(2)<1/n then error(ch3);return;end;
nbech1=int(tw(1)*n);nbech2=int(tw(2)*n);
nbech=nbech1+nbech2;end;
if int(n/2)==n/2 then
  n1=n/2;
else 
  n1=(n-1)/2+1;
end;

fc=fc(1);
alph1=fc*n;
k1=int(alph1);
k2=int(alph1-.5);
if k1==k2,flag=2;else flag=1;end;
itrans=k1+1:k1+nbech;
ad=ones(1,k1);ad(1,n1)=0;
ai=[];
for j=1:nbech,..
ai=[ai;dirich(n,itrans(j)-1,flag)+dirich(n,-itrans(j)+1,flag)];end;
bi=fsfir(ad,flag);
if k1<=1,k1=2;end;
m1=2:2:2*(k1-1);nm1=prod(size(m1));
nmax1=mini(3,nm1);
m1=m1(nm1-nmax1+1:nm1);
m3=2*(k1+nbech):2:2*n;nm3=prod(size(m3));
nmax3=mini(3,nm3);
[q1,q2]=mini(abs(m3-n*ones(m3)));
nmax3=mini(q2,nmax3);
m3=m3(1:nmax3);
m=[m1 m3];
if flag==2,m=m+ones(m);end;
nm=prod(size(m));
//	contraintes de bornes
// $$$$$$$$$ [ab,bb,zb]=bound(typ)
//	contraintes inegalite
a=ai(:,m)'.*.[1;1];
a1=[0*ones(2*nmax1,1);ones(nmax3,1).*.[-1;1]];
a=[a a1];
b=(-bi(m)').*.[1;1];
eps1=eps*ones(nmax1,1).*.[1;-1];eps1=eps1+ones(eps1);
eps1(2*nm)=0;
b=b+eps1;
z=ones(nm,1).*.[1;-1];
//	resolution
a=[ab;a];
b=[bb;b];
z=[zb;z];
a=[a diag(z)];
c=0*ones(1,maxi(size(a)));c(nbech+1)=1;
zz=simplx(c,a,b);
ad(itrans)=zz(1:nbech)';
rp=bi+zz(1:nbech)'*ai;
rp=rp(1:n);
att=zz(nbech+1);

function [bidon]=dirich(n,p,q)
bidon=0.5;


