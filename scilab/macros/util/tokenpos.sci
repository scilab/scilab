function kdf=tokenpos(str,sep)
//return a vector of indices giving the beginning of the included
//tokens
  if argn(2)<2 then sep=[' ',ascii(9)],end
  
  if size(str,'*')<> 1 then 
    error('numtok : str must be a single string')
  end
  if or(length(sep)<>1) then 
    error('numtok : delimitor must be a character or a vector of "+...
	  "characters')
  end  
  str=sep(1)+str+sep(1)
  k1=strindex(str,sep)
  i=find(k1(2:$)-k1(1:$-1)>1)
  if i<>[] then
    kdf=[k1(i);k1(i+1)-2]'
  else
    kdf=[]
  end
endfunction
