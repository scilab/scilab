function b=%i_f_cell(a,b)
if prod(b.dims)==0 then
  b.dims=[1 1];b.entries=list(a);
elseif b.dims(1)<>1 then
  error(6)
else
  b.entries($+1)=a
  b.dims(1)=b.dims(1)+1
end
endfunction



