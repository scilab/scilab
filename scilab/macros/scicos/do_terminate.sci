function [alreadyran,%cpr]=do_terminate(scs_m,%cpr)
// Copyright INRIA

  if prod(size(%cpr))<2 then   alreadyran=%f,return,end
  par=scs_m.props;

  if alreadyran then
    alreadyran=%f
    //terminate current simulation
    win=xget('window')
    ierr=execstr('[state,t]=scicosim(%cpr.state,par.tf,par.tf,'+..
		 '%cpr.sim,''finish'',par.tol)','errcatch')
    xset('window',win)
    %cpr; //not always called with second arg
    %cpr.state=state
    if ierr<>0 then
      kfun=curblock()
      corinv=%cpr.corinv
      if kfun<>0 then
	path=corinv(kfun)
	xset('window',curwin)
	bad_connection(path,..
		       ['End problem with hilited block';lasterror()],0,0,-1,0)
      else
	message(['End problem:';lasterror()])
      end    
    end
    xset('window',curwin)
  end
endfunction
