function path=mfile_path(nam)
// Copyright INRIA
fil=nam+'.m';nf=length(fil)
path=[];
for k=1:size(mfiles,'*')
  pk=mfiles(k);
  kk=strindex(pk,'/');
  if fil==part(pk,kk($)+1:length(pk)) then
    path=pk;
    break
  end
end
endfunction
