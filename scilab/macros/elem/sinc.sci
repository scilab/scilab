function y=sinc(x)
//               [  sin(x(i))/x(i) if x(i)~=0
// computes y(i)=[
//               [  1 if x(i)~=0
  y=ones(x)
  kz=find(x<>0)
  y(kz)=sin(x(kz))./(x(kz));
endfunction
