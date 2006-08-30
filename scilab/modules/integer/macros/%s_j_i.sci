function r=%s_j_i(a,n)
isn=inttype(n)
if or(double(iconvert(a,isn))<>a)|or(n<0) then
  r=a.^double(n)
else
  if size(a,'*')==1 then
    r=iconvert(a.^double(n),inttype(n))
  else
    if size(n,'*')<>1 then
      error(43)
    else
      n=double(n)
      r=a
      for i=2:n,r=r.*a,end
    end
  end
end  
endfunction
