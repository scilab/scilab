function a=%egraphic_c_egraphic(a,b)
  if or(getfield(1,a)<>getfield(1,b)) then
    error('Incompatible edge graphics fields')
  end
  F=getfield(1,a);F=F(5:$)
  for f=F
    a(f)=[a(f) b(f)];
  end
endfunction
