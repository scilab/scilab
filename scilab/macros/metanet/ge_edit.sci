function r=ge_edit(kmen,win)
//Copyright INRIA
//Author : Serge Steer 2002
  w=string(win)
  mens=['NewNode','NewArc','Move','Move Region',..
	"Copy Region To ClipBoard",'Paste','Delete',..
	'Delete Region','Properties','Undo']
  execstr('global EGdata_'+w+'; EGdata_'+w+'.Cmenu='''+mens(kmen)+'''')
  if mens(kmen)=='Undo' then
    old=xget('window');xset('window',win);seteventhandler("")
    ge_do_undo()
    execstr('global EGdata_'+w+'; EGdata_'+w+'.Edited=%t')
    execstr('EGdata_'+w+'.Cmenu=[]')
    seteventhandler("ge_eventhandler") ;xset('window',old)
  end
  r=%t
endfunction
