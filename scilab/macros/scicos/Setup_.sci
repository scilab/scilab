function Setup_()
    Cmenu='Open/Set'
    %wpar=do_setup(scs_m(1))
    if length(%wpar(3))>5 then
      %scicos_solver=%wpar(3)(6)
    end
    if or(scs_m(1)<>%wpar) then
      scs_m(1)=%wpar
      edited=%t
    end
      
endfunction
