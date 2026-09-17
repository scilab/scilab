// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function main_menubar_cb(key)
    // Private internal function
    // * initially called by scilab.start
    // * afterwards called by the menu(File).callback set according to
    //   SCI/modules/gui/etc/main_menubar.xml

    if key=="openFile"
        msg = gettext("Select a file to open");
        ext = [ "*.sce|*.sci"               _("Scilab scripts")
                "*.xcos|*.zcos|*.xmi|*.ssp" _("Xcos diagrams")
                "*.tst"                     _("Scilab Tests")
                "*.sod"                     _("Scilab Data")
                "*.sce|*.sci|*.xcos|*.zcos|*.xmi|*.ssp|*.tst|*.sod" _("All Scilab files")
                ];
        %fileToOpen = uigetfile(ext, pwd(), msg);
        if %fileToOpen ~= "" then
            ext = convstr(fileparts(%fileToOpen, "extension"));
            if or(ext==[".xcos" ".zcos" ".xmi" ".ssp"])
                xcos(%fileToOpen);
            elseif or(ext==[".sce" ".sci" ".tst"])
                editor(%fileToOpen);
            elseif ext==".sod"
                load(%fileToOpen);
            else
                if getos()=="Windows"
                    winopen(%fileToOpen)
                elseif getos()=="Linux"
                    host("xdg-open "+%fileToOpen+" &")
                else
                    host("open "+%fileToOpen+" &")
                end
            end
        end

    elseif  key=="setFavoriteDirs"
        // Setting the list of directories of interest
        // = major directories + Scinotes favorites
        //
        prefFile = SCIHOME + "/scinotesConfiguration.xml";
        try
            tagPath = "//Profile/favoriteDirectories/Directory";
            r = xmlGetValues(tagPath, "path", prefFile);
        catch
            // clear last error in case where no value is found
            errclear()
            r = []
        end

        try
            tagPath = "//Profile/recentFiles/document";
            r2 = xmlGetValues(tagPath, "path", prefFile);
        catch
            // clear last error in case where no value is found
            errclear()
            r2 = []
        end

        try
            tagPath = "//Profile/openFiles/document";
            r2 = [r2 ; xmlGetValues(tagPath, "path", prefFile)];
        catch
            // clear last error in case where no value is found
            errclear()
        end

        if isempty([r;r2]) then
            return;
        end

        if ~isempty(r2) then
            r2 = fileparts(r2, "path");
        end

        r = getlongpathname([r ; r2])

        // Shortening paths with aliases when possible
        fs = filesep()
        if getos() == "Windows" then
            r = strsubst(r, WSCI+fs, "SCI"+fs);
        else
            r = strsubst(r, SCI+fs, "SCI"+fs);
        end
        r = strsubst(r, SCIHOME, "SCIHOME");
        r = strsubst(r, TMPDIR, "TMPDIR");
        r = strsubst(r, home, "home");
        r = unique(r)

        // Removing main dirs duplicated in the Scinotes favorites
        tmp = ["SCI" "SCIHOME" "TMPDIR" "home"];
        tmp = strcat([tmp tmp+"\/" tmp+"\\"],"|");
        tmp = grep(r,"_^("+tmp+")$_", "r");
        r(tmp) = [];
        [?, k] = gsort(convstr(r),"g","i");
        r = r(k);

        // Finding the handle of the submenu "Files => Go to Bookmark"
        // It has a sub-submenu with Label = "TMPDIR", coming from the
        // SCI/modules/gui/etc/main_menubar.xml configuration file.
        root = get(0);
        old = root.showhiddenhandles;   // to be restored before leaving
        root.showhiddenhandles = "on";
        // console
        c = root.children($); // The console is the first child
        f = c.children($);    // The "File" menu is the first one created
        f.callback = "main_menubar_cb(""setFavoriteDirs"")";
        menu_found = %f;
        for i = 1:length(f.children)
            for j = 1:length(f.children(i).children)
                if f.children(i).children(j).Label=="TMPDIR"
                    menu_found = %t;
                    break;
                end
            end
            if menu_found
                break;
            end
        end
        if menu_found
            h = f.children(i);
            // We update the list of bookmarks
            //     Deleting the list of other bookmarks
            //     keeping only "TMPDIR","SCIHOME", "SCI" and "home":
            delete(h.children(1:$-4));
            // Creating the new list
            for d = r'
                uimenu(h, "Label", d, "Callback", "cd("""+d+"""),");
            end
        end
        root.showhiddenhandles = old;
    end
endfunction
