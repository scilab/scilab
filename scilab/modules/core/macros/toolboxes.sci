// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//===========================================================
function [y]=toolboxes(path,flag,force) 
// toolboxes loading or compiling 
// if flag == %t then contribs are recompiled 
// if a loader.sce is not found or if force == %t 
// path is a directory to explore for contribs
  global %toolboxes
  global %toolboxes_dir
//===========================================================
// functions build, distrib_clean, distrib_zip only used 
// by toolboxes macro
//===========================================================
function y = build(i)
  global %toolboxes
  global %toolboxes_dir
  y = 'p=pwd();chdir("""+%toolboxes_dir+%toolboxes(i)+""");exec(""builder.sce"");chdir(p)';
endfunction
//===========================================================
function distrib_clean ()
  global %toolboxes; 
  global %toolboxes_dir;
  // 
  if %toolboxes == [] then 
    find_contribs('./');
  end
  contribs = %toolboxes;
  for k = 1:size(contribs,'*') 
    flag = fileinfo(contribs(k)+'/loader.sce')<>[] ;
    flag = flag & fileinfo(contribs(k)+'/Makefile')<>[] ;
    if flag then 
      chdir(contribs(k)) ;
      mprintf('Cleaning %s\n',contribs(k))
      execstr('unix_s(''make distclean'')','errcatch');
      chdir('../');
    end
  end
endfunction
//===========================================================  
function [] = distrib_zip (op,name)
  global %toolboxes; 
  global %toolboxes_dir;
  // 
  [lhs,rhs]=argn(0) 
  if rhs <= 1 then 
    name = 'win' 
  end 
  if rhs <= 0 then 
    op = 'zip' 
  end 
  if %toolboxes == [] then 
    find_contribs('./');
  end
  contribs = %toolboxes;
  for k = 1:size(contribs,'*') 
    flag = fileinfo(contribs(k)+'/loader.sce')<>[] ;
    job = "find . \( -name ''*.o'' -o -name ''*.la'' -o -name ''*.lo'' -o"+ ..
	  " -name ''*.obj'' -name ''*.a'' \) -exec \rm -f {} \;" 
    if flag then 
      write(%io(2),'build archive for '+ contribs(k));
      chdir(contribs(k)) ;
      execstr('unix_s("""+job+""")','errcatch');
      chdir('../');
      select op 
       case 'tar' then 
	unix_s('tar cfz '+contribs(k)+'-bin-'+name+'-`date -I`'+'.tgz '+ ..
	       contribs(k));
       case 'zip' then 
	unix_s('zip -r '+contribs(k)+' '+contribs(k)+'-bin-'+name+'-`date -I`'+'.zip');
      end
    end
  end
endfunction
//===========================================================
  
  [lhs,rhs] = argn(0) 
  y = [];
  if rhs == 1 & typeof(path)=="constant" then 
    // execute the toolbox loader
    y='exec("""+%toolboxes_dir+%toolboxes(path)+filesep()+"loader.sce"+""");';
    return 
  end
    
  if rhs == 0 then 
    path = SCI+'/contrib' 
  end
  if rhs <= 1 then 
    flag = %f 
  end
  
  if rhs <= 2 then 
    force = %f 
  end
  
  cur_wd = pwd();
  chdir(path);
  files = listfiles('.');
  contribs = [];
  for k = 1:size(files,'*') 
    if fileinfo(files(k)+'/loader.sce')<>[] then 
      contribs = [contribs;files(k)];
    end
  end
  if flag then 
    // recompilation if no loader.sce 
    for k = 1:size(contribs,'*') 
      if fileinfo(contribs(k)+'/loader.sce')==[] | force 
	      chdir(contribs(k)) ; 
	      exec('builder.sce');
	      chdir('../');
      else
	      write(%io(2),contribs(k)+gettext(" is already compiled.\n"));
      end
    end
  end

  if contribs<>[] & grep(sciargs(),"-nw")==[] then 
  	if ( ~fromjava() & ~fromc() ) then
    	delmenu(gettext("&Toolboxes"));
    	h = uimenu("parent", 0, "label", gettext("&Toolboxes"));
    	for k=1:size(contribs,'*')
    	 m = uimenu(h,'label', string(contribs(k)), 'callback','execstr(toolboxes('+string(k)+'))');
    	end
    	unsetmenu(gettext("&Toolboxes"));
    end
  end
  
  %toolboxes = contribs;
  %toolboxes_dir = pathconvert(path);
  chdir(cur_wd);
endfunction
//===========================================================

