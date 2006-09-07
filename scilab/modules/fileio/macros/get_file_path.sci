function path = get_file_path(name)
//-------------------------------
// returns loader full path
path ='./';
[units,typs,nams]=file();
clear units typs
for k=size(nams,'*'):-1:1
  l=strindex(nams(k),name);
  if l<>[] then
    path=part(nams(k),1:l($)-1);
    break
  end
end
if path == "" then path = pwd() ; end
endfunction
