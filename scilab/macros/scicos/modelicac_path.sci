function path=modelicac_path()
  path=get_function_path('modelicac_path')  
  path=part(path,1:length(path)-25)+'bin/'
  if ~MSDOS then
    path = pathconvert(path,%t,%t)+'modelicac'; 
  else
    path = pathconvert(path,%t,%t)+'modelicac.exe'; 
  end
endfunction
