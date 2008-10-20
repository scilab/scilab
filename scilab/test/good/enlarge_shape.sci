function v1=enlarge_shape(dims,dims1,v)
//Utility function for cell insertion
//dims :  vector of old dimensions
//dims1 : vector of new dimensions
//v     : list of entries
//v1    : new list of entries
//
// undefined elements are set to []
// and(dims1>=dims) must be true
ki=find(dims1>dims)
if ki==nd then // the last dimension -> add empty entries at the end
  for k=prod(dims)+1:prod(dims1),v(k)=[];v1=v;end
else // enlarge and merge
  v1=list();for k=1:prod(dims1),v1(k)=[],end
  //merge compute the new index of M entries into enlarged cell
  I1=[];
  for k=size(dims1,'*'):-1:1
    ik1=(1:dims(k))'
    if ik1<>[] then
      if dims1(k)>1 then
	if size(I1,'*')>1 then
	  I1=(dims1(k)*I1).*.ones(ik1)+ones(I1).*.(ik1-1)
	else
	  I1=dims1(k)*I1+ik1-1
	end
      else
	I1=dims1(k)*I1+ik1-1
      end
    end
  end
  for iw=1:size(I1,'*'),v1(I1(iw)+1)=v(iw);end
end
endfunction
