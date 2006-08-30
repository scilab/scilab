function r=%i_p_s(a,n)
if or(int(n)<>n)|or(n<0) then
  r=double(a)^n
else
  if size(a,'*')==1 then
    r=iconvert(double(a)^n,inttype(a))
  else
    if size(n,'*')<>1 then
      error(43)
    else
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
