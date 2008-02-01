function a=%ngraphic_c_ngraphic(a,b)
  if or(getfield(1,a)<>getfield(1,b)) then
    error('Incompatible node graphics fields')
  end
  F=getfield(1,a);F=F(4:$)
  for f=F
    a(f)=[a(f) b(f)];
  end
endfunction
