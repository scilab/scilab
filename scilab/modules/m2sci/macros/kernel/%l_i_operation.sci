// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function operation = %l_i_operation(field,inslist,operation)
    if field=="dims" then
        operation.out(1).infer.dims = inslist
    else
        error(gettext("Not yet implemented."))
    end
endfunction

/*
Architecture of an operation tlist (here for "1+2"):
 s: [operation] tlist with fields:
      operator = "+"
      operands: list:
          (1) : [cste] tlist with fields:
              value = 1
              infer: [infer] tlist with fields:
                  dims: list:
                      (1) = 1
                      (2) = 1
                  type: [type] tlist with fields:
                      vtype = 1
                      property = 0
                  contents: [contents] tlist with fields:
                      index:  empty list()
                      data:  empty list()
          (2) : [cste] tlist with fields:
              value = 2
              infer: [infer] tlist with fields:
                  dims: list:
                      (1) = 1
                      (2) = 1
                  type: [type] tlist with fields:
                      vtype = 1
                      property = 0
                  contents: [contents] tlist with fields:
                      index:  empty list()
                      data:  empty list()
      out: list:
          (1) : [variable] tlist with fields:
              name = "ans"
              infer: [infer] tlist with fields:
                  dims: list:
                      (1) = 1
                      (2) = 1
                  type: [type] tlist with fields:
                      vtype = 1
                      property = 0
                  contents: [contents] tlist with fields:
                      index:  empty list()
                      data:  empty list()
*/
