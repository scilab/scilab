function ok=check_mac(txt)
//errcatch doesnt work poperly
// Copyright INRIA
  ok=%t
  if execstr('comp(mac)','errcatch')<>0 then
    message(['Incorrect syntax: ';
	     lasterror()])
    ok=%f
  end
endfunction
