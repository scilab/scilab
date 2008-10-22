// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xg2ps(figureNumber, fileName)

	warnobsolete("xs2ps", "5.1");
	warning(msprintf(gettext("""%s"" has been replaced by ""%s"""),"xg2ps","xs2ps"));
	
	xs2ps(figureNumber, fileName);

endfunction
