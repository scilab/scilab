function Run_()
    nc_save=4
    Cmenu='Open/Set'
    ok=%t
    [ok,%tcur,%cpr,alreadyran,needcompile,%state0]=do_run(%cpr)
    if ok then newparameters=list(),end

