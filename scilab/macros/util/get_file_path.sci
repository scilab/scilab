function dir = get_file_path(name)
//-------------------------------
// returns loader full path
dir ='./';
[units,typs,nams]=file();
clear units typs
for k=size(nams,'*'):-1:1
  l=strindex(nams(k),name);
  if l<>[] then
    dir=part(nams(k),1:l($)-1);
    break
  end
end
if dir == "" then dir = "." ; end
endfunction
