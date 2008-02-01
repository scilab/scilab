function save_graph(g,path)
// Copyright INRIA
// path
  if argn(2)<2 then path='./',end
  if type(path)<>10|prod(size(path))<>1 then
    error('Second argument must be a string')
  end
  path=pathconvert(path,%t,%t)
  if isdir(path) then
    path=path+g.name+'.graph'
  else
    l=length(path)
    if part(path,l-5:l)<>'.graph' then
      path=path+'.graph'
    end
  end

  check_graph(g)
  save(path,g)
endfunction
