function k=newest(varargin)
// given to files names f1 and f2 newest returns 
// the index of the newest file 
// non-existant files are supposed to be the oldest ones
//!
// Copyright INRIA
  n=size(varargin)
  if n==1 then names=varargin(1),else names=varargin,end
  n=prod(size(names))
  dat=zeros(1,n)
  for k=1:prod(size(names))
    nk=names(k)
    if strindex(nk,['SCI/','SCI\','sci/','sci\'])==1 then
      nk=SCI+part(nk,4:length(nk))
    elseif strindex(nk,['~/','~\'])==1 then
      nk=home+part(nk,2:length(nk))
    end
    info=fileinfo(nk)
    if info==[] then 
      dat(k)=0
    else
      dat(k)=info(6)
    end
  end
  [dat,k]=sort(dat)
  k=k(1)
endfunction
