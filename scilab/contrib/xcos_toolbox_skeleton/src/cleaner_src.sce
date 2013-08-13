// This file is released under the 3-clause BSD license. See COPYING-BSD.

function cleaner_src()
    src_dir = get_absolute_file_path("cleaner_src.sce");

    for language = ["c","cpp","fortran"]
        cleaner_file = src_dir + filesep() + language + filesep() + "cleaner.sce";
        if isfile(cleaner_file) then
            exec(cleaner_file);
            mdelete(cleaner_file);
        end
    end
endfunction

cleaner_src();
clear cleaner_src; // remove cleaner_src on stack
