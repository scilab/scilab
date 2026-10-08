// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2016, 2018, 2021 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function ret = tbx_generate_pofile(tbx_name, tbx_path)
    // tbx_generate_pofile(tbx_name, tbx_path)   // deprecated (6.0)
    // tbx_generate_pofile(tbx_name)             // deprecated (6.0)
    // tbx_generate_pofile(tbx_path)             // 6.0
    // tbx_generate_pofile()                     // 6.0  path = pwd()

    fname = "tbx_generate_pofile"
    rhs = argn(2)
    ret = []

    // CHECKING INPUT PARAMETERS
    // -------------------------
    if and(rhs <> [0 1 2]) then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, fname, 0, 1))
    end

    if rhs==2
        msg = "%s: %s(name, path) is obsolete. Please use %s(path) instead.\n"
        warning(msprintf(msg, fname, fname, fname))  // no translation

    elseif rhs==0
        tbx_path = pwd()
    else
        tbx_path = tbx_name
        if type(tbx_path) <> 10 then
            msg = _("%s: Argument #%d: Text(s) expected.\n")
            error(msprintf(msg, fname, rhs))
        end
        tbx_path = tbx_path(1)
        // May be
        //  * either the former tbx_generate_pofile(tbx_name) (until 5.5.2)
        //  * or the new        tbx_generate_pofile(tbx_path) (from 6.0.0)
        if grep(tbx_path,["/" "\"])==[] && ~isdir(tbx_path) then // only name was provided
            tbx_path = pwd()
        end
        if ~isdir(tbx_path) then
            msg = _("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n")
            error(msprintf(msg, fname, tbx_path))
        end
    end

    // Retrieving the toolbox name
    // ---------------------------
    tbx_name = tbx_get_name_from_path(tbx_path)
    //
    old = pwd();
    cd(tbx_path);
    if getos() == "Windows" then
        XGETTEXT= WSCI + "\tools\gettext\xgettext";
    else
        XGETTEXT="xgettext";
    end
    // -F : sort by file
    XGETTEXT_OPTIONS = " --omit-header --language=C --no-wrap --sort-by-file " + ..
        "-k --keyword=gettext:2 --keyword=_:2 " + ..
        "--keyword=dgettext:2 --keyword=_d:2 --keyword=xmlgettext:2";

    EXTENSIONS = ["c" "h" "cpp" "cxx" "hxx" "hpp" "java"];
    EXTENSIONS_MACROS = ["sci" "sce" "start" "quit"];
    EXTENSIONS_XML = ["xml" "xsl"];
    TARGETDIR = "locales";

    mkdir(TARGETDIR);
    srcFiles = getFilesList("src", EXTENSIONS);
    srcFiles = [srcFiles ; getFilesList("sci_gateway", EXTENSIONS)];
    srcFiles = [srcFiles ; getFilesList("macros", EXTENSIONS_MACROS)];
    srcFiles = [srcFiles ; getFilesList("etc", EXTENSIONS_MACROS)];
    srcFiles = [srcFiles ; getFilesList("classes", EXTENSIONS_MACROS)];

    //manage xml preferences files
    xmlFiles = getFilesList("etc", EXTENSIONS_XML);

    if size(xmlFiles, "*") > 0 then
        xmlTmpFile = fullpath(TMPDIR + "/tmpLoc.xml");
        srcFiles = [srcFiles; xmlTmpFile];
        xmlFake = mopen(xmlTmpFile, "w");
        search = "\(\s*(.*)\s*,\s*(.*)\s*\)\""/";
        replace = "xmlgettext(""\1"", ""\2"")";
        for i = 1:size(xmlFiles, "*")
            content = mgetl(xmlFiles(i));
            newLine = sedLoc(content, "/\""_d"+search, replace);// "_d(xxx,xxx)"
            newLine = sedLoc(newLine, "/\""dgettext"+search, replace);
            newLine = sedLoc(newLine, "/\""gettext"+search, replace);
            newLine = sedLoc(newLine, "/\""_"+search, replace);
            mputl(newLine, xmlFake);
        end
        mclose(xmlFake);
    end

    // parse all files
    if srcFiles==[] then
        cd(old)
        return
    end
    tmp = fullfile(TMPDIR,"xgettext_srcFiles.txt")
    mputl(srcFiles, tmp)
    potFilename = tbx_name + ".pot"
    cmd = XGETTEXT + XGETTEXT_OPTIONS + " --files-from="""+tmp+""" -d " + ..
          tbx_name + " -p " + TARGETDIR + " -o " + potFilename;
    status = host(cmd)
    deletefile(tmp)
    if exists("xmlTmpFile") then
        deletefile(xmlTmpFile);
    end

    TARGETDIR = TARGETDIR + filesep()
    fi = fileinfo(TARGETDIR + potFilename);
    if fi == [] | fi(1) == 0 then
        //nothing to extract
        deletefile(TARGETDIR + potFilename);
        rmdir(TARGETDIR);
        cd(old)
        return
    end

    // Finalizing the new en_US.po version
    // -----------------------------------
    //add header
    header = ["msgid """"";
    "msgstr """"";
    """Content-Type: text/plain; charset=UTF-8\n""";
    """Content-Transfer-Encoding: 8bit\n""";""];

    potFile = mgetl(TARGETDIR + potFilename);

    // We need C strings format to be used as gettext key as in updateLocalization.sh
    // "" -> \"
    // '' -> '
    potFile = strsubst(potFile, """""", "\""");
    potFile = strsubst(potFile, "''''", "''");
    // previous strsust, introduced `msgstr "` and `msgid "` ; restore double quoted end of line
    potFile = strsubst(potFile, "msgstr \""", "msgstr """"");
    potFile = strsubst(potFile, "msgid \""", "msgid """"");

    // Making location paths relative to the toolbox root
    potFile = strsubst(potFile, "#: "+pathconvert(tbx_path), "#: "); // Call pathconvert to be sure to have a trailing file separator
    if isdef("xmlTmpFile", "l") then
        potFile = strsubst(potFile, "#: "+xmlTmpFile, "#: A-XML-file");
    end

    potFile = [header ; potFile];
    mputl(potFile, TARGETDIR + potFilename);

    // msguniq in case of the modified Scilab script contained the same message in C
    if getos() == "Windows" then
        cmd = WSCI + "\tools\gettext\msguniq"
    else
        cmd = "msguniq"
    end
    cmd = cmd + " --use-first --no-wrap --sort-by-file "
    cmd = cmd + " -o " + TARGETDIR + potFilename + ..
                " " + TARGETDIR + potFilename;
    s = host(cmd);
    potFile = mgetl(TARGETDIR + potFilename);

    if ~isfile(TARGETDIR + "en_US.po") then
        mputl(potFile, TARGETDIR + "en_US.po");
    end

    // Merging former defined *.po files with the new REF one
    // ------------------------------------------------------
    poFiles = findfiles("locales", "*.po")'
    if potFile <> []
        if getos() == "Windows" then
            cmd = WSCI + "\tools\gettext\msgmerge"
        else
            cmd = "msgmerge"
        end
        cmd = cmd + " --no-wrap --sort-by-file --silent" // --silent added to avoid '........ done.' messages in error output
        for f = poFiles
            newPo = TARGETDIR + f
            Cmd = cmd + " -o " + newPo + ..
                        " " + newPo + " " + TARGETDIR + potFilename
            s = host(Cmd);
         end
    end

    cd(old);
    ret = fullfile(tbx_path, TARGETDIR, "en_US.po");
endfunction

function result = sedLoc(str, findExp, replaceExp)
    result = str;
    index = grep(result, findExp, "r");
    while index <> []
        idx = index(1);
        [startPos, endPos, match, captured] = regexp(result(idx), findExp);

        if captured <> [] then
            //multiple matches on the same line
            for i=1:size(captured, "r")
                replace = replaceExp;
                for j = 1:size(captured, "c")
                    replace = strsubst(replace, "\" + string(j), captured(i,j));
                end

                if size(replace, "*") > 1 & (startPos <> 1 | endPos <> length(result(idx))) then
                    //replace partial line by multiline expression
                    replace(1) = part(result(idx), 1:startPos) + " " + replace(1);
                    replace($) = replace($) + " " + part(result(idx), (endPos+1):length(result(idx)));

                    result = [result(1:(idx-1)); replace; result((idx+1):$)];
                elseif size(replace, "*") > 1 then
                    //replace entire line by multiline expression
                    result = [result(1:(idx-1)); replace; result((idx+1):$)];
                else
                    //replace partial line by 1-line expression
                    result(idx) = strsubst(result(idx), match(i), replace);
                end
            end
        end

        //update index with new "file"
        index = grep(result, findExp, "r");
    end
endfunction

function ret = getFilesList(folder, mask)
    if ~isdir(folder) then
        ret = [];
        return;
    end

    old = pwd();
    cd(folder)

    ret = [];

    files = ls();

    for j = 1:size(files, "*")
        if isdir(files(j)) then
            ret = [ret ; getFilesList(files(j), mask)];
        end
    end

    for i = 1:size(mask, "*")
        srcFiles = findfiles(pwd(), "*." + mask(i));
        if srcFiles <> [] then
            ret = [ret ; pwd() + filesep() + srcFiles];
        end
    end

    cd(old);
endfunction
