
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Ghislain HELIOT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=demo_folder_choice(path,ch)

    if ch=="anim" then
        deff("[]=demoexc(num)","exec(path+demolist(num,2),-1)")
        fs = get("figure_style");
        while %t then
            num=x_choose_modeless(demolist(:,1),"Choose a demo");
            if num==0 then
                set("figure_style",fs);
                lines(oldln(1));
                return
            else
                set("figure_style","old");
                xdel();
                clf();
                demoexc(num);
            end
        end

    else

        if ch=="root" then
            deff("[]=demoexec(num)","exec(demolist(num,2),-1)")
            while %t then
                num=x_choose_modeless(demolist(:,1),["Click to choose a demo";
                "(see also in SCIDIR/demos/...)"]);
                if num==0 then
                    lines(oldln(1))
                    return
                else
                    demoexec(num);
                end
            end

        else
            deff("[]=demoex(num)","exec(path+demolist(num,2),-1)")
            while %t then
                num=x_choose_modeless(demolist(:,1),"Choose a demo");
                if num==0 then
                    lines(oldln(1))
                    return
                else
                    xdel(0);
                    clf();
                    demoex(num);
                end
            end
        end
    end

    mode(0);

endfunction
