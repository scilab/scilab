function add_palette(title,path)
  global scicos_pal
  path=pathconvert(path,%f,%t)
  if fileinfo(path)==[] then  error('file: '+path+' do not exist'),end
  if or(fileparts(path,'extension')==['.cos','.cosf']) then
    k=find(scicos_pal(:,1)==title)
    if k==[] then
      scicos_pal=[scicos_pal;title,path]
    else
      k1=find(scicos_pal(k,1)==path)
      if k1==[] then scicos_pal=[scicos_pal;title+' ('+string(size(k,'*'))+')',path],end
    end
  else
    error('second argument should be a path to a .cos or .cosf file')
  end
endfunction
