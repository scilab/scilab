function ge_graph(kmen,win)
//Copyright INRIA
//Author : Serge Steer 2002
  w=string(win)
  execstr('global EGdata_'+w+'; mens=EGdata_'+w+'.Menus(1)(2:$)')   
  // just call ge_eventhandler to have all event treated in the same function
  ge_eventhandler(win,-1,-1,mens(kmen))
endfunction
