function Load_()

    Cmenu='Open/Set'
    if edited&~super_block then
      message(['Diagram has not been saved'])
      if alreadyran then do_terminate(),end  //terminate current simulation
      alreadyran=%f
    end
    disablemenus()
    [ok,scs_m,%cpr,edited]=do_load()
    if super_block then edited=%t;end
    if ok then
        if ~set_cmap(scs_m(1).options('Cmap')) then 
	scs_m(1).options('3D')(1)=%f //disable 3D block shape 
        end
      %wdm=scs_m(1).wpar
      xset('alufunction',3);xbasc();xselect();
      set_background()
      
      window_set_size()

      xselect();

      if type(scs_m(1).context)==10 then
	errcatch(-1,'continue')
	execstr(scs_m(1).context) 
	errcatch(-1)
	if iserror(-1) then   
	  message('Cannot evaluate context')
	  errclear(-1)
	end
      else
	scs_m(1).context=' '
      end

      xset('alufunction',6)
      drawobjs(scs_m),
      if pixmap then xset('wshow'),end
      if size(%cpr)==0 then
	needcompile=4
	alreadyran=%f
      else
	%state0=%cpr.state
	needcompile=0
	alreadyran=%f
      end
    end
    enablemenus()
endfunction
