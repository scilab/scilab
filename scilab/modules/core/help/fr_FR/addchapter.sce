// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan Cornet
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// ====================================================================
// help core module
// Add chapter
// ====================================================================

path = SCI+'/modules/core/help/'+getlanguage();
add_help_chapter("Scilab",path,%T);
clear path add_help_chapter;
