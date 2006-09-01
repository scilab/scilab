function b=%s_c_cblock(a,b)
  if a==[] then return,end
   v2=getfield(2,b)
  
  if size(a,1)<>size(v2,1) then error(5),end
  if type(a)==type(v2) then
    setfield(2,[a v2],b)
  else
    setfield(0,'cblock',b)
    setfield(2,a,b);
  end
endfunction
