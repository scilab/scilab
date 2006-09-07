function genlib(nam,path,force,verbose,names)
// get all .sci files in the specified directory
// Copyright Inria/Enpc 
  
  if exists('force','local')==0 then force = %f,end
  if exists('verbose','local')==0 then verbose = %f,end
  
  //
  W=who('get');
  np=predef();
  predefined=or(W($-np+1:$)==nam);
  
  updatelib=%f //true if updating an already defined library
  //check if a library with the same name exists
  oldlib=[];old_path=[];old_names=[];
  if exists(nam)==1 then 
    execstr('oldlib='+nam)
    if typeof(oldlib)=='library' then
      //yes, get its path and function list
      old_names=string(oldlib);clear oldlib
      old_path=old_names(1);old_names(1)=[];
    end
  end
  if exists('path','local')==0&old_path<>[] then
    path=old_path
    updatelib=%t
  end
  // convert path according to MSDOS flag 
  // without env variable substitution
  path1 = pathconvert(path,%t,%f); 
  // with env subsitution 
  path = pathconvert(path,%t,%t); 
  if exists('names','local')==0 then 
    // list the sci files 
    if part(getos(),1:6)=='CYGWIN' then
      // ls *.sci requires less resources than ls path/*.sci
      files=path+listfiles('*.sci',%f)
    else
       files=listfiles(path+'*.sci',%f)
    end
    if files==[] | files== "" then 
      error('I cannot find any sci files in '+path);
      return ;
    end
    names = basename(files,%f);
  else
    files = path+names
    names=strsubst(names,'.sci','')
  end
  
  names_changed=%t
  if updatelib then
    if and(sort(names)==sort(old_names)) then names_changed=%f,end
  end

  modified=%f

  if force == %t then 
    modified = %t;
    for i=1:size(files,'*')  // loop on .sci files
      scif = files(i); 
      if verbose then 
	write(%io(2),' '+names(i) + '.sci compilation forced');
      end
      // getf sci file and save functions it defines as a .bin file
      getsave(scif);
    end  
  else
     for i=1:size(files,'*')  // loop on .sci files
       scif = files(i); 
       binf = strsubst(scif,'.sci','.bin')
       binf_info = fileinfo(binf); 
       recompile = %f ; 
       if binf_info == [] then 
	 recompile = %t ;
       else 
	  scif_info = fileinfo(scif); 
	  if ( scif_info(6) > binf_info(6)) then 
	    recompile = %t ; 
	  end
       end
       if recompile == %t then 
	 if verbose then 
	   write(%io(2),'Processing file '+names(i) + '.sci');
	 end
	 // getf sci file and save functions it defines as a .bin file
	 getsave(scif);
	 modified = %t;
       end  
     end
  end
  
  if modified then 
    if verbose then write(%io(2),'Regenerate names and lib');end
    if names_changed
      mputl(names,path+'names') // write 'names' file in directory 
    end
    // create library
    execstr(nam+'=lib('''+path1+''')')
    //save it
    if execstr('save('''+path1+'lib'''+','+nam+')','errcatch')<>0 then
      error(path+'lib file cannot be created')
    end
  else
     execstr(nam+'=lib('''+path1+''')')
  end
  if names_changed
    if ~predefined then
      execstr(nam+'=resume('+nam+')')
    else
      write(%io(2),[ '   Library file '+path1+'lib'+ ' has been updated, '
		     '   but cannot be loaded into Scilab,'
		     '   because '+nam+' is a protected variable.']);
    end
  end
endfunction


function getsave(fl)
// utility function 
// performs a getf on file fl 

  prot=funcprot();funcprot(0)
  nold=size(who('get'),'*')

//  ierr=execstr('getf(fl)','errcatch') // get functions defined in file 'fl'
  ierr=exec(fl,'errcatch',-1)
  if ierr<>0 then
  	clear ierr
    mprintf('  Warning: Error in file '+fl+' :\n    ""'+lasterror()+'""\n"+...
	    '  file ignored')
  else
    clear ierr
    // lookfor names of the functions defined in file 'fl'
    new=who('get')
    new=new(1:(size(new,'*')-nold-1))
    // create output file name
    fl=strsubst(fl,'.sci','.bin')
    // save all functions in the output file
    [u,ierr]=mopen(fl,'wb')
    if ierr<>0 then 
      clear ierr
      nf=length(fl)
      if nf>40 then fl='...'+part(fl,nf-40:nf),end
      error('Impossible to open file '+fl+' for writing, ')
    end
    clear ierr
    if new<>[] then 
      execstr('save(u,'+strcat(new($:-1:1),',')+')'); 
    else 
      mprintf('  function ""'+fl+'"" does not contain any functions');
    end
    mclose(u)
  end
  funcprot(prot)
endfunction
