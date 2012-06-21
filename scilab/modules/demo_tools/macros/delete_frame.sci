//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function delete_frame(my_fig_handle,fr_position)

  my_frame         = findobj("tag", "frame_"    +string(fr_position));
  my_frame_title     = findobj("tag", "title_frame_"+string(fr_position));
  my_listbox       = findobj("tag", "listbox_"  +string(fr_position));

  delete(my_frame);
  delete(my_frame_title);
  delete(my_listbox);

endfunction
