function [s,errmsg] = mtlb_sprintf(fmt,varargin)
[lhs,rhs]=argn()
count=0
//count % in fmt
nfmt=size(strindex(fmt,'%'),'*')
//number of format arguments 
nv=size(varargin)
if nv==1 then
  a=varargin(1)
  na=size(a,'*')
  mult=max(na/nfmt,1)
  s='';
  for k=1:mult,loc=a(1+(k-1)*nfmt:1+k*nfmt-1);
	l=list()
	for j=1:size(loc,'*'); l(j)=loc(j);end
	s=s+msprintf(fmt,l(:));
  end 
elseif nv==0 then
  s=msprintf(fmt)
else
  sz=[]
  for k=1:nv
    sz=[sz size(varargin(k),1)]
  end
  if and(sz==1) then
    mult=max(nv/nfmt,1)
    fmt=strcat(fmt(ones(1,mult))) // duplicate format 
    s=msprintf(fmt,varargin(:))
  else
    error('mtlb_printf this particular case is not implemented')
  end
end
K=strindex(s,'\n')
if K<>[] then
  w=s
  s=[]
  //if K(1)<>1 then K=[-1 K],end
  K=[-1 K]
  for k=2:size(K,'*')
    s=[s;part(w,K(k-1)+2:K(k)-1)]
  end
  if K($)<>length(w)-1 then
    s=[s;part(w,K($)+2:length(w))]
  end
end
endfunction
