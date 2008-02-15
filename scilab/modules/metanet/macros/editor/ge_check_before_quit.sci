
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_check_before_quit()
//Used by scilab.quit to ask user for saving
//look for running edit_graph windows
  L=who('global');
  L=L(part(L,1:7)=='EGdata_')
  ws=strsubst(L,'EGdata_','')
  for k=1:size(ws,'*')
    ierr=execstr('win=ws(k)','errcatch')
    if ierr==0 then
      
      w=string(win)
      execstr(['global EGdata_'+w
	       'edited=EGdata_'+w+'.Edited;'
	       'GraphList=EGdata_'+w+'.GraphList'
	       'path=EGdata_'+w+'.Path'])
      if edited then
	r=x_message(['graph '+GraphList.name+' is modified'
		     'Do you want to save it?'],['yes','no'])
	if r==1 then
	  x_message('Use the Graph/Save menu to save it')
	  while edited
	    xpause(100000)
	    execstr(['global EGdata_'+w;
		     'if type(EGdata_'+w+')<>16 then'
		     '   edited=%f'
		     'else'
		     '   edited=EGdata_'+w+'.Edited;'
		     'end'])
	  end
	end
      end
    end
  end
endfunction

