function [win_l,cwp]=window(wtype,n,par)
//[win_l,cwp]=window(wtype,n,par)
//macro which calculates symmetric window
// wtype :window type (re,tr,hn,hm,kr,ch)
// n     :window length
// par   :parameter 2-vector (kaiser window: par(1)=beta>0)
//       :                   (chebyshev window:par=<dp,df>)
//       :                   dp=main lobe width (0<dp<.5)
//       :                   df=side lobe height (df>0)
// win   :window
// cwp   :unspecified Chebyshev window parameter
//!
//author: C. Bunks   date: 8 Sept 1988
// Copyright INRIA

  [lhs,rhs]=argn(0);
  cwp=-1;
  
  //Pre-calculations
  
  no2=(n-1)/2;
  xt=(-no2:no2);
  un=ones(1:n);
  
  //Select the window type
  
  select wtype
  case 're' then           //Rectangular window.
    win_l=un
  case 'tr' then           //Triangular window.
    win_l=un-2*abs(xt)/(n+1);
  case 'hm' then           //Hamming window.
    win_l=.54*un+.46*cos(2*%pi*xt/(n-1));
  case 'hn' then           //Hanning window.
    win_l=.5*un+.5*cos(2*%pi*xt/(n-1));
  case 'kr' then           //Kaiser window with parameter beta (n,beta)
    Beta=par(1);
    if Beta>0 then
      xt=2*xt/(n-1);
      xt=Beta*sqrt(un-xt.*xt);
      y=xt/2;
      yb=Beta/2;
      e=un;
      eb=1.;
      de=un;
      deb=1.;
      for i=1:25,
	de=de.*y/i;
	deb=deb*yb/i;
	sde=de.*de;
	sdeb=deb*deb;
	e=e+sde;
	eb=eb+sdeb;
      end
      win_l=e/eb;
    else
      error('Parameter beta out of bounds (beta]0) --- program termination');
    end
  case 'ch' then           //Chebyshev window 
    
    //    calculting element of par which is negative
    if par(1)<0 then,
      unknown='dp';
      df=par(2);
    else if par(2)<0 then,
	unknown='df';
	dp=par(1);
    else,
      error('Parameter par should be [dp,df] where one of dp, df is equal to -1')
    end
  end,
  
  select unknown
  case 'dp' then,
    arg2=1/cos(%pi*df);
    coshin2=log(arg2+sqrt(arg2*arg2-1));
    dp=2/(exp((n-1)*coshin2)+exp(-(n-1)*coshin2));
    cwp=dp;
  case 'df' then
    arg1=(1+dp)/dp;
    coshin1=log(arg1+sqrt(arg1*arg1-1));
    df=.5*(exp(coshin1/(n-1))+exp(-coshin1/(n-1)));
    df=1/df;
    df=imag(log(df+%i*sqrt(1-df*df)))/%pi;
    cwp=df;
  end,
  
  //Pre-calculation
  
  np1=int((n+1)/2);
  ieo=2*np1-n;
  xn=n-1;
  fnf=n;
  x0=(3-cos(2*%pi*df))/(1+cos(2*%pi*df));
  alpha=(x0+1)/2;
  Beta=(x0-1)/2;
  c2=xn/2;
  
  //Obtain the frequency values of the Chebyshev window
  
  f=(0:n-1)/fnf;
  xarg=alpha*cos(2*%pi*f)+Beta*un;
  pm1=dp*cos(c2*imag(log(xarg+%i*sqrt(un-xarg.*xarg))));
  arg=c2*log(xarg+sqrt(xarg.*xarg-un));
  pp1=dp*.5*(exp(arg)+exp(-arg));
  
  dx=0*un;
  for i=1:n,
    if abs(xarg(i))<=1 then
      dx(i)=1;
    end,
  end,
  
  pr=dx.*pm1+(un-dx).*pp1;
  pi=0*un;
  
  if ieo<>1 then
    pr=pr.*cos(%pi*f);
    pi=-pr.*sin(%pi*f);
    antisym=[1*ones(1:int(n/2)+1),-1*ones(int(n/2)+2:n)];
    pr=pr.*antisym;
    pi=pi.*antisym;
  end,
  
  //Calculate the window coefficients using the inverse DFT
  
  twn=2*%pi/fnf;
  xj=(0:n-1);
  for i=1:np1;
    w(i)=sum(pr.*cos(twn*(i-1)*xj)+pi.*sin(twn*(i-1)*xj));
  end,
  c1=w(1);
  w=w/c1;
  if ieo==1 then
    win_l(np1:n)=w(1:np1);
    win_l(1:np1-1)=w(np1-1:-1:1);
  else,
    win_l(np1+1:n)=w(1:np1);
    win_l(1:np1)=w(np1:-1:1);
  end
  win_l=real(win_l');
  
  //Error in window type
  
   else
      error('Unknown window type'),
   end
endfunction
