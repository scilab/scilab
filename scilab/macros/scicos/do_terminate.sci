function do_terminate(scs_m,%cpr,%tcur)
// Copyright INRIA
  if prod(size(%cpr))<2 then return,end
  par=scs_m.props;

  if alreadyran then
    //terminate current simulation
    win=xget('window')
    ierr=execstr('[state,t]=scicosim(%cpr.state,%tcur,par.tf,'+..
		 '%cpr.sim,''finish'',par.tol)','errcatch')
    xset('window',win)
    if ierr<>0 then
      message(['End problem:';lasterror()])
    end
  end
endfunction
