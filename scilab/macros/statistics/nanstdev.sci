function [s]=nanstdev(x,orien)
//
//This function    computes the standard  deviation of   the values of a
//vector or matrix x (ignoring the NANs).
//
//For a vector or a matrix x, y=nanstdev(x)  returns in the scalar y the
//standard deviation of all the entries of x (ignoring the NANs).
//
//y=nanstdev(x,'r')  (or,  equivalently, y=nanstdev(x,1)) is the rowwise
//standard deviation. It returns in each  entry of the  row vector y the
//standard deviation of each column of x (ignoring the NANs).
//
//y=nanstdev(x,'c')  (or,    equivalently,   y=nanstdev(x,2))    is  the
//columnwise standard deviation. It returns in  each entry of the column
//vector y the standard deviation of each row of x (ignoring the NANs).
//
//author: carlos klimann
//
//date: 2000-01-25
//
//Fixings 2001-10-01. Communication from 
//Collewet Guylaine <guylaine.collewet@cemagref.fr>
//

  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs==0|rhs>2 then error('nanstdev requires one or two inputs.'), end
  if rhs==1 then 
    s=sqrt((sum((x(~isnan(x))-nanmean(x)).^2))/(length(x(~isnan(x)))-1))
  elseif orien=='*' then
    s=sqrt((sum((x(~isnan(x))-nanmean(x)).^2))/(length(x(~isnan(x)))-1))
  elseif orien=='r'|orien==1 then
    isn=isnan(x)
    y=nanmean(x,'r')
    x(isn)=0
    s=sqrt((sum((x-(ones(size(x,'r'),1)*y)).^2,'r'))./(sum(bool2s(~isn),'r')-1))
  elseif orien=='c'|orien==2 then
    isn=isnan(x)
    y=nanmean(x,'c')
    x(isn)=0
    s=sqrt((sum((x-(y*ones(1,size(x,'c')))).^2,'c'))./(sum(bool2s(~isn),'c')-1))
  else
    error('2rd argument of nanstdev must be equal to ''c'', ''r'', 1 or 2');
  end
endfunction
