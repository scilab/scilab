function a=%cblock_c_s(a,b)
  if b==[] then return,end
   v=getfield($,a)
  
  if size(v,1)<>size(b,1) then error(5),end
  if type(b)==type(v) then
    setfield($,[v b],a)
  else
    setfield($+1,b,a)
  end
endfunction
