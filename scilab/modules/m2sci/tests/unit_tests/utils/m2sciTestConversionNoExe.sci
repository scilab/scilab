// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function convTxt = m2sciTestConversionNoExe(functionName)

mfilesPath = "SCI/modules/m2sci/tests/unit_tests/mfilesnoexec/";

exec("SCI/modules/m2sci/tests/unit_tests/utils/sci_m2sciUnknownDims.sci", -1);
exec("SCI/modules/m2sci/tests/unit_tests/utils/sci_m2sciUnknownType.sci", -1);

mfile2sci(mfilesPath + functionName + ".m", TMPDIR, %F, %F, -1, %T);

convTxt = mgetl(TMPDIR + filesep() + functionName + ".sci" );

endfunction
