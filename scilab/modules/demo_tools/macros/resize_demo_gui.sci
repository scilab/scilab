//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function resize_demo_gui()

demo_fig = findobj("tag", "scilab_demo_fig");

margin_x   = demo_fig.userdata.margin_x;    // Horizontal margin between each elements
margin_y   = demo_fig.userdata.margin_y;    // Vertical margin between each elements

axes_w = demo_fig.axes_size(1);
axes_h = demo_fig.axes_size(2);

for fr_position = 1:demo_fig.userdata.frame_number
  curFrame = findobj("Tag", "frame_"+string(fr_position));
  curTitle = findobj("Tag", "title_frame_"+string(fr_position));
  curListbox = findobj("Tag", "listbox_"+string(fr_position));
  scilabLogo = findobj("Tag", "scilab_logo");

  frame_w = (axes_w-(demo_fig.userdata.frame_number+1)*margin_x)/demo_fig.userdata.frame_number;
  frame_h = axes_h-margin_y*2;

  demo_fig.userdata.frame_w = frame_w;
  demo_fig.userdata.frame_h = frame_h;

  if fr_position <> 1 then
    this_frame_w   = frame_w;
    this_frame_h   = frame_h;
    this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
    this_frame_pos_y = axes_h-(margin_y+frame_h);
  else
    this_frame_w   = frame_w;
    this_frame_h   = frame_h - 150 ;
    this_frame_pos_x = (margin_x+(fr_position-1)*(margin_x+frame_w));
    this_frame_pos_y = axes_h-(margin_y+frame_h)+150;
  end

  if ~isempty(curFrame) then
    set(curFrame, "position", [ this_frame_pos_x this_frame_pos_y this_frame_w this_frame_h ]);
  end
  if ~isempty(curTitle) then
    set(curTitle, "position", [ this_frame_pos_x+10 axes_h-(margin_y)-9 this_frame_w-20 18 ]);
  end
  if ~isempty(curListbox) then
    set(curListbox, "position", [ this_frame_pos_x+5 this_frame_pos_y+5 this_frame_w-12 this_frame_h-14-10 ]);
  end
  if ~isempty(scilabLogo) then
    set(scilabLogo, "position", [ margin_x axes_h-(margin_y+frame_h) frame_w 150]);
  end

end

endfunction
