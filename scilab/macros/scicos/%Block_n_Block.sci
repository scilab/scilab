function r=%Block_n_Block(b1,b2)
  //check if 2 blocks are differents
  r=%f
  fn=getfield(1,b1)
  r=or(fn<>getfield(1,b2))
  if r then return,end
  for k=2:size(fn,'*')
    r=b1(fn(k))<>b2(fn(k))
    if r then return,end
  end
endfunction
