function l=modelica_libs()
//build a vector of strings, each xstring contains a path to a modelica library
  
  //the implicit toolbox path
  path=get_function_path('compile_modelica');path=part(path,1: ...
						  length(path)-27)
  
  
  l=[ 
      pathconvert(path+'modelica/Electrical/',%t,%t); 
      pathconvert(path+'modelica/Hydrau/',%t,%t); 
    ]
endfunction
