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

function XcosMenuOpenSet()

  global inactive_windows

  if or(curwin==winsid()) then 
    gh_current_window = scf(curwin);
  end

  if ~%diagram_open then
    %kk=Select(1)
    if size(scs_m.objs)<%kk then
      ierr=1
    else
      ierr=execstr('xxx=scs_m.objs(%kk).model.sim','errcatch','n')
    end
    if ierr==0 then
      if xxx<>'super' then
        //## test for palette block
        ierr2=execstr('xxxx=scs_m.objs(%kk).gui','errcatch','n')
        if ierr2==0 then
          if xxxx<>'PAL_f' then
            ierr=1;
          end
        else
          ierr=1;
        end
      end
    end
    if ierr<>0 then
      messagebox(['This window is not active anymore or';
	       'the browser is not up-to-date.'],'modal')
      %scicos_navig=[]  // stop navigation
      Scicos_commands=[]
      Cmenu=[];%pt=[]
      return
    end
    
    inactive_windows(1)($+1)=super_path;inactive_windows(2)($+1)=curwin

    super_path = [super_path, %kk] ; 
    
    [o, modified, newparametersb, needcompileb, editedb] =..
	clickin( scs_m.objs(%kk));

    indx=find(curwin==inactive_windows(2))
    if indx<>[] then
        inactive_windows(1)(indx)=null();inactive_windows(2)(indx)=[]
    end
    
    edited = edited | editedb ;
    super_path($-size(%kk,2)+1:$) = [] ;
    
    if editedb then
      enable_undo = %f  //undo is disabled if super block not explicitely open
                        //otherwise the undo returns to an unpredictable past
      needcompile = max(needcompile, needcompileb)
      %Path = list('objs',%kk)
      
      if or(curwin==winsid()) then 
        gh_current_window = scf(curwin);
      end
      
      scs_m = update_redraw_obj(scs_m, %Path,o) ;//scs_m.objs(%kk)=o
    end
    
    if modified then
      newparameters = mark_newpars(%kk,newparametersb,newparameters) ; 
    end
    return
  end
   
//////////////////////////////////////////////////////////////////////
  
  %xc = %pt(1); %yc = %pt(2); //** last mouse position
  
  %kk = getobj(scs_m,[%xc;%yc]) ; //** acquire the index in the current diagram 
  %Path = list('objs',%kk)      ; //** create the path to the object   
  

  //** '%kk' is the object index

  //**-----------------------------------------------------------------
  if %kk<>[] then //** if the double click is not in the void ---------
    
    drawlater(); 
    Select_back = Select; 
    selecthilite(Select_back, "off") ; //  unHilite previous objects
    Select = [%kk %win];               //** select the double clicked block 
    selecthilite(Select, "on") ;       
    drawnow();
				       
    inactive_windows(1)($+1)=super_path;inactive_windows(2)($+1)=curwin
		       
    super_path = [super_path, %kk] ; 
    [o, modified, newparametersb, needcompileb, editedb] = clickin( ...
	scs_m(%Path) );

    indx=find(curwin==inactive_windows(2))
    if indx <> [] then
      inactive_windows(1)(indx)=null();inactive_windows(2)(indx)=[]
    end
    
    //** BEWARE : "clickin can modify the "Cmenu" 
    //to force the creation of a Link  
    
    //** this POC potentially dangerous !!!
    if Cmenu=="XcosMenuLink" then
      %pt = [%xc, %yc]   ;
      super_path($) = [] ;
      return ; //** ---> EXIT point
    end
    
   
    edited = edited | editedb ;
    super_path($-size(%kk,2)+1:$) = [] ;
    
    if editedb then
      scs_m_save = scs_m       ; //** save the old diagram 
      nc_save    = needcompile ; //** and its state 

      if typeof(o)=="Block" & o.model.sim=="super" then
         enable_undo = 2  //special code in case the content of SB has been changed
      else
         enable_undo = %t
      end

      if ~pal_mode then
	needcompile = max(needcompile, needcompileb)
      end
      
      if or(curwin==winsid()) then
        gh_current_window = scf(curwin);
      end
      
      scs_m = update_redraw_obj(scs_m, %Path,o) ; //** DANGER DANGER DANGER
      
    end
    
    // note if block parameters have been modified
    if modified  then
      newparameters = mark_newpars(%kk,newparametersb,newparameters) ; //** DANGER
    end
    
  end //**.. the double click was not in the void 
  
  Cmenu = []; %pt = [] ;
  
endfunction
