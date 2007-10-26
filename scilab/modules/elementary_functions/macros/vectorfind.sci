function ind=vectorfind(m,v,job)
//Copyright INRIA  
  if min(size(v))<>1 then error('second argument should be a vector'),end

  if convstr(part(job,1))=='r' then
    if size(v,'*')<>size(m,2) then error('Incompatible sizes'),end
    ind=1:size(m,1)
    for k=1:size(m,2)
      ind=ind(find(m(ind,k)==v(k)));
      if ind==[] then break,end
    end
  elseif convstr(part(job,1))=='c' then
    if size(v,'*')<>size(m,1) then error('Incompatible sizes'),end
    ind=1:size(m,2)
    for k=1:size(m,1)
      ind=ind(find(m(k,ind)==v(k)))
      if ind==[] then break,end
    end
  else
    error('third argument should be r[ow] or c[olumn]')
  end
endfunction
