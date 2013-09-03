
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- JVM_NOT_MANDATORY -->

T = temp_law_csa(10, 9, 8, 7, 6, []);

if norm(T - 10 * log(1+7)/log(2+7)) > 1e-3 then pause,end
