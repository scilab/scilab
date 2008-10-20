function []=velod(i)
//------------ generation of the rear wheel
// Copyright INRIA
  nnn=24
  i1=(i-1)*nstep+1
  ang=xx(6,i1)*i1/n2
  l=(1/nnn)*( (2*%pi+ang)*(0:nnn)+ ang*(ones(1,nnn+1)-(0:nnn)))'
  dec = ones(l);sinl=r1*sin(l);cosl=r1*cos(l);
  cphi1=cos(xx(4,i1));
  sphi1=sin(xx(4,i1));
  cthe1=cos(xx(5,i1));
  sthe1=sin(xx(5,i1));
  //  unused 
  //  cpsi1=cos(xx(6,i1));
  //  spsi1=sin(xx(6,i1));
  xrearar = cosl*cphi1-sinl*(sphi1.*cthe1)+dec*xx(1,i1);
  yrearar=  cosl*sphi1+sinl*(cphi1.*cthe1)+dec*xx(2,i1);
  zrearar = sinl*sthe1+dec*xx(3,i1);
  ang=xx(20,i1)*i1/380
  l=(1/nnn)*( (2*%pi+ang)*(0:nnn)+ ang*(ones(1,nnn+1)-(0:nnn)))'
  dec = ones(l);sinl=r1*sin(l);cosl=r1*cos(l);
  cphi1=cos(xx(18,i1));
  sphi1=sin(xx(18,i1));
  cthe1=cos(xx(19,i1));
  sthe1=sin(xx(19,i1));
  // unused
  // cpsi1=cos(xx(20,i1));
  // spsi1=sin(xx(20,i1));
  xfrontar = cosl*cphi1-sinl*(sphi1.*cthe1)+dec*xx(21,i1);
  yfrontar=  cosl*sphi1+sinl*(cphi1.*cthe1)+dec*xx(22,i1);
  zfrontar = sinl*sthe1+dec*xx(23,i1);
  xnr=ct*xfrontar-st*yfrontar;
  ynr=cp*(st*xfrontar+ct*yfrontar)+sp*zfrontar;
  xnt=ct*xf(:,i)-st*yf(:,i);
  ynt=cp*(st*xf(:,i)+ct*yf(:,i))+sp*zf(:,i);
  xnf=ct*xrearar-st*yrearar;
  ynf=cp*(st*xrearar+ct*yrearar)+sp*zrearar;
  xpoly(xnt,ynt,'lines');
  xpoly(xnr,ynr,'lines');
  xpoly(xnf,ynf,'lines');
  [nn2,nn1]=size(xnr);
  xx1=[xnr(1:nn2/2)';xnr((nn2/2+1):nn2)'];
  yy1=[ynr(1:nn2/2)';ynr((nn2/2+1):nn2)'];
  xsegs(xx1,yy1);
  xx1=[xnf(1:nn2/2)';xnf((nn2/2+1):nn2)'];
  yy1=[ynf(1:nn2/2)';ynf((nn2/2+1):nn2)'];
  xsegs(xx1,yy1);
endfunction



