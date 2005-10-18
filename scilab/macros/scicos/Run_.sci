function Run_()
    nc_save=4
    Cmenu='Open/Set'
    ok=%t
    %mm=getfield(1,%scicos_context)
    for %mi=%mm(3:$)
      ierr=execstr(%mi+'=%scicos_context(%mi)','errcatch')
      if ierr<>0 then
	break
      end
    end
    [ok,%tcur,%cpr,alreadyran,needcompile,%state0,..
                                            %scicos_solver]=do_run(%cpr)
    scs_m.props.tol(6)=%scicos_solver
    if ok then newparameters=list(),end
endfunction
