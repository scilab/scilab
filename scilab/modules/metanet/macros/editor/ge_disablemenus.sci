function ge_disablemenus(men)
	
	//Copyright INRIA
	//Author : Serge Steer 2002
	
	[lhs,rhs]=argn();
	
	f   = gcf();
	win = f.figure_id;
	
	if rhs<1 then men=EGdata.Menus;end
	
	for k=1:size(men)
		unsetmenu(win,men(k)(1))
	end
	
endfunction
