function [hinfnorm,frequency]=h_norm(Sl,rerr)
// produces the infinitynorm  of a state-space system 
// (the maximum over all frequencies of the maximum singular value).
//      [hinfnorm [,frequency]]=h_norm(Sl,rerr)
//      [hinfnorm [,frequency]]=h_norm(Sl)
//
//  Sl        : the state space system (see syslin)
//  Rerr      : max. relative error, default value 1e-8
//
//  hinfnorm  : the infinitynorm of Sl
//  frequency : frequency at which hinfnorm is achieved
//  see also: linfn, linf
//!
//  Version 3.2, 09-27-1990
//  Adapted from 
//  N.A. Bruinsma   T.U.Delft/Philips Research Eindhoven, see also
//  Systems & Control Letters, vol. 14 pp. 287-293.
//  Copyright INRIA
Sl1=Sl(1);
[lhs,rhs]=argn(0);
eps=1.d-8;
flag='ss';if Sl1(1)=='r' then Sl=tf2ss(Sl);end
if Sl(7)=='d' then hinfnorm=dhnorm(Sl);frequency=[];return;end
[a,b,c,d]=Sl(2:5);
eiga=spec(a);
if maxi(real(eiga)) >= -1e-12 then  ...
  write(%io(2),'Warning : system is not stable ! '),end
if rhs==1 then rerr=1e-8; end;
[no,ns] = size(c); [ns,ni] = size(b);
if mini(ni,no) == 1 then isiso = 2; else isiso = 1; end;
[p,a] = hess(a); [u,d,v] = svd(d); b = p' * b * v; c = u' * c * p;
dtd = diag(d'*d); ddt = diag(d*d'); dtc = d' * c;
aj = sqrt(-1)*eye(ns); R1 = ones(ni,1); S1 = ones(no,1);
l = [];
// compute starting value
q = ((imag(eiga) + 0.01 * ones(eiga)) ./ real(eiga)) ./ abs(eiga);
[q,i] = maxi(q); w = abs(eiga(i));
svw = norm( c * ((w*aj*eye()-a)\b) + d );
sv0 = norm( -c * (a\b) + d );
svdd = norm(d);
[lb,i] = maxi([svdd sv0 svw]);l=lb;
w = [1.d30 0 w ]; M = w(i);
// to avoid numerical problems with Rinv and Sinv if lb == norm(d), lb must be
// enlarged to at least (1+1e-3)*lb;
if lb == svdd then lb=1.001*lb+eps;end;
for it = 1:15,
  gam = (1 + 2 * rerr) * lb; gam2 = gam * gam;
  Rinv = diag(R1./(dtd - gam2 * R1));
  Sinv = diag(S1./(ddt - gam2 * S1));
  H11 = a-b*Rinv*dtc;
  evH = spec([H11 -gam*b*Rinv*b'; gam*c'*Sinv*c  -H11']);
 
  idx = find(abs(real(evH)) < 1e-8 & imag(evH) >= 0);
  imev= imag(evH(idx));
  [imev] = sort(imev);
  q = maxi(size(imev));
  if q <= 1 then
  // q=1 can only happen in the first step if H-norm==maxsv(D) or H-norm==maxsv(0)
  // due to inaccurate eigenvalue computation (so gam must be an upper bound).
    ub = gam;
//    pause;
  else
    M =  0.5 * (imev(1:q-1) + imev(2:q)); M = M(1:isiso:q-1);
    sv=[];
    for j = 1:maxi(size(M)),
      sv = [sv maxi(svd(d + c*((M(j)*aj*eye() - a)\b)))];
    end;
    lb = maxi(sv);l=[l;lb];
  end;
end;
if M == 1.d30 then
  lb=svdd;
write(%io(2),"Warning:norm cannot be computed rel. accuracy smaller than 1e-3")
write(%io(2),'Hinfnorm is probably exactly max sv(D)')
write(%io(2),'The system might be all-pass')
end;
if exists('ub')==0 then ub=lb;end
hinfnorm = 0.5 * (ub+lb); frequency = M;

endfunction
function gama=dhnorm(Sl,tol,gamamax)
//discrete-time case (should be tested!!!)
//disp('warning: discrete-time h_norm is not fully tested!')
[lhs,rhs]=argn(0);
if rhs==1 then tol=0.00001;gamamax=10000000;end
if rhs==2 then gamamax=1000;end
gamamin=sqrt(%eps);
n=0;
while %T
gama=(gamamin+gamamax)/2;n=n+1;
if n>1000 then warning('dhnorm: more than 1000 iterations!');return;end
 if dhtest(Sl,gama) then
  gamamax=gama; else gamamin=gama
 end
if (gamamax-gamamin)<tol then return;end
end

endfunction
function ok=dhtest(Sl,gama)
//test if discrete hinfinity norm of Sl is < gama
[A,B,C,D]=Sl(2:5);B=B/sqrt(gama);C=C/sqrt(gama);D=D/gama;
R=eye()-D'*D;
[n,n]=size(A);Id=eye(n,n);Z=0*Id;
Ak=A+B*inv(R)*D'*C;
e=[Id,-B*inv(R)*B';Z,Ak'];
Aa=[Ak,Z;-C'*inv(eye()-D*D')*C,Id];
[As,Es,w,k]=schur(Aa,e,'d');
//Testing magnitude 1 eigenvalues.
[al,be]=spec(As,Es);
finite=find(abs(be)>0.00000001);
finite_eigen=al(finite)./be(finite);
bad=find( abs(abs(finite_eigen)-1) < 0.0000001);
if bad<>[] then ok=%f;return;end
//if k<>n then ok=%f;return;end
ws=w(:,1:n);
x12=ws(1:n,:);
phi12=ws(n+1:2*n,:);
if rcond(x12) > 1.d-6 then
X=phi12/x12;
z=eye()-B'*X*B
ok= mini(real(spec(z))) > -%eps
else
ok=%t;end
endfunction
