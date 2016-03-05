// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "atomsDownload", 1));
    end

    if type(file_out) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "atomsDownload", 2));
    end

    if (rhs>2) & (type(md5sum) <> 10) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "atomsDownload", 3));
    end

    // Check input parameters size
    // =========================================================================

    if size(url_in, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsDownload",1));
    end

    if size(file_out, "*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"atomsDownload",2));
    end

    if (rhs>2) & (size(md5sum,"*") <> 1) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"atomsDownload",3));
    end

    // Check input parameters value
    // =========================================================================

    if regexp(url_in,"/^(https?|ftp|file):\/\//","o") <> 1 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: String that starts with ''http(s)?://'',''ftp://'' or ''file://'' expected.\n"),"atomsDownload",1));
    end

    if (rhs>2) & (length(md5sum)<>32) then
        error(msprintf(gettext("%s: Wrong length for input argument #%d: String which has 32-characters length expected.\n"),"atomsDownload",3));
    end

    // curl, wget or httpdownload
    // =========================================================================

    CURL     = %F;
    WGET     = %F;
    HTTPDOWNLOAD = %F;

    // Maybe the detection has already been done

    if atomsGetConfig("downloadTool") == "wget" then
        WGET = %T;
    elseif atomsGetConfig("downloadTool") == "curl" then
        CURL = %T;
    elseif atomsGetConfig("downloadTool") == "httpdownload" & getos() == "Windows" then
        HTTPDOWNLOAD = %T;
    else
        // Default values according to platform
        if LINUX | SOLARIS | BSD then

            // Need to detect under Linux platforms
            [rep, stat, err] = unix_g("wget --version");

            if stat == 0 then
                WGET = %T;
                atomsSetConfig("downloadTool", "wget");
            else
                [rep, stat, err] = unix_g("curl --version");
                if stat == 0 then
                    CURL = %T;
                    atomsSetConfig("downloadTool", "curl");
                else
                    error(msprintf(gettext("%s: Neither Wget or Curl found: Please install one of them\n"), "atomsDownload"));
                end
            end
        elseif MACOSX | getos() == "Windows" then
            CURL = %T;
            atomsSetConfig("downloadTool", "curl");
        end
    end

    // Build the command
    // =========================================================================
    winId = [];

    if regexp(url_in, "/^(https?|ftp):\/\//", "o") == 1 then
        proxy_host_arg = "";
        proxy_user_arg = "";
        timeout_arg  = "";

        // Timeout configuration
        if CURL then
            // Curl
            timeout_arg = " --connect-timeout ";
        else
            // wget
            timeout_arg = " --timeout=";
        end

        timeout = strtod(atomsGetConfig("downloadTimeout"));

        if ~isnan(timeout) then // Value found in config file
            timeout_arg = timeout_arg + string(timeout);
        else // Default timeout
            timeout_arg = timeout_arg + "5";
        end

        timeout_arg = timeout_arg + " ";

        // Proxy configuration
        if (atomsGetConfig("useProxy") == "True") & (atomsGetConfig("proxyHost") <> "") then
            // Host
            proxy_host = atomsGetConfig("proxyHost");

            // Port
            if atomsGetConfig("proxyPort") <> "" then
                proxy_host =  proxy_host + ":" + atomsGetConfig("proxyPort");
            end

            // Host/port Argument
            if CURL then
                // Curl
                proxy_host_arg = " --proxy "+ proxy_host;
            else
                // wget
                proxy_host_arg = " http_proxy="""+proxy_host+""" ";
            end

            // Username/Password
            if and([atomsGetConfig("proxyUser");atomsGetConfig("proxyPassword")]<> "") then
                if CURL then
                    // Curl
                    proxy_user_arg = " --proxy-user "+atomsGetConfig("proxyUser")+":"+atomsGetConfig("proxyPassword");
                else
                    // wget
                    proxy_user_arg = " --proxy-user="""+atomsGetConfig("proxyUser")+""" --proxy-password="""+atomsGetConfig("proxyPassword")+"""";
                end
            end

        end

        if WGET & atomsGetConfig("useProxy") == "False" then
            proxy_user_arg = " --no-proxy";
        end


        if getos() == "Windows" & CURL then
            download_cmd = """" + pathconvert(SCI+"/tools/curl/curl.exe",%F) + """" + proxy_host_arg + proxy_user_arg + timeout_arg + " -s """ + url_in + """ -o """ + file_out + """";
        elseif CURL then
            // curl
            download_cmd = "curl "+proxy_host_arg+proxy_user_arg+timeout_arg+" -s "+url_in + " -o " + file_out;
        else
            // wget
            download_cmd = proxy_host_arg+"wget"+proxy_user_arg+timeout_arg+" "+url_in + " -O " + file_out;
        end

        winId = atomsOpenProgressBar(_("Download in progress... Please be patient."), %f);

        err = [];
        [rep,stat,err] = unix_g(download_cmd);

        // Second try with httpdownload
        if ( HTTPDOWNLOAD | stat<>0 ) & (getos() == "Windows") then
            imode = ilib_verbose();
            ilib_verbose(0) ;
            id  = link(SCI+"/bin/windows_tools.dll","httpdownload","c");
            stat  = call("httpdownload", url_in, 1, "c", file_out, 2, "c", "out", [1,1], 3, "d");
            ulink(id);
            ilib_verbose(imode);

            // Save the parameter to always download with httpdownload
            if stat == 0 then
                atomsSetConfig("downloadTool","httpdownload");
            else
                select stat,
                case -1 then mprintf(gettext("%s: Error: the response status from the URL %s is invalid.\n"), "atomsDownload", url_in),
                case -2 then mprintf(gettext("%s: Error while opening an Internet connection.\n"), "atomsDownload"),
                case -3 then mprintf(gettext("%s: Error while opening the URL %s.\n"), "atomsDownload", url_in),
                case -4 then mprintf(gettext("%s: Error while creating the file %s on disk.\n"), "atomsDownload", file_out),
                case -5 then mprintf(gettext("%s: Error while retrieving the size of file at URL %s.\n"), "atomsDownload", url_in),
                case -6 then mprintf(gettext("%s: Error while reading the file from the URL %s.\n"), "atomsDownload", url_in),
                case -7 then mprintf(gettext("%s: Error while writing the file %s on disk.\n"), "atomsDownload", file_out),
                case -8 then mprintf(gettext("%s: Error while downloading the file from the URL %s.\n"), "atomsDownload", url_in),
                case -9 then mprintf(gettext("%s: Error: out of memory.\n"), "atomsDownload"),
                end
            end
        end

        if stat <> 0 then
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
