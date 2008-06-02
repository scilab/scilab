
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

deff('y=test_func(x)','y=x^2');

x0 = 10;

T_init = compute_initial_temp(x0, test_func, 0.8, 1000, []);

[x_best, f_best, mean_list, var_list, temp_list, f_history, x_history] = optim_sa(x0, test_func, 10, 1000, T_init, %F, []);

if (x_best==%nan) | (x_best==%inf) then pause,end
