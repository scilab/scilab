function r=with_modelica_compiler()
  // check if modelica_compiler exists
  if MSDOS then
    path=pathconvert(SCI+'/bin/modelicac.exe',%f,%t)
  else
    path=pathconvert(SCI+'/bin/modelicac',%f,%t)
  end
  r=fileinfo(path)<>[]
endfunction
