function Run_()
    nc_save=4
    Cmenu='Open/Set'
    ok=%t
    [ok,%tcur,%cpr,alreadyran,needcompile,%state0,..
                                   %scicos_solver]=do_run(%cpr)
    scs_m.props.tol(6)=%scicos_solver
    if ok then newparameters=list(),end
endfunction
