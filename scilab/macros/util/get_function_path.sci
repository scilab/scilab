function path=get_function_path(name)
libname=whereis(name)
if libname==[] then
  error(name +' is not a library function')
end
execstr('t=string('+libname+')')
path=t(1)+name+'.sci'
if strindex(path,['SCI/','SCI\'])==1 then
  path=SCI+part(path,4:length(path))
end
endfunction
