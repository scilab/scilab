function [q]=iqr(x,orien)
//
//This  function computes the interquartile  range IQR= upper quartile -
//lower quartile of a vector or a matrix x.
//
//For  a  vector or  a matrix  x, q=iqr(x) returns  in  the scalar q the
//interquartile range of all the entries of x.
//
//q=iqr(x,'r')    (or,   equivalently,  q=iqr(x,1))    is   the  rowwise
//interquartile range. It returns in each entry of the  row vector q the
//interquartile range of each column of x.
//
//q-iqr(x,'c')    (or,  equivalently, q=iqr(x,2))    is  the  columnwise
//interquartile range. It returns in  each entry of  the column vector q
//the interquartile range of each row of x.
//
//References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
//Statistics, J.Wiley & Sons, 1990.
//
//author: carlos klimann
//
//date: 1999-05-12
//
  if x==[] then q=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs==0 then error('iqr requires at least one input.'), end
  if rhs==1 then
    qq=quart(x)
  else
    qq=quart(x,orien)
  end
  q=qq(3)-qq(1)
  return
endfunction
