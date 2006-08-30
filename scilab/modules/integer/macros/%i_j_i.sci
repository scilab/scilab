function r=%i_j_i(a,n)
if or(n<0) then
  r=double(a).^double(n)
else
  if size(a,'*')==1 then
    r=iconvert(double(a).^double(n),inttype(a))
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
