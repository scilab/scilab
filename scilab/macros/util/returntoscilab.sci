function [txt,files]=returntoscilab()
files= listfiles(TMPDIR+'/Workspace')
n=size(files,1)
if n==0 then 
  txt=[],
  return,
else
  txtg='['+files(1)
  txtd='resume('+files(1)
  for i=2:n
    txtg=txtg+','+files(i)
    txtd=txtd+','+files(i)
  end
  txtg=txtg+']'
  txtd=txtd+')'
  txt=txtg+'='+txtd
end
endfunction
