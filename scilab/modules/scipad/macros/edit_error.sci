function answ=edit_error(clearerror)
  if argn(2)==0 then clearerror=%f; end
  [str,n,line,func]=lasterror(clearerror)
  if n>0 then
    if func=="" then
      answ="the last error did not occur inside a library function"
    else
      p=get_function_path(func)
      if p<>[] then
        scipad(p)
        TCL_EvalStr("scipad eval {blinkline "+..
           string(line) +" "+func+"}")
        answ="opening file "+p+" on line "+string(line)+" of function "+func
      else
        answ="the source file of the function "+func+...
              " was not found in any library path"
      end
    end
  else
    answ="no pending error recorded!"
  end
endfunction

