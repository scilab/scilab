function M=%hm_k_s(M1,M2)
// Copyright INRIA
// V.C.
hmsize = double(M1.dims);
ssize = size(M2);
ssize = [ssize ones(1,length(hmsize)-length(ssize))];
hmsize = [hmsize ones(1,length(ssize)-length(hmsize))];
subs=list()
for k=length(ssize):-1:1
  ind = (1:ssize(k))';
  subs(k) = ind(:,ones(1,hmsize(k)));
end
M = M2(subs(:));
endfunction

