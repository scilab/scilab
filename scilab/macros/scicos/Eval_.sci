function Eval_()
    Cmenu='Open/Set'
    disablemenus()
    %now_win=xget('window')
    ierr=execstr(scs_m.props.context,'errcatch')
     xset('window',%now_win)
    if ierr==0 then 
      [scs_m,%cpr,needcompile,ok]=do_eval(scs_m,%cpr)
      if needcompile<>4&size(%cpr)>0 then %state0=%cpr.state,end
      alreadyran=%f
    else
      message(['Incorrect context definition, ';lasterror()] )
    end
    enablemenus()
endfunction
