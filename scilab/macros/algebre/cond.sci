function c=cond(A)
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
    [t,n]=typename()
    n=stripblanks(n(find(t==type(A))))
    fun='%'+n+'_cond'
    if exists(fun)==1 then
      execstr('c='+fun+'(A)')
    else
      error('cond not defined for type ""'+n+'"" .'+..
	     'Check argument or define function '+fun)
    end
  end
endfunction
