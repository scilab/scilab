function SelectLink_()
// Copyright INRIA
  //** This function is called after a single left click 

  Select=[]; SelectRegion=list() ; //** init internal variable
  //  At this point Select=[]

  //**--------------- Navigator ----------------------------------
  //** case 10000 : the user has clicked in the Navigator window
  //**              in this case, clear the variable and exit
  if windows( find(%win==windows(:,2)), 1 )==100000 then
    Cmenu=[] ; 
    %pt=[]   ;
    return   ; //** -->EXIT point 
  end
  //**------------------------------------------------------------ 
  
  //** In all the other cases :

  kc = find(%win==windows(:,2)) ; //** find the clicked window in the internal SCICOS 
                                  //** active windows list datastructure

  //** case 0 : is not an active SCICOS window
  if kc==[] then
    //** This message has been suppressed : have a good day :)
    //** message("1 - This window is not an active scicos window");
    Cmenu=[]; %pt=[]; return

  //** case : kc index a "negative" windows id  --> the user has clicked in a palette window
   elseif windows(kc,1) < 0 then //click dans une palette
     kpal = -windows(kc,1) ;    //** recover thw windows palette id
     palette = palettes(kpal) ; //** acquire all the palette
     k = getobj(palette,%pt)  ; //** isolate the clicked block

     if k<>[] then 
       Select=[k,%win]; Cmenu=[]; %pt=[]; //** there is a block on this coordinate
       return                             //** Select the block then return back
     else
       Cmenu=[];%pt=[]; return            //** no block : clear variable and return back
     end

   //** case : the user has clicked in the current editor window ( both main or superblock )
    elseif %win==curwin then // click dans la fenetre courante
       k = getobj(scs_m,%pt) //** look for the object
       
       if k<>[] then //** if some object is found 
         
	 Cmenu = check_edge(scs_m.objs(k),[],%pt); //** check if the click is on a port
	 //** N.B. if the click is over an output port [Cmenu = "Link"]                        
         if Cmenu==[] then //** if is NOT over a port 
              Select=[k,%win]; Cmenu=[]; %pt=[]; return //** it is just an object selection 
         end
       
       else //** click in the void 
         
	 Cmenu=[]; %ppt=%pt; %pt=[]; return  //** save last position "in the void" '%ppt'
                                             //** for the eventual 'Paste' operation 
       end

    //** case : the click is inside a superblock 
    elseif slevel>1 then
       execstr('k=getobj(scs_m_'+string(windows(kc,1))+',%pt)');
       //** N.B. llok at the trick needed for the superblock level execution ;)
       //** N.B. if the click is over an output port [Cmenu = "Link"
       if k<>[] then //** if the click is over a valid object the object is selected 
         Select=[k,%win]; Cmenu=[]; return
       else  //** if the click in in the void 
         Cmenu==[];%pt=[];return
       end
    
    //** case : This window is not an active scicos window
    else 
     message('2 - This window is not an active scicos window')
     Cmenu=[];%pt=[];return

  end

endfunction


