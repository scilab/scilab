function a=%ce_c_s(a,b)
if prod(a.dims)==0 then
  a.dims=int32([1 1]);a.entries=list(b);
elseif a.dims(1)<>1 then 
  error(5)
else
  a.entries($+1)=b
  a.dims(2)=a.dims(2)+1
end
endfunction



