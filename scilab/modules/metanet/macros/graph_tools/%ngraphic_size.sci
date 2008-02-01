function s=%ngraphic_size(d)
  Fd=getfield(1,d)
  s=size(d(Fd(4)));s=s(2)
endfunction
