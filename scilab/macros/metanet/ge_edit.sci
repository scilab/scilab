function r=ge_edit(kmen,win)
//Copyright INRIA
//Author : Serge Steer 2002
  w=string(win)
  mess=['Click on a point to add a node'
	'Left click on first on arc tail node then on arc head node'
	'Left click on a node, move and left click new position'
	'Left click on a point, drag, left click to validate selection, left click to fix position'
	'Left click on a point, drag, left click to validate selection'
	'Left click to fix position'
	'Left click on the node or the arc to delete'
	'Left click on a point, drag, left click to validate selection'
	'Left click on the node or the arc to open properties dialog'
	' ']
  mens=['NewNode','NewArc','Move Node','Move Region',..
	"Copy Region To ClipBoard",'Paste','Delete',..
	'Delete Region','Properties','Give default names','Undo']
  execstr('global EGdata_'+w+'; EGdata_'+w+'.Cmenu='''+mens(kmen)+'''')
  if mens(kmen)=='Undo' then
    old=xget('window');xset('window',win);seteventhandler("")
    ge_do_undo()
    execstr('global EGdata_'+w+'; EGdata_'+w+'.Edited=%t')
    execstr('EGdata_'+w+'.Cmenu=[]')
    seteventhandler("ge_eventhandler") ;xset('window',old)
  elseif mens(kmen)=='Give default names' then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_default_names()
    execstr('global EGdata_'+w+'; EGdata_'+w+'.Edited=%t')
    execstr('EGdata_'+w+'.Cmenu=[]')
    seteventhandler("ge_eventhandler") ;xset('window',old)
  else
    xinfo(mess(kmen))
  end
  r=%t
endfunction
