// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) 2007 - INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xbasc(win_num)
//xbasc([win_num])
// Clear the graphic window win_num and erase the recorded graphics of
// window win_num
// if win_num is omited, it's the current graphic window
// win_num can also be a vector of window Id to clear a set of windows
//!

  [lhs,rhs]=argn(0);
  if rhs==0, win_num=xget("window"), delete('all') ;end
  [n1,n2]=size(win_num);
  yyy=xget('window');
  for xxx=win_num, xset('window',xxx);delete('all');end
  xset('window',yyy);
endfunction 


