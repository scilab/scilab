// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Add an URL to the list of repositories, and returns

function repositories = atomsRepositories(level)
	
	rhs           = argn(2);
	repositories  = [];
	
	official_repositories = pathconvert(SCI+"/modules/atoms/etc/repositories",%F);
	allusers_repositories = pathconvert(SCI+"/.atoms/repositories",%F);
	user_repositories     = pathconvert(SCIHOME+"/atoms/repositories",%F);
	
	// official repositories
	// =========================================================================
	
	if (rhs == 0) | ((rhs == 1) & (level == "official")) then
		if fileinfo(official_repositories) <> [] then
			url_list = mgetl(official_repositories);
			for i=1:size(url_list,"*")
				repositories = [ repositories ; url_list(i)  "official" ];
			end
		end
	end
	
	// All users repositories
	// =========================================================================
	
	if (rhs == 0) | ((rhs == 1) & (level == "allusers")) then
		if fileinfo(allusers_repositories) <> [] then
			url_list = mgetl(allusers_repositories);
			for i=1:size(url_list,"*")
				repositories = [ repositories ; url_list(i)  "allusers" ];
			end
		end
	end
	
	// User repositories
	// =========================================================================
	
	if (rhs == 0) | ((rhs == 1) & (level == "user")) then
		if fileinfo(user_repositories) <> [] then
			url_list = mgetl(user_repositories);
			for i=1:size(url_list,"*")
				repositories = [ repositories ; url_list(i)  "user" ];
			end
		end
	end
	
endfunction
