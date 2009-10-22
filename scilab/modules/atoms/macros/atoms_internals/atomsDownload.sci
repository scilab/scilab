// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function atomsDownload(url_in,file_out,md5sum)
	
	// Operating system detection
	// =========================================================================
	
	if ~MSDOS then
		OSNAME = unix_g('uname');
		MACOSX = (strcmpi(OSNAME,"darwin") == 0);
		LINUX  = (strcmpi(OSNAME,"linux") == 0);
	else
		MACOSX = %F;
		LINUX  = %F;
	end
	
	// Check input parameters number
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs < 2 | rhs > 3 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"atomsDownload",2,3));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(url_in) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDownload",1));
	end
	
	if type(file_out) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDownload",2));
	end
	
	if (rhs>2) & (type(md5sum) <> 10) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDownload",3));
	end
	
	// Check input parameters size
	// =========================================================================
	
	if size(url_in,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDownload",1));
	end
	
	if size(file_out,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsDownload",2));
	end
	
	if (rhs>2) & (size(md5sum,"*") <> 1) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsDownload",3));
	end
	
	// Check input parameters value
	// =========================================================================
	
	if regexp(url_in,"/^(http|ftp|file):\/\//","o") <> 1 then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A string that start with ''http://'',''ftp://'' or ''file://'' expected.\n"),"atomsDownload",1));
	end
	
	if (rhs>2) & (length(md5sum)<>32) then
		error(msprintf(gettext("%s: Wrong length for input argument #%d: A string which has 32-characters length expected.\n"),"atomsDownload",3));
	end
	
	// Build the command
	// =========================================================================
	
	if regexp(url_in,"/^(http|ftp):\/\//","o") == 1 then
		
		proxy_host_arg = "";
		proxy_user_arg = "";
		timeout_arg    = "";
		
		// Timeout configuration
		
		if MSDOS | MACOSX then
			// Curl
			timeout_arg = " --connect-timeout ";
		else
			// wget
			timeout_arg = " --timeout=";
		end
		
		timeout = string(strtod(atomsGetConfig("timeout")));
		
		if timeout<> "0" then
			timeout_arg = timeout_arg + timeout;
		else
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
			if MSDOS | MACOSX then
				// Curl
				proxy_host_arg = " --proxy "+ proxy_host;
			else
				// wget
				proxy_host_arg = " http_proxy="""+proxy_host+""" ";
			end
			
			// Username/Password
			if and([atomsGetConfig("proxyUser");atomsGetConfig("proxyPassword")]<> "") then
				if MSDOS | MACOSX then
					// Curl
					proxy_user_arg = " --proxy-user "+atomsGetConfig("proxyUser")+":"+atomsGetConfig("proxyPassword");
				else
					// wget
					proxy_user_arg = " --proxy-user="""+atomsGetConfig("proxyUser")+""" --proxy-password="""+atomsGetConfig("proxyPassword")+"""";
				end
			end
			
		end
		
		if MSDOS then
			download_cmd = """" + pathconvert(SCI+"/tools/curl/curl.exe",%F)+""""+proxy_host_arg+proxy_user_arg+timeout_arg+" -s "+url_in + " -o " + file_out;
		elseif MACOSX then
			download_cmd = "curl "+proxy_host_arg+proxy_user_arg+timeout_arg+" -s "+url_in + " -o " + file_out;
		else
			download_cmd = proxy_host_arg+"wget"+proxy_user_arg+timeout_arg+" "+url_in + " -O " + file_out;
		end
		
		[rep,stat,err] = unix_g(download_cmd);
		
		if stat <> 0 then
			mprintf(gettext("%s: The following file hasn''t been downloaded:\n"),"atomsDownload");
			mprintf(gettext("\t - URL            : ''%s''\n"),url_in);
			mprintf(gettext("\t - Local location : ''%s''\n"),file_out);
			error("");
		end
	
	elseif regexp(url_in,"/^file:\/\//","o") == 1 then
		
		if MSDOS then
			url_pattern = "file:///";
		else
			url_pattern = "file://";
		end
		
		file_in = pathconvert(part(url_in,length(url_pattern):length(url_in)),%F);
		
		if copyfile(file_in,file_out) <> 1 then
			mprintf(gettext("%s: The following file hasn''t been copied:\n"),"atomsDownload");
			mprintf(gettext("\t - source      : ''%s''\n"),file_in);
			mprintf(gettext("\t - destination : ''%s''\n"),file_out);
			error("");
		end
		
	end
	
	// If md5sum is gived, check the md5sum of the downloaded file
	// =========================================================================
	
	if rhs>2 then
		
		filemd5 = getmd5(file_out);
		
		if filemd5 <> md5sum then
			mprintf(gettext("%s: The downloaded file doesn''t check the MD5SUM:\n"),"atomsDownload");
			mprintf(gettext("\t - file            : ''%s''\n"),file_out);
			mprintf(gettext("\t - MD5SUM expected : ''%s''\n"),md5sum);
			mprintf(gettext("\t - MD5SUM watched  : ''%s''\n"),filemd5);
			error("");
		end
		
	end
	
endfunction
