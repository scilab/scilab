function str=%ce_string(c)
str=[];
  val=c.entries
  dims=double(c.dims);  
  if type(val)==15 then // Whole cell array display
    for k=1:length(val)
      tp=typeof(val(k))
      if tp=="st" then
	str(k)="{"+strcat(string(double(val(k).dims)),"x")+" struct}"
	//str(k)=%st_string(val(k))
      else
	sz=size(val(k))
	if prod(sz)==0 then 
	  str(k)='{}'
	else
	  str(k)='{'+strcat(string(sz),'x')+' '+tp+'}'
	end
      end
    end
  else // cell display
    tp=typeof(val)
    if tp=="st" then
      str="{"+strcat(string(double(val.dims)),"x")+" struct}"
    else
      sz=size(val)
      if prod(sz)==0 then 
	str='{}'
      else
	str='{'+strcat(string(sz),'x')+' '+tp+'}'
      end
    end
  end
  
  if size(dims,'*')>2 then
    str=hypermat(dims,str)
  else
    str=matrix(str,dims(1),-1)
  end
endfunction

