function count = mtlb_fprintf(varargin)
// Copyright INRIA
// Emulation function for fprintf() Matlab function
// S.S. V.C.

[lhs,rhs]=argn()

count=0
// No fid given: mtlb_fprintf(fmt,...)
// Output is the screen

if size(varargin)==1 & type(varargin(1))==10 then
mprintf("%s \n",varargin(1))
count=length(varargin(1))
elseif type(varargin(1))==10 then
  fmt=varargin(1)
  nfmt=size(strindex(fmt,"%"),"*")
  nv=size(varargin)-1
  if nv==1 then
    a=varargin(2)
    na=size(a,"*")
    
    // If more values than formats
    mult=max(na/nfmt,1)
    fmt=strcat(fmt(ones(1,mult))) // duplicate format
    
    l=list()
    A=a
    for k=1:size(a,"*")
      l($+1)=A(k)
    end
    mprintf(fmt,l(:))
    count=size(a,"*")
  elseif nv==0 then
    error("In mtlb_fprintf: mprintf("+fmt+") is not implemented")
  else
    sz=[]
    for k=1:nv
      sz=[sz size(varargin(k+1),1)]
    end
    // Size of args is verified because Scilab mprintf function
    // does not work if args have more than one row
    if and(sz==1) then // All args have only one row
      mult=max(nv/nfmt,1)
      fmt=strcat(fmt(ones(1,mult))) // duplicate format 
      mprintf(fmt,varargin(2:$))
      count=size(sz,"*")
    else
      error("In mtlb_fprintf: mprintf Scilab function does not work with more than one row variables !")
    end
  end  
// mtlb_fprintf(fid,fmt,...)
else 
  fid=varargin(1)
  fmt=varargin(2)
  
  // count % in fmt
  nfmt=size(strindex(fmt,"%"),"*")
  nv=size(varargin)-2
  if nv==1 then
    a=varargin(3)
    na=size(a,"*")
    
    mult=max(na/nfmt,1)
    fmt=strcat(fmt(ones(1,mult))) // duplicate format
    
    l=list()
    for k=1:size(a,"*")
      l(k)=a(k)
    end
    if or(fid==[1 2]) then
      mprintf(fmt,l(:))
      count=size(a,"*")
    else
      mfprintf(fid,fmt,l(:))
      count=size(a,"*")
    end
  elseif nv==0 & nfmt==0 & type(varargin(2))==10
    mfprintf(fid,"%s",varargin(2))
    count=length(varargin(2))
  elseif nv==0 then
    if or(fid==[1 2]) then
      error("In mtlb_fprintf: mprintf(format) is not implemented")
    else
      error("In mtlb_fprintf: mfprintf(fid,format) is not implemented")
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
	count=size(sz,"*")
      else
	mfprintf(fid,fmt,varargin(3:$))
	count=size(sz,"*")
      end
    else
      if or(fid==[1 2]) then
	error("In mtlb_fprintf: mprintf Scilab function does not work with more than one row variables !")
      else
	mfprintfMat(fid,varargin(3:$),fmt)
	for k=1:nv
	  count=count+size(varargin(k+2),"*")
	end
      end 
    end
  end
end
endfunction
