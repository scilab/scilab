function t=trimmean(x,discard,orien)
//
//A  trimmed mean  is calculated  by discarding  a certain
//percentage of the lowest and the highest scores and then
//computing  the  mean   of  the  remaining  scores.   For
//example, a  mean trimmed  50% is computed  by discarding
//the lower  and higher 25%  of the scores and  taking the
//mean of the remaining scores.
//
//The median  is the mean trimmed 100%  and the arithmetic
//mean is the mean trimmed 0%.
//
//A  trimmed mean  is  obviously less  susceptible to  the
//effects of  extreme scores than is  the arithmetic mean.
//It is therefore less susceptible to sampling fluctuation
//than the mean for  extremely skewed distributions. It is
//less  efficient (The  efficiency of  a statistic  is the
//degree to  which the statistic is stable  from sample to
//sample.    That  is,  the   less  subject   to  sampling
//fluctuation  a  statistic  is,  the  more  efficient  it
//is. The efficiency of statistics is measured relative to
//the  efficiency  of other  statistics  and is  therefore
//often called the relative efficiency. If statistic A has
//a  smaller   standard  error  than   statistic  B,  then
//statistic  A is  more efficient  than statistic  B.  The
//relative efficiency of two  statistics may depend on the
//distribution involved.   For instance, the  mean is more
//efficient than  the median for  normal distributions but
//not  for  some   extremely  skewed  distributions.   The
//efficiency of a statistic can  also be thought of as the
//precision  of  the  estimate:  The  more  efficient  the
//statistic,  the  more precise  the  statistic  is as  an
//estimator         of         the         parameter.[from
//http://davidmlane.com/hyperstat/A12977.html])  than  the
//mean for normal distributions.
//
//Trimmed  means  are often  used  in  Olympic scoring  to
//minimize the effects  of extreme ratings possibly caused
//by            biased            judges.            [from
//http://davidmlane.com/hyperstat/A11971.html]
//
//
//For a vector  or matrix x, t=trimmean(x,discard) returns
//in  scalar t the  mean of  all the  entries of  x, after
//discarding discard/2 highest values and discard/2 lowest
//values.
//
//t=trimmean(x,discard,'r')       (or,       equivalently,
//t=trimmean(x,discard,1))  returns in  each entry  of the
//row vector t the trimmed mean of each column of x.
//
//t=trimmean(x,discard,'c')       (or,       equivalently,
//t=trimmean(x,discard,2))  returns in  each entry  of the
//column vector t the trimmed mean of each row of x.
//
//References:  Luis   Angel  Garcia-Escudero  and  Alfonso
//Gordaliza,  Robustness Properties  of Means  and Trimmed
//Means, JASA, Volume 94, Number 447, Sept 1999, pp956-969
//
//author: carlos klimann
//
//date: 2002-10-10
//
  [lhs,rhs]=argn(0)
  if rhs==0 then error('trimmean requires at least two input parameters.'), end
  if x==[] then t=%nan;return,end
  if rhs==1 then  error('trimmean requires at least two input parameters.'), end
  if rhs==2 then 
    if type(discard)<>1 then error('2nd input parameter must be a number'), end
    if discard > 100 | discard <0 then
      error('second input parameter must be in the 0-100 range.'), end
      sizx=size(x,'*')
      nomdis=sizx*discard/200
      if floor(nomdis)==0 then nomdis=1, end
      disp(nomdis)
      x=sort(x)
      t=sum(x(floor(nomdis):ceil(sizx-nomdis))) / (2*nomdis)
      return,
  end
  if rhs==3 then
    if orien=='r' | orien==1 then
      sizx=size(x,'r')
      nomdis=sizx*discard/200
      if floor(nomdis)==0 then nomdis=1, end
      x=sort(x,'r')
      t=sum(x(floor(nomdis):ceil(sizx-nomdis),:),'r') / (2*nomdis)
      return,
    end
    if orien=='c' | orien==2 then
      sizx=size(x,'c')
      nomdis=sizx*discard/200
      if floor(nomdis)==0 then nomdis=1, end
      t=sum(x(:,floor(nomdis):ceil(sizx-nomdis)),'c') / (2*nomdis)
      return,
    end,
  end
  error('trimmean requires at most three input parameters')
endfunction
