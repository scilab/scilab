function [y]=toolboxes(dir,flag,force) 
// toolboxes loding or compiling 
// if flag == %t then contribs are recompiled 
//    if a loader.sce is not found or if force == %t 
// dir is a directory to explore for contribs
  global %toolboxes
  global %toolboxes_dir
  [lhs,rhs]=argn(0) 
  y=[];
  if rhs == 1 & typeof(dir)=="constant" then 
    // execute the toolbox loader
    y='exec("""+%toolboxes_dir+%toolboxes(dir)+"/"+"loader.sce"+""")";
    return 
  end
    
  if rhs == 0 then 
    dir = SCI+'/contrib' 
  end
  if rhs <= 1 then 
    flag = %f 
  end
  if rhs <= 2 then 
    force = %f 
  end
  cur_wd= getcwd();
  chdir(dir);
  files= listfiles('.');
  contribs=[]
  for k=1:size(files,'*') 
    if fileinfo(files(k)+'/builder.sce')<>[] then 
      contribs=[contribs;files(k)];
    end
  end
  if flag then 
    // recompilation if no loader.sce 
    for k= 1:size(contribs,'*') 
      if fileinfo(contribs(k)+'/loader.sce')==[] | force 
	chdir(contribs(k)) ; 
	exec('builder.sce');
	chdir('../');
      else
	 write(%io(2),contribs(k)+' is already compiled\n');
      end
    end
  end

  if contribs<>[] & grep(sciargs(),"-nw")==[] then 
    delmenu('toolboxes')
    addmenu('toolboxes',contribs);
    // If you also want a build meny 
    //addmenu('build',contribs);
  end
  %toolboxes= contribs;
  %toolboxes_dir= pathconvert(dir);
  chdir(cur_wd);
endfunction

function y=build(i)
  global %toolboxes
  global %toolboxes_dir
  y='p=getcwd();chdir("""+%toolboxes_dir+%toolboxes(i)+""");exec(""builder.sce"");chdir(p)';
endfunction
  
function distrib_clean ()
  global %toolboxes; 
  global %toolboxes_dir;
  // 
  if %toolboxes==[] then 
    find_contribs('./');
  end
  contribs=%toolboxes;
  for k= 1:size(contribs,'*') 
    flag = fileinfo(contribs(k)+'/loader.sce')<>[] 
    flag = flag & fileinfo(contribs(k)+'/Makefile')<>[] 
    if flag then 
      chdir(contribs(k)) ;
      mprintf('Cleaning %s\n',contribs(k))
      execstr('unix_s(''make distclean'')','errcatch');
      chdir('../');
    end
  end
endfunction
  
function []=distrib_zip (op,name)
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
  if %toolboxes==[] then 
    find_contribs('./');
  end
  contribs=%toolboxes;
  for k= 1:size(contribs,'*') 
    flag = fileinfo(contribs(k)+'/loader.sce')<>[] 
    job="find . \( -name ''*.o'' -o -name ''*.la'' -o -name ''*.lo'' -o"+...
	" -name ''*.obj'' -name ''*.a'' \) -exec \rm -f {} \;" 
    if flag then 
      write(%io(2),'build archive for '+ contribs(k));
      chdir(contribs(k)) ;
      execstr('unix_s("""+job+""")','errcatch');
      chdir('../');
      select op 
       case 'tar' then 
	unix_s('tar cfz '+contribs(k)+'-bin-'+name+'-`date -I`'+'.tgz '+...
	       contribs(k));
       case 'zip' then 
	unix_s('zip -r '+contribs(k)+' '+contribs(k)+'-bin-'+name+'-`date -I`'+'.zip');
      end
    end
  end
endfunction


    
  
  
