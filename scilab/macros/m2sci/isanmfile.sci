function r=isanmfile(nam)
// Copyright INRIA
r=%f
if exists('sci_'+nam)==1 then
  r=%t
  return
end
if find(nam==['grid','hold','gca'])<>[] then
  // special case of untranslated function without arguments
  r=%t
  return
end 
if Recmode then
  for k=1:size(Paths,'*')
    [u,ierr]=file('open',Paths(k)+nam+'.m','old')
    if ierr==0 then
      file('close',u)
      r=%t
      break
    end
  end
else
  [u,ierr]=file('open',file_path+nam+'.m','old')
  if ierr==0 then
    file('close',u)
  end
  r=ierr==0
end
