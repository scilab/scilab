// for the GLoad feature 
// Copyright INRIA
  k=strindex(gl_name,'.')
  if k<>[] then 
  select part(gl_name,k($)+1:length(gl_name));
  case "sci" then getf(gl_name), 
  case "sce" then exec(gl_name),
  case "scg" then xload(gl_name),
  case "bin" then load(gl_name), 
  case "cos" then scicos(gl_name)
  case "cosf" then scicos(gl_name)  
  else error("Gload : unknown suffix in file name "+gl_name);
  end
else
  error("Gload : unknown file type "+gl_name);
end

