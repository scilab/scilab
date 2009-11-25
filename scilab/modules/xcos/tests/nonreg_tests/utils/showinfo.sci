// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function showinfo(scs_m)

//  Description:
//
//    Show basic info on given diagram structure
//
//  Input argument(s):
//
//    scs_m - MANDATORY - structure
//      Diagram structure, typically obtained after a 'load' instruction
//
//  Output argument(s):
//
//    -
//
//  Usage:
//
//    showinfo(scs_m)
//
//  Algorithm: (grep "^\s*//--" showinfo.sci | awk -F "//-- " '{print "//  " $1 $2}')
//
//    Display number of objects in the diagram
//    For each object
//      Determine and display object type + some info
//        Block
//        Link
//        Text
//        Unknown
//                                                                          
//  Notes:
//
//    -
//
//  2008/03/31  Laurent Vaylet    Creation
//  YYYY/MM/DD  ???               Modification: ???
//

//-- Display number of objects in the diagram
nbObjs = length(scs_m.objs);
mprintf("\n\n %d objects in diagram %s:\n\n", nbObjs, "test.cos")

//-- For each object
for k = 1:nbObjs

  //-- Determine and display object type + some info
  select typeof(scs_m.objs(k))

    //-- Block
    case "Block" then
      mprintf(" - Object #%d is a block whose simulation function is ""%s""\n", ..
        k, scs_m.objs(k).model.sim)

    //-- Link
    case "Link" then
      mprintf(" - Object #%d is a link from block #%d to block #%d\n", k, ..
        scs_m.objs(k).from(1), scs_m.objs(k).to(1))

    //-- Text
    case "Text" then
      mprintf(" - Object #%d is some text\n", k)

    //-- Unknown
    else
      mprintf(" - Object #%d is of unknown type\n", k)
  end
end
endfunction

