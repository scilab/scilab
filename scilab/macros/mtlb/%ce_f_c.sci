function a=%ce_f_c(a,b)
if prod(a.dims)==0 then
  a.dims=int32([1 1]);a.entries=list(b);
elseif a.dims(1)<>1 then
  error(6)
else
  a.entries($+1)=b
  a.dims(1)=a.dims(1)+1
end
endfunction



