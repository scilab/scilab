function r=%c_a_c(a,b)
if size(b,'*')==1 then
  b=b(ones(a))
elseif size(a,'*')==1 then
  a=a(ones(b))
else
  error(8)
end
r=a+b
endfunction
