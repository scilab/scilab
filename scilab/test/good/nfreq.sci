function [m]=nfreq(x)
//
//Frequence of the values in a real or complex vector or a  real
//or complex matrix x.
//
//For a real or complex vector or a  real or complex matrix x,
//m=freq(x) returns in the first column of the size(x,'*')x2 matrix
//m the values of x and in the second column of this matrix the
//frequences of the corresponding values.
//
//author: carlos klimann
//
//date: 2000-01-11
//
  lxnozero=size(x,'*');
  xx=matrix(x,lxnozero,1);
  m=[]
  while lxnozero>0 do
    xnozero=find(xx-xx(1)<>0)',
    l=lxnozero,
    lxnozero=size(xnozero,'*'),
    m=[m ; xx(1) l-lxnozero],
    xx=xx(xnozero),
  end
endfunction
