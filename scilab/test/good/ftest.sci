function [f,p]=ftest(samples)
//
//Given a  number nc of samples each  of them composed of nr
//observations this fonction computes in f the Fischer ratio
//(it is the  ratio between  nr times  the variance   of the
//means of  samples  and the  mean of the  variances of each
//sample).  Additionnally it gives (in p) the p-value of the
//computed  Fischer ratio.  samples   is a matrix  with type
//nrxnc.
//
//References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
//Statistics, J.Wiley & Sons, 1990.
//
//author: carlos klimann
//
//date: 2000-06-29
//
  [nr nc]=size(samples)
  xbar=mean(samples,'r')
  xbarbar=mean(xbar)
  sx2=sum((xbar-xbarbar).^2)/(nc-1)
  sp2=(sum(sum((samples-ones(nr,1)*xbar).^2),'r'))/(nc*(nr-1))
  f=(nr*sx2)/sp2
  p=1-cdff("PQ",f,(nc-1),(nc*(nr-1)))
endfunction
