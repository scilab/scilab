function s=filesuffix(path)
  k=strindex(path,'.')
  if k<>[] then
    s=part(path,k($)+1:length(path))
  else
    s=''
  end
endfunction
