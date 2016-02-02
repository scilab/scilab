
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->

pop_in = list();
pop_in(1) = 2;

pop_out = coding_ga_binary(pop_in,'code',[]);

if length(pop_out(1))~=8 then pause,end
if pop_out(1)~='11111111' then pause,end

pop_in_2 = coding_ga_binary(pop_out,'decode',[]);

if pop_in_2(1)~=2 then pause,end
