function count = mtlb_fprintf(varargin)
[lhs,rhs]=argn()
count=0
if type(varargin(1))==10 then //mtlb_fprintf(fmt,...)
  fmt=varargin(1)
  nfmt=size(strindex(fmt,'%'),'*')
  nv=size(varargin)-1
  if nv==1 then
    a=varargin(2)
    na=size(a,'*')
    mult=max(na/nfmt,1)
    fmt=strcat(fmt(ones(1,mult))) // duplicate format
    l=list()
    A=a
    for k=1:size(a,'*'),l($+1)=A(k); end
    mprintf(fmt,l(:))
  elseif nv==0 then
    mprintf(fmt)
  else
    sz=[]
    for k=1:nv
      sz=[sz size(varargin(k+1),1)]
    end
    if and(sz==1) then
      mult=max(nv/nfmt,1)
      fmt=strcat(fmt(ones(1,mult))) // duplicate format 
      mprintf(fmt,varargin(2:$))
    else
    error('mtlb_fprintf this particular case is not implemented')
    end
  end  
else //mtlb_fprintf(fid,fmt,...)
  fid=varargin(1)
  fmt=varargin(2)
  
  //count % in fmt
  nfmt=size(strindex(fmt,'%'),'*')
  nv=size(varargin)-2
  if nv==1 then
    a=varargin(3)
    na=size(a,'*')
    mult=max(na/nfmt,1)
    fmt=strcat(fmt(ones(1,mult))) // duplicate format
    l=list()
    for k=1:size(a,'*'),l(k)=a(k); end
    if or(fid==[1 2]) then
      mprintf(fmt,l(:))
    else
      mfprintf(fid,fmt,l(:))
    end
  elseif nv==0 then
    if or(fid==[1 2]) then
      mprintf(fmt)
    else
      mfprintf(fid,fmt)
    end 
  else
    sz=[]
    for k=1:nv
      sz=[sz size(varargin(k+2),1)]
    end
    if and(sz==1) then
      mult=max(nv/nfmt,1)
      fmt=strcat(fmt(ones(1,mult))) // duplicate format 
      if or(fid==[1 2]) then
	mprintf(fmt,varargin(3:$))
      else
	mfprintf(fid,fmt,varargin(3:$))
      end
    else
      error('mtlb_fprintf this particular case is not implemented')
    end
  end
end
endfunction
