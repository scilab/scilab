function [%pt,scs_m,needcompile] = do_delete(%pt,scs_m,needcompile)
//!
// Copyright INRIA
//** 
//** 21 June 2006 
//** 29 June 2006 
// do_delete - delete a scicos object
// get first object to delete

  win = %win;
  xc = %pt(1); yc = %pt(2)  ;
  K = getobj(scs_m,[xc;yc]) ;
  
  //** check if yoy click over an empty object 
  if K==[] then
         return ;
  end

  needreplay = replayifnecessary();
  scs_m_save = scs_m,nc_save=needcompile;
  
  [scs_m,DEL] = do_delete1(scs_m,K,%t);
  //** I'm here :)
  
  //** WARNING! : potentially dangerous code here : can ruin the coherence !
  //**            it not create problema because is impossible to manipoulate
  //*             deleted object (that results as negative gh_ datastructure 
  //**            index  
  
  
  if DEL<>[] then //** if any object ha been deleted .....
   
    needcompile = 4 ; //** signal to the compiler 
    
    //suppress right-most deleted elements
    while getfield(1,scs_m.objs($)) == 'Deleted' then
      
      scs_m.objs($) = null();
      
      if lstsize(scs_m.objs)==0 then
           break
      end
    
    end
    
    [scs_m_save,nc_save,enable_undo,edited,needreplay] = resume(scs_m_save,nc_save,%t,%t,needreplay);
  
  end


endfunction
