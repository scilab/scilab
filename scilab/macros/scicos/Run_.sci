function Run_()
    nc_save=4
    Cmenu='Open/Set'
    ok=%t
    [ok,%tcur,%cpr,alreadyran,needcompile,%state0,%solver]=do_run(%cpr)
    scs_m(1)(3)(6)=%solver
    if ok then newparameters=list(),end
endfunction
