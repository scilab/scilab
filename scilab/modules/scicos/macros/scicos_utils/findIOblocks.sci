// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [idxWrite, idxRead] = findIOblocks(scs_m)

//  Description:
//
//    Find index(es) of "Write to File" blocks in given diagram structure
//
//  Input argument(s):
//
//    scs_m - MANDATORY - structure
//      Diagram structure, typically obtained after a 'load' instruction
//
//  Output argument(s):
//
//    idxWrite - MANDATORY - array of integers
//      Index(es) of "Write to File" blocks in input diagram structure
//
//    idxRead - MANDATORY - array of integers
//      Index(es) of "Read from File" blocks in input diagram structure
//
//  Usage:
//
//    [idxWrite, idxRead] = findIOblocks(scs_m)
//
//  Algorithm: (grep "^\s*//--" findIOblocks.sci | awk -F "//-- " '{print "//  " $1 $2}')
//
//    Initialize outputs
//    For each object in the diagram (scs_m.objs)
//      Is it a block ? 
//        Is it a "Write to File" block ?
//          Add its index to the related output list
//        Is it a "Read from File" block ?
//          Add its index to the related output list
//                                                                          
//  Notes:
//
//    -
//                                                                          
//  2008/03/31  Laurent Vaylet    Creation
//  YYYY/MM/DD  ???               Modification: ???
//

  //-- Initialize outputs
  idxWrite = [];
  idxRead  = [];
  
  //-- For each object in the diagram (scs_m.objs)
  nbObjs = length(scs_m.objs);
  for k = 1:nbObjs
  
    //-- Is it a block ? 
    if typeof(scs_m.objs(k)) == "Block"
    
      //-- Is it a "Write to File" block ?
      if scs_m.objs(k).model.sim == "writef"
    
        //-- Add its index to the related output list
        idxWrite($+1) = k;
      end
      
      //-- Is it a "Read from File" block ?
      if scs_m.objs(k).model.sim == "readf"
    
        //-- Add its index to the related output list
        idxRead($+1) = k;
      end
    end
  end

endfunction

