function Setup_()
    Cmenu='Open/Set'
    %wpar=do_setup(scs_m.props)
    %scicos_solver=%wpar.tol(6)
    if or(scs_m.props<>%wpar) then
      scs_m.props=%wpar
      edited=%t
    end

endfunction
