function r=%s_p_i(a,n)
isn=inttype(n)
if or(double(iconvert(a,isn))<>a)|or(n<0) then
  r=a^double(n)
else
  if size(a,'*')==1 then
    r=iconvert(a^double(n),inttype(n))
  else
    if size(n,'*')<>1 then
      error(43)
    else
      n=double(n)
      r=a
      if size(a,1)==size(a,2) then
	for i=2:n,r=r*a,end
      else
	for i=2:n,r=r.*a,end
      end
    end
  end
end  
endfunction
