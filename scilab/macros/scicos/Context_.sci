function Context_()
    Cmenu='Open/Set'
    while %t do
      [context,ok]=do_context(scs_m(1)(5))
      if ~ok then break,end
      errcatch(-1,'continue')
      execstr(context) 
      errcatch(-1)
      if iserror(-1) then   
	message('Cannot evaluate context')
	errclear(-1)
      else
	scs_m(1)(5)=context;
	disablemenus()
	[scs_m,%cpr,needcompile,ok]=do_eval(scs_m,%cpr)
	if needcompile<>4&size(%cpr)>0 then %state0=%cpr(1),end
	edited=%t
	alreadyran=%f
	enablemenus()
	break,
      end
    end

