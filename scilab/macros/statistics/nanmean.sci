function [m]=nanmean(val,orient)
//
//This function returns in scalar m the mean of the values (ignoring the
//NANs) of  a  vector  or  matrix  val.
//
//For  a vector   or  matrix val,  m=nanmean(val) or  m=nanmean(val,'*')
//returns in scalar m the mean of all the entries (ignoring the NANs) of
//val.
//
//m=nanmean(val,'r')  (or,   equivalently, m=nanmean(val,1))  returns in
//each entry of  the row vector  m  of type 1xsize(val,'c')  the mean of
//each column of val (ignoring the NANs).
//
//m=nanmeanf(val,'c')(or, equivalently, m=nanmean(val,2))  returns    in
//each entry of the column vector m of  type size(val,'c')x1 the mean of
//each row of val (ignoring the NANs).
//
//author: carlos klimann
//
//date: 2000-02-15
//
  [lhs,rhs]=argn(0)
  if rhs<1|rhs>2 then error('nanmean requires one or two inputs.'), end
  if val==[] then m=[], return,end
  isn=isnan(val)
  val(isn)=0
  if rhs==1 then
    m=sum(val)/sum(bool2s(~isn))
  elseif orient=='*' then
    m=sum(val)/sum(bool2s(~isn))
  elseif orient=='r'|orient=='c'|orient==1|orient==2 then
    m=sum(val,orient)./sum(bool2s(~isn),orient)
  else error('Second parameter for nanmean must be *, r, c, 1 or 2'),
  end
endfunction
