// ====================================================================

// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// ====================================================================
function bOK = test_java(java_filename)
  
  bOK = %F;
  
  exec 'SCI/modules/javasci/tests/unit_tests/javac.sci';
  exec 'SCI/modules/javasci/tests/unit_tests/java.sci';
  
  [rep,stat] = javac(java_filename);
  if stat <> 0 then pause,end;

  [rep,stat] = java(java_filename);
  if stat <> 0 then pause,end;

  disp(rep);
  
  [path,fname,extension] = fileparts(java_filename);
  mdelete(fname+'.class');
	bOK = %T;
	
endfunction
// ====================================================================
