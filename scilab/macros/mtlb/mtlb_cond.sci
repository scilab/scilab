function c=mtlb_cond(A,flag)
// Copyright INRIA
// Emulation function for cond() Matlab function
// F.B.

if A == [] then c = 0, return, end
if size(A,1) <> size(A,2) &  flag <> 2 then error('Matrix must be square'),end

if flag == 1
  c = norm(A,1)*norm(inv(A),1);
elseif flag == 2
  if size(A,1) == size(A,2) then  
    c = cond(A);
  else //case rectangular matrix
    s = svd(A)
    if s($) == 0 then
      c = %inf;
    else
      c = s(1)/s($);
    end
  end
elseif flag == "fro"
  c = norm(A,'fro')*norm(inv(A),'fro');
elseif flag == "inf"
  c = norm(A,'inf')*norm(inv(A),'inf');
else
  error("The second input argument must be 1, 2,''fro'' or ''inf''");  
end
endfunction