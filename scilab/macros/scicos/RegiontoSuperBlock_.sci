function RegiontoSuperBlock_()
// Copyright INRIA

//** Very complex and critical functions inside : handle with care ;)     

//** BEWARE: both functions ('do_region2block' and 'do_select2block' contains some 
//** completely un useful drawobj() operations inside the 'do_delete2' sub function
//** For the moment the drawobj() are not executed because this function ia called
//** with the options %f that disable the graphics update.
//** In all the cases the the situation is restored by the final "Replot" operation.
//
  Cmenu=[]
   
  if Select==[] then 
    if (%win <> curwin) then
      return// only in main window
    end
    
    //** if nothing is selected -> call a region selection function 
    [%pt, scs_m] = do_region2block(%pt,scs_m); //** see file: "do_region2block.sci"
	      //** this type of operations is OK (old Scicos code here :)                                  
  else
    
    if Select(1,2)<>curwin then
      //** if the selected object are not in the current window ... exit  
      return ; //** --> Exit point 
    end
    
    //** if the selected object are in the current window use the function below in this file 
    [%pt, scs_m] = do_select2block(%pt, scs_m);  //** ---> see below in this file :)
	//** with the last bugfix the 'Text' object are correctly handled, but this routine is
	//** much slower that the "old" standard mode.     
  end
  
  Cmenu='Replot'; %pt=[];

endfunction


