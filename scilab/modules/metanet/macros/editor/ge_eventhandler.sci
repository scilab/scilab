
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_eventhandler(win,x,y,ibut)
// Entry point for all edition actions (menus and events)

  if or(win==winsid()) then //does the windows still exists
    old=gcf(); scf(win)
    seteventhandler("")  
  else //window has been deleted by an asynchronous xdel()
    ok=ge_do_quit()
    if ~ok then //re_create editgraph window with its menus
      scf(win);
      ge_create_menus(win)
      seteventhandler('ge_eventhandler')  
    end
    return
  end
  w=string(win)
  // get the data associated with this window
  execstr('global EGdata_'+w+'; EGdata=EGdata_'+w)

  if type(ibut)==1 then //ge_eventhandler called by a graphic window event
    if ibut<0 then 
      if ibut==-1000 then //the window has been closed by the window manager
	ok=ge_do_quit(),
	if ~ok then //re_create editgraph window with its menus
	  scf(win);
	  ge_create_menus(win)
	  seteventhandler('ge_eventhandler')   
	end
	return
      end
      seteventhandler("ge_eventhandler"),return,
    end
    
    if ibut>10 then 
      if ~ge_shortcut(ibut) then 
	seteventhandler("ge_eventhandler"),
	scf(old),
	return;
      end
    end
    Cmenu=EGdata.Cmenu// get current edition mode
    if Cmenu==[]  then 
      seteventhandler("ge_eventhandler"),
      scf(old),
      return,
    end
    scf(win)
    [x,y]=xchange(x,y,'i2f')
  else    // ge_eventhandler is called by a menu callback  
    Cmenu=ibut
  end
  
  edited=%f
  seteventhandler(""),//disable graphic window events
  select Cmenu
    //----------------- Edit button submenus ----------------- 
   case 'Undo' then
    EGdata.GraphList=ge_do_undo(EGdata.GraphList)
    EGdata.Edited=%t
    EGdata.Cmenu=[]
   case 'Give default names' then
    EGdata.GraphList=ge_do_default_names(EGdata.GraphList)
    EGdata.Edited=%t;EGdata.Cmenu=[];
   case "NewNode" then //just set the Add Node mode
    if EGdata.Cmenu<>Cmenu then //set new edition mode
      xinfo('Click on a point to add a node')
      EGdata.Cmenu=Cmenu
    else //add node
      EGdata.GraphList=ge_new_node(EGdata.GraphList,x,y)
      EGdata.Edited=%t
    end
   case "NewArc" then //just set the Add Arc mode
    if EGdata.Cmenu<>Cmenu then //set new edition mode
      xinfo('Left click on first on arc tail node then on arc head node')
      EGdata.Cmenu=Cmenu
    else
      EGdata.GraphList=ge_new_edge( EGdata.GraphList,x,y)
      EGdata.Edited=%t
    end
   case "Move Node" then  //just set the Move Node mode
    if EGdata.Cmenu<>Cmenu then //set new edition mode
      xinfo('Left click on a node, move and left click new position')
      EGdata.Cmenu=Cmenu
    else
      EGdata.GraphList=ge_do_move(EGdata.GraphList,x,y)
      EGdata.Edited=%t
    end
   case "Move Region" then  
    if EGdata.Cmenu<>Cmenu then //set new edition mode
      xinfo('Left click on a point, drag, left click to validate selection, left click to fix position') 
      EGdata.Cmenu=Cmenu
    else
      EGdata.GraphList=ge_do_move_region(EGdata.GraphList,x,y)
      EGdata.Edited=%t
    end
   case "Delete" then 
    if EGdata.Cmenu<>Cmenu then //set new edition mode
      xinfo('Left click on the node or the arc to delete')
      EGdata.Cmenu=Cmenu
    else
      EGdata.GraphList=ge_do_delete(EGdata.GraphList,x,y)
      EGdata.Edited=%t
    end
   case "Delete Region" then 
    if EGdata.Cmenu<>Cmenu then //set new edition mode
      xinfo('Left click on a point, drag, left click to validate selection')
      EGdata.Cmenu=Cmenu
    else
      EGdata.GraphList=ge_do_delete_region(EGdata.GraphList,x,y)
      EGdata.Edited=%t
    end
   case "Paste" then 
    if EGdata.Cmenu<>Cmenu then //set new edition mode
      xinfo('click on the top left point of the destination region')
      EGdata.Cmenu=Cmenu
    else
      EGdata.GraphList=ge_do_paste(EGdata.GraphList,x,y)
      EGdata.Edited=%t
    end
   case "Properties" then
    if EGdata.Cmenu<>Cmenu then //set new edition mode
      xinfo('Left click on the node or the arc to open properties dialog')
      EGdata.Cmenu=Cmenu
    else
      EGdata.GraphList=ge_do_properties(EGdata.GraphList,x,y)
      EGdata.Edited=%t
    end
   case "Copy Region To ClipBoard"  then
    if EGdata.Cmenu<>Cmenu then //set new edition mode
      xinfo('Left click on a point, drag, left click to validate selection')
      EGdata.Cmenu=Cmenu
    else
      ge_copy_region_to_cb(EGdata.GraphList,x,y)
      EGdata.Edited=%t
    end
    
    //----------------- Graph button submenus ----------------- 
   case "New" then
    [ok,GraphList]=ge_do_new()
    if ok then
      EGdata.GraphList=GraphList
      EGdata.Edited=%f
      ge_clear_history()
      ge_do_replot(GraphList)
    end
    EGdata.Cmenu=[];
   case "Zoom" then
    EGdata=ge_do_zoom(EGdata)
    EGdata.Cmenu=[];
   case "Replot" then
    ge_do_replot(EGdata.GraphList)
    EGdata.Cmenu=[];
   case "SaveAs"
    [GraphList,ok,path]=ge_do_SaveAs( EGdata.GraphList,EGdata.Path)
    if ok then
      ge_drawtitle(GraphList.name)
      EGdata.Edited=%f
      EGdata.Path=path
    end
    EGdata.Cmenu=[];
   case "Save"
    ok=ge_do_save(EGdata.GraphList,EGdata.Path)   
    if ok then EGdata.Edited=%f,end
    EGdata.Cmenu=[];
   case "Load"
    edited=EGdata.Edited
    [ok,GraphList,edited,path]=ge_do_load()
    if ok then
      EGdata.GraphList=GraphList
      EGdata.Edited=edited
      EGdata.Path=path
      ge_clear_history()
      ge_do_replot(EGdata.GraphList)
    end
    EGdata.Cmenu=[];
   case "Options" then
    EGdata.GraphList=ge_do_options(EGdata.GraphList)
    ge_do_replot(EGdata.GraphList)
    EGdata.Cmenu=[];
   case "Settings" then
    EGdata.GraphList=ge_do_settings(EGdata.GraphList)
    //ge_do_replot(EGdata.GraphList)
    EGdata.Cmenu=[];
   case "Export" then
    ge_do_export(EGdata.GraphList)
    EGdata.Cmenu=[];
   case "Information" then
    ge_do_information(EGdata.GraphList)
    EGdata.Cmenu=[];
   case "Find Node" then
    ge_do_find('Node',EGdata.GraphList)
    EGdata.Cmenu=[];
   case "Find Arc" then
    ge_do_find('Arc',EGdata.GraphList)
    EGdata.Cmenu=[];
   case "Add Node Data Field" then
    [ok,EGdata.GraphList]=ge_do_add_node_data(EGdata.GraphList)
    if ok then EGdata.Edited=%t;end
    EGdata.Cmenu=[];
   case "Add Arc Data Field" then
    [ok,EGdata.GraphList]=ge_do_add_edge_data(EGdata.GraphList)
    if ok then EGdata.Edited=%t;end
    EGdata.Cmenu=[];
    
   case "Quit" then
    if ge_do_quit() then 
      delete(gcf()),return
    end
    EGdata.Cmenu=[];
    
    //----------------- Compute button submenus ----------------- 
   case "Shortest path" then 
    xinfo('Select the head and tail nodes')
    ge_do_shortestpath(EGdata.GraphList)
    EGdata.Cmenu=[];
   case "Circuit" then 
    xinfo('')
    ge_do_circuit(EGdata.GraphList)
    EGdata.Cmenu=[];
   case "Salesman" then 
    xinfo('')
    ge_do_salesman(EGdata.GraphList)
    EGdata.Cmenu=[];

    //----------------- Help button submenus ----------------- 
   case "Help" then
    help('edit_graph_menus')
  end
  
  //finish
  execstr('EGdata_'+w+'=EGdata;')
  seteventhandler("ge_eventhandler");
  show_pixmap()
  scf(old)
endfunction


