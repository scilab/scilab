function [alreadyran,%cpr] = do_terminate(scs_m,%cpr)
// Copyright INRIA

//** Terminate the current simulation 

  if prod(size(%cpr))<2 then //** if some condition on the compiler is true  
       alreadyran = %f ; 
       return          ;  //** EXIT 
  end
  
  par = scs_m.props ;

  if alreadyran then  //** if the simulation is not finished 
    alreadyran = %f ;
    //terminate current simulation
    //** win = xget('window');
    
    gh_win = gcf(); 
    
      ierr = execstr('[state,t]=scicosim(%cpr.state,par.tf,par.tf,'+'%cpr.sim,''finish'',par.tol)','errcatch')
    
    //** xset('window',win)
    scf(gh_win);
    
            
    %cpr ; //get write access at the variable  
    %cpr.state = state ;//not always called with second arg
    
    //**----------------------------------
    if ierr<>0 then
      kfun = curblock() ;
      corinv = %cpr.corinv
      if kfun<>0 then
	path = corinv(kfun)
	//** xset('window',curwin)
	scf(curwin); 
	bad_connection(path, ['End problem with hilited block';lasterror()],0,1,0,-1,0,1)
      else
	message(['End problem:';lasterror()])
      end    
    
    end
    //**---------------------------------
    
    //** xset('window',curwin)
    scf(curwin);
     
  end
endfunction
