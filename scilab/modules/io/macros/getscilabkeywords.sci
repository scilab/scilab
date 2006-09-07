// Allan CORNET INRIA 2005 
// Based on E.Segre dynamickeywords.sce 

function list_keywords=getscilabkeywords()

  scilab_primitives=[];
  scilab_commands=[];
  predef_variables=[];
  scilab_functions=[];
  scicos_functions=[];
   
  [scilab_primitives,scilab_commands]=what();
	  
  //predefined variables
  names=who("get");
  predef_variables=names(($-predef())+1:$);
  
  //library functions
  libvar=[];
  for i=1:size(names,1)
    if type(eval(names(i)))==14 then
      libvar=[libvar;names(i)];
    end
  end
  
  for i=1:size(libvar,1)
  libfun=string(eval(libvar(i)));
    scilab_functions=[scilab_functions;libfun(2:$)];
  end
  
   //scicos basic functions: read the lib
  [l,s,b]=listvarinfile(SCI+"/macros/scicos/lib");
  load(SCI+"/macros/scicos/lib");
  n=string(eval(l));
  scicos_functions=(n(2:$));
  execstr("clear "+l);

  list_keywords=list(scilab_primitives,scilab_commands,predef_variables,scilab_functions,scicos_functions);
    
endfunction
