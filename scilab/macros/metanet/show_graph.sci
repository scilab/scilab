function win=show_graph(GraphList,smode,scale,winsize)
//Copyright INRIA
//Author : Serge Steer 2002
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
    //check if  current editgraph window exist
  global EGcurrent
  if type(EGcurrent)<>1|size(EGcurrent,'*')<>1 then
    smode='new'
  end
  if smode=='new' then
    win=editgraph(GraphList)//,scale,winsize)
  else
    win=EGcurrent;w=string(win)
    GraphList=ge_complete_defaults(GraphList)
    ge_do_new(GraphList)
  end

endfunction

