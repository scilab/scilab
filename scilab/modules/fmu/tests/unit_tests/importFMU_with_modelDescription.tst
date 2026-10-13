//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// <-- NO CHECK REF -->

//Test of import FMU with model description 

path = fmigetPath();
//model Cruise Control
[fmu,obj] = importFMU(fullfile(path, "tests", "unit_tests", "SCADE_CruiseControl__CruiseControl_FMU.fmu")); 
//unzipped
assert_checkequal(fmu.modelFileName, "SCADE_CruiseControl__CruiseControl_FMU");
//library
assert_checkequal(isfile(fmu.libraryFile), %t);
//xml File
assert_checkequal(isfile(fmu.xmlFile), %t);
//xml descriptions
assert_checkequal(typeof(fmu.descriptionFMU), "fmiDescript");
assert_checkequal(typeof(fmu.modelVariables), "InfoFMU");


//We don't need to calling the functions 'fmiModelDescription' and 'fmiModelVariables', 
//because they have been called in 'importFMU'.

//Calling of some standard functions of FMI
assert_checkequal(fmiGetModelTypesPlatform(fmu), "standard32");
assert_checkequal(fmiGetVersion(fmu), "1.0");

//new instance of the model
instance = fmiInstantiateModel(fmu);
assert_checkequal(typeof(instance.modelFMU), "FMU");
//fmiComponent
assert_checkequal(typeof(instance.modelInstance), "pointer");
refs = 0:4;
//all ok
fmiSetReal(instance, refs,[0,1,2,3,4])

//Test of max an min values
defaultMaxValue = instance.modelFMU.modelVariables.Real.max;
defaultMinValue = instance.modelFMU.modelVariables.Real.min;
//max = 0 for #4 value  
instance.modelFMU.modelVariables.Real.max(4) = 0;
errmsg = msprintf(_('%s: Wrong value for input argument #%d: Less expected for #%s valueReference(s).\n'), 'fmiSetReal', 3, "3");
assert_checkerror('fmiSetReal(instance, refs,[0,1,2,3,4])',errmsg);

//max = 0 for #4 value and max = 1 for #2 value
instance.modelFMU.modelVariables.Real.max(4) = 0;
instance.modelFMU.modelVariables.Real.max(2) = 1;
errmsg = msprintf(_('%s: Wrong value for input argument #%d: Less expected for #%s valueReference(s).\n'), 'fmiSetReal', 3, "[1,3]");
assert_checkerror('fmiSetReal(instance, refs,[0 25 2 15 4])',errmsg);
assert_checkerror('fmiSetReal(instance, refs,[0;25;2;15;4])',errmsg);

instance.modelFMU.modelVariables.Real.max = defaultMaxValue;

//min = 10 for #1 value  
instance.modelFMU.modelVariables.Real.min(1) = 10;
errmsg = msprintf(_('%s: Wrong value for input argument #%d: More expected for #%s valueReference(s).\n'), 'fmiSetReal', 3, "0");
assert_checkerror('fmiSetReal(instance, refs,[9 1 2 3,4])',errmsg);

//min = 0 for #2 value; min = 1 for #3 value; min = -5 for #3 value  
instance.modelFMU.modelVariables.Real.min(2) = 1;
instance.modelFMU.modelVariables.Real.min(3) = 0;
errmsg = msprintf(_('%s: Wrong value for input argument #%d: More expected for #%s valueReference(s).\n'), 'fmiSetReal', 3, "[0,1,2]");
assert_checkerror('fmiSetReal(instance, refs,[-5 0 -7 1 1])',errmsg);
assert_checkerror('fmiSetReal(instance, refs,[-5;0;-7;1;1])',errmsg);

//bad references 
refs = 10:15
errmsg = msprintf(_('%s: Wrong values for input argument #%d: References for start values expected.\n'), 'fmiSetReal', 2);
assert_checkerror('fmiSetReal(instance, refs,[1 2 3 4 5 6])',errmsg);

//twice the same reference
errmsg = msprintf(_('%s: Wrong values for input argument #%d: Unique references expected.\n'), 'fmiSetReal', 2);
assert_checkerror('fmiSetReal(instance, [0,0,1,4],[1 2 3 4])',errmsg); 
 







 
