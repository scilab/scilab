function res=determ(W,k)
// determinant of a polynomial or rational matrix by FFT
// W=square polynomial matrix
// k=``predicted'' degree of the determinant of W i.e. k is
// an integer larger or equal to the actual degree of W.
// Method: evaluate the determinant of W for the Fourier frequencies
// and apply inverse fft to the coefficients of the determinant.
// See also detr
//F.D.!
// Copyright INRIA
if W==[] then res=1;return;end;
if size(W,1)<>size(W,2) then 
  error('argument must be a square matrix'), 
end
n1=size(W,1) 
if n1==1 then res=W,return,end
//upper bound of the determinant degree
maj=n1*maxi(degree(W))+1;

if argn(2)==1 then 
  k=1;while k < maj,k=2*k;end
end
if n1>8 then write(%io(2),'Computing determinant: Be patient...');end
//          DEFAULT VALUES 
epsa=1.d-10;
epsr=0;//no relative rounding
//
e=0*ones(k,1);e(2)=1;
if k==1 then ksi=1,else ksi=fft(e,-1);end

fi=[];
if ~isreal(W,0)  then
  for kk=1:k,fi=[fi,det(horner(W,ksi(kk)))];end
  zzz=poly(fft(fi,1),varn(W),'c');
  res=clean(real(zzz),epsa,epsr)+%i*clean(imag(zzz),epsa,epsr);
  return
else
  for kk=1:k,fi=[fi,det(freq(W,ones(W),ksi(kk)))];end
  res=clean(real(poly(fft(fi,1),varn(W),'c')),epsa,epsr);
end
endfunction
