function r=ge_graph(kmen,win)
//Copyright INRIA
//Author : Serge Steer 2002
  w=string(win)
  execstr('global EGdata_'+w+'; mens=EGdata_'+w+'.Menus(1)(2:$)')   


  r=%f
  
  select mens(kmen)
  case "New" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_new()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Zoom" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_zoom()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Replot" then
    old=xget('window');xset('window',win);seteventhandler("")  
    execstr('EGdata=EGdata_'+w)
    GraphList=EGdata.GraphList
    xbasc()
    ge_set_winsize()
    ge_drawobjs(GraphList),
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "SaveAs"
    seteventhandler("")  
    execstr('GraphList=EGdata_'+w+'.GraphList')
    [GraphList,ok]=ge_do_SaveAs(GraphList)
    if ok then
      execstr('EGdata_'+w+'.GraphList=GraphList;EGdata_'+w+'.Edited=%f;')
    end
    seteventhandler("ge_eventhandler") 
  case "Save"
    seteventhandler("")  
    execstr('GraphList=EGdata_'+w+'.GraphList')
    ok=ge_do_save(GraphList)   
    if ok then
      execstr('EGdata_'+w+'.Edited=%f;')
    end
    seteventhandler("ge_eventhandler") 
  case "Load"
    old=xget('window');xset('window',win);seteventhandler("")  
    execstr('EGdata=EGdata_'+w)
    edited=EGdata.Edited
    [ok,GraphList,edited]=ge_do_load()
    if ok then
      EGdata.GraphList=GraphList
      EGdata.Edited=edited
      ge_clear_history()
      xbasc()
      ge_set_winsize()
      ge_drawobjs(GraphList),
      execstr('EGdata_'+w+'.GraphList=GraphList')
    end
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Options" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_options()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Defaults" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_defaults()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Default names" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_default_names()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Export" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_export()
    seteventhandler("ge_eventhandler") ;xset('window',old)
    
  case "Quit" then
    old=xget('window');xset('window',win);seteventhandler("")  
    if ge_do_quit() then 
      xdel(win),
    else
      seteventhandler("ge_eventhandler") ;xset('window',old)
    end
  end
endfunction
