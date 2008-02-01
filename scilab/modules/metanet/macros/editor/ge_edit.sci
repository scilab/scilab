function ge_edit(kmen,win)
//Copyright INRIA
//Author : Serge Steer 2002
  w=string(win)
  execstr('global EGdata_'+w+'; mens=EGdata_'+w+'.Menus(2)(2:$)')   

  //execstr('global EGdata_'+w+'; EGdata_'+w+'.Cmenu='''+mens(kmen)+'''')
  // just call ge_eventhandler to have all event treated in the same function
  ge_eventhandler(win,-1,-1,mens(kmen))
endfunction
