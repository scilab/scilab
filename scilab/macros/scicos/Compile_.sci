function Compile_()
    nc_save=4
    Cmenu='Open/Set'
    needcompile=4,
    [%cpr,ok]=do_compile(scs_m)
    if ok then
      newparameters=list()
      %tcur=0 //temps courant de la simulation
      alreadyran=%f
      %state0=%cpr.state;
      needcompile=0;
    end
endfunction
