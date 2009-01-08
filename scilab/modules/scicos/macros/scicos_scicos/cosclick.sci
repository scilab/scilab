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
//** win : Scilab/Scicos windows where the event occour
//** 

  [lhs,rhs] = argn(0) ;
  
  Cmenu=[]; %pt=[]; btn = 0 ; //** Clear variables
  
  //** --> This control is really indispensable because the user can                       //
  //**     - click over a Scicos window, then (very quickly)                               //
  //**     - Close the window clicking over the [X] button                                 //
  //** if the current Scicos windows is NOT present in the list of Scilab window           //
  if ~or( winsid()==curwin ) then //**                                                     //
    //** disp("...cosclic.sci: fast Select then CloseWindow...|:"); pause; //** debug only //
    win = curwin   ;
    Cmenu = "Quit" ; 
    return         ; //** EXIT Point //
  end   
  //**----------------------------------------------------------------------------//
  
  if rhs==1 then
      [btn, xc, yc, win, str] = xclick(flag) //** not used now (was used in the past) 
    else
      [btn, xc ,yc ,win ,str ] = xclick()    //** <- This is used in the main scicos_new() loop:      
  end                                        //**    CLEAR ANY PREVIOUS
                                             //EVENT in the queue
  //** DEBUG ONLY
  //** disp("...Start ....."); disp (btn, xc ,yc ,win ,str); disp("...End ......");

  //**--------------------------------------------------------------------------- //

  %pt = [xc,yc] ; //** acquire the position  
  
  //**--------------------------------------------------------------------------
  //** cosclic() filter and command association 
  
  //**--------------------------------------------------------------------------
 
  if btn==-1000 then //** window closing check 

 //**------------------------------------------------------------
  //** The window has been closed 
    
    if win==curwin then  //** in the current window ? 
      Cmenu = "Quit" ;     
    else                 //** not the current window 
      Cmenu = []    ;
      %pt   = []    ;
    end
    
    return  //** --> EXIT  
  end

  //** a dynamic menu has been selected 
  if (win==-1)&(btn==-2)&part(str,1:7)=="execstr" then
    from = max(strindex(str,'_'))+1 ;
    to   = max(strindex(str,'('))-1 ;
    win  = evstr(part(str,from:to)) ;
  end

  
  //**----- 
  // If the event is a TCL event then win=[]
  global inactive_windows
  if win<>[] & find(win==inactive_windows(2))<>[] then
    global Scicos_commands
    pathh = inactive_windows(1)(find(win==inactive_windows(2)))

  //**----------------------------------------------------------------------------
   if (btn==-2) then
      cmd = 'Cmenu='+part(str,9:length(str)-1)+';execstr(''Cmenu=''+Cmenu)'
    
    elseif (btn==0) then
      cmd = 'Cmenu = '"MoveLink'"'
    
    elseif (btn==3) then
      cmd='Cmenu=''SelectLink''' //** indirect call via commands 
    
    elseif (btn==10) then 
      cmd='Cmenu='"Open/Set'"'
    
    elseif or(btn==[2 5 12]) then
      cmd='Cmenu = '"Popup'"';
    
    elseif (btn>=32) & (btn<288)
      //09/10/2007, Alan's patch search in %scicos_short 
      if exists('%scicos_short') then
        ind = find(ascii(btn)==%scicos_short(:,1))
        if ind<>[] then
          ind=ind($)
          cmd='Cmenu='''+%scicos_short(ind,2)+''''
        else
          cmd='Cmenu=''SelectLink'''//** indirect call via commands for shortcut
        end
      else
        cmd='Cmenu=''SelectLink'''
      end
    
     elseif (btn==1000) then //** [CTRL] + [LeftMouseButtonPress]
      cmd='Cmenu = '"Smart Move'"'; //** Smart Move 
     else
      cmd='Cmenu=''SelectLink'''
    end
    //**----------------------------------------------------------------------------

    Scicos_commands = ['%diagram_path_objective='+sci2exp(pathh)+';%scicos_navig=1';
		        cmd+';%win=curwin;%pt='+sci2exp(%pt)+';xselect();%scicos_navig=[]';
		      ]
    return ; //** EXIT POINT for the indirect command (to be explored)
  
  //** -----------------------------------------------------------
  elseif (btn==3) then //** Single click : Left Mouse Button : no window check         
    Cmenu = "SelectLink" ; //** direct command: "Link" or selection of a block 
    
  //** -----------------------------------------------------------
  elseif (btn==0) then //** Press button : Left Mouse Button : no window check 
    Cmenu = "MoveLink"   ; //** Normal Move

  elseif (btn==1000) then //** [CTRL] + [LeftMouseButtonPress] 
    Cmenu = "Smart Move"   ; //** Smart Move
  
  //**-------------------------------------------------------------    
  elseif (btn==10) & (win==curwin) then //** "Left Mouse Double Click" in the current Scicos window
    Cmenu="Open/Set"  //** Possible cases : 1 - Void (empty)
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
        Cmenu = "Open/Set"  //double click in the navigator: mode open-set --> Navigator Window
       else
	Cmenu = "Duplicate" //** Double Click In a Palette windows ---> jump to Duplicate  
      end
    else
      Cmenu=[]; %pt=[]; //** otherwise, clear state variable
    end
  
  //** ----------------------------------------------------------- 
  
  //**--------------------- RIGHT MOUSE BUTTON -> POPUP -------------------------------------
  elseif or( btn==[2 5 12] ) then  //** any RIGHT mouse button events (click, press, d.click)
    Cmenu = "Popup";               //** means a popup request 
    return         ; //** --> EXIT to 'Popup' execution 
  //**---------------------------------------------------------------------------------------    
  
  elseif btn == -2 then  // Dynamic Menu (top of window) mouse selection
    win = curwin ;
    //** the format of the 'str' callback string is :
    //** "execstr(<Name_of_menu>_<win_id>(<menu_index>)),  e.g. "execstr(Diagram_1000(1))"
    //** <Name_of_menu> : is the label at the top of menu selection (static label present on the window) e.g. "File"
    //** <win_id>       : is the window id Scilab number (eg 1000 for the main SCICOS window)
    //** <menu_index>   : is the numeric index of the menu selected (in case of multiple menu).
    if strindex(str,'_'+string(curwin)+'(')<>[] then // str contains the information of the Scicos dynamic menu selection
      %pt=[] ; //** empty variable: no information about mouse position inside a dynamic menu  
      //** Cmenu is empty ( [] )
      //**      execstr( <Diagram_1000(1)>  )
      execstr('Cmenu='+part(str,9:length(str)-1));   //**
      //**  Cmenu = menus('Diagram')(1)      ; //** needs explanation  
      //**  disp (Cmenu);
      execstr('Cmenu='+Cmenu) ;
      //**  At the end 'Cmenu' contains the string show in the dinamic selection menu (e.g. "Replot")   
      return ; //** ---> EXIT POINT  
    else // click in an other dynamic menu
      execstr(str,'errcatch'); //** error handling 
      return ; //** ---> EXIT POINT     
    end
    
    //**-------------------------------------------------------------    
   
    //** Keys combos, mouse/key combos and sigle key shortcut
    elseif btn > 31 then //** [CTRL] + [.] combination  

    //** ------ Key combos ------------------------------------ 
        
    // [CRTL]+[x] --> Cut
    if (btn==1120) then //** [CRTL]+[x] --> Cut
      Cmenu="Cut"; %pt=[];
    
    elseif (btn==1099) then            //** [CTRL]+[c] --> Copy
      Cmenu="Copy"; %pt=[];
    
    elseif (btn==1118) then            //** [CTRL]+[v] --> Paste 
      Cmenu="Paste"; %pt = [xc,yc] ;   //** acquire the position for the "Paste"
    
    elseif (btn==127)| (btn==8) then  //** [Delete] or [backspace]key 
      Cmenu="Delete"; %pt=[];
    
    //** ----- Mouse + Keyb. combos ---------------------------  
    elseif (btn==1003) then  //** [CTRL]+[Left btn click] 
      Cmenu="CtrlSelect";    //** Multiple, additive object selection 
    
    elseif (btn==1122) then                //**  [CTRL]+[z] --> Undo 
      Cmenu="Undo";
    //**-------------------------------------------------------
    
    elseif (btn==1115) then                //** [CRTL]+[s] --> Save the diagram 
      Cmenu="Save";

    elseif (btn==1097) then                //** [CRTL]+[a] --> Select all 
      Cmenu="Select All";

    
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
