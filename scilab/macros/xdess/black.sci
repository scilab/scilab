function []=black(sl,fmin,fmax,pas,comments)
//Black's diagram (Nichols chart) for a linear system sl.
//sl can be a continuous-time, discrete-time or sampled SIMO system
//Syntax:
//
//           black( sl,fmin,fmax [,pas] [,comments] )
//           black(frq,db,phi [,comments])
//           black(frq, repf  [,comments])
//  
//  sl       : SIMO linear system (see syslin). In case of multi-output
//             system the outputs are plotted with differents symbols.
//            
//  fmin     : minimal frequency (in Hz).
//  fmax     : maximal frequency (in Hz).
//  pas      : logarithmic discretization step. (see calfrq for the
//             choice of default value).
//  comments : character strings to comment the curves.
//
//  frq      : (row)-vector of frequencies (in Hz) or (SIMO case) matrix
//             of frequencies.
//  db       : matrix of modulus (in Db). One row for each response.
//  phi      : matrix of phases (in degrees). One row for each response.
//  repf     : matrix of complex numbers. One row for each response.

// Copyright INRIA
//To plot the grid of iso-gain and iso-phase of y/(1+y) use abaque()
//%Example
//  s=poly(0,'s')
//  h=syslin('c',(s**2+2*0.9*10*s+100)/(s**2+2*0.3*10.1*s+102.01))
//  abaque();
//  black(h,0.01,100,'(s**2+2*0.9*10*s+100)/(s**2+2*0.3*10.1*s+102.01)')
//  //
//  h1=h*syslin('c',(s**2+2*0.1*15.1*s+228.01)/(s**2+2*0.9*15*s+225))
//  black([h1;h],0.01,100,['h1';'h'])
//See also:
//  bode nyquist  abaque freq repfreq 
//!
[lhs,rhs]=argn(0);
pas_def='auto' //
//
//
ilf=0
typ=type(sl)
//-compat next line added for list/tlist compatibility
if typ==15 then typ=16,end
select typ
case 16 then  // sl,fmin,fmax [,pas] [,comments]
  typ=sl(1);typ=typ(1);
  if typ<>'lss'&typ<>'r' then
    error(97,1)
  end
  select rhs
  case 1 then //sl
   comments=' '
   [frq,repf]=repfreq(sl);
   [d,phi]=dbphi(repf);
   sl=[] 
  case 2 then // sl,frq
   comments=' '
   [frq,repf]=repfreq(sl,fmin);
   [d,phi]=dbphi(repf);
   fmin=[];sl=[]
  case 3 ,
   if type(fmax)==1 then
      comments=' '
      [frq,repf]=repfreq(sl,fmin,fmax,pas_def),
      [d,phi]=dbphi(repf);
      sl=[]
   else
      comments=fmax
      [frq,repf]=repfreq(sl,fmin);
      [d,phi]=dbphi(repf);
      fmin=[];sl=[]
   end
  case 4 ,
    if type(pas)==1 then 
      comments=' ',
    else 
      comments=pas;pas=pas_def
    end,
    [frq,repf]=repfreq(sl,fmin,fmax,pas)
    [d,phi]=dbphi(repf);
  case 5 then,
    [frq,repf]=repfreq(sl,fmin,fmax,pas)
    [d,phi]=dbphi(repf);
  else 
    error('invalid call: sys,fmin,fmax [,pas] [,com]')
  end;
//bode(sl,fmin,fmax,pas,comments)
case 1 then //frq,db,phi [,comments] or frq, repf [,comments]
  select rhs
  case 2 , //frq,repf
    comments=' '
    [phi,d]=phasemag(fmin),fmin=[]
  case 3 then
    if type(fmax)==1 then
      comments=' '//frq db phi
      d=fmin,fmin=[]
      phi=fmax,fmax=[]
    else
      [phi,d]=phasemag(fmin);fmin=[]
      comments=fmax
     end;
   case 4 then 
     comments=pas;d=fmin;fmin=[];phi=fmax;fmax=[]
   else 
     error('invalid call :frq,db,phi,[com] ou frq,repf,[com]')
   end;
   frq=sl;sl=[];[mn,n]=size(frq);
   if mn<>1 then
      ilf=1;
   else
      ilf=0;
   end;
else 
   error('invalid call to black')
end;

[mn,n]=size(phi);
//
if comments==' ' then
   comments(mn)=' ';
   mnc=0;
   strf='051'
else
   mnc=mn;
  strf='151'
end;
xmn=floor(min(phi)/90)*90
xmx=ceil(max(phi)/90)*90
ymn=min(d)
ymx=max(d)
rect=[ymn;xmn;ymx;xmx]
//[xmn,xmx,npx]=graduate(-360,0)
//[ymn,ymx,npy]=graduate(mini(d),maxi(d))
rect=[xmn,ymn,xmx,ymx]
leg=strcat(comments,'@')

//plot2d(phi',d',(1:mn),strf,leg,rect,[10,npx,10,npy]);
plot2d(phi',d',(1:mn),strf,leg,rect);
kf=1
phi1=phi+5*ones(phi);
xgeti=xget("mark");
xset("mark",2,xgeti(2));
xset("clipgrf");



kk=1;p0=[phi(:,kk) d(:,kk)];ks=1;dst=0;
dx=rect(3)-rect(1)
dy=rect(4)-rect(2)
dx2=dx^2;dy2=dy^2

while kk<n
  kk=kk+1
  dst=dst+mini(((phi(:,kk-1)-phi(:,kk))^2)/dx2+((d(:,kk-1)-d(:,kk))^2)/dy2)
  if dst>0.001 then
  if mini(abs(frq(:,ks(prod(size(ks))))-frq(:,kk))./frq(:,kk))>0.2 then
   ks=[ks kk]
   dst=0
  end
  end
end
kf=1
for k=1:mn,
    xnumb(phi(k,ks),d(k,ks),frq(kf,ks),0);
    xpoly(phi(k,ks),d(k,ks),'marks',0);
  kf=kf+ilf
end;
xclip();
xtitle('h(2i.pi.f) ','phase','magnitude');
//     contour 2.3 db
mbf=2.3;
lmda=exp(log(10)/20*mbf);
r=lmda/(lmda**2-1);
npts=100;
crcl=exp(%i*(-%pi:(2*%pi/npts):%pi));
lgmt=log(-r*crcl+r*lmda*ones(crcl));
plot2d([180*(imag(lgmt)/%pi-ones(lgmt))]',[(20/log(10)*real(lgmt))]',...
     [2,-(mnc+1)],"100",'2.3db curve'),
xset("mark",xgeti(1),xgeti(2));
endfunction
