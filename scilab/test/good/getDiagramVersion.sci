function version = getDiagramVersion(diagram)
  [lhs,rhs] = argn(0);
  version = get_scicos_version();
  if rhs <> 1 then
    error(999,  msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "getDiagramVersion", 1));
  end
  
  if typeof(diagram) <> "diagram" then //check inputs
    error(999 ,msprintf(_("%s: Wrong type for argument %d: diagram structure expected"), "getDiagramVersion", 1));
    return;
  end
  
  // check version
  if find(getfield(1, diagram) == "version") <> [] & diagram.version <> "" then
      version=diagram.version;
  else
    version=find_scicos_version(diagram);
  end
endfunction
