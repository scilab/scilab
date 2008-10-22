function [m]=nanmeanf(val,fre,orient)
//
//This function returns in scalar m the mean of the values (ignoring the
//NANs) of  a  vector  or  matrix  val, each counted  with   a frequency
//signaled by the  corresponding values of the  integer vector or matrix
//fre with the same type of val.
//
//For     a   vector   or      matrix   val,   m=nanmeanf(val,fre)    or
//m=nanmeanf(val,fre,'*')    returns in scalar  m  the   mean of all the
//entries (ignoring   the NANs) of   val, each  value  counted  with the
//multiplicity indicated by the corresponding value of fre.
//
//m=nanmeanf(val,fre,'r')(or,     equivalently,   m=nanmeanf(val,fre,1))
//returns in each entry of the row vector m  of type 1xsize(val,'c') the
//mean of each column  of  val (ignoring  the NANs), each  value counted
//with the multiplicity indicated by the corresponding value of fre.
//
//m=nanmeanf(val,fre,'c')(or,   equivalently,     m=nanmeanf(val,fre,2))
//returns in each  entry of the column  vector m of type size(val,'c')x1
//the mean of  each row of val  (ignoring the NANs),  each value counted
//with the multiplicity indicated by the corresponding value of fre.
//
//author: carlos klimann
//
//date: 2000-01-17
//
  [lhs,rhs]=argn(0)
  if rhs<2|rhs>3 then error('nanmeanf requires two or three inputs.'), end
  if val==[]|fre==0 then m=[], return,end
  isn=isnan(val)
  fre(isn)=0
  val(isn)=0
  if rhs==2 then
    m=sum(val.*fre)/sum(fre)
  elseif orient=='*' then
    m=sum(val.*fre)/sum(fre)
  elseif orient=='r'|orient=='c'|orient==1|orient==2 then
    m=sum(val.*fre,orient)./sum(fre,orient)
  else ('Third parameter for nanmeanf must be *, r, c, 1 or 2'),
  end
endfunction
