function conflicts=ge_check_names(g)

  names=g.node_name;
  re=find(names=='');ne=size(re,'*');
  [names,k1]=gsort(names,'g','i');
  [N,k2]=unique(names(ne+1:$));
  k2=[0 k2];
  conflicts=list(re);
  k3=find(k2(2:$)-k2(1:$-1)>1);
  for i=k3
    sel=((k2(i)+1):(k2(i+1)-1));
    conflicts($+1)=[k1(ne+k2(i+1)),k1(ne+sel)];
  end
endfunction
