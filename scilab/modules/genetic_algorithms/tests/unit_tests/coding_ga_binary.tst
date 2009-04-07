
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->

pop_in = list();
pop_in(1) = 2;

pop_out = coding_ga_binary(pop_in,'code',[]);

if length(pop_out(1))~=8 then pause,end
if pop_out(1)~='11111111' then pause,end

pop_in_2 = coding_ga_binary(pop_out,'decode',[]);

if pop_in_2(1)~=2 then pause,end
