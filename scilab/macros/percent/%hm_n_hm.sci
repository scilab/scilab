function r=%hm_n_hm(a,b)
r=and(a.dims==b.dims)
if r then
  r=hypermat(a.dims,a.entries<>b.entries)
else
  r=%t
end
endfunction
