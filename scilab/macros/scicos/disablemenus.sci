function disablemenus(xinfo_flag)
//DisableAllMenus()
return
// Copyright INRIA
//**   

//** Disable the dynamic menu to avoid unintentional command generation during
//** interactive operations (Move, OpenSet, Link creation, etc.)

//** if a "xinfo_flag" is specified, the default xinfo message is not printed 

//** [lhs,rhs] = argn()

if rhs<1 then //** if no argument is specified
  xinfo("Please be patient...") ; //** print the default message 
end   

//   curwin=xget('window') //** Please left this instruction commented 
//** if rhs<1 then         //** obsolete code inside 
//**   men = menus(1)
//** end

men = menus(1); 
for k=1:size(men,'*')
  unsetmenu(curwin,men(k)); //** 
end

endfunction
