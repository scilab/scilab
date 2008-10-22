function fresp = cepstrum(w,mag)
// Uses the complex-cepstrum (Oppenheim & Schafer, Digital
//Signal Processing, p. 501) to generate, at the frequencies 
//w, a complex frequency response fresp whose magnitude is 
//equal to magnitude data mag and whose phase corresponds
//to a stable, minimum phase transfer function. 
//
// Copyright INRIA
[w,ind]=sort(-w);w=-w;mag=mag(ind);
dnum=length(w);

pw=sqrt(w(dnum)*w(1));
if pw<1.d-8 then error('invalid frequency range');end
tcomes=(w.*w)/pw/pw;

dw=acos((1-tcomes)./(1+tcomes));
lowf=dw(1);
highf=dw(length(dw));
if lowf <= 0 | highf >=%pi
  error('w should be positive')
end

nn=ceil((log(2*%pi/min(lowf,%pi-highf)))/log(2));
npts=2*(2^nn);hnpts =2^nn;
if npts<4096 then npts=4096;hnpts=2048;end

lmagin =(1/log(10))*log(mag);lindw =(%pi/hnpts)*(0:hnpts-1);
lmag=zeros(1,hnpts);topval=length(dw);
p=find(lindw<dw(1));lmag(p)=lmagin(1)*ones(1,length(p));
p=find(dw(topval)<=lindw);
lmag(p)=lmagin(topval)*ones(1,length(p));
for i=2:topval
  p=find(lindw>=dw(i-1) & lindw<dw(i));
  wrat=lindw(p) - dw(i-1)*ones(1,length(p));
  wrat=(1/(dw(i)-dw(i-1)))*wrat;
  lmag(p)=(ones(1,length(p))-wrat)*lmagin(i-1)+wrat*lmagin(i);
end
linmag=exp(log(10)*lmag);

dome=[lindw,(2*%pi)-fliplr(lindw)];
mag=[linmag,fliplr(linmag)];

ymag=log(mag.*mag);ycc=fft(ymag,1);nptso2=npts/2;xcc=ycc(1:nptso2);
xcc(1)=xcc(1)/2;xhat=exp(fft(xcc,-1));domeg=dome(1:2:nptso2-1);
xhat = xhat(1:nptso2/2);nptsslo=length(dw);nptsfst = length(domeg);

if domeg(1)<=dw(1) & domeg(nptsfst)>=dw(nptsslo)
  fresp=zeros(1,nptsslo);
  for i=1:nptsslo
    p=min(find(domeg>=dw(i)));
    wrat=(dw(i)-domeg(p-1))/(domeg(p)-domeg(p-1));
    fresp(i)=wrat*xhat(p) + (1-wrat)*xhat(p-1);
  end
else
  error('not sampled high enough')
end
fresp=fresp(:);

endfunction
function m=fliplr(m)
//utility fct.
[p,q]=size(m);
m=m(:,q:-1:1);
endfunction
