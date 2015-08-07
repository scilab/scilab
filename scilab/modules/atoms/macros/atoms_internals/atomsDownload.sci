// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Internal function

function atomsDownload(url_in,file_out,md5sum)

    // Operating system detection + Architecture detection
    // =========================================================================
    [OSNAME, ARCH, LINUX, MACOSX, SOLARIS,BSD] = atomsGetPlatform();

    // Check input parameters number
    // =========================================================================
    rhs  = argn(2);

    if rhs < 2 | rhs > 3 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"), "atomsDownload", 2, 3));
    end

    // Check input parameters type
    // =========================================================================
    if type(url_in) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"), "atomsDownload", 1));
    end

    if type(file_out) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"), "atomsDownload", 2));
    end

    if (rhs>2) & (type(md5sum) <> 10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"), "atomsDownload", 3));
    end

    // Check input parameters size
    // =========================================================================

    if size(url_in, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDownload",1));
    end

    if size(file_out, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDownload",2));
    end

    if (rhs>2) & (size(md5sum,"*") <> 1) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDownload",3));
    end

    // Check input parameters value
    // =========================================================================

    if regexp(url_in,"/^(https?|ftp|file):\/\//","o") <> 1 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: String that starts with ''http(s)?://'',''ftp://'' or ''file://'' expected.\n"),"atomsDownload",1));
    end

    if (rhs>2) & (length(md5sum)<>32) then
        error(msprintf(gettext("%s: Wrong length for input argument #%d: String which has 32-characters length expected.\n"),"atomsDownload",3));
    end
   
    if regexp(url_in, "/^(http|ftp):\/\//", "o") == 1 then 
        winId = atomsOpenProgressBar(_("Download in progress... Please be patient."), %f);
	ret = getURL(url_in,file_out);
        if ret <> file_out then
            mprintf(gettext("%s: The following file hasn''t been downloaded:\n"), "atomsDownload");
            mprintf(gettext("\t - URL      : ''%s''\n"), url_in);
            mprintf(gettext("\t - Local location : ''%s''\n"), file_out);
            if isdef("err") then
                atomsCloseProgressBar(winId);
                error(strcat(err, ascii(10)));
            end
        end

    elseif regexp(url_in,"/^file:\/\//","o") == 1 then

        if getos() == "Windows" then
            url_pattern = "file:///";
        else
            url_pattern = "file://";
        end

        file_in = pathconvert(part(url_in,length(url_pattern)+1:length(url_in)),%F);

        if copyfile(file_in,file_out) <> 1 then
            mprintf(gettext("%s: The following file has not been copied:\n"),"atomsDownload");
            mprintf(gettext("\t - source    : ''%s''\n"),file_in);
            mprintf(gettext("\t - destination : ''%s''\n"),file_out);
            atomsCloseProgressBar(winId);
            error(strcat(lasterror(), ascii(10)));
        end

    end

    // If md5sum is gived, check the md5sum of the downloaded file
    // =========================================================================

    if rhs>2 then

        filemd5 = getmd5(file_out);

        if filemd5 <> md5sum then
            mprintf(gettext("%s: The downloaded file does not match the MD5SUM:\n"),"atomsDownload");
            mprintf(gettext("\t - file      : ''%s''\n"),file_out);
            mprintf(gettext("\t - MD5SUM expected : ''%s''\n"),md5sum);
            mprintf(gettext("\t - MD5SUM watched  : ''%s''\n"),filemd5);
            atomsCloseProgressBar(winId);
            error("");
        end

    end

    // Close progress bar handle, if not closed yet
    // =========================================================================
    if (~isempty(winId))
        atomsCloseProgressBar(winId);
    end
endfunction
