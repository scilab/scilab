function Context_()
    Cmenu='Open/Set'
    while %t do
      %now_win=xget('window')
      [context,ok]=do_context(scs_m.props.context)
      xset('window',%now_win)
      if ~ok then break,end
      if execstr(context,'errcatch') <>0 then
	message(['Error occur when evaluating context:'
		 lasterror() ])
      else
	scs_m.props.context=context;
	disablemenus()
	[scs_m,%cpr,needcompile,ok]=do_eval(scs_m,%cpr)
	if needcompile<>4&size(%cpr)>0 then %state0=%cpr.state,end
	edited=%t
	alreadyran=%f
	enablemenus()
	break,
      end
    end
endfunction
