function r=rank(A,tol)
//Matrix rank
  if type(A)==1 then
    if A==[] then r=0;return,end  
    s = svd(A);
    if argn(2) == 1 then
      tol = max(size(A)) * s(1) * %eps;
    end
    r = size(find(s > tol),'*');
  else
    [t,n]=typename();n=stripblanks(n(find(t==type(A))))
    fun='%'+n+'_rank'
    if exists(fun)==1 then
      if argn(2)==1 then
	execstr('r='+fun+'(A)')
      else
	execstr('r='+fun+'(A,tol)')
      end
    else
      error('rank not defined for type ""'+n+'"" .'+..
	    'Check argument or define function '+fun)
    end
  end
endfunction
