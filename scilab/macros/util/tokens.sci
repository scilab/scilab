function t=tokens(str,sep)
  // returns the tokens included in a string
  if argn(2)<2 then sep=[' ',ascii(9)],end
  if size(str,'*')<> 1 then 
    error('numtok : str must be a simgle string')
  end
  kdf=tokenpos(str,sep) 
  t=[]
  for k=1:size(kdf,1)
    t=[t;part(str,kdf(k,1):kdf(k,2))]
  end
endfunction
