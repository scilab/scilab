function a=%cblock_f_cblock(a,b)
  if length(a)<>length(b) then error(6),end
  for k=2:length(a)
    setfield(k,[getfield(k,a);getfield(k,b)],a)
  end
endfunction
