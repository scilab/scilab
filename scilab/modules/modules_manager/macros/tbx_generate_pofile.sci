// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function ret=tbx_generate_pofile(tbx_name, tbx_path)

    rhs = argn(2);

    if ~or(rhs == [1,2]) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"),"tbx_generate_pofile", 1, 2));
    end

    if rhs == 1 then
        tbx_path = pwd();
    end

    if type(tbx_name) <> 10 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "tbx_generate_pofile", 1));
    end

    if type(tbx_path) <> 10 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "tbx_generate_pofile", 1));
    end

    if ~isdir(tbx_path) then
        error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"), "tbx_generate_pofile", tbx_path));
    end

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
            //multiple matches on the same line, YOUHOU !
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

