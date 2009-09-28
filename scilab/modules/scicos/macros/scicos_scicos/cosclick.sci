//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [btn, %pt, win, Cmenu ] = cosclick(flag)
//**    
//** btn : button / event id (numeric code)
//** %pt : mouse position[x,y] of the event
//** win : Scilab/Scicos windows where the event occur
//** 

  [lhs,rhs] = argn(0) ;
  
  Cmenu=[]; %pt=[]; btn = 0 ; //** Clear variables
  
  //** --> This control is really indispensable because the user can                       //
  //**     - click over a Scicos window, then (very quickly)                               //
  //**     - Close the window clicking over the [X] button                                 //
  //** if the current Scicos windows is NOT present in the list of Scilab window           //
  if ~or( winsid()==curwin ) then 
    win = curwin   ;
    Cmenu = "XcosMenuQuit" ; 
    return         ; //** EXIT Point //
  end   
  //**----------------------------------------------------------------------------//
  
  if rhs==1 then
    [btn, xc, yc, win, str] = xclick(flag) //** not used now (was used in the past) 
  else
    [btn, xc ,yc ,win ,str ] = xclick()    //** <- This is used in the main scicos_new() loop:      
  end                                        //**    CLEAR ANY PREVIOUS
  //**--------------------------------------------------------------------------- //

  %pt = [xc,yc] ; //** acquire the position  
  //**--------------------------------------------------------------------------
  //** cosclick() filter and command association 
  
  //**--------------------------------------------------------------------------
 
  if btn==-1000 then //** window closing check 

    //**------------------------------------------------------------
    //** The window has been closed 
    
    if win==curwin then  //** in the current window ? 
      Cmenu = "XcosMenuQuit" ;     
    else                 //** not the current window 
      Cmenu = []    ;
      %pt   = []    ;
    end
    return  //** --> EXIT  
  end

  //** a dynamic menu has been selected 
  //if (win==-1)&(btn==-2)&part(str,1:5)=="exec(" then
  //  from = max(strindex(str,'_'))+1 ;
  //  to   = max(strindex(str,'('))-1 ;
  //  win  = evstr(part(str,from:to)) ;
  //end

  
  //**----- 
  // If the event is a TCL event then win=[]
  global inactive_windows
  if win<>[] & find(win==inactive_windows(2))<>[] then
    //the event occured into an incative window
    global Scicos_commands
    pathh = inactive_windows(1)(find(win==inactive_windows(2)))
 
    //**----------------------------------------------------------------------------
    if (btn==-2) then //** a dynamic menu has been selected 
      //cmd = 'Cmenu='+sci2exp(str);
      cmd=str
    elseif (btn==0) then
      cmd = 'Cmenu = '"XcosMenuMoveLink'"'
    elseif (btn==3) then
      cmd='Cmenu=''XcosMenuSelectLink''' //** indirect call via commands 
    elseif (btn==10) then 
      cmd='Cmenu='"XcosMenuOpenSet'"'
    elseif or(btn==[2 5 12]) then
      cmd='Cmenu = '"XcosMenuPopup'"';
    elseif (btn>=32) & (btn<288)
      //09/10/2007, Alan's patch search in %scicos_short 
      if exists('%scicos_short') then
	ind = find(ascii(btn)==%scicos_short(:,1))
	if ind<>[] then
	  ind=ind($)
	  cmd='Cmenu='''+%scicos_short(ind,2)+''''
	else
	  cmd='Cmenu=''XcosMenuSelectLink'''//** indirect call via commands for shortcut
	end
      else
	cmd='Cmenu=''XcosMenuSelectLink'''
      end
      
    elseif (btn==1000) then //** [CTRL] + [LeftMouseButtonPress]
      cmd='Cmenu = '"XcosMenuSmartMove'"'; //** Smart Move 
    else
      cmd='Cmenu=''XcosMenuSelectLink'''
    end
    //**----------------------------------------------------------------------------

    Scicos_commands = ['%diagram_path_objective='+sci2exp(pathh)+';%scicos_navig=1';
		       cmd+';%win=curwin;%pt='+sci2exp(%pt)+';xselect();%scicos_navig=[]';
		      ]
    return ; //** EXIT POINT for the indirect command (to be explored)
  
  //** -----------------------------------------------------------
  elseif (btn==3) then //** Single click : Left Mouse Button : no window check         
    Cmenu = "XcosMenuSelectLink" ; //** direct command: "Link" or selection of a block 
    
  //** -----------------------------------------------------------
  elseif (btn==0) then //** Press button : Left Mouse Button : no window check 
    Cmenu = "XcosMenuMoveLink"   ; //** Normal Move

  elseif (btn==1000) then //** [CTRL] + [LeftMouseButtonPress] 
    Cmenu = "XcosMenuSmartMove"   ; //** Smart Move
  
  //**-------------------------------------------------------------    
  elseif (btn==10) & (win==curwin) then //** "Left Mouse Double Click" in the current Scicos window
    Cmenu="XcosMenuOpenSet"  //** Possible cases : 1 - Void (empty)
                      //**                  2 - Block
                      //**                  3 - Link
                      //**                  4 - Super Block
		      //**                  5 - Text   
  //** ----------------------------------------------------------- 
  elseif (btn==10) & (win<>curwin) then //** Left Mouse Button : Double click 
    jj = find(windows(:,2)==win)        //** not in current Scicos windows (eg palette or navigator)
    //** if jj is NOT empty means that you are in a Palette or in a Navigator  
    if jj <> [] then
      if or(windows(jj,1)==100000) then
        Cmenu = "XcosMenuOpenSet"  //double click in the navigator: mode open-set --> Navigator Window
       else
	Cmenu = "XcosMenuDuplicate" //** Double Click In a Palette windows ---> jump to Duplicate  
      end
    else
      Cmenu=[]; %pt=[]; //** otherwise, clear state variable
    end
  
  //** ----------------------------------------------------------- 
  
  //**--------------------- RIGHT MOUSE BUTTON -> POPUP -------------------------------------
  elseif or( btn==[2 5 12] ) then  //** any RIGHT mouse button events (click, press, d.click)
    Cmenu = "XcosMenuPopup";               //** means a popup request 
    return         ; //** --> EXIT to 'Popup' execution 
  //**---------------------------------------------------------------------------------------    
  
  elseif btn == -2 then  // Dynamic Menu (top of window) mouse selection
    win = curwin ;
    execstr(str); //set win and Cmenu variables (see scicos_menubar)
    %pt=[]
    disp(Cmenu,win)
    return
    //verifier si ce qui suit est utile (selection d'un menu dans une
    //autre fenetre que la fenetre courante. Si necessaire remplacer le
    //callback dans scicos_menubar par "win=xxx;Cmenu=''yyy''"
    
//     if strindex(str,'_'+string(curwin)+'(')<>[] then // str contains the information of the Scicos dynamic menu selection
//       %pt=[] ; //** empty variable: no information about mouse position inside a dynamic menu  
//       //** Cmenu is empty ( [] )
//       //**      execstr( <Diagram_1000(1)>  )
//       execstr('Cmenu='+part(str,9:length(str)-1));   //**
//       //**  Cmenu = menus('Diagram')(1)      ; //** needs explanation  
//       //**  disp (Cmenu);
//       execstr('Cmenu='+Cmenu) ;
//       //**  At the end 'Cmenu' contains the string show in the dynamic selection menu (e.g. "Replot")   
//       return ; //** ---> EXIT POINT  
//     else // click in an other dynamic menu
//       execstr(str,'errcatch'); //** error handling 
//       return ; //** ---> EXIT POINT     
//     end
    
    //**-------------------------------------------------------------    
   
    //** Keys combos, mouse/key combos and sigle key shortcut
    elseif btn > 31 then //** [CTRL] + [.] combination  

    //** ------ Key combos ------------------------------------ 
        
    // [CRTL]+[x] --> Cut
    if (btn==1120) then //** [CRTL]+[x] --> Cut
      Cmenu="XcosMenuCut"; %pt=[];
    
    elseif (btn==1099) then            //** [CTRL]+[c] --> Copy
      Cmenu="XcosMenuCopy"; %pt=[];
    
    elseif (btn==1118) then            //** [CTRL]+[v] --> Paste 
      Cmenu="XcosMenuPaste"; %pt = [xc,yc] ;   //** acquire the position for the "Paste"
    
    elseif (btn==127)| (btn==8) then  //** [Delete] or [backspace]key 
      Cmenu="XcosMenuDelete"; %pt=[];
    
    //** ----- Mouse + Keyb. combos ---------------------------  
    elseif (btn==1003) then  //** [CTRL]+[Left btn click] 
      Cmenu="XcosMenuCtrlSelect";    //** Multiple, additive object selection 
    
    elseif (btn==1122) then                //**  [CTRL]+[z] --> Undo 
      Cmenu="XcosMenuUndo";
    //**-------------------------------------------------------
    
    elseif (btn==1115) then                //** [CRTL]+[s] --> Save the diagram 
      Cmenu="XcosMenuSave";

    elseif (btn==1097) then                //** [CRTL]+[a] --> Select all 
      Cmenu="XcosMenuSelectAll";

    
    //**-------------------------------------------------------
    else
    //** ---- Shortcut ----------------------------------------
      Cmenu = %tableau( min (100,btn-31) ); //** (global) %tableau is the standard single key shorcut conversion
                                            //**          table; input (key_code-31) ; output: a string
                                            //** it uses "%scicos_short" preloaded variable 
      if Cmenu==emptystr() then  //** if the strings is empty means 
         Cmenu=[]; %pt=[];       //** that no single key shorcut is associated            
      end
             
    end //** end of keyboard combinations 
  
  end //** end of cosclik() filter 

endfunction
