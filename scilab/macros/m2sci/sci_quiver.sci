function [stk,txt,top]=sci_quiver()
// Copyright INRIA
txt=[]
select rhs
case 2 then //quiver(U,V)
  nx=gettempvar(1)
  ny=gettempvar(2)
  txt=[nx+' = 1:size('+stk(top-1)(1)+'''*'')'
      nx+' = ['+nx+';'+nx+' + matrix('+stk(top-1)(1)+',1,-1)]'
      ny+' = 1:size('+stk(top)(1)+'''*'')'
      ny+' = ['+ny+';'+ny+' + matrix('+stk(top)(1)+',1,-1)]']
case 3 then //quiver(U,V,linespec) or quiver(U,V,scale)
  if stk(top)(5)=='10' then //quiver(U,V,linespec)
    set_infos('quiver '+stk(top)(1)+' option not yet handled',1)
  elseif stk(top)(5)=='1' then //quiver(U,V,scale)
    set_infos('quiver '+stk(top)(1)+' option not yet handled',1)
  else //??
    set_infos('quiver '+stk(top)(1)+' option not yet handled',1)
  end
  nx=gettempvar(1)
  ny=gettempvar(2)
  txt=[nx+' = 1:size('+stk(top-2)(1)+'''*'')'
      nx+' = ['+nx+';'+nx+' + matrix('+stk(top-2)(1)+',1,-1)]'
      ny+' = 1:size('+stk(top-1)(1)+'''*'')'
      ny+' = ['+ny+';'+ny+' + matrix('+stk(top-1)(1)+',1,-1)]']
       
case 4 then //quiver(U,V,linespec,'filled') or quiver(X,Y,U,V)
  if stk(top)(5)<>'10' then //quiver(x,y,u,v)
    nx=gettempvar(1)
    ny=gettempvar(2)
    txt=[nx+' = matrix('+stk(top-3)(1)+',1,-1)'
	nx+' = ['+nx+';'+nx+' + matrix('+stk(top-1)(1)+',1,-1)]'
	nx+' = matrix('+stk(top-2)(1)+',1,-1)'
	ny+' = ['+ny+';'+ny+' + matrix('+stk(top)(1)+',1,-1)]']
  else
    set_infos('quiver '+stk(top-1)(1)+' option not yet handled',1)
  end
  
case 5 then //quiver(X,Y,U,V,linespec) or quiver(X,Y,U,V,scale)
  set_infos('quiver '+stk(top)(1)+' option not yet handled',1)
  nx=gettempvar(1)
  ny=gettempvar(2)
  txt=[nx+' = matrix('+stk(top-3)(1)+',1,-1)'
      nx+' = ['+nx+';'+nx+' + matrix('+stk(top-1)(1)+',1,-1)]'
      nx+' = matrix('+stk(top-2)(1)+',1,-1)'
      ny+' = ['+ny+';'+ny+' + matrix('+stk(top)(1)+',1,-1)]']
case 6 then //quiver(X,Y,U,V,linespec,'filled') 
    set_infos('quiver '+stk(top)(1)+' option not yet handled',1)
  nx=gettempvar(1)
  ny=gettempvar(2)
  txt=[nx+' = matrix('+stk(top-3)(1)+',1,-1)'
      nx+' = ['+nx+';'+nx+' + matrix('+stk(top-1)(1)+',1,-1)]'
      nx+' = matrix('+stk(top-2)(1)+',1,-1)'
      ny+' = ['+ny+';'+ny+' + matrix('+stk(top)(1)+',1,-1)]']
end
stk=list('xarrows'+rhsargs([nx,ny]),'0','?','?','?')
endfunction
