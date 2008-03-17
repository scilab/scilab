// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Allan CORNET
// Based on E.Segre dynamickeywords.sce 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt




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
  if with_module('scicos') then
    [l,s,b]=listvarinfile(SCI+"/modules/scicos/macros/scicos/lib");
    load(SCI+"/modules/scicos/macros/scicos/lib");
    n=string(eval(l));
    scicos_functions=(n(2:$));
    execstr("clear "+l);
   end

  list_keywords=list(scilab_primitives,scilab_commands,predef_variables,scilab_functions,scicos_functions);
    
endfunction
