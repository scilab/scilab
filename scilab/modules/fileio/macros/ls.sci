// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge STEER
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) INRIA - Sylvestre LEDRU
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function files=ls(varargin)
	
	// interface to ls
	
	opts=[];
	
	if size(varargin)==0 then
		path = "./";
	else
		path = varargin(1);
		for k=2:size(varargin)
			opts = [opts varargin(k)];
		end
	end
	
	if size(opts,'*')<>0 then
		warning(msprintf(gettext("%s: Options ignored.\n"),"ls"));
	end
	
	if or(path==['PWD' 'SCIHOME']) then path=evstr(path),end
	
	if MSDOS then 
    if or(path==['WSCI']) then path=evstr(path),end
  end
  if or(path==['SCI' '~' 'TMPDIR' 'home' ]) path=path+'/',end
	
	// dir returns names without the dirname
	files = listfiles(path);
	
endfunction
