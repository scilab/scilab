function n=mtlb_norm(A,p)
// Copyright INRIA
// Emulation function for norm() Matlab function
// V.C.

rhs=argn(2)

// n = norm(A)
if rhs==1 then
  n=norm(A)
// n = norm(A,p)
else
  if type(p)<>10 then
    if isinf(p) & p<0 then // p=-%inf
      if find(size(A)==1)==[] & ~isempty(A) then
	n=max(sum(abs(A'),1))
      else
	n=norm(A,p)
      end
    else
      n=norm(A,p)
    end
  else
    n=norm(A,p)
  end
end
endfunction
