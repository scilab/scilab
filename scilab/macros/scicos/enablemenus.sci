function enablemenus(men)
//EnableAllMenus()
return
// Copyright INRIA

[lhs,rhs] = argn()

//** check if the current window is in the Scicos windows list 
if ~or(curwin==winsid()) then
  return
end

//curwin=xget('window') //** Please left this instruction commented 
//** if rhs<1 then      //** obsolete code : is never used 
//**   men = menus(1)
//** end

men = menus(1) ;
for k=1:size(men,'*')
  setmenu(curwin,men(k)); //** reactivate the dynamic menu 
end

//** ... for the Super Block Diagram do NOT reactivate the "Simulate" menu
if super_block then
  unsetmenu(curwin,'Simulate');
end

xinfo(" "); 

endfunction
