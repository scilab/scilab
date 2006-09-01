function M=%hm_k_hm(M1,M2)
// Copyright INRIA
// V.C.
hm1size = double(M1.dims);
hm2size = double(M2.dims);
hm1size = [hm1size ones(1,length(hm2size)-length(hm1size))];
hm2size = [hm2size ones(1,length(hm1size)-length(hm2size))];
subs=list()
for k=length(hm2size):-1:1
  ind = (1:hm2size(k))';
  subs(k) = ind(:,ones(1,hm1size(k)));
end
M = M2(subs(:));
endfunction
