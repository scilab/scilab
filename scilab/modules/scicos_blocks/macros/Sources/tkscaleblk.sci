//  Scicos
//
// Copyright (C) DIGITEO - Clément DAVID <clement.david@scilab.org>
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function block=tkscaleblk(block,flag)
  if flag == 1 then
    // Output update
    slider = findobj("Tag", block.label + "#slider");
    if slider <> [] then
      // calculate real value
      value = (block.rpar(1) + block.rpar(2) - slider.value) / block.rpar(3);
      
      w = slider.parent;
      if w <> [] then
        w.info_message = string(value);
      end

      block.outptr(1) = value;
    end
  elseif flag == 4 then
    // Initialization

    // if already exists (stopped) then reuse
    f = findobj("Tag", block.label);
    if f <> [] then
      return;
    end

    f = figure("Tag", block.label, "Figure_name", "TK Source: " + block.label);

    // delete standard menus
    delmenu(f.figure_id, gettext("&File"));
    delmenu(f.figure_id, gettext("&Tools"));
    delmenu(f.figure_id, gettext("&Edit"));
    delmenu(f.figure_id, gettext("&?"));
    toolbar(f.figure_id, "off");

    f.position = [0 0 80 200];

    // slider
    bounds = block.rpar(1:2);
    initial = mean(bounds);
    uicontrol(f, "Style", "slider", "Tag", block.label + "#slider", ..
      "Min", bounds(1), "Max", bounds(2), "Value", initial, ..
      "Position", [0 0 20 200], "SliderStep", [block.rpar(3) 2*block.rpar(3)]);
    
    // labels
    labels = string([bounds(2)                  ;..
                     mean([bounds(2) initial])  ;..
                     initial                    ;..
                     mean([bounds(1) initial])  ;..
                     bounds(1)]);
    labels = strcat(labels, "<br /><br /><br />");
    uicontrol(f, "Style", "text", "String", labels(1), ..
              "Position", [30  0 50 200]);
    
    // update default value
    block.outptr(1) = initial / block.rpar(3);
  elseif flag == 5 then
    // Ending
    f = findobj("Tag", block.label);
    if f <> [] then
      close(f);
    end
  end
endfunction

