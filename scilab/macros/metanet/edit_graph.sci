function win=edit_graph(GraphList,%zoom,%wsiz)
// edit_graph - graph editor
//%SYNTAX
// edit_graph(GraphList)
//%PARAMETERS
// GraphList    : scilab list, edit_graph main data structure
//!
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA
[lhs,rhs]=argn(0)

if rhs<2 then %zoom=1,end
if rhs<3 then %wsiz=[],end 

//Initialisation
[Menus,Shorts]=initial_editgraph_tables()

if winsid()<>[] then 
  old=xget('window');win=max(winsid())+1
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
  GraphList=editgraph_diagram()
end

GraphList=ge_complete_defaults(GraphList)


//initialize graphics
xset('window',win);
set("figure_style","old")
xselect();driver('Rec');
pixmap=%f

if %wsiz==[] then
  rect=ge_dig_bound(GraphList);
  if rect<>[] then 
    %wsiz=min(%zoom*[rect(3)-rect(1),rect(4)-rect(2)], [1000,800]);
  else
    %wsiz=[600,400]
  end
end
xset('wpdim',%wsiz(1),%wsiz(2))

selection=tlist(['Sel','Nodes','Arcs'],[],[])
EGdata=tlist(['egdata','GraphList','Cmenu','Win','Zoom', ...
	     'Wsize','Menus','Edited','ArcId','NodeId','ShortCuts','Sel','Path'],GraphList,[], ...
	    win,%zoom,%wsiz,[],edited,0,0,[],selection,%fil)
ge_set_winsize()

//keyboard shortcuts
execstr('load .editgraph_short','errcatch')  
EGdata.ShortCuts=Shorts;

w=string(win)


EGdata.Menus=Menus

//Create the global variable indexed by the editor window number

execstr('EGdata_'+w+'=EGdata; global  EGdata_'+w+' EGcurrent')
EGcurrent=win //set current Editgraph window
ge_clear_history()
ge_drawobjs(GraphList)
if xget('pixmap') then xset('wshow'),end
//create the menu buttons
ge_create_menus(win)
seteventhandler('ge_eventhandler')   
if old<>[] then xset('window',old),end
endfunction

