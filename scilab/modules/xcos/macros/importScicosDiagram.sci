function out = importScicosDiagram(source)
  out = [];
  
  [lhs,rhs] = argn(0);
  if rhs <> 1 then
    error(999,  msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "importScicosDiagram", 1));
    return;
  end
  
  if typeof(source) == 'diagram' then //import scs_m structure
    out = source;    
  elseif typeof(source) == 'string' then //import cos or cosf file
    [path,name,ext] = splitfilepath_cos(source);
    ext2 = convstr(ext, "u");
    if ext2 == "COS" then
      load(source);
      out = scs_m;
      clear scs_m;
    elseif ext2 == "COSF" then
      exec(source, -1); 
      out = scs_m;
      clear scs_m;
    else
      error(999 ,msprintf(_("%s: Wrong type for argument %d: String or diagram structure expected"), "importScicosDiagram", 1));
      return;
    end
  else
    error(999, msprintf(_("%s: Wrong type for argument %d: String or diagram structure expected"), "importScicosDiagram", 1));
    return;
  end
  
  version = getDiagramVersion(out);
  if version <> get_scicos_version() then
    out = do_version(out, version);
  end
  
endfunction
