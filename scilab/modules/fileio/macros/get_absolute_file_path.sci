function path = get_absolute_file_path(name)
//-------------------------------
// returns loader full path
path ="";
if ~MSDOS then	sep = '/'; else sep = '\' ;end
[units,typs,nams]=file();
clear units typs
for k=size(nams,'*'):-1:1
  l=strindex(nams(k),name);
  if l<>[] then
    path= part(nams(k),1:l($)-1);
    break
  end
end

if ~MSDOS then
  if part(path,1)<>'/' then 
    path=getcwd()+sep+path
  end
else 
  if (part(path,1) == '\') & (part(path,2) == '\') then
  // UNC Path
  else
    if (part(path,2)<>':') then 
      path=getcwd()+sep+path
    end
  end


end
endfunction
