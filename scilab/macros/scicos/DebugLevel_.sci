function DebugLevel_()
  Cmenu='Open/Set'
  pause
  n=dialog('Set debugging level (0,1,2)',string(scicos_debug()))
  n=evstr(n)
  if n==0|n==1|n==2 then
    scicos_debug(n)
  end
endfunction
