function ok=check_mac(txt)
//errcatch doesnt work poperly
// Copyright INRIA
  ok=%t
  errcatch(-1,'continue')
  if execstr('comp(mac)','errcatch')<>0 then
    message(['Incorrect syntax: ';
	     lasterror()])
    ok=%f
  end
endfunction
