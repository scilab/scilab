function [m,index] = nanmax(x,orient)
//
//This function gives for a  real or a  numerical matrix x a his largest
//element m (but ignoring the NANs).
//
//For x,  a numerical vector or matrix,  m=nanmax(x) returns in scalar m
//the largest  element of x (ignoring the  NANs).  The form  [m,index] =
//nanmax(x,orient) gives in addition of the value of the largest element
//of x (ignoring the NANs) in scalar m, and    the index of this element
//in x, as a  2-vector.
//
//m=nanmax(x,'r') gives in the 1xsize(x,2) matrix m the largest elements
//(ignoring the    NANs)   of each   column    of  x.   If   the    form
//[m,index]=nanmax(x,'r')  is used,    the elements of  the  1xsize(x,2)
//matrix index are the indexes  of the  largest  elements  (ignoring the
//NANs) of each column of x in the corresponding column.
//
//m=nanmax(x,'c') gives in the size(x,2)x1 matrix m the largest elements
//(ignoring    the    NANs) of    each   row   of   x.    If  the   form
//[m,index]=nanmax(x,'c') is  used,   the  elements of   the size(x,2)x1
//matrix index are  the  indexes of the  largest  elements (ignoring the
//NANs) of each row of x in the corresponding row.
//
//author: carlos klimann
//
//date: 2000-01-14
//
  [lhs,rhs]=argn(0)
  if rhs==0|rhs>2 then error('nanmax requires one or two inputs.'), end
  if x==[] then m=[], return,end
  x(isnan(x))=-%inf
  if lhs==0|lhs==1 then
    if rhs==2 then
      if orient=='r'|orient=='c', m=max(x,orient),
      else  error('second parameter for nanmax must be r or c'),
      end
    else m=max(x),
    end
  elseif lhs==2 then 
    if rhs==2 then
      if orient=='r'|orient=='c', [m, index]=max(x,orient),
      else  error('second parameter for nanmax must be r or c'),
      end
    else [m,index]=max(x),
    end
  end
endfunction
