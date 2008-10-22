
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function win=edit_graph(GraphList,%zoom,%wsiz)
// edit_graph - graph editor
//%SYNTAX
// edit_graph(GraphList)
//%PARAMETERS
// GraphList    : scilab list, edit_graph main data structure


  if find(sciargs()=="-nogui") <>[] then
    warning(" Scilab in no gui mode : edit_graph unavailable");
    win=-1
    return
  end;


  [lhs,rhs]=argn(0)

  if rhs<2 then %zoom=1,end
  if rhs<3 then %wsiz=[],end 

  //Initialisation
  [Menus,Shorts]=initial_editgraph_tables()

  if winsid()<>[] then 
    old=gcf();win=max(winsid())+1
  else
    old=[];win=0
  end

  edited=%f
  %path='./'

  //initial graph data structure
  %fil=''
  if rhs>=1 then 
    if type(GraphList)==10 then //diagram is given by its filename
      %fil=GraphList
      [ok,GraphList,edited]=ge_do_load(%fil)
      if ~ok then return,end
    else //diagram is given by its datastructure
      if typeof(GraphList)<>'graph' then 
	error('first argument must be a graph data structure'),
      end
    end
  else //empty graphlist
    GraphList=ge_new_graph()
  end




  EGdata=tlist(['egdata','GraphList','Cmenu','Win','Zoom', ...
		'Wsize','Menus','Edited','ShortCuts','Path'],GraphList,[], ...
	       win,%zoom,%wsiz,[],edited,[],%fil)
  
  //initialize graphics
  ge_win_handle=scf(win);
  ge_win_handle.pixmap='on';


  ge_set_winsize()

  //keyboard shortcuts
  execstr('load .editgraph_short','errcatch')  
  EGdata.ShortCuts=Shorts;
  EGdata.Menus=Menus

  w=string(win)


  //Create the global variable indexed by the editor window number
  clearglobal('EGdata_'+w,'EGhist_'+w)
  execstr('EGdata_'+w+'=EGdata; global  EGdata_'+w+' EGcurrent')
  EGcurrent=win //set current Editgraph window
  ge_clear_history()
  ge_axes_handle=gca()
  set(ge_axes_handle,'user_data',tlist(['gindex','node','edge'],[],[]))

  ge_draw_graph(GraphList)

  //create the menu buttons
  ge_create_menus(win)
  seteventhandler('ge_eventhandler')   
  if type(old)==9 then scf(old),end
endfunction

