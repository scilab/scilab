function conflicts=ge_check_names(g)

  names=g.node_name;
  re=find(names=='');ne=size(re,'*');
  [names,k1]=gsort(names,'g','i');
  
  conflicts=list(re);
  if size(names,'*') >1 then
    k3=find(names(2:$)==names(1:$-1));
    if k3<>[] then
      k3=unique([k3 k3+1])
      conflicts(2)=k1(k3)
    end
  end
endfunction
