function r=%hm_o_hm(a,b)
r=and(a.dims==b.dims)
if r then
  r=hypermat(a.dims,a.entries==b.entries)
else
  r=%f
end
endfunction
