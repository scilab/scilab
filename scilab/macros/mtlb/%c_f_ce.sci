function b=%c_f_ce(a,b)
if prod(b.dims)==0 then
  b.dims=int32([1 1]);b.entries=list(a);
elseif b.dims(2)<>1 then
  error(6)
else
  b.entries(0)=a
  b.dims(1)=b.dims(1)+1
end
endfunction



