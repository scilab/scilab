function win=show_graph(GraphList,smode,scale,winsize)
//Copyright INRIA
//Author : Serge Steer 2002
  //
  p=funcprot();funcprot(0) 
  deff('ge_eventhandler(win,x,y,ibut)',' ') //just to avoid defered asynchronous actions
  global EGcurrent
  select argn(2)
  case 1 then
    smode='rep',scale=1,winsize=[600 400]
  case 2 then
    if type(smode)==10 then
      scale=1,
    else
      scale=smode
      smode='rep'
    end
    winsize=[600 400]
  case 3 then
    winsize=[600 400]
  end
  if and(smode<>['rep','new']) then 
    error('Second argument should be ""rep"" or ""new""')
  end
  
  if smode=='rep' then
    if EGcurrent==[] then
      disp('EGcurrent==[]')
      smode='new',t=%f
    else
      w=string(EGcurrent)
      execstr('global EGdata_'+w+';t=type(EGdata_'+w+')==1')
      if t then
	smode='new'
	disp('EGdata_'+w+'==[]')
      else
	smode='rep',
      end
    end
  end


  if smode=='new' then
    win=editgraph(GraphList)//,scale,winsize)
  else //replace
    old=[];if winsid()<>[] then old=xget('window'),end
    if EGcurrent==[] then  disp('EGcurrent==[]'),pause,end
    win=EGcurrent;
    xset('window',win);seteventhandler("") 
    GraphList=ge_complete_defaults(GraphList)
    
    w=string(win)
    ge_do_new(GraphList)
    clear ge_eventhandler 
    seteventhandler("ge_eventhandler"),
    if old<>[] then xset('window',old),end
  end
  funcprot(p)
endfunction
function r=%graph_i_s(varargin),
  pause,
endfunction 
