function a=%cblock_f_cblock(a,b)
  if length(a)==1 then a=b,return,end
  if length(b)==1 then return,end

  if length(a)<>length(b) then error(6),end
  for k=2:length(a)
    setfield(k,[getfield(k,a);getfield(k,b)],a)
  end
endfunction
