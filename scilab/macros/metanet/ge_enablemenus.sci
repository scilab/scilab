function ge_enablemenus(men)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
[lhs,rhs]=argn()
win=xget('window')
if rhs<1 then 
 execstr('global  EGdata_'+string(win)+';men=EGdata_'+string(win)+'.Menus')
end

for k=1:size(men)
  setmenu(win,men(k)(1))
end
endfunction
