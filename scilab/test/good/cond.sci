function c=cond(A)
// Copyright INRIA
// c=cond(A) condition number for 2-norm
  if type(A)==1 then
    if A==[] then c=1,return,end
    if size(A,1)<>size(A,2) then error('Matrix must be square'),end
    s=svd(A)
    if s($)==0 then
      c=%inf
    else
      c=s(1)/s($)
    end
  else
    if type(A)==16|type(A)==17 then
      n=getfield(1,A);n=n(1)
    else
      [t,n]=typename()
      n=stripblanks(n(find(t==type(A))))
    end
    fun='%'+n+'_cond'
    if exists(fun)==1 then
      execstr('y='+fun+'(A,flag)')
      
    else
       error('cond not defined for type ""'+n+'"" .'+..
	     'Check argument or define function '+fun)
    end
  end
endfunction
