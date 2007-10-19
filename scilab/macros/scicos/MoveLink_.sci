function MoveLink_()
// Copyright INRIA

//** This function is activated by PRESSING the LEFT mouse buttom 
 
  
  //** -------------- NAVIGATOR -------------------------------------------- 
  //** the press is inside a navigator window
  if windows(find(%win==windows(:,2)),1)==100000 then //** Navigator window:
    Cmenu=[] ; %pt=[] ; //** clear the variable 
    return; //** and exit 
    
  //** ------------------ NOT in the current window ------------------------
  elseif %win<>curwin then //** the press is not in the current window 
    
    kc = find(%win==windows(:,2)); 
    
    if kc==[] then //** the press is not inside an scicos actiview window 
      //** This message has been suppressed : have a good day :)
      //** message("This window is not an active scicos window")
      Cmenu=[]; %pt=[]; return
    elseif windows(kc,1)<0 then //** the press is inside a palette 
      kpal = -windows(kc,1)    ; 
      palette = palettes(kpal) ;
      %kk = getobj(palette,%pt) ; //** get the obj inside the palette 
    elseif slevel>1 then //** the press is over a block inside a superblock window
      execstr('%kk=getobj(scs_m_'+string(windows(kc,1))+',%pt)')
    end
    
    if %kk<>[] then //** press over a valid block 
        Cmenu="Duplicate"
        Select=[%kk,%win]
    else           //** press in the void   
        Cmenu="SelectRegion"
        Select=[];
    end
  
  //** The PRESS is in the CURRENT window -----------------------------------    
  else //** ... the press is in the current window 
    
    //** look for an object 
    %kk = getobj(scs_m,%pt)
    
    //** if an object id found 
    if %kk<>[] then
      ObjSel = size (Select) ; //** [row, col]
      ObjSel = ObjSel(1) ; //**  row 
      //**--------------------------------------------------------------
      if ObjSel<=1 then //** with zero or one object already selected 
        
	Cmenu = check_edge(scs_m.objs(%kk),"Move",%pt);
	//** N.B. if the click is over an output port [Cmenu = "Link"]       
        
	if Cmenu=="Link" then
	  Select = []; //** Execute "Link" : deselct any previously selected object
	else 
	  Select = [%kk, %win]; //** Execute "Move" with the object selected 
	end 
		
      else //** more than one object is selected 
        SelectedObjs = Select(:,1)'; //** isolate the object column and put in a row 
	if or(%kk==SelectedObjs) then //** check if the user want to move the aggregate
	  Cmenu = "Move";
	  //** Select information is preserved    
	else
	  Cmenu  = "Move";
	  Select = [%kk, %win]; //** user want to move only the object in the focus
	end 
      
      end    
      //**---------------------------------------------------------------
      
    else //** if the press is in the void of the current window 
       Cmenu = "SelectRegion" ; //** "SelectRegion" will be called later 
       %ppt = []; Select = [] ; //** NB: the %pt information is
                                //preserved for "SelectRegion"
                                //operation 
       if with_gtk()|MSDOS then %ppt=%pt; end	// first click under windows treated as
						// press move (always the case under gtk)		
    end
  
  end

endfunction


