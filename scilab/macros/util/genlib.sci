function genlib(nam,path,force,verbose)
// get all .sci files in the specified directory
// Copyright Inria/Enpc 
  
  [lhs,rhs]=argn(0); 
  if rhs <= 2 then force = %f ; end 
  if rhs <= 3 then verbose = %f ; end 
    
  // convert path according to MSDOS flag 
  // without env variable substitution
  path1 = pathconvert(path,%t,%f); 
  // with env subsitution 
  path = pathconvert(path,%t,%t); 


  // list the sci files 
  files=listfiles(path+'*.sci',%f)

  if files==[] | files== "" then 
    error('I cannot find any sci files in '+path);
    return ;
  end
  
  names = basename(files,%f);
  
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
	   write(%io(2),' '+names(i) + '.sci must be recompiled');
	 end
	 // getf sci file and save functions it defines as a .bin file
	 getsave(scif);
	 modified = %t;
       end  
     end
  end
  
  if modified then 
    if verbose then 
      write(%io(2),' regenerate names and lib');
    end
    // write 'names' file in directory given by path
    u=file('open',path+'names','unknown');
    write(u,names,'(a)');file('close',u)
    // create library
    execstr(nam+'=lib('''+path1+''')')
    [u,ierr]=mopen(path+'lib','wb')
    // save library in directory given by path
    if ierr<>0 then 
      error(path+'lib file cannot be opened for writing')
    end
    execstr('save(u,'+nam+')');mclose(u)
  else
     execstr(nam+'=lib('''+path1+''')')
  end
  execstr(nam+'=resume('+nam+')')
endfunction


function getsave(fl)
// utility function 
// performs a getf on file fl 

  prot=funcprot();funcprot(0)
  nold=size(who('get'),'*')

  ierr=execstr('getf(fl)','errcatch') // get functions defined in file 'fl'
  if ierr<>0 then
    warning(['Error in file '+fl+' :""'+lasterror()+'""; file ignored'])
  else
    // lookfor names of the functions defined in file 'fl'
    new=who('get')
    new=new(1:(size(new,'*')-nold-1))
    // create output file name
    fl=strsubst(fl,'.sci','.bin')
    // save all functions in the output file
    [u,ierr]=mopen(fl,'wb')
    if ierr<>0 then 
      error('Impossible to open file '+fl+' for writing, ')
    end
    if new<>[] then 
      execstr('save(u,'+strcat(new($:-1:1),',')+')'); 
    else 
      write(%io(2),'function ""'+fl+'"" does not contain any functions');
    end
    mclose(u)
  end
  funcprot(prot)
endfunction
