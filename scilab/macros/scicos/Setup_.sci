function Setup_()
    Cmenu='Open/Set'
    %wpar=do_setup(scs_m(1))
    %scicos_solver=%wpar.tol(6)
    if or(scs_m(1)<>%wpar) then
      scs_m(1)=%wpar
      edited=%t
    end

endfunction
