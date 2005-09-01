function ge_compute(kmen,win)
//Copyright INRIA
//Author : Serge Steer 2002
    w=string(win)
    execstr('global EGdata_'+w+'; mens=EGdata_'+w+'.Menus(3)(2:$)')   
    ge_eventhandler(win,-1,-1,mens(kmen))
endfunction
