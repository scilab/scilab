function [a,nvarst] = mtlb_fscanf(fid,fmt,sz)
// Copyright INRIA
// Emulation function for fscanf() Matlab function
// S.S. V.C.

[lhs,rhs]=argn()

// If sz not given then read all file data
if rhs<3 then
  sz=%inf
end

nmx=prod(sz)
nvars=0

// Replicate the format many times to emulate Matlab format reuse
nfmt=size(strindex(fmt,"%"),"*")
fmt=strcat(fmt(ones(1,20/nfmt)))

a=[];
typ=10;
nvt=0;
nvarst=0;

while %t do
  // 20 values are read
  lvars=mfscanf(fid,fmt);

  if lvars==[] then // End of file
    break
  else
    nvars=size(lvars,2)
    nvarst=nvarst+nvars
    nv=min(nvars,nmx)
    if nv<>0 then
      if typ==10 then 
	for k=1:nv
	  typ=min(typ,type(lvars(k)))
	end
      end
      if typ==1&type(a)==10 then
	a=ascii(a)'
      end
      if typ==1 then
	for k=1:nv
	  if type(lvars(k))==1 then
	    a=[a;lvars(k)]
	  else
	    a=[a;ascii(lvars(k))']
	  end
	end
      else
	for k=1:nv
	  a=[a;lvars(k)]
	end

      end
    end
    nvt=nvt+nv
  end
end

nv=nvt
if typ==1 then
  if size(sz,"*")<>1 then 
    nv=size(a,"*")
    n=ceil(nv/sz(1))
    if n*sz(1)>nv then 
      a(n*sz(1))=0
    end
    a=matrix(a,sz(1),n),
  end
else
  if size(sz,"*")<>1 then
    if sz(1)<=nv then
      A=ascii(a)'
      nv=size(A,"*")
      n=ceil(nv/sz(1))
      if n*sz(1)>nv then 
	A(nv+1:n*sz(1))=ascii(" ")
      end
      A=matrix(A,sz(1),n)
      a=[]
      for l=1:sz(1)
	a=[a;ascii(A(l,:))]
      end
    end
  else
    a=strcat(a)
  end 
end
endfunction
