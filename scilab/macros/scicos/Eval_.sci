function Eval_()
    Cmenu='Open/Set'
    disablemenus()
    ierr=execstr(scs_m(1)(5),'errcatch')
    if ierr==0 then 
      [scs_m,%cpr,needcompile,ok]=do_eval(scs_m,%cpr)
      if needcompile<>4 then %state0=%cpr(1),end
      alreadyran=%f
    else
      message(['Incorrect context definition,';
	  'see message in Scilab window'])
    end
    enablemenus()

