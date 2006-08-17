function [gopt]=gamitg(g,r,PREC,options)
//[gopt]=gamitg(G,r,PREC,options);
//
//  On input:
//  ---------
//     *  G contains the parameters of plant realization (syslin list)
//           b = ( b1 , b2 ) , 	c = ( c1 ) ,    d = ( d11  d12)
//			            ( c2 )          ( d21  d22)
//     *  r(1) and r(2) are the dimensions of d22 (rows x columns)
//     *  PREC is the desired relative accuracy on the norm
//     *  available options are
//        - 't': traces each bisection step, i.e., displays the lower
//		 and upper bounds and the current test point.
//
//  On output:
//  ---------
//     *  gopt:  optimal Hinf gain
//
//!
//  History:
//  -------
//      author: P. Gahinet, INRIA
//      last modification:
//****************************************************************************
// Copyright INRIA

 
//user interface. The default values are:
//     PREC=1.0e-3;      options='nul';
//************************************************
[lhs,rhs]=argn(0);
select rhs,
case 2 then PREC=1.0e-3; options='nul';
case 3 then if type(PREC)==10 then options=PREC; PREC=1.0e-3;
                             else options='nul'; end,
end
 
 
//constants
//*********
INIT_LOW=1.0e-3; INIT_UPP=1.0e6;  INFTY=10e10;
RELACCU=1.0e-10; //relative accuracy on generalized eigenvalue computations
gopt=-1;
 
 
//parameter recovery
[a,b1,b2,c1,c2,d11,d12,d21,d22]=smga(g,r);
 
 
//dimensions
[na,na]=size(a); twona=2*na;
[p1,m2]=size(d12),
[p2,m1]=size(d21),
 
 
 
 
//CHECK HYPOTHESES
//****************
 
if m2 > p1 then
  write(%io(2),'WARNING: the dimensions of D12 are inadequate');
end
 
if p2 > m1 then
  write(%io(2),'WARNING: the dimensions of D21 are inadequate');
end
 
[u12,s12,v12]=svd(d12);   s12=s12(1:m2,:);
[u21,s21,v21]=svd(d21);   s21=s21(:,1:p2);
u12=u12(:,1:m2);   //d12 = u12 s12 v12' with s12 square diagonal
v21=v21(:,1:p2);   //d21 = u21 s21 v21'
 
//rank condition on D12 and D21
//-----------------------------
if s12(m2,m2)/s12(1,1) <= 100*%eps then
  write(%io(2),'WARNING: D12 is not full rank at the machine precision');
end
 
if s21(p2,p2)/s21(1,1) <= 100*%eps then
  write(%io(2),'WARNING: D21 is not full rank at the machine precision');
end
 
//(A,B2,C2) stabilizable + detectable
//-------------------------------------
noa=maxi(abs(a));  nob2=maxi(abs(b2)); noc2=maxi(abs(c2));
 
ns=st_ility(syslin('c',a,b2,c2),1.0e-10*maxi(noa,nob2));
if ns<na then
  write(%io(2),'WARNING: (A,B2) is nearly unstabilizable');
end
 
nd=dt_ility(syslin('c',a,b2,c2),1.0e-10*maxi(noa,noc2));
if nd>0 then
  write(%io(2),'WARNING: (C2,A) is nearly undetectable');
end
 
//Imaginary axis zeros: test the Hamiltonian spectra at gamma=infinity
//-----------------------------------------------------------------
 
ah=a-b2*v12/s12*u12'*c1;  bh=b2*v12; ch=u12'*c1;
hg=[ah,-bh/(s12**2)*bh';ch'*ch-c1'*c1,-ah'];
spech=spec(hg);
if mini(abs(real(spech))) < 1.0e-9*maxi(abs(hg)) then
  write(%io(2),'WARNING: G12 has zero(s) near the imaginary axis');
end
 
 
ah=a-b1*v21/s21*u21'*c2;  ch=u21'*c2; bh=b1*v21;
jg=[ah',-ch'/(s21**2)*ch;bh*bh'-b1*b1',-ah];
specj=spec(jg);
if mini(abs(real(specj))) < 1.0e-9*maxi(abs(jg)) then
  write(%io(2),'WARNING: G21 has zero(s) near the imaginary axis');
end
 
 
 
 
//COMPUTATION OF THE LOWER BOUND SIGMA_D
//--------------------------------------
if m2 < p1 then
sig12=norm((eye(p1,p1)-u12*u12')*d11);
else
sig12=0;
end
 
if p2 < m1 then
sig21=norm(d11*(eye(m1,m1)-v21*v21'));
else
sig21=0;
end
sigd=maxi(sig12,sig21);
 
 
 
 
//SEARCH WINDOW INITIALIZATION
//****************************
// Initialize the search window [lower,upper] where `lower' and `upper'
// are lower and upper bounds on the Linf norm of G. When no such
// bounds are available, the window is arbitrarily set to [INIT_LOW,INIT_UPP]
// and the variables LOW and UPP keep record of these initial values so that
// the window can be extended if necessary.
 
upper=INIT_UPP;     UPP=INIT_UPP;
 
if sigd==0 then
   lower=INIT_LOW; LOW=INIT_LOW;
else
   lower=sigd; LOW=0;
end
 
 
 
//HAMILTONIAN SETUP
//------------------
sh22=m1+m2;
h11=[a,0*eye(a);-c1'*c1,-a'];
h12=[-b1,-b2;c1'*d11,c1'*d12];
h21=[d11'*c1,b1';d12'*c1,b2'];
h22=eye(m1,m1); h22(sh22,sh22)=0;
 
sj22=p1+p2;
j11=[a',0*eye(a);-b1*b1',-a];
j12=[-c1',-c2';b1*d11',b1*d21']
j21=[d11*b1',c1;d21*b1',c2];
j22=eye(p1,p1); j22(sj22,sj22)=0;
 
d1112s=[d11,d12]'*[d11,d12];
d1121s=[d11;d21]*[d11;d21]';
 
 
T_EVALH=1; //while 1, Hamiltonian spectrum of H must be tested
T_EVALJ=1; //while 1, Hamiltonian spectrum of J must be tested
T_POSX=1;   //while 1, the nonnegativity of X must be tested
T_POSY=1;   //while 1, the nonnegativity of Y must be tested
 
 
 
//********************************************************
// 		GAMMA ITERATION STARTS
//********************************************************
 
for iterations=1:30,   //max iterations=30
 
ga=sqrt(lower*upper);   //test point gamma = log middle of [lower,upper]
gs=ga*ga;
 
if str_member('t',options) then
   write(%io(2),[lower,ga,upper],..
                               '(''min,cur,max = '',3e20.10)');
end
 
// Search window management:
//--------------------------
//   If the gamma-iteration runs into one of the initial arbitrary bounds
//   LOW or UPP, extend the search window to allow for continuation
 
if ga<10*LOW then lower=LOW/10; LOW=lower; end
                    // expand search window toward gamma<<1
if ga>UPP/10 then upper=UPP*10; UPP=upper; end
                    // expand search window toward gamma>>1
 
 
 
 
DONE=0 //DONE=1 indicates that the current gamma has been classified and
       //the next iteration can start
 
 
//----------------------------------------------------------------------
//  FIRST TEST : PURE IMAGINARY EIGENVALUES IN HAMILTONIAN SPECTRUM ?
//----------------------------------------------------------------------
 
hg=h11-(h12/(gs*h22-d1112s))*h21;
 
if T_EVALH==1 then
   hev=spec(hg);
   if mini(abs(real(hev))) <= RELACCU*maxi(abs(hev)) then
      lower=ga; DONE=1;   //Hg HAS EVAL ON iR -> NEXT LOOP
      if str_member('t',options) then
         write(%io(2),'H has pure imaginary eigenvalue(s)');
      end
   end
end
 
 
if DONE==0 then
   jg=j11-(j12/(gs*j22-d1121s))*j21;
 
   if T_EVALJ==1 then
     jev=spec(jg);
     if mini(abs(real(jev))) <= RELACCU*maxi(abs(jev)) then
        lower=ga; DONE=1;   //Jg HAS EVAL ON iR -> NEXT LOOP
        if str_member('t',options) then
           write(%io(2),'J has pure imaginary eigenvalue(s)');
        end
      end
   end
end
 
 
 
//---------------------------------------------------------
//  SECOND TEST : NONNEGATIVITY OF THE RICCATI SOLUTIONS
//---------------------------------------------------------
 
 
if DONE==0 then
 
   //compute orthon. bases of the negative invariant subspace of H
   [uh,d]=schur(hg,'c');
   px=uh(1:na,1:na);   qx=uh(na+1:twona,1:na);
 
   //nonnegativity test for X (or Y):
   //--------------------------------
   //  * if |f(i)| < RELACCU then discard this eval (this corresponds
   //    to ||X||-> infty and the sign is ill-defined
   //  * if |e(i)| < RELACCU then X is nearly singular in this direction.
   //    We then consider X is actually singular and that the eval can be
   //    discarded
   //  * For the remaining entries, if e(i)/f(i) < - RELACC/100 then X is
   //    diagnosed as indefinite. Otherwise X is nonnegative.
 
   if T_POSX==1 then
      [e,f]=spec(qx,px);
      i=1;
      while i<=na,
        if mini(abs([e(i),f(i)])) >= RELACCU & real(e(i)/f(i)) <= 0 then
          lower=ga;  DONE=1;  T_EVALH=0;  i=na+1;
          if str_member('t',options) then
                          write(%io(2),'X is indefinite');end
        else
          i=i+1;
        end
      end
   end
end
 
 
 
if DONE==0 then
 
   //compute orthon. bases of the negative inv. subspace of J
   [uj,d]=schur(jg,'c');
   py=uj(1:na,1:na);   qy=uj(na+1:twona,1:na);
 
   if T_POSY==1 then
      [e,f]=spec(qy,py);
      i=1;
      while i<=na,
        if mini(abs([e(i),f(i)])) >= RELACCU & real(e(i)/f(i)) <= 0 then
          lower=ga;  DONE=1;  T_EVALJ=0;  i=na+1;
          if str_member('t',options) then
                           write(%io(2),'Y is indefinite');end
        else
          i=i+1;
        end
      end
   end
end
 
 
//--------------------------------------------------------------
//  THIRD TEST : COMPARE THE SPECTRAL RADIUS OF XY WITH gamma**2
//--------------------------------------------------------------
 
 
if DONE==0 then
 
   [e,f]=spec(qy'*qx,py'*px);
   if maxi(real(e-gs*f)) <= 0 then
      upper=ga;
      if str_member('t',options) then
                      write(%io(2),'rho(XY) <= gamma**2'); end
   else
      lower=ga; T_POSX=0; T_POSY=0; T_EVALH=0; T_EVALJ=0;
      if str_member('t',options) then
                      write(%io(2),'rho(XY) > gamma**2'); end
   end
end
 
 
 
//------------------
// TERMINATION TESTS
//------------------
 
if lower > INFTY then
   write(%io(2),..
     'controllable & observable mode(s) of A near the imaginary axis');
   gopt=lower;
   return;
else if upper < 1.0e-10 then
   gopt=upper;
   write(%io(2),..
     'All modes of A are nearly nonminimal so that || G || is almost 0');
   return;
else if 1-lower/upper < PREC,
   gopt=sqrt(lower*upper);
   return;
end, end, end
 
 
end//end while
 
gopt=sqrt(lower*upper);
 
endfunction
function [bool]=str_member(c,s)
//**********************
// tests whether the character c occurs in the string s
// returns %T (true) if it does, %F (false) otherwise.
 
for i=1:length(s),
  if part(s,i)==c then bool=%t; return; end
end
bool=%f;
endfunction
