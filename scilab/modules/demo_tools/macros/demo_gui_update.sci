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

function script_path = demo_gui_update()
    global subdemolist;

    // Suppression d'une figure précédemment dessinée, si figure il y a ...
    all_figs = winsid();
    all_figs = all_figs(all_figs >= 100001); // All Scilab graphic windows opened for demos
    for fig_index = 1:size(all_figs, "*")
        fig_to_del = get_figure_handle(all_figs(fig_index));
        if ~isempty(fig_to_del) then
            delete(fig_to_del);
        end
    end

    // Handle de la figure
    demo_fig = get("scilab_demo_fig");

    // Frame sur laquelle on a cliqué
    my_selframe_num = msscanf(gcbo.tag, "listbox_%d");

    // Récupération de la liste des démos
    my_index = gcbo.value;
    if my_index == [] then
        script_path = [];
        return;
    end

    my_data = gcbo.user_data;

    script_path = my_data(my_index(1,1),2);
    if grep(script_path,"dem.gateway.sce") == 1 then
        // On est dans le cas ou une nouvelle frame va être affichée

        // Mise à jour du nombre de frame
        demo_fig.userdata.frame_number = my_selframe_num+1;
        resize_demo_gui(demo_fig.userdata.frame_number);
        previous_demolist = demo_fig.userdata.subdemolist;

        mode(-1);
        exec(script_path,-1); // This script erases subdemolist variable if needed

        // Create a temporary variable for userdata
        // because mixing handles and structs can lead to problems
        ud = demo_fig.userdata;
        ud.subdemolist = subdemolist;
        demo_fig.userdata = ud;
        clearglobal subdemolist

        frame = get("frame_" + string(my_selframe_num+1));

        b = frame.border;
        b.title = my_data(my_index(1,1),1)
        frame.border = b;

        listbox = get("listbox_" + string(my_selframe_num+1));
        listbox.string = demo_fig.userdata.subdemolist(:, 1);

        listbox.userdata = demo_fig.userdata.subdemolist;

        //Prints an arrow if its a submenu
        a = grep(listbox.userdata(:,2),"dem.gateway.sce")
        if listbox.string(a) <> [] then
            listbox.string(a) = "<html>"+listbox.string(a)+" &#x2023; </html>";
        end
        ud = demo_fig.userdata;
        ud.subdemolist = previous_demolist;
        demo_fig.userdata = ud;
    else
        // Mise à jour du nombre de frame
        demo_fig.userdata.frame_number = my_selframe_num;
        resize_demo_gui(demo_fig.userdata.frame_number);
    end
endfunction
