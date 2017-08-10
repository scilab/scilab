// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function clean_help(dirs)

    // dirs is a set of directories for which html manuals are to be deleted

    lines(0);

    global %helps;
    global %helps_modules;
    %HELPS=[%helps_modules;%helps];

    SCI_long = pathconvert(getlongpathname(SCI),%F,%F);

    //--------------------------------------------------------------------------
    // Sauvegarde de l'environnement initial
    //--------------------------------------------------------------------------

    current_directory   = pwd();
    saved_helps         = %HELPS;
    %helps_save         = %helps;
    %helps_modules_save = %helps_modules;

    //------------------------------------------------------------------
    // Patch because scicos is not written in xml
    //------------------------------------------------------------------
    %helps(grep(%helps,filesep()+"modules"+filesep()+"scicos"+filesep()),:) = [];

    all_scilab_help     = %F;

    [lhs,rhs] = argn(0);

    // Trop de paramêtres
    // ---------------------------------------------------------------------

    if rhs > 1 | rhs < 0 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"clean_help",0,1));
    end

    // Cas par défaut : Aide en ligne de Scilab
    // ---------------------------------------------------------------------

    if (rhs == 0) | (dirs == []) then

        all_scilab_help      = %T;

        dirs_to_build          = %HELPS;
        dirs_to_build_m        = %helps_modules;
        dirs_to_build_c        = %helps;

        dirs                   = get_xml_path(dirs_to_build(:,1));
        dirs_m                 = get_xml_path(dirs_to_build_m(:,1));
        dirs_c                 = get_xml_path(dirs_to_build_c(:,1));

        dirs(     find(dirs   == "")) = [];
        dirs_m(   find(dirs_m == "")) = [];
        dirs_c(   find(dirs_c == "")) = [];
    end

    // On transforme le ou les chemins donnés en chemin absolu
    // ---------------------------------------------------------------------

    for k=1:size(dirs,"*");
        if ~isdir(dirs(k)) then
            error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",dirs(k)));
        end

        chdir(dirs(k));
        if getos() == "Windows" then
            dirs(k) = getlongpathname(pwd());
        else
            dirs(k) = pwd();
        end
        chdir(current_directory);
    end

    if all_scilab_help then
        for k=1:size(dirs_m,"*");
            if ~isdir(dirs_m(k)) then
                error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",dirs_m(k)));
            end
            chdir(dirs_m(k));
            if getos() == "Windows" then
                dirs_m(k) = getlongpathname(pwd());
            else
                dirs_m(k) = pwd();
            end
            chdir(current_directory);
        end

        for k=1:size(dirs_c,"*");
            if ~isdir(dirs_c(k)) then
                error(msprintf(gettext("%s: Directory %s does not exist or read access denied."),"xmltoformat",dirs_c(k)));
            end
            chdir(dirs_c(k));
            if getos() == "Windows" then
                dirs_c(k) = getlongpathname(pwd());
            else
                dirs_c(k) = pwd();
            end
            chdir(current_directory);
        end
    end

    mprintf(gettext("-- Deleting help files --\n"));

    if all_scilab_help then

        // Clean up helptools dir
        // ---------------------------------------------------------------------

        if isfile(SCI+pathconvert("/modules/helptools/master_"+getlanguage()+"_help.xml",%f,%f)) then
            mprintf(".");
            mdelete(SCI+pathconvert("/modules/helptools/master_"+getlanguage()+"_help.xml",%f,%f));
        end

        // Now, remove the generated files

        format_to_test = ["chm";"ps";"pdf";"jar"];

        for k=1:size(format_to_test,"*")
            help_file_to_rm = pathconvert(SCI+"/modules/helptools/"+format_to_test(k)+"/scilab_"+getlanguage()+"_help."+format_to_test(k),%f,%f);
            log_file_to_rm  = pathconvert(SCI+"/modules/helptools/scilab_"+getlanguage()+"_help."+format_to_test(k)+".log",%f,%f);

            if fileinfo(help_file_to_rm)<>[] then
                mprintf(".");
                mdelete(help_file_to_rm);
                mdelete(log_file_to_rm);
            end
        end

        html_dir = pathconvert(SCI+"/modules/helptools/html/"+getlanguage());

        // remove the generated html tree

        if isdir(html_dir) then
            mprintf(".");
            rmdir(html_dir,"s");
        end

        // Now, remove the temporary files

        format_to_test = ["chm";"ps";"pdf";"javaHelp";"html"];

        for k=1:size(format_to_test,"*")
            dir_to_rm = pathconvert(SCI+"/modules/helptools/"+format_to_test(k)+"/scilab_"+getlanguage()+"_help",%f,%f);
            if isdir(dir_to_rm) then
                mprintf(".");
                rmdir(dir_to_rm,"s");
            end
        end

        // Cleanup directories one by one
        // ---------------------------------------------------------------------

        for k=1:size(dirs_m,"*") // help directory

            files_to_delete = listfiles([ ..
            dirs_m(k)+"/.list*"; ..
            dirs_m(k)+"/.last*"; ..
            ]);

            for i=1:size(files_to_delete,"*")
                if isfile(pathconvert(files_to_delete(i),%f,%f)) then
                    mprintf(".");
                    mdelete(files_to_delete(i));
                end
            end
        end

        // Toolbox directories
        // ---------------------------------------------------------------------

        for k=1:size(dirs_c,"*") // help directory

            files_to_delete = listfiles([ ..
            dirs_c(k)+"/.list*"; ..
            dirs_c(k)+"/.last*"; ..
            dirs_c(k)+"/master_help.xml"; ..
            ]);

            for i=1:size(files_to_delete,"*")
                if isfile(pathconvert(files_to_delete(i),%f,%f)) then
                    mprintf(".");
                    mdelete(files_to_delete(i));
                end
            end

            // Now, remove the temporary files

            dir_to_rm = pathconvert(dirs_c(k)+"/scilab_"+getlanguage()+"_help",%f,%f);
            if isdir(dir_to_rm) then
                mprintf(".");
                rmdir(dir_to_rm,"s");
            end

            // Now, remove the generated files

            format_to_test = ["chm";"ps";"pdf";"jar"];

            for i=1:size(format_to_test,"*")
                help_file_to_rm = pathconvert(dirs_c(k)+"/../../"+format_to_test(i)+"/scilab_"+getlanguage()+"_help."+format_to_test(i),%f,%f);
                log_file_to_rm  = pathconvert(dirs_c(k)+"/scilab_"+getlanguage()+"_help."+format_to_test(i)+".log",%f,%f);

                if isfile(file_to_rm) then
                    mprintf(".");
                    mdelete(file_to_rm);
                    mdelete(log_file_to_rm);
                end
            end

            html_dir = pathconvert(dirs_c(k)+"/../../html/"+getlanguage());

            // remove the generated html tree

            if isdir(html_dir) then
                mprintf(".");
                rmdir(html_dir,"s");
            end

        end

    else

        for k=1:size(dirs,"*") // help directory

            dir_language = guess_lang(dirs(k));

            files_to_delete = listfiles([ ..
            dirs(k)+"/.list*"; ..
            dirs(k)+"/.last*"; ..
            dirs(k)+"/master_help.xml"; ..
            ]);

            for i=1:size(files_to_delete,"*")
                if isfile(pathconvert(files_to_delete(i),%f,%f)) then
                    mprintf(".");
                    mdelete(files_to_delete(i));
                end
            end

            // Now, remove the temporary files

            dir_to_rm = pathconvert(dirs(k)+"/scilab_"+dir_language+"_help",%f,%f);

            if isdir(dir_to_rm) then
                mprintf(".");
                rmdir(dir_to_rm,"s");
            end

            // Now, remove the generated files

            format_to_test = ["chm";"ps";"pdf";"jar"];

            for i=1:size(format_to_test,"*")
                help_file_to_rm = pathconvert(dirs(k)+"/../../"+format_to_test(i)+"/scilab_"+dir_language+"_help."+format_to_test(i),%f,%f);
                log_file_to_rm  = pathconvert(dirs(k)+"/scilab_"+dir_language+"_help."+format_to_test(i)+".log",%f,%f);

                if isfile(help_file_to_rm) then
                    mprintf(".");
                    mdelete(help_file_to_rm);
                    mdelete(log_file_to_rm);
                end
            end

            html_dir = pathconvert(dirs(k)+"/../../html/"+dir_language);

            // remove the generated html tree

            if isdir(html_dir) then
                mprintf(".");
                rmdir(html_dir,"s");
            end

        end

    end

    mprintf("\n");

    // On remet l'environement initial
    //--------------------------------------------------------------------------

    chdir(current_directory);
    %helps         = %helps_save;
    %helps_modules = %helps_modules_save;
    %HELPS         = saved_helps;

endfunction



function dirs_out = get_xml_path(dirs_in)

    dirs_out = [];

    for k=1:size(dirs_in,"*")

        if basename(dirs_in(k)) == "jar" then

            help_basepath = part(dirs_in(k),1:length(dirs_in(k))-4) + filesep() + "help";

            if isdir(help_basepath + filesep() +getlanguage()) then
                dirs_out(k) = help_basepath + filesep() + getlanguage();
            elseif isdir(help_basepath+filesep()+getdefaultlanguage()) then
                dirs_out(k) = help_basepath+filesep()+getlanguage();
            else
                dirs_out(k) = "";
            end

        else
            dirs_out(k) = dirs_in(k);
        end

    end

endfunction


function language_out = guess_lang(dir_in)

    language_out = getlanguage();

    [my_start,my_end,my_match] = regexp(basename(dir_in),"/([a-z][a-z]_[A-Z][A-Z])$/");

    if my_start <> [] then
        language_out = part(my_match,1:5);
    end

endfunction
