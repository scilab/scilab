function ge_eventhandler(win,x,y,ibut)
//Copyright INRIA
//Author : Serge Steer 2002

  if or(win==winsid()) then //does the windows still exists
    old=xget('window'); xset('window',win)
    seteventhandler("")  
  else //window has been deleted by an asynchronous xdel()
    ok=ge_do_quit()
    if ~ok then //re_create editgraph window with its menus
      xset('window',win);
      ge_create_menus(win)
      seteventhandler('ge_eventhandler')  
    end
    return
      
  end

  if ibut<0 then 
    if ibut==-1000 then //the window has been closed by the window manager
      ok=ge_do_quit(),
      if ~ok then //re_create editgraph window with its menus
	xset('window',win);
	ge_create_menus(win)
	seteventhandler('ge_eventhandler')   
      end
      return
    end
    seteventhandler("ge_eventhandler"),return,
    
  end

  w=string(win)
  if ibut>2 then 
    if ~ge_shortcut(ibut) then seteventhandler("ge_eventhandler"),xset('window',old),return;end
  end

  execstr('global EGdata_'+w+'; Cmenu=EGdata_'+w+'.Cmenu')

  if Cmenu==[]  then seteventhandler("ge_eventhandler"),xset('window',old), return,end

  [x,y]=xchange(x,y,'i2f')
  GL='EGdata_'+w+'.GraphList';
  xset("alufunction",6),edited=%f
  if Cmenu=="NewNode" then
    execstr(GL+'=ge_add_node('+GL+',node_x=x,node_y=y)')
    edited=%t
  elseif Cmenu=="NewArc" then
    execstr(GL+'=ge_newarc('+GL+',x,y)')
  elseif Cmenu=="Move Node" then  
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

  
