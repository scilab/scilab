function r=with_modelica_compiler()
  // check if modelica_compiler exists
  if MSDOS then
    path=pathconvert(SCI+'/modules/scicos/bin/modelicac.exe',%f,%t)
  else
    path=pathconvert(SCI+'/modules/scicos/bin/modelicac',%f,%t)
  end
  r=fileinfo(path)<>[]
endfunction
