function New_()
    Cmenu='Open/Set'
    %r=2
    if edited then
      %r=message(['Diagram has not been saved';
	  'Are you sure you want to proceed'],['No';'Yes'])
    end
    if %r==2 then
      if alreadyran then do_terminate(),end  //terminate current simulation
      alreadyran=%f
      scs_m=scicos_diagram()
      %wpar=scs_m.props; 
      window_set_size()
      Cmenu='Replot'
      edited=%f
    end
endfunction
