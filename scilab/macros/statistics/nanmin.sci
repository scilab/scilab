function [m,index] = nanmin(x,orient)
//
//This function gives for a real or a numerical matrix  x a his smallest
//element m (but ignoring the NANs).
//
//For x,  a numerical vector or matrix,  m=nanmin(x) returns in scalar m
//the smallest element of x  (ignoring the NANs).   The form [m,index] =
//nanmin(x,orient)   gives  in addition  of  the  value  of the smallest
//element of x (ignoring the NANs) in scalar  m, index the index of this
//element in x, as a 2-vector.
//
//m=nanmin(x,'r')    gives in  the  1xsize(x,2)   matrix  m the smallest
//elements  (ignoring  the  NANs) of  each  column  of  x.   If the form
//[m,index]=nanmin(x,'r')  is used,   the  elements of  the  1xsize(x,2)
//matrix are the indexes of the smallest elements (ignoring the NANs) of
//each column of x in the corresponding column.
//
//m=nanmin(x,'c')   gives in   the  size(x,2)x1 matrix   m  the smallest
//elements  (ignoring  the  NANs)  of  each row   of  x.    If the  form
//[m,index]=nanmin(x,'c')  is   used, the  elements   of the size(x,2)x1
//matrix are the indexes of the smallest elements (ignoring the NANs) of
//each row of x in the corresponding row.
//
//author: carlos klimann
//
//date: 2000-01-14
//
  [lhs,rhs]=argn(0)
  if rhs==0|rhs>2 then error('nanmin requires one or two inputs.'), end
  if x==[] then m=[], return,end
  x(isnan(x))=%inf
  if lhs==0|lhs==1 then
    if rhs==1 then
      m=min(x)
    else
      if orient=='r'|orient=='c' then
	m=min(x,orient),
      else
	error('second parameter for nanmin must be r or c'),
      end
    end
  else
    if rhs==0|rhs==1 then
      [m,index]=min(x)
    else
      if orient=='r'|orient=='c' then
	[m,index]=min(x,orient)
      else
	error('second parameter for nanmin must be r or c'),
      end
    end
  end
endfunction
