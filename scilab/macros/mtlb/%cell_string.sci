function str=%cell_string(c)
str=[];
  val=c.entries
  dims=double(c.dims);  
  for k=1:size(val)
    tp=typeof(val(k))
    if tp=="st" then
      str=%struct_string(val(k))
    else
      sz=size(val(k))
      if prod(sz)==0 then 
	str(k)='{}'
      else
	str(k)='{'+strcat(string(sz),'x')+' '+tp+'}'
      end
    end
  end
  if size(dims,'*')>2 then
    str=hypermat(dims,str)
  else
    str=matrix(str,dims(1),-1)
  end
endfunction

