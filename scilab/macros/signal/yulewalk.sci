function [Nz,Dz]=yulewalk(Norder, frq, mag)
//YULEWALK  filter design using a least-squares method.
//	Hz = YULEWALK(N,frq,mag) finds the N-th order iir filter
//
//		               n-1         n-2            
//		   B(z)   b(1)z     + b(2)z    + .... + b(n)
//	     H(z)= ---- = ---------------------------------
//		            n-1       n-2
//		   A(z)    z   + a(1)z    + .... + a(n)
//
//which matches the magnitude frequency response given by vectors F and M.
//Vectors frq and mag specify the frequency and magnitude of the desired
//frequency response. The frequencies in frq must be between 0.0 and 1.0,
//with 1.0 corresponding to half the sample rate. They must be in
//increasing order and start with 0.0 and end with 1.0. 
//
// Example: f=[0,0.4,0.4,0.6,0.6,1];H=[0,0,1,1,0,0];Hz=yulewalk(8,f,H);
//fs=1000;fhz = f*fs/2;  
//xbasc(0);xset('window',0);plot2d(fhz',H');
//xtitle('Desired Frequency Response')
//[frq,repf]=repfreq(Hz,0:0.001:0.5);
//xbasc(1);xset('window',1);plot2d(fs*frq',abs(repf'));
//xtitle('Obtained Frequency Response')
//
// Copyright INRIA
[LHS,RHS]=argn(0);
if RHS <>3
   error('Wrong number of input parameters.')
end
   npt=512;
  thelap=fix(npt/25);
   
[mf,nf]=size(frq);
[mm,nn]=size(mag);
if mm ~= mf | nn ~= nf
   error('You must specify the same number of frequencies and amplitudes.')
end
nbrk=max(mf,nf);
if mf < nf
   frq=frq';
   mag=mag';
end

if abs(frq(1)) > %eps | abs(frq(nbrk) - 1) > %eps
   error('The first frequency must be 0 and the last 1.')
end

npt=npt+1;
Ht=zeros(1,npt);

nint=nbrk-1;
df=frq(2:nf)-frq(1:nf-1);
if (or(df < 0))
   error('Frequencies must be non-decreasing.')
end

nb=1;
Ht(1)=mag(1);
for i=1:nint
    if df(i) == 0
       nb=nb - thelap/2;
       ne=nb + thelap;
    else
       ne=int(frq(i+1)*npt);
    end
    if (nb < 0 | ne > npt)
       error('Too abrupt amplitude change near end of frequency interval.')
    end
    j=nb:ne;
    if ne == nb
        inc=0;
    else
        inc=(j-nb)/(ne-nb);
    end
    Ht(nb:ne)=inc*mag(i+1) + (1 - inc)*mag(i);
    nb=ne + 1;
end
Ht=[Ht Ht(npt-1:-1:2)];
n=max(size(Ht));
n2=fix((n+1)/2);
nb=Norder;
nr=4*Norder;
nt=0:1:nr-1;
R=real(fft(Ht.*Ht,1));
R =R(1:nr).*(0.54+0.46*cos(%pi*nt/(nr-1)));
Rwindow=[1/2,ones(1,n2-1),zeros(1,n-n2)]; 

nr=max(size(R));
Rm=toeplitz(R(Norder+1:nr-1),R(Norder+1:-1:2));
Rhs=- R(Norder+2:nr);
denf=[1 Rhs/Rm'];

A=polystab(denf);
nA=size(A,'*');
Dz=poly(A(nA:-1:1),'z','c');
Qh=numf([R(1)/2,R(2:nr)],A,Norder);	// compute additive decomposition
Qhsci=poly(Qh( size(Qh,'*'):-1:1 ),'z','c')
aa=A(:);bb=Qh(:);
nna=max(size(aa));nnb=max(size(bb));
Ss=2*real((fft([Qh,zeros(1,n-nnb)],-1) ./ fft([A,zeros(1,n-nna)],-1)));
hh=fft(exp(fft( Rwindow.*fft(log(Ss),1),-1)),1);
impr=filter(1,A,[1 zeros(1,nr-1)]);
B=real(hh(1:nr)/toeplitz(impr,[1 zeros(1,nb)])');
nB=size(B,'*');
Nz=poly(B(nB:-1:1),'z','c');
B =real(numf(hh(1:nr),A,nb));
if LHS==1 then
  Nz=syslin('d',Nz/Dz);
end



endfunction
function b=polystab(a);
//Utility function for use with yulewalk: polynomial stabilization.
//	polystab(A), where A is a vector of polynomial coefficients,
//	stabilizes the polynomial with respect to the unit circle;
//	roots whose magnitudes are greater than one are reflected
//	inside the unit circle.
if length(a) == 1, b=a; return, end
ll=size(a,'*');
ap=poly(a(ll:-1:1),'s','coeff');
v=roots(ap); i=find(v~=0);
vs=0.5*(sign(abs(v(i))-1)+1);
v(i)=(1-vs).*v(i) + vs./conj(v(i));
b=a(1)*coeff(poly(v,'s'));
b =b(ll:-1:1);
if ~or(imag(a))
	b=real(b);
end
  
endfunction
function y=filter(b,a,x)
//Clone of Matlab filter function
//
// filter Digital filter.
//Y=filter(B, A, X) filters the data in vector X with the
//filter described by vectors A and B to create the filtered
//data Y.  
//Implementation of the standard difference equation:
//
//    y(n)=b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
//        - a(2)*y(n-1) - ... - a(na+1)*y(n-na)
na=size(a,'*');nb=size(b,'*');
if na<nb then a=[a,zeros(1,nb-na)];na=nb;end;
y=rtitr(poly(b(nb:-1:1),'z','c'),poly(a(na:-1:1),'z','c'),x)
if na>nb then y=y(na-nb+1:length(y));end

endfunction
function b=numf(h,a,nb)
//NUMF	Find numerator B given impulse-response h of B/A and denominator A
//NB is the numerator order.  This function is used by yulewalk.
  
nh=max(size(h));
impr=filter(1,a,[1 zeros(1,nh-1)]);
b=h/toeplitz(impr,[1 zeros(1,nb)])';
endfunction
