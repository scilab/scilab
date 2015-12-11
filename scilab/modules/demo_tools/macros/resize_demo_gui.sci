//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
