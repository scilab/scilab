function [coefs]=regress(x,y)
//
//This function computes the regresion coefficients of two
//variables x and y, both numerical vectors of same number
//of elements  n. coefs=[a  b] be a  1x2 matrix  such that
//Y=a+bX will be the equation of the ordinary least square
//approximation to our data.
//
//References:   Wonacott,    T.H.    &   Wonacott,   R.J.;
//Introductory Statistics, J.Wiley & Sons, 1990.
//
//author: carlos klimann
//
//date: 2001-02-12
//
  coefs=[]
  if (type(x) <> 1)|(type(y)<>1) then error('Both parameters must be numerical.'),end
  lx=length(x)
  if lx<>length(y) then error('Both parameters must have the same number of elements'), end
  if lx==0 then error('Input parameters are void'), end
  x=matrix(x,lx,1)
  y=matrix(y,lx,1)
  xbar=sum(x)/lx
  ybar=sum(y)/lx
  coefs(2)=sum((x-xbar).*(y-ybar))/sum((x-xbar).^2)
  coefs(1)=ybar-coefs(2)*xbar
endfunction
