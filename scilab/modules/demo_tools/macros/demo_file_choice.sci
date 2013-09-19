
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
