function [x,frequ]=linfn(G,PREC,RELTOL,options);
//[x,frequ]=linfn(G,PREC,RELTOL,options);
//  Computes the Linf (or Hinf) norm of a transfer function
//			     -1
//      G(s) = D + C (sI - A)  B
//
//  This norm is well-defined as soon as the realization
//  (A,B,C) has no imaginary eval which is both controllable and observable.
//
//  The algorithm follows the paper by G. Robel (AC-34 pp. 882-884, 1989).
//  The case D=0 is not treated separately due to superior accuracy of
//  the general method when (A,B,C) is nearly nonminimal.
//
//  In the general case (A neither stable nor antistable), no upper bound is
//  prespecified. If by contrast A is stable or antistable, lower
//  and upper bounds are computed using the associated Lyapunov
//  solutions (see Glover).
// Copyright INRIA

//  On input:
//  ---------
//     *  G is a syslin list
//     *  PREC is the desired relative accuracy on the norm
//     *  RELTOL: relative threshold to decide when an eigenvalue can be
//		  considered on the imaginary axis.
//     *  available options are
//          - 'trace': traces each bisection step, i.e., displays the lower
//		       and upper bounds and the current test point.
//          - 'cond':  estimates a confidence index on the computed value
//		       and issues a warning if computations are
//		       ill-conditioned
//
//  On output:
//  ---------
//     *  x is the computed norm.
//     *  freq:  list of the frequencies for which ||G|| is attained, i.e.,
//          such that ||G (j om)|| = ||G||. If -1 is in the list, the norm
//          is attained at infinity. If -2 is in the list, G is all-pass in
//          some direction so that ||G (j omega)|| = ||G|| for all
//          frequencies omega.
//!
//
//  Called macros:
//  -------------
//	heval_test, cond_test, list_set
//
//  History:
//  -------
//      author: P. Gahinet, INRIA
//      last modification: Oct 3nd, 1991
//****************************************************************************
 
 
//constants
//*********
INIT_LOW=1.0e-4; INIT_UPP=1.0e5;  INFTY=10e10;
frequ=[];
 
 
//user interface. The default values are:
//     PREC=1.0e-3; RELTOL=1.0e-10; options='nul';
//************************************************
[lhs,rhs]=argn(0);
select rhs,
case 1 then PREC=1.0e-3; RELTOL=1.0e-10; options='nul';
case 2 then RELTOL=1.0e-10;
            if type(PREC)==10 then options=PREC; PREC=1.0e-3;
                             else options='nul'; end,
case 3 then if type(RELTOL)==10 then options=RELTOL; RELTOL=1.0e-10;
                               else options='nul'; end,
end
 
 
//recover realization
//*******************
[a,b,c,d]=G(2:5);
 
 
//SCALING
//*******						  ||B||.||C||
//  Scale A,B,C so that ||A||=||B||=||C||=1. With scale:= -----------,
//							     ||A||
//  and DD:=D/scale, AA:=A/||A||, BB:=B/||B||, CC:=C/||C||, we have
//					       -1
//       || G || = scale * || DD + CC (sI - AA)   BB ||
//
//  From now on, the parameters A,B,C,D are scaled
 
noa=norm(a,'inf'); nob=norm(b,'inf'); noc=norm(c,'inf'); nobc=nob*noc;
 
if nobc==0, x=norm(d); return, end
 
scale=nobc/noa; a=a/noa; b=b/nob; c=c/noc; d=d/scale; nd=norm(d);
 
 
//test the spectrum of A
//**********************
 
s=real(spec(a));
 
if mini(abs(s)) < RELTOL then
   write(%io(2),..
     'WARNING: the A matrix has eigenvalues near the imaginary axis');
end
 
 
//Search window initialization
//****************************
// Initialize the search window [lower,upper] where `lower' and `upper'
// are lower and upper bounds on the Linf norm of G. When no such
// bounds are available, the window is arbitrarily set to [INIT_LOW,INIT_UPP]
// and the variables LOW and UPP keep record of these initial values so that
// the window can be extended if necessary.
 
if maxi(s)*mini(s) > 0 then
// A is stable or antistable: use associated Lyapunov equations
// to derive lower and upper bounds.
   p=lyap(a',-b*b','c');
   q=lyap(a,-c'*c,'c');
   s=sqrt(abs(spec(p*q)));
 
   lower=maxi(nd,maxi(s)); LOW=0;
   upper=nd+2*sum(diag(s));   UPP=100*upper;
 
else
 
   if nd==0 then  lower=INIT_LOW; LOW=INIT_LOW;
   else          lower=nd;       LOW=0;         end
   upper=INIT_UPP;     UPP=INIT_UPP;
 
end
 
 
 
//form the constant parts of the pencil (E,F) (see G. Robel).
//***********************************************************
[na,na]=size(a); twona=2*na;
[p,m]=size(d);
nf=twona+mini(m,p); //size of e and f
 
// to ensure that D'*D is of size min(m,p), replace (a,b,c,d) by
// (a',c',b',d') if m>p
if m>p then
    a=a'; d=d'; aux=b; b=c'; c=aux';
end
 
e=eye(2*na,2*na); e(nf,nf)=0;
nul=0; nul(na,na)=0;
f=[a,nul;-c'*c,-a']; f(nf,nf)=0;
dd=d'*d; Cd=c'*d;
 
 
//----------------------
// 	BISECTION STARTS
//----------------------
 
while %t,
 
ga=sqrt(lower*upper);   //test point gamma = log middle of [lower,upper]
 
if part(options,1)=='t' then
  write(%io(2),[scale*lower,scale*ga,scale*upper],..
     '(''lower,current,upper = '',3e20.10)');
end
 
bga=b/ga; Cdga=Cd/ga;
f(1:na,twona+1:nf)=-bga;
f(na+1:twona,twona+1:nf)=Cdga;
f(twona+1:nf,1:nf)=[Cdga',bga',eye(dd)-dd/(ga**2)];
 
 
// Test for generalized eigenvalues on the imaginary axis
// ------------------------------------------------------
 
[dist,frequ]=heval_test(e,f,RELTOL,'test');
 
if dist < RELTOL then
    lower=ga;  LOW=0;
//  eigenvalue on the imaginary axis: gamma < ||G||
else
    upper=ga;  UPP=100*upper;
//  gamma > ||G||
end
 
 
// Search window management:
//--------------------------
//   If the gamma-iteration runs into one of the initial arbitrary bounds
//   LOW or UPP, extend the search window to allow for continuation
 
if ga<10*LOW then lower=LOW/10; LOW=lower; end
                    // expand search window toward gamma<<1
if ga>UPP/10 then upper=UPP*10; UPP=upper; end
                    // expand search window toward gamma>>1
 
 
// Termination tests
//------------------
 
if lower > INFTY then
   write(%io(2),..
     'controllable & observable mode(s) of A near the imaginary axis');
   x=scale*lower;
   return;
else if upper < 1.0e-10 then
   x=scale*upper;
   write(%io(2),..
     'All modes of A are nearly nonminimal so that || G || is almost 0');
   return;
else if 1-lower/upper < PREC,
   ga=sqrt(lower*upper);
   x=scale*ga;
 
// Compute all the frequencies achieving ||G||
   if lower<>0 then
       bga=b/lower; Cdga=Cd/lower;
       f(1:na,twona+1:nf)=-bga;
       f(na+1:twona,twona+1:nf)=Cdga;
       f(twona+1:nf,1:nf)=[Cdga',bga',eye(dd)-dd/(lower**2)];
       [dist,frequ]=heval_test(e,f,RELTOL,'freq');
   end
   if frequ==[] then
      write(%io(2),'The computed value of || G || may be inaccurate');
   end
 
// evaluate the condition of the eigenproblem of (e,f) near || G ||
   if part(options,1)=='c' then
      gt=1.1*ga;
      f=[a,nul,-b/gt;cc,at,Cd/gt;dc/gt,bt/gt,eye(dd)-dd/(gt**2)]
      co=cond_test(e,f,frequ,RELTOL);
      if co < RELTOL then
         write(%io(2),'The computed value of || G || may be inaccurate');
      end
   end
//-----------
 
   return;
end, end, end
 
end//end while


endfunction
function [dist,frequ]=heval_test(e,f,TOL,option);
//[dist,frequ]=heval_test(e,f,TOL,option);
// This procedure estimates the distance of the generalized spectrum
// of the pencil  f - lambda e  to the imaginary axis. Here e is always
// of the form  diag(I_(nf-nz),0_nz). The distance is 0 whenever there are
// (nearly) infinite eigenvalues or eigenvalues of the form 0/0.
//
// The eigenvalues are computed via a generalized Schur decomposition
// of  f - lambda e . Let (a(i),b(i)) : i=1..nf be the output of gspec.
// Three cases must be distinguished:
//    * both a(i) and b(i) are << 1 -> singularity of the pencil
//    * b(i)<<1 and a(i) close to 1 -> infinite eigenvalue
//    * both a(i) and b(i) are close to 1 -> finite eigenvalue.
//
// Let nz denote the rank deficiency of e which is also the size of D'*D.
// For gamma > ||G||, the generalized spectrum of (e,f) consists of exactly
// nz infinite modes and nf-nz finite ones. By contrast, there may be
// additional singularities or infinite modes for ||D|| <= gamma <= ||G||,
// depending on whether ||D|| = ||G|| or ||D|| < ||G||.
//
// If ||D|| < ||G||, there are still exactly nz infinite modes for gamma in
// [ ||D|| , ||G|| ]. At gamma=||G||, some finite mode(s) hit the imaginary
// axis and their imaginary part omega is such that ||G(j omega)|| = ||G||.
//
// If ||D|| == ||G|| now, we always have ||G (infinity)|| = ||G||
// and if moreover some pair (a(i),b(i)) is nearly (0,0), then
// || G (j omega) || = || G || for all omega's (direction along which G is
// all-pass). Note that G is all-pass iff there are nz pairs
// (a(i),b(i)) nearly equal to (0,0). Finally, finite modes which hit
// the imaginary axis still yield frequencies for which ||G|| is attained.
//
// Two options are available in this function:
// * When option='test', the function counts the number of finite modes.
// If less than nf-nz (nf=order of f), it concludes gamma <= ||G|| and returns
// dist=0. Otherwise, it estimates the distance of the finite spectrum
//				  min | Re(l_i) |
// to the imag. axis computed as  --------------- where the l_i's denote
//				    max | l_i |
// the pencil finite eigenvalues.
// * With the option 'freq' (used for gamma = ||G||), it furthermore returns
// all frequencies for which ||G|| is attained. Infinite frequencies are
// denoted by -1 and if ||G(j omega)|| == ||G|| for all omega's, frequ=[-2];
//
//
// Input:
//    * (e,f): pencil
//    * TOL: relative tolerance on the size of eigenvalue real parts.
//    * option: 'test' or 'freq'.
//
// Output
//    * dist: distance of the spectrum to the imaginary axis as defined above.
//    * frequ: list of frequencies for which ||G|| is attained.
//
//! 
//balancing
frequ=[]; evals=[];
[f,xx]=balanc(f);
[nf,nf]=size(f);
nz=nf-sum(diag(e)); //rank deficiency of e
 
 
//Generalized Schur decomposition of the pencil (f,e)
//---------------------------------------------------
[a,b]=spec(f,e);
 
 
if option=='test' then
//***********************************
//Simple test and computation of dist
//***********************************
 
// Check that there are exactly nz infinite modes of (e,f) and compute dist
 
nai=0; //nai: number of infinite or (0,0) modes (b(i) << 1)
for i=1:nf,
  bi=abs(b(i));
  if bi < 100*TOL then nai=nai+1;
  else                 evals=[evals,a(i)/bi]; end
end
 
if nai>nz then dist=0; else dist=mini(abs(real(evals)))/maxi(abs(evals)); end
 
 
else   //option = 'freq'
//*************************************************
//Compute the frequency for which ||G|| is attained
//*************************************************
 
// Here gamma is appx equal to ||G||. Distinguish two cases:
// ||D|| < ||G|| and ||D|| = ||G||.
 
if mini(svd(f(nf-nz+1:nf,nf-nz+1:nf))) < TOL then
//-----------------------------------------------
// f(nf-nz+1:nf,nf-nz+1:nf)= I - (D'*D)/||G||**2 -> case ||D||=||G||
 
   noa=maxi(abs(a));  na=0;     //na -> # pairs (0,0)
   frequ=[-1];  //||G|| is always attained for s=infinity in this case
   for i=1:nf,
      bi=b(i);
      if abs(bi) < 100*TOL then
         if abs(a(i)) < 100*TOL*noa, na=na+1; end
      else
         evals=[evals,a(i)/bi];
      end
   end
 
   if na>0 then    // G is all-pass along some direction
      frequ=[-2];
      if na>=nz, write(%io(2),'G is all-pass'); end
   else if evals<>[] then
      maxabs=maxi(abs(evals));
      for i=1:maxi(size(evals)),
         if abs(real(evals(i))) <= TOL*maxabs,
                         frequ=[frequ,abs(imag(evals(i)))]; end
      end,
   end,end
 
 
else // case ||D|| < ||G||
//------------------------
 
   for i=1:nf,
      bi=b(i);
      if abs(bi) > 100*TOL, evals=[evals,a(i)/bi]; end
   end
 
   maxabs=maxi(abs(evals));
   for i=1:maxi(size(evals)),
      if abs(real(evals(i))) <= TOL*maxabs,
                     frequ=[frequ,abs(imag(evals(i)))]; end
   end
 
end //endif
 
frequ=list_set(frequ,1.0e-5); //eliminate redundancy in frequ
 
 
end //endif
 

endfunction
function [c]=cond_test(e,f,frequ,TOL);
//[c]=cond_test(e,f,frequ,TOL);
//   This procedure returns a confidence index for the computed gamma = ||G||
//   at which some generalized eigenvalue(s) of (e,f) meets the imaginary
//   axis. Specifically, it considers gamma := 1.1*computed value of ||G||
//   and computes how close (e,f) is to have imaginary eigenvalues
//   for this gamma. If very close, this indicates (e,f) has generalized
//   eigenvalues near the imaginary axis for all gamma's in an interval
//   around ||G|| whence the computed value is likely to be inaccurate.
//! 
[nf,nf]=size(f);
c=1;
 
for i=1:maxi(size(frequ)),
   s=svd(f-%i*frequ(i)*e);
   c=mini(c,s(nf)/s(1));
   if c < TOL then return; end
end


endfunction
function [l]=list_set(l,TOL);
//[l]=list_set(l,TOL);
//  eliminates redundant elements in a list. Two entries are considered
//  identical when their difference is smaller then TOL (in relative terms)
//! 
nl=maxi(size(l));
i=1;
 
while i < nl,
  entry=l(i);  TOLabs=TOL*entry;
  j=i+1;
  while j <= nl,
    if abs(l(j)-entry) <= TOLabs then
       l=[l(1:j-1),l(j+1:nl)];
       nl=nl-1;
    else
       j=j+1;
    end
  end
  i=i+1;
end
endfunction
