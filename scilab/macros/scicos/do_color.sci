function [scs_m] = do_color(%win, %pt, scs_m)
// Copyright INRIA
//
// do_block - edit a block icon
//
  
  if %win<>curwin then
     return ; //** Exit point  
  end 
  
  if Select==[] then
    
     xc = %pt(1); yc = %pt(2);
    
     KK = getobj(scs_m,[xc;yc])
     
     if KK==[] then
         return ; //** Exit point -> No object found 
     end 
  
  else
    
   //** BEWARE : look at the win_id of the selected object !!!
   //** TO DO: look at the variable  and filter the right object 
   
//**   disp("Select=");disp(Select);

   KK = Select(:,1)'; //** take all the object selected 

  end
  //
  //**-----------------------------------------------------
  
  coul = getcolor('Choose a color',1);
  
  //**----------------------------------------------------------
  //** the code below is modified according the new graphics API
  gh_curwin = scf(%win);
  
  //** at this point I need to build the [scs_m] <-> [gh_window] datastructure 
  //** I need an equivalent index for the graphics 
  
  o_size = size (gh_curwin.children.children ) ; //** the size:number of all the object 
  
  //** "k" is the object index in the data structure "scs_m"
  //** compute the equivalent "gh_k" for the graphics datastructure 
  //** gh_k = o_size(1) - k + 1 ; //** semi empirical equation :)
  //** Alan Layec has do this in the function get_gri();
  //** gh_blk = gh_curwin.children.children(gh_k);
  
  for K = KK //** for all the selected object(s)
    o = scs_m.objs(K) ; //** the scs_m object  
    gh_obj_K = get_gri(K,o_size(1)); //** compute the index in the graphics DS 
    gh_compound = gh_curwin.children.children(gh_obj_K); //** get the compound handle 
    
    //** ------------------ Link --------------------------
    if typeof(o)=="Link" then
      [nam,pos,ct] = (o.id,o.thick,o.ct) ;
      c = coul ;
      
      if c<>[] then
	connected = connected_links(scs_m,K);
	for kc = connected
	  o = scs_m.objs(kc) ;
	  ct = o.ct          ; //** <---- Haio :(
	  
	  if ct(1)<>c then
	    o.ct(1) = c ;
	    gh_compound.children.foreground = c ;
	    scs_m.objs(kc) = o ;
	  end
	
	end
      
      end
    
    //** ------------------  Block -------------------------  
    elseif typeof(o)=="Block" then
      
      if type(o.graphics.gr_i)==10 then,
	o.graphics.gr_i = list(o.graphics.gr_i,[]),
      end
      
      if o.graphics.gr_i(2)==[] then
	coli = 0 ; 
      else
	coli = o.graphics.gr_i(2);
      end
      
      coln = coul ;
      
      if coln<>[] then
	
	if coln<>coli then
	  o.graphics.gr_i(2) = coln ; 
	  scs_m.objs(K) = o ;
          update_gr(gh_obj_K,o)
	  //size_of_graphic_objext = size(gh_compound.children) ;
	  //first_graphic_objext = size_of_graphic_objext(1)    ;
	  //gh_compound.children(first_graphic_objext).background = coln; //** update the propriety
	end
      
      end
    
    //** ----------------- Text ---------------------------  
    elseif  typeof(o)=='Text' then
      //not implemented
    end
  end

 //** pause

  drawnow(); show_pixmap();

endfunction
