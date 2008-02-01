function a=%nodedata_c_nodedata(a,b)
  if or(getfield(1,a)<>getfield(1,b)) then
    error('Incompatible node data fields')
  end
  F=getfield(1,a);F=F(2:$)

  for f=F
    a(f)=[a(f) b(f)];
  end
endfunction
