//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function []=test_FPU()
  // BUG 3443
  foo=8.8353017163510351e-10;
  pow_return = (sqrt(foo) == foo^(0.5));

  if (pow_return == %T)
    disp("[Pow Test] Succeded");
  else
    disp("[Pow Test] -*-FAILED-*- -> Bug 3443");
  end

  // Normal call
  x = 9007199254740994.0;
  y = 1.0 - 1/65536.0;
  z = x + y;
  t = z - x;
  op_return = (t == 0);

  if (op_return == %T)
    disp("[Op Test] Double Value -> Diff from Scilab 4");
  else
    disp("[Op Test] Extended Value -> Same as in Scilab 4");
  end

endfunction
