function a=%edgedata_c_edgedata(a,b)
  if or(getfield(1,a)<>getfield(1,b)) then
    error('Incompatible edge data fields')
  end
  F=getfield(1,a);F=F(2:$)

  for f=F
    a(f)=[a(f) b(f)];
  end
endfunction
