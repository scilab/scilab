function Undo_()
// Copyright INRIA

    Cmenu = [] ;
    %pt = [];

    if enable_undo==2 then  // open SB's may have been affected
      %r=2
      %r=message(['Undo operation undoes changes in a subdiagram';
                  'These changes will be lost for ever'],['Proceed'; ...
		    'Cancel'])
      if %r==2 then 
         return,
      else
         scs_m = scs_m_save ; //** recover the previous diagram
	 Select = [] ; //** unselect object  
         needcompile = nc_save ;
         supers=findopenchildren(scs_m,super_path,list(super_path))
         Scicos_commands=[]
         for i=1:size(supers)
            Scicos_commands=[Scicos_commands;
         	 '%diagram_path_objective='+sci2exp(supers(i))+';%scicos_navig=1';
                 'Cmenu='"Replot'"']
         end
         enable_undo = %f ; //** "Undo" is a single level function 
      end
    
    elseif enable_undo then
         scs_m = scs_m_save ; //** recover the previous diagram
	 Select = [] ; //** unselect object  
         needcompile = nc_save ;
         Cmenu='Replot'     
         enable_undo = %f ; //** "Undo" is a single level function 
    else
      message('No more undo available.')
    end
    
endfunction
