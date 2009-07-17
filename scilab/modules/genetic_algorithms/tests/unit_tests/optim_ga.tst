
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->

deff('y=test_func(x)','y=x^2');

ga_params = init_param();
ga_params = add_param(ga_params,'dimension',1);

[pop_opt, fobj_pop_opt, pop_init, fobj_pop_init] = optim_ga(test_func, 50, 10, 0.1, 0.7, %F, ga_params);

if length(pop_opt)~=length(pop_init) then pause,end
if size(fobj_pop_opt)~=size(fobj_pop_init) then pause,end
