function r=isdir(fil)
//checks  if given path is a directory
//see C function stat
  if or(fil==['SCI','~']) then fil=fil+'/',end

  mask=int32(61440);//o0170000
  dirtype=16384;//o0040000
  x=fileinfo(fil)
  if x==[] then 
    r=%f
  else
    r=double(int32(x(2))&mask)==dirtype
  end
endfunction
