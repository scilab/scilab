//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr> - Serge Steer
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
function scs_m = do_icon_edit(%pt,scs_m)
  
  K=Select(find(Select(:,2)==%win),1) //look for selected blocks in the current window
  if K==[] then
    K = getblock(scs_m, %pt(:))
  end

  if K==[] then
    messagebox(_("No selected block in the current Scicos window."),'error','modal')
    return
  end
   
  if size(K,'*')>1 then
    messagebox(_("Only one block can be selected in current window for this operation."),'error','modal')
    return
  end  
  
  o=scs_m.objs(K)
  if typeof(o)<>'Block' then
    messagebox(_("Only  blocks can be selected for this operation."),'error','modal')
    return
  end    
 
  gr_i=scs_m.objs(K).graphics.gr_i ;
  sz=scs_m.objs(K).graphics.sz ;
  if type(gr_i)<>15 then  
    gr_i=list(gr_i,[]);
  end

  gh_curwin=gcf();//Preserve current Window
  gh_axes=gca();
  coli=gr_i(2);

  //Create the edition window  
  win=max(winsid())+1;
  fig=scf(win);show_window();
  fig.axes_size=fig.axes_size*sz(1)/sz(2);
  fig.auto_resize='off';
  ax=gca();
  if coli<>[] then ax.background=coli;end//background
  //ax.isoview='on';
  ax.margins=zeros(4,1)
  ax.data_bounds=[0 0;1 1];
  ax.tight_limits = "on"
  // draw the current icon (creates the graphical objects)
  sz=[1 1];orig=[0 0];
  deff('c=scs_color(c)',' ')
  //patch because xstringb has been overloaded by scicos
  //newfun('xstringb3',funptr('xstringb'))
  //instr=strsubst(gr_i(1),'xstringb','xstringb3')
  instr=gr_i(1)
  if execstr(instr,"errcatch")<>0 then
    messagebox(msprintf(_("The current icon depends on block parameter\n"+..
	     "part of the icon cannot be imported here\n"+..
	     "Use icon menu to check the content")),'modal')
  end
  clearfun('xstringb3')
  ///remove unused default menus
  if getos() <> 'Windows' then
    emen='Edit'
  else
    global LANGUAGE
    if LANGUAGE=="eng" then
       emen='Edit';
    else
       emen='&Editer';
    end
  end	
  delmenu(win,emen);delmenu(win,'3D Rot.')
					      
  exec("SCI/modules/scicos/macros/scicos_scicos/entity_menu.sce",-1);
  add_entity_menu(win,emen) //add the edition menu
  ged(10,win)//start entity picker
  
  //Infinite loop waiting for edition to finish
  realtimeinit(0.2);count=0
  while or(win==winsid())&fig.user_data==[] 
    count=count+1;
    realtime(count)
  end
  if and(win<>winsid()) then scf(gh_curwin),return,end //window destroyed 
  //Edition finish, disable handler and menus
  delmenu(win,emen)

  seteventhandler('') //remove the ged event handler
  
  //Restore current window
  scf(gh_curwin)
  //Update Icon if requested
  if fig.user_data=='ok' then //user finished edition by "Ok"
    gr_i(1)=gen_code(win) //create the Scilab code of the graphics
    //check it
    mac=null();deff('[]=mac()',gr_i(1),'n') 
    if check_mac(mac) then
      //update scicos object
      o=scs_m.objs(K)
      o.graphics.gr_i=gr_i
      scs_m.objs(K)=o
      o_size = size(gh_axes.children);
      gr_k=get_gri(K,o_size(1))
      drawlater() ;
      update_gr(gr_k,o)
      draw(gh_axes);
    end
  end
  delete(fig)
endfunction
