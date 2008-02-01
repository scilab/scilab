function s=%egraphic_size(d)
  Fd=getfield(1,d)
  s=size(d(Fd(5)));s=s(2)
endfunction
