function ge_eventhandler(win,x,y,ibut)
//Copyright INRIA
//Author : Serge Steer 2002
  if ibut<0 then 
    if ibut==-1000 then ge_do_quit(),end//the window has been closed
    return,
  end
  old=xget('window');xset('window',win)
  seteventhandler("")  
  w=string(win)
  if ibut>2 then 
    if ~ge_shortcut(ibut) then seteventhandler("ge_eventhandler"), xset('window',old);return;end
  end

  execstr('global EGdata_'+w+'; Cmenu=EGdata_'+w+'.Cmenu')

  if Cmenu==[]  then seteventhandler("ge_eventhandler"), xset('window',old);return,end

  [x,y]=xchange(x,y,'i2f')
  GL='EGdata_'+w+'.GraphList';
  xset("alufunction",6),edited=%f
  if Cmenu=="NewNode" then
    execstr(GL+'=ge_add_node('+GL+',node_x=x,node_y=y)')
    edited=%t
  elseif Cmenu=="NewArc" then
    execstr(GL+'=ge_newarc('+GL+',x,y)')
  elseif Cmenu=="Move" then  
    execstr(GL+'=ge_do_move('+GL+',x,y)')
  elseif Cmenu=="Move Region" then  
    execstr(GL+'=ge_do_move_region('+GL+',x,y)')
  elseif Cmenu=="Delete" then
    execstr(GL+'=ge_do_delete('+GL+',x,y)')
  elseif Cmenu=="Delete Region" then
    execstr(GL+'=ge_do_delete_region('+GL+',x,y)')
  elseif Cmenu=="Paste" then
    execstr(GL+'=ge_do_paste('+GL+',x,y)') 
  elseif Cmenu=="Properties" then
    execstr(GL+'=ge_do_properties('+GL+',x,y)') 
  elseif Cmenu=="Copy Region To ClipBoard"  then
    execstr('ge_copy_region_to_cb('+GL+',x,y)') 
    execstr('EGdata_'+w+'.Cmenu=[]')
  elseif Cmenu=="Undo" then
    ge_do_undo()
    execstr('EGdata_'+w+'.Cmenu=[]')
//  elseif Cmenu=="Select" then 
//  to used later for a selection based edition
//     execstr('ge_do_select('+GL+',x,y)') 
 //    execstr('EGdata_'+w+'.Cmenu=[]')
    
  end  
  execstr('EGdata_'+w+'.Edited=EGdata_'+w+'.Edited|edited')
  seteventhandler("ge_eventhandler")  
  xset('alufunction',3)
  if xget('pixmap') then xset('wshow'),end
  xset('window',old)
endfunction

  
