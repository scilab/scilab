// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2009-2010 -  DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function generated_files = xmltochm(dirs,titles,directory_language,default_language)

    warnobsolete(scilabRemovedVersion="5.5.1");

    all_scilab_help    = %F;

    if getos() <> "Windows" then
        warning(msprintf(gettext("%s: This function is only supported on Windows platforms.\n"),"xmltochm"));
        return;
    end

    lhs = argn(1);
    rhs = argn(2);

    select rhs

    case 0 then
        generated_files = xmltoformat("chm");
        dirs = [];

    case 1 then
        generated_files = xmltoformat("chm", dirs);

    case 2 then
        generated_files = xmltoformat("chm", dirs, titles);

    case 3 then
        generated_files = xmltoformat("chm", dirs, titles, directory_language);

    case 4 then
        generated_files = xmltoformat("chm", dirs, titles, directory_language, default_language);

    else
        error(msprintf(gettext("%s: Wrong number of input argument(s): At most %d expected.\n"), "xmltochm", 4));
    end

    if (dirs == []) then
        all_scilab_help = %T;
    end

    if getos() == "Windows" then
        if (generated_files <> []) then
            if (getHHCpath() <> "") then
                for i = 1:size(generated_files, "*")
                    currentpath = pwd();
                    [path,fname,extension] = fileparts(getshortpathname(generated_files(i)));
                    cd(path);
                    cmdline = getHHCpath() + " " + fname + extension;
                    [msg,err] = dos(cmdline);
                    cd(currentpath);
                    chm_origin = "htmlhelp.chm";

                    chm_name = tokens(generated_files(i), filesep());
                    chm_name = chm_name($-1);
                    chm_name = chm_name + ".chm";

                    chm_src = strsubst(generated_files(i), "htmlhelp.hhp", chm_origin);
                    dir_src_idx = strindex(chm_src, filesep());
                    dir_src_chm = part(chm_src,1:dir_src_idx($));

                    if all_scilab_help then
                        if (i == 1) then
                            dir_dest_chm = part(chm_src,1:dir_src_idx($-2)) + "chm" + filesep();
                        else
                            dir_dest_chm = part(chm_src,1:dir_src_idx($-3)) + "chm" + filesep();
                        end
                    else
                        dir_dest_chm = part(chm_src,1:dir_src_idx($-3)) + "chm" + filesep();
                    end

                    chm_destination = dir_dest_chm  + chm_name;

                    copyfile(chm_src, chm_destination);
                    generated_files(i) = chm_destination;
                end
            else
                warning(msprintf(gettext("%s: hhc.exe not found.\n"), "xmltochm"));
            end
        else
            warning(msprintf(gettext("%s: no file generate.\n"), "xmltochm"));
        end
    end

endfunction

function HHC_path = getHHCpath()

    if win64() then
        std_path = getshortpathname(getenv("ProgramFiles(x86)", "") + filesep() + "HTML Help Workshop" + filesep() + "hhc.exe");
    else
        std_path = getshortpathname(std_path = getenv("ProgramFiles", "") + filesep() + "HTML Help Workshop" + filesep() + "hhc.exe");
    end

    if (fileinfo(std_path) == []) then
        HHC_path = "";
    else
        HHC_path = std_path;
    end

endfunction
