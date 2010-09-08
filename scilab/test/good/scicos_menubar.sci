//  Scicos
//
//  Copyright (C) 2008 - INRIA - Simone Mannori
//  Copyright (C) 2009 - INRIA - Serge Steer
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
// See the file ./license.txt
//
 
function scicos_menubar(gh_current_window,scicos_menus,isMainDiagram)
//Creates the Scicos menubar.
//scicos_menus is a list, each entry is a two column string array. 
//  The first column gives the menus unique Id, 
//  the second gives the menu labels
  
//  the first row give the main menu data while the following rows
//  gives the submenus  data
 
//** Serge Steer Sep 2009, addmenu replaced by uimenu
  
  curwin=gh_current_window.figure_id;
  
  //** Remove the default Scilab graphics window menus 
  delmenu(curwin,_("&File"))  ; 
  delmenu(curwin,_("&Edit"))  ;
  delmenu(curwin,_("&Tools")) ;
  delmenu(curwin,_("&?"))     ; 
  //** Remove the toolbar 
  toolbar(curwin, "off"); //** by Vincent C. :)

  c=gh_current_window.children;
  //destroy all existing uimenu if any
  del=[]
  for k=1:size(c,'*')
    if c.Type=='uimenu' then del=[del c],end
  end
  if del<>[] then delete(del),end

  //create the Scicos menus
  for menu=scicos_menus
    if menu(1,1)<>'Unvisible'&(menu(1,1)<>'XcosMenuSimulate'|isMainDiagram) then
      mainmenu=uimenu(gh_current_window,'label',menu(1,2))
      for k=2:size(menu,1)
	//the menus callback are trapped by cosclick so just pass the
        //callback function name. This function will be exec'ed by scicos itself
	//
	callback=sci2exp(curwin,'win')+';'+sci2exp(menu(k,1),'Cmenu')
	submenu=uimenu(mainmenu,'label',menu(k,2),'Callback',list(4,callback),'Tag',menu(k,1))
      end
    end
  end
  
  if isMainDiagram then
    //haltscicos is a Scilab function which set a C flag to make the
    //simulation stop. Take care that this menu is for immediate action
    //so it does not modify the Cmenu variable as the others
    callback='haltscicos'
    mainmenu=uimenu(gh_current_window,'label',_("Stop"),'Callback',list(4,callback),'Tag','XcosMenuStop')
    mainmenu.Enable='off';
  end

endfunction
