//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//

function resize_demo_gui(frame_number)
    axes_w = frame_number * 250; // axes width
    demo_fig = get("scilab_demo_fig");
    demo_fig.axes_size(1) = axes_w;
    demo_fig.children($:-1:$-(frame_number-1)).visible = "on";
    //hide other frame
    demo_fig.children($-frame_number:-1:1).visible = "off";
    //clean listbox
    demo_fig.children($-frame_number:-1:1).children(1).string = "";
endfunction
