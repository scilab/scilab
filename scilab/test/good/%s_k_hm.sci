function M=%s_k_hm(M1,M2)
// Copyright INRIA
// V.C.
hmsize = double(M2.dims);
ssize = size(M1);
ssize = [ssize ones(1,length(hmsize)-length(ssize))];
hmsize = [hmsize ones(1,length(ssize)-length(hmsize))];
subs=list()
for k=length(hmsize):-1:1
  ind = (1:hmsize(k))';
  subs(k) = ind(:,ones(1,ssize(k)));
end
M = M2(subs(:));
endfunction

