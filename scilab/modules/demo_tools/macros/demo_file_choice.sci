
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// demolist
// demopath
// demolistlabel

function [] = demo_file_choice()

    while %t then

        if ~exists("demolistlabel") then
            demolistlabel = "choose a demo";
        end

        num = x_choose_modeless(demolist(:,1),demolistlabel);

        if num == 0 then

            demo_clean();
            return;

        else

            script_path = demolist(num,2);

            if (exists("demopath")) & (demopath <> "") then
                script_path = pathconvert( demopath + "/" + script_path , %F , %T );
            end

            if grep(script_path,"dem.gateway.sce") == 1 then

                // Gateway

                previous_demolist      = demolist;
                previous_demopath      = demopath;
                previous_demolistlabel = demolistlabel;

                exec(script_path,-1);
                demo_file_choice();

                demolist      = previous_demolist;
                demopath      = previous_demopath;
                demolistlabel = previous_demolistlabel;

            elseif grep(script_path,"dem.sce") == 1 then
                // Demonstration
                demo_clean();
                demo_run(script_path);
            else
                // Old system
                exec(script_path,-1);

            end
        end
    end

endfunction


function [] = demo_clean()

    // Suppression de toutes les fenêtres graphiques créé
    // par les précédentes démos

    opened_windows = winsid();

    for i=1:size(opened_windows,"c")
        if (opened_windows(i)>=100000) & (opened_windows(i)<=100100) then
            xdel(opened_windows(i));
        end
    end

endfunction
