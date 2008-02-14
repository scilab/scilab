
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Bruno JOFRET <bruno.jofret@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function res=sci2c(sciFile)



  res=0
// Get default arguments
  [lhs,rhs]=argn(0)

  if rhs < 1 then
    disp("Usage : sci2c(<file_name>)")
    return
  end

  disp("Begin of a Scilab 2 C Tool...")
  disp("Need a better name for further sexy behaviour...")
  return


endfunction
