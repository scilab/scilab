function [a,nvars,errmsg,nextindex] = mtlb_sscanf(s,fmt,sz)
[lhs,rhs]=argn()
if lhs==4 then error('mtlb_sscanf: nextindex not implemented'),end
if rhs<3 then sz=%inf,end
nmx=prod(sz)
nvars=0

errmsg=''
//replicate the format many times to emulate Matlab format reuse
fmt=strcat(fmt(ones(1,50)))

lvars=msscanf(s,fmt);
if lvars==-1 then
  a=''
  return
  errmsg='End of string reached before a datun has been read'
else
  nvars=size(lvars)
  nv=min(nvars,nmx)
  if nv==0 then
    a=[]
  else
    typ=10
    a=[]
    for k=1:nv,typ=min(typ,type(lvars(k))),end
    if typ==1 then
      for k=1:nv
	if type(lvars(k))==1 then
	  a=[a;lvars(k)]
	else
	  a=[a;ascii(lvars(k))']
	end
      end
      if size(sz,'*')<>1 then 
	nv=size(a,'*')
	n=ceil(nv/sz(1))
	if n*sz(1)>nv then a(n*sz(1))=0;end
	a=matrix(a,sz(1),n),
      end
    else
      for k=1:nv
	a=[a;lvars(k)]
      end
      if size(sz,'*')<>1 then
	if sz(1)<=nv then
	  A=ascii(a)'
	  nv=size(A,'*')
	  n=ceil(nv/sz(1))
	  if n*sz(1)>nv then A(nv+1:n*sz(1))=ascii(' ');end
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
  end
end
endfunction
