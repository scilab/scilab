function b=%s_c_ce(a,b)
if prod(b.dims)==0 then
  b.dims=int32([1 1]);b.entries=list(a);
elseif b.dims(1)<>1 then 
  error(5)
else
  b.entries($+1)=a
  if size(b.dims,'*')<2 then 
    b.dims(2)=2
  else
    b.dims(2)=b.dims(2)+1
  end
end
endfunction



