function AboutScicos_()
// Copyright INRIA


  Cmenu=[] ; %pt=[];

  if MSDOS & evstr(TCL_EvalStr('file exists $env(COMSPEC)'))     then
    num=message([strsubst(get_scicos_version(),"scicos","Scicos -")+"-";...
             "For more information visit:";...
	     "     www.scicos.org      "],['Open URL','Cancel']);

    if num==1 then
      TCL_EvalStr('exec $env(COMSPEC) /c start http://www.scicos.org &')
    end
    
  else
    message([strsubst(get_scicos_version(),"scicos","Scicos -")+"-";...
             "For more information visit:";...
	     "     www.scicos.org      "]);
  end

endfunction
