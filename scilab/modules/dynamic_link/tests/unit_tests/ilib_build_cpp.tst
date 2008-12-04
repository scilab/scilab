// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

//Here with give a complete example on adding new primitive to Scilab
//create the procedure filese

//creating the interface file


i=['#include <string>'
'extern ""C"" {'
'#include ""stack-c.h""'
'int sci_cppfind(char *fname) {'
'  int m1 = 0, n1 = 0, l1;'
'  char *inputString1, *inputString2;'
'  int m2 = 0, n2 = 0, l2;'
'  int m3 = 0, n3 = 0;'
'  double *position = NULL; /* Where we will store the position */'
'  CheckRhs(2,2); /* Check the number of input argument */'
'  CheckLhs(1,1); /* Check the number of output argument */'
'  GetRhsVar(1, ""c"", &m1, &n1, &l1); /* Retrieve the first input argument */'
'  inputString1=cstk(l1);'
'  GetRhsVar(2, ""c"", &m2, &n2, &l2); /* Retrieve the second input argument */'
'  inputString2=cstk(l2);'
'  std::string myMessage (inputString1);'
'  std::string search (inputString2);'
'  m3=1;n3=1;'
'  position = new double[1];'
'  if (myMessage.find(search) != std::string::npos) {'
'    position[0] = myMessage.find(search); /* The actual operation */'
'  } else {'
'    position[0] = -1; /* Substring not found */'
'  }'
'  CreateVarFromPtr(Rhs+1,""d"",&m3,&n3,&position); /* Create the output argument */'
'  LhsVar(1) = Rhs+1;'
'  delete[] position;'
'  return 0;'
'}'
'}'];

mputl(i,'sci_cppfind.cxx');

//creating the shared library (a gateway, a Makefile and a loader are 
//generated. 

files=['sci_cppfind.cxx'];
ilib_build('foo',['cppfind','sci_cppfind'],files,[]);

// load the shared library 

exec loader.sce 

// Small test to see if the function is actually working.
if cppfind("my very long string","long") <> 8 pause, end
if cppfind("my very long string","very") <> 3 pause, end
if cppfind("my very long string","short") <> -1 pause, end
