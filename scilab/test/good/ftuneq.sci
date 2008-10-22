function [f,p]=ftuneq(varargin)
//
//This function computes the F ratio for samples of unequal
//size.
//
//"The most  efficient design is  to make all  samples the
//same  size n.   However when  this is  nor  feasible, it
//still  is possible  to modify  the  ANOVA calculations."
//Note  that  the  definition  of  xbarbar  is  no  longer
//mean(xbar), but  rather a weighted  average with weights
//ni.  Additionnally  it gives (in  p) the p-value  of the
//computed Fischer ratio.
//
//Given a number  a of samples each of  them composed of n_i
//(i from 1  to a) observations this fonction  computes in f
//the Fischer  ratio (it is  the ratio between nr  times the
//variance  of the  means of  samples  and the  mean of  the
//variances of each sample).
//
//References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
//Statistics, J.Wiley & Sons, 1990.
//
//author: carlos klimann
//
//date: 2000-06-29
//
  [lhs,rhs]=argn(0)
  data=[]
  total=0
  sse=0
  for i=1:rhs
    len(i)=size(varargin(i),'*'),
    mat=matrix(varargin(i),len(i),1),
    partial=sum(mat),
    xbar(i)=partial/len(i),
    total=total+partial,
    data=[data; mat]
    sse=sse+sum((mat-xbar(i)).^2)
  end
  xbarbar=total/sum(len)
  ssa=len'*((xbar-xbarbar).^2)
  msa=ssa/(rhs-1)
  deglib=sum(len-1)
  mse=sse/deglib
  f=msa/mse
  p=1-cdff("PQ",f,(rhs-1),deglib)
endfunction
