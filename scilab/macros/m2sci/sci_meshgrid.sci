function [stk,txt,top]=sci_meshgrid()
// Copyright INRIA
txt=[]
if rhs<=2 then
  if rhs==1 then
    X=stk(top)(1);[mx,nx]=checkdims(stk(top));
    Y=X;my=mx;ny=nx;
  else
    X=stk(top-1)(1);[mx,nx]=checkdims(stk(top-1));
    Y=stk(top)(1);[my,ny]=checkdims(stk(top));
  end
  txt='// inline translation of meshgrid('+makeargs([X,Y])+')'
  if lhs==2 then
    [U,V]=lhsvarsnames()
    if mx==1 then
      txt=[txt;U+'='+X]
    else
      txt=[txt;
	  U+'=matrix('+X+',1,-1)']
    end
    if ny==1 then
      txt=[txt;V+'='+Y]
    else
      txt=[txt;
	  V+'=matrix('+Y+',-1,1)']
    end
    
    txt=[txt;
	U+'='+U+'(ones('+U+'),:)'
	V+'='+V+'(:,ones('+V+')'')']
    stk=list(list(U,'0','?','?','1'),list(V,'0','?','?','1'))
  else
    U=lhsvarsnames()
    if U==[] then U=gettempvar(),end
    if mx==1 then
      txt=[txt;U+'='+X]
    else
      txt=[txt;
	  U+'=matrix('+X+',1,-1)']
    end
    
    txt=[txt;
	U+'='+U+'(ones('+U+'),:)']
    stk=list(U,'0','?','?','1')
  end
else
  X=stk(top-2)(1)
  Y=stk(top-1)(1)
  Z=stk(top)(1)
  X='mtlb_meshgrid('+makeargs([X,Y,Z])+')'
  stk=list(list(X,'-1','?','?','1'),list(X,'-1','?','?','1'))
end
endfunction
