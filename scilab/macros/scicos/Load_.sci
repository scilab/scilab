function Load_()
    Cmenu='Open/Set'
    if edited&~super_block then
      num=x_message(['Diagram has not been saved'],['OK','Go Back'])
      if num==2 then return;end
      if alreadyran then do_terminate(),end  //terminate current simulation
      alreadyran=%f
    end
    disablemenus()
    [ok,scs_m,%cpr,edited]=do_load()
    if super_block then edited=%t;end
    if ok then
      if ~set_cmap(scs_m.props.options('Cmap')) then 
	scs_m.props.options('3D')(1)=%f //disable 3D block shape 
      end
      options=scs_m.props.options
      xset('alufunction',3);xbasc();xselect();
      set_background()
      
      pwindow_set_size()
      window_set_size()
      
      xselect();

      if type(scs_m.props.context)==10 then
	%now_win=xget('window')

	[%scicos_context,ierr]=script2var(scs_m.props.context,%scicos_context)
	//for backward compatibility for scifunc
	if ierr==0 then
	  %mm=getfield(1,%scicos_context)
	  for %mi=%mm(3:$)
	    ierr=execstr(%mi+'=%scicos_context(%mi)','errcatch')
	    if ierr<>0 then
	      break
	    end
	  end
	end
	//end of for backward compatibility for scifunc
	if ierr  <>0 then
	  message(['Error occur when evaluating context:'
		   lasterror() ])
	end
	xset('window',%now_win)
      else
	scs_m.props.context=' '
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
