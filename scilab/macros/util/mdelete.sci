function mdelete(filename)
	
	// Copyright INRIA
	// Scilab Project - V. Couvert
	// Delete a file given by filename (or a set of files if meta-characters used in filename)
	
	// Modified by Pierre MARECHAL
	// Scilab Team
	// Date : 2006-06-29
	// Fix the bug2041
	
	// Date : 2007-01-16
	// Fix the bug2288 (Thanks to M. Dubois, http://dubois.ensae.net )
	
	rhs=argn(2);
	if rhs<>1 then
		error(77);
	end
	
	if type(filename)<>10 | size(filename,"*")<>1 then
		error("Wrong type argument, expecting a character string");
	end
	
	// Handle file path
	if MSDOS then
		filename=strsubst(filename,"\","/");
	end
	
	// File path
	k=strindex(filename,"/");
	
	if k==[] then
		file_path="./";
	else
		file_path = part(filename,1:k($));
		filename  = part(filename,k($)+1:length(filename));
	end
	
	if MSDOS then
		fullfilename = strsubst(file_path+filename,"/","\");
		lst_files    = listfiles(fullfilename);
		if lst_files<>[] then
			cmd="del """+fullfilename+"""";
			unix(cmd);
		end
	else
		lst_files=listfiles(file_path+filename);
		if lst_files<>[] then
			cmd="rm -f """+file_path+filename+"""";
			unix(cmd);
		end
	end
	
endfunction
