function Context_()
// Copyright INRIA

    Cmenu = 'Replot' ; //** is it really necessary ? 
    
    while %t do
      gh_now_win = gcf() ; //** get the handle 
      %now_win = gh_now_win.figure_id ; //** get the window id 
       [context, ok] = do_context(scs_m.props.context) ; //** the real executing function 
      scf(%now_win) ; //** pur the focus in the current window 
      
      if ~ok then break, end ; //** in case of problem exit 
      
      
      clear %scicos_context  // to make sure only parent context is used
      //** this silly comments means: the %scicos_context is cleared locally BUT
      //** the semi-global variable Scilab mechanism AVOID the destruction of the
      //** upper level environment. As results, the local environment is cleared
      //** and the upper environment is preserved   
      
      if ~exists('%scicos_context') then //** create a structure "just in case"
	%scicos_context = struct() ;
      end
      
        //** context eval here
	[%scicos_context, ierr] = script2var(context, %scicos_context)
        
	//for backward compatibility for scifunc
        if ierr==0 then
	  %mm = getfield(1,%scicos_context)
	  for %mi=%mm(3:$)
	    ierr = execstr(%mi+'=%scicos_context(%mi)','errcatch')
	    if ierr<>0 then
	      break; //** in case of error exit 
	    end
	  end
        end
        //end of for backward compatibility for scifunc
      
      if ierr <>0 then
	message(['Error occur when evaluating context:'
		 lasterror() ]);

      else //** if the first check is ok 
	scs_m_save = scs_m       ; 
	nc_save    = needcompile ;
	scs_m.props.context = context;
	  do_terminate(); 
	  [scs_m,%cpr,needcompile,ok] = do_eval(scs_m, %cpr)
	  if ok then
	    if needcompile<>4 & size(%cpr)>0 then %state0=%cpr.state, end
	    edited = %t ;
	    alreadyran = %f ;
	    enable_undo=%t  
	  else
	    scs_m=scs_m_save
	    needcompile=nc_save
	    enable_undo=%f
	  end

	break ; //** EXIT Point from the while()
      end
    
    end //** of the "infinite" while 
endfunction



