// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xclear(win_num)
//xbasc([win_num])
// Clear the graphic window win_num and erase the recorded graphics of
// window win_num
// if win_num is omited, it's the current graphic window
// win_num can also be a vector of window Id to clear a set of windows
//!

  [lhs,rhs]=argn(0);
  vvv=xget("old_style");

  if vvv==0 then
    if rhs==0, win_num=xget("window"), win=get('current_figure'),
      set(win,'visible','off') ;end
      [n1,n2]=size(win_num);
      yyy=xget('window');
      for xxx=win_num, xset('window',xxx); win=get('current_figure'),
	set(win,'visible','off') ;end
	xset('window',yyy);
  else
    if rhs==1 then
      oldxclear(win_num)
    else
      oldxclear()
    end
  end
endfunction 
