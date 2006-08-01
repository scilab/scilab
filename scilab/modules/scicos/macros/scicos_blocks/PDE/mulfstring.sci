function x=mulfstring(a,b)
// Copyright INRIA
// développé par EADS-CCR
  if ( a == [] | b == []) then
     x=[];
     return;
  end
  [l,m]=size(a);[m,n]=size(b);x=[];
  for j=1:n,
    y=[];
    for i=1:l,
      t=' ';
      for k=1:m,
        if (k > 1) then
          //t=t+'+('+a(i,k)+')*'+'('+b(k,j)+')';
          t=addfv(t,mulfv(a(i,k),b(k,j)));
        else
          //t='(' + a(i,k) +')*' + '(' + b(k,j) + ')';
          t=mulfv(a(i,k),b(k,j));
        end
      end
      y=[y;t];
    end
    x=[x y];
  end
endfunction
