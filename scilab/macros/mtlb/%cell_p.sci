function %cell_p(c)
str=[]
val=c.entries
dims=c.dims;  
for k=1:size(val)
 sz=size(val(k))
 tp=typeof(val(k))
 if prod(sz)==0 then 
   str(k)='[]'
 else
   str(k)='['+strcat(string(sz),'x')+' '+tp+']'
 end
end
if size(dims,'*')>2 then
  str=hypermat(dims,str)
else
  str=matrix(str,dims(1),-1)
end
disp(str)
endfunction
