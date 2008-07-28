//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) INRIA - 2008 - Allan CORNET
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ./license.txt
//

function r = with_modelica_compiler()
  // check if modelica compiler exists in current $PATH

  compilername = 'modelicac'

  // try to launch 'modelicac' with an '--help' switch and test result:
  //  - ierr == 0 means that command completed successfully
  //  - ierr != 0 means an error occurred (most likely file was not found)
  try
    [rep,ierr,err] = unix_g(compilername + ' --help')
    if (ierr == 0) then
      r = %t
    else
      r = %f
    end
  catch
    r = %f
  end
endfunction

