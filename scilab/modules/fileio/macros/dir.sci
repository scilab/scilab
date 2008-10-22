// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function L=dir(str)
	
	mask     = int32(61440);
	dirtype  = 16384;
	
	if argn(2)==1 then
	
    if type(str) <> 10 then 
      error(999,msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"),'dir',1));
    end 

		files    = listfiles(str);
		basepath = str;
	else
		files    = listfiles();
		basepath = "";
	end
	
	n = size(files,"*");
	
	if n==0 then
	
		files = [];
		dt    = [];
		bytes = [];
		isd   = [];
		
	else
		
		files    = gsort(files,"lr","i");
		dt       = zeros(n,1);
		bytes    = zeros(n,1);
		isd(n,1) = %f;
		
		for k=1:n
			if (basepath == '') then
				[x,ierr] = fileinfo(files(k));
			else
				[x,ierr] = fileinfo(basepath+"/"+files(k));
			end
		
			if ( (x == []) & (ierr== -1) ) then
				[x,ierr] = fileinfo(files(k));
			end
			
			if x<>[] then
				dt(k)    = x(6);
				bytes(k) = x(1);
				isd(k)   = (double(int32(x(2)) & mask) == dirtype);
			end
		
		end
		
	end
	
	L = tlist(['dir','name','date','bytes','isdir'],files,dt,bytes,isd);
	
endfunction
