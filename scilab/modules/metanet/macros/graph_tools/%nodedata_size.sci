function s=%nodedata_size(d)
  Fd=getfield(1,d)
  if size(Fd,'*')<2 then 
    s=0
  else
    s=size(d(Fd(2)));s=s(2)
  end
endfunction
