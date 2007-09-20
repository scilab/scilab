function ok=do_save(scs_m,filenamepath)   
// saves scicos data structures scs_m and %cpr on a binary file
// filename is an optional to indicate the name of the file (name of
// the diagram is not affected)
//!
// Copyright INRIA
  if pal_mode then scs_m=do_purge(scs_m),end
  //file path
  if size(scs_m.props.title,'*')<2 then 
    path='./'
  else
    path=scs_m.props.title(2)
  end
  //open file
  if ~super_block&~pal_mode then
    //update %cpr data structure to make it coherent with last changes
    if needcompile==4 then
      %cpr=list()
    else
      [%cpr,%state0,needcompile,alreadyran,ok]=do_update(%cpr,%state0,needcompile)
      if ~ok then return,end
      %cpr.state=%state0
    end
  else
    %cpr=list()
  end
  if argn(2)>1 then
    fname=filenamepath
  else
    fname=path+scs_m.props.title(1)+'.cos'
  end
  [u,err]=mopen(fname,'wb')
  if err<>0 then
    message('Directory write access denied')
    ok=%f
    return
  end
  //save
  if execstr('save(u,scs_m,%cpr)','errcatch')<>0 then
    message(['Save error:';lasterror()])
    ok=%f
    mclose(u)
    return
  end
  mclose(u)
  if pal_mode then update_scicos_pal(path,scs_m.props.title(1),fname),end
  ok=%t
endfunction
