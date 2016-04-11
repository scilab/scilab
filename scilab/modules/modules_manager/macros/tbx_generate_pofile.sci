// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
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
    XGETTEXT_OPTIONS=" --omit-header -k --keyword=dgettext:2 --keyword=xmlgettext:2 --keyword=_d:2 --language=python ";

    EXTENSIONS=["c" "h" "cpp" "cxx" "hxx" "hpp" "java"];
    EXTENSIONS_MACROS=["sci" "sce" "start" "quit"];
    EXTENSIONS_XML=["xml" "xsl"];
    TARGETDIR="locales";

    mkdir(TARGETDIR);
    srcFiles = getFilesList("src", EXTENSIONS);
    srcFiles = [srcFiles ; getFilesList("sci_gateway", EXTENSIONS)];
    srcFiles = [srcFiles ; getFilesList("macros", EXTENSIONS_MACROS)];
    srcFiles = [srcFiles ; getFilesList("etc", EXTENSIONS_MACROS)];

    //manage xml preferences files
    xmlFiles = getFilesList("etc", EXTENSIONS_XML);

    if size(xmlFiles, "*") > 0 then
        xmlTmpFile = TMPDIR + "/tmpLoc.xml";
        srcFiles = [srcFiles; xmlTmpFile];
        xmlFake = mopen(xmlTmpFile, "w");
        for i = 1:size(xmlFiles, "*")
            content = mgetl(xmlFiles(i));
            newLine = sedLoc(content, "/\""_d\(\s*(.*)\s*,\s*(.*)\s*\)\""/", "xmlgettext(""\1"", ""\2"")");// "_d(xxx,xxx)"
            newLine = sedLoc(newLine, "/\""dgettext\(\s*(.*)\s*,\s*(.*)\s*\)\""/", "xmlgettext(""\1"", ""\2"")");
            mputl(newLine, xmlFake);
        end
        mclose(xmlFake);
    end

    //parse all files
    srcFiles = strcat(srcFiles, " ");
    cmd = XGETTEXT + XGETTEXT_OPTIONS + " -d " + tbx_name + " " + srcFiles + " -p " + TARGETDIR + " -o " + "en_US.po.tmp";
    host(cmd);

    if exists("xmlTmpFile") then
        deletefile(xmlTmpFile);
    end

    fi = fileinfo(TARGETDIR + "/en_US.po.tmp");
    if fi == [] | fi(1) == 0 then
        //nothing to extract
        deletefile(TARGETDIR + "/en_US.po.tmp");
        rmdir(TARGETDIR);
        cd(old);
        ret = [];
        return;
    end

    //add header
    header = ["msgid """"";
    "msgstr """"";
    """Content-Type: text/plain; charset=UTF-8\n""";
    """Content-Transfer-Encoding: 8bit\n""";""];

    poFile = mgetl(TARGETDIR + "/en_US.po.tmp");
    poFile = [header ; poFile];
    mputl(poFile, TARGETDIR + "/en_US.po");
    deletefile(TARGETDIR + "/en_US.po.tmp");

    cd(old);
    ret = tbx_path + filesep() + TARGETDIR + filesep() + "en_US.po";
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

