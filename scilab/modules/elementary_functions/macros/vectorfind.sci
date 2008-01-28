function ind=vectorfind(m,v,job)
//Copyright INRIA  
  if min(size(v))<>1 then
    error(msprintf(gettext("%s: Wrong size for second input argument: Vector expected.\n"),"vectorfind"));
  end

  if convstr(part(job,1))=='r' then
    if size(v,'*')<>size(m,2) then
      error(msprintf(gettext("%s: Wrong size for input arguments: Incompatible sizes.\n"),"vectorfind"));
    end
    ind=1:size(m,1)
    for k=1:size(m,2)
      ind=ind(find(m(ind,k)==v(k)));
      if ind==[] then break,end
    end
  elseif convstr(part(job,1))=='c' then
    if size(v,'*')<>size(m,1) then
      error(msprintf(gettext("%s: Wrong size for input arguments: Incompatible sizes.\n"),"vectorfind"));
    end
    ind=1:size(m,2)
    for k=1:size(m,1)
      ind=ind(find(m(k,ind)==v(k)))
      if ind==[] then break,end
    end
  else
    error(msprintf(gettext("%s: Wrong value for third input argument: ''%s'' or ''%s'' expected.\n"),"vectorfind","r[ow]","c[olumn]"));
  end
endfunction
