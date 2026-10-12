// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// The help browser renders the XML pages directly: at build time, only the
// images of the <scilab:image> blocks and .zcos diagrams are pre-generated,
// into <toolbox>/help/images, so that a broken example shows up here rather
// than as a missing image. The inline help index (<toolbox>/inline/<lang>/
// links.json) is also built here.
//
// The generation runs in another Scilab loading the freshly built toolbox
// (its etc/*.start): the examples call functions of the toolbox, which is not
// loaded in the session running the builder.
//
// The images of every language are generated at once. The historical calling
// convention -- one call per language, from each help/<lang>/build_help.sce --
// still works: the first call does the job, the next ones find the images up
// to date and return without launching anything.

function tbx_build_help(moduletitle, path)

    fname = "tbx_build_help";
    rhs = argn(2);

    // Number of input arguments

    if and(rhs <> [1 2]) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), fname, 1, 2));
    end

    // Input argument N°1
    // → name

    if type(moduletitle) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1));
    end

    if size(moduletitle, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1));
    end

    // Input argument N°2
    // → path: the <toolbox>/help directory, or one of its <lang> subdirectories

    if rhs < 2 then
        path = pwd();
    else
        if type(path) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2));
        end

        if size(path, "*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"), fname, 2));
        end

        if ~isdir(path) then
            error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"), fname, path));
        end
    end

    path = pathconvert(path, %f, %t);

    // <toolbox>/help, holding the <lang> dirs and the images
    if grep(basename(path), "/^[a-z]{2}_[A-Z]{2}$/", "r") <> [] then
        help_path = fullpath(fullfile(path, ".."));
        langs = basename(path);
    else
        help_path = fullpath(path);
        langs = findfiles(help_path);
        langs = langs(grep(langs, "/^[a-z]{2}_[A-Z]{2}$/", "r"));
    end
    help_path = strsubst(help_path, "\", "/");
    tbx_path = fullpath(fullfile(help_path, ".."));
    index = help_path + "/images/help_images.index";

    // <toolbox>/inline/<lang>/links.json, read by loadToolboxInlineHelp from
    // the loader. Only the languages of this call: one call per language is
    // the historical convention.
    for lang = langs'
        if isdir(help_path + "/" + lang) then
            generate_inline_links(lang, help_path + "/" + lang);
        end
    end

    // The generator is installed with the helptools data, under
    // SCI/modules/helptools/tools: in a source tree, a Windows install, and a
    // Linux install, where bin/scilab sets SCI to <prefix>/share/scilab.
    // Checked all the same: a missing generator would otherwise only show up
    // as an obscure failure of the child.
    gen_script = SCI + "/modules/helptools/tools/generate_help_images.sce";
    if ~isfile(gen_script) then
        warning(msprintf(gettext("%s: ''%s'' not found: the help images of ''%s'' cannot be pre-generated.\n"), fname, gen_script, help_path));
        return;
    end

    // Nothing to do when the manifest is newer than every page of help/ and
    // every diagram of the toolbox -- typically the calls following the first
    // one, for the other languages -- or when no page asks for an image.
    newest = 0;
    has_images = %f;
    todo = tbx_path;
    while todo <> []
        d = todo($);
        todo($) = [];
        entries = findfiles(d);
        for k = 1:size(entries, "*")
            f = d + filesep() + entries(k);
            if isdir(f) then
                todo = [todo; f];
                continue;
            end
            [dummy1, dummy2, ext] = fileparts(f);
            ext = convstr(ext);
            if ext == ".zcos" then
                newest = max(newest, fileinfo(f)(6));
            elseif ext == ".xml" & strindex(strsubst(f, "\", "/"), help_path + "/") == 1 then
                newest = max(newest, fileinfo(f)(6));
                if ~has_images then
                    // <scilab:image> whatever the prefix bound to the Scilab
                    // namespace (not <db:imagedata>), or a .zcos diagram
                    has_images = grep(mgetl(f), ["/:image[\s>]/" "/\.zcos/"], "r") <> [];
                end
            end
        end
    end

    if isfile(index) then
        if fileinfo(index)(6) > newest then
            mprintf(gettext("Help images of %s are up to date.\n"), help_path);
            return;
        end
    elseif ~has_images then
        return;
    end

    // Build the help images in another Scilab loading the prebuilt toolbox:
    // the <scilab:image> examples call its own functions. add_help_chapter()
    // is stubbed out so that the .start does not register a help chapter
    // there.
    code = [
        "funcprot(0);"
        "function ok = add_help_chapter(helptitle, path, modulemode), ok = %t; endfunction"
    ];

    f = listfiles(fullfile(tbx_path, "etc/*.start"));
    if f <> [] then
        code = [code ; sprintf("exec(""%s"", -1);", strsubst(f, "\", "/"))];
    else
        warning(msprintf(gettext("%s: .start file was not found in ''%s'': the images of the examples calling the functions of the toolbox cannot be pre-generated.\n"), fname, tbx_path));
    end

    // See modules/helptools/tools/generate_help_images.sce: every language of
    // help/ in a single run, stale images pruned.
    code = [
        code
        sprintf("GEN_OUT = ""%s/images"";", help_path)
        sprintf("GEN_HELP_DIRS = ""%s"";", help_path)
        "GEN_TIMINGS = %f;"
        sprintf("exec(""%s"");", strsubst(gen_script, "\", "/"))
    ];

    mprintf(gettext("Pre-generating the help images of %s...\n"), help_path);

    tmp = tempname();
    mputl(code, tmp);

    [status, out, err] = scilab(file=tmp, args="-nouserstartup");
    if ~isempty(out) then
        printf("%s\n", out);
    end

    if status <> 0 then
        if ~isempty(err) then
            printf("%s\n", err);
        end
        // Not fatal: the help browser still generates the missing images on
        // the fly, at the cost of a wait on their first display.
        warning(msprintf(gettext("%s: the help images of ''%s'' could not be pre-generated.\n"), fname, help_path));
    end

    deletefile(tmp);
endfunction
