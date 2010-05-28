// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ret=check_versions(varargin)
  [lhs,rhs]=argn(0);
  ret=%T;
  modules=getmodules();
  szmodules=size(modules);

  for i=1:1:szmodules(1)
    file_to_check=SCI+'/modules/'+modules(i)+'/version.xml';
    mprintf(gettext("Checking: %s\n"),modules(i))
    if isfile(file_to_check) then
      try
        if getos() == 'Windows' then
          stat = unix_w(SCI+"\modules\helptools\bin\xmllint\xmllint --noout --valid "+file_to_check);
        else
    	    stat = unix_w("xmllint --noout --valid "+file_to_check);
        end
      catch
        ret=%F;
        mprintf(gettext("%s: Please check: %s.\n"),"check_versions",file_to_check);
      end
    else
	  mprintf(gettext("%s: File doesn''t exist: %s"),"check_versions",file_to_check);
    end
  end
  
endfunction 
 