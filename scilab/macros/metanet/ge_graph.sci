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
    execstr('GraphList=EGdata_'+w+'.GraphList;path=EGdata_'+w+'.Path')
    [GraphList,ok,path]=ge_do_SaveAs(GraphList,path)
    if ok then
      ge_drawtitle(GraphList.name)
      execstr('EGdata_'+w+'.GraphList=GraphList;EGdata_'+w+'.Edited=%f;EGdata_'+w+'.Path=path;')
    end
    seteventhandler("ge_eventhandler") 
  case "Save"
    seteventhandler("")  
    execstr('GraphList=EGdata_'+w+'.GraphList;path=EGdata_'+w+'.Path')
    ok=ge_do_save(GraphList,path)   
    if ok then
      execstr('EGdata_'+w+'.Edited=%f;')
    end
    seteventhandler("ge_eventhandler") 
  case "Load"
    old=xget('window');xset('window',win);seteventhandler("")  
    execstr('EGdata=EGdata_'+w)
    edited=EGdata.Edited
    [ok,GraphList,edited,path]=ge_do_load()
    if ok then
      EGdata.GraphList=GraphList
      EGdata.Edited=edited
      EGdata.Path=path
      ge_clear_history()
      xbasc()
      ge_set_winsize()
      ge_drawobjs(GraphList),
      execstr('EGdata_'+w+'=EGdata;')
    end
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Options" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_options()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Settings" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_settings()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Export" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_export()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Information" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_information()
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Find Node" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_find('Node')
    seteventhandler("ge_eventhandler") ;xset('window',old)
  case "Find Arc" then
    old=xget('window');xset('window',win);seteventhandler("")  
    ge_do_find('Arc')
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
