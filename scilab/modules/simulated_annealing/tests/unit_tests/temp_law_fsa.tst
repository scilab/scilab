
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM_NOT_MANDATORY -->

T = temp_law_fsa(10, 9, 8, 7, 6, []);

if norm(T - 10 * (1+6)/(2+6)) > 1e-3 then pause,end
