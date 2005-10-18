function genmoc(path,force,verbose)
// make moc files in the specified directory
// Copyright Inria
  
  [lhs,rhs]=argn(0); 
  if rhs <= 2 then force = %f ; end 
  if rhs <= 3 then verbose = %f ; end 
  if ~with_ocaml() then
    error('Scilab has not been built with Ocaml, Modelica compiler unavailable')
  end
  if MSDOS then
    compilerpath=pathconvert(SCI+'/bin/modelicac.exe',%f,%t)
  else
    compilerpath=pathconvert(SCI+'/bin/modelicac',%f,%t)
  end

  // with env subsitution 
  path = pathconvert(path,%t,%t); 


  // list the mo files 
  files=listfiles(path+'*.mo',%f)

  if files==[] | files== "" then 
    error('I cannot find any mo files in '+path);
    return ;
  end
  
  names = basename(files,%f);
  
  if force == %t then 
    for i=1:size(files,'*')  // loop on .mo files
      mof = files(i); 
      if verbose then 
	write(%io(2),' '+names(i) + '.mo compilation forced');
      end
      mocf = strsubst(mof,'.mo','.moc')
      unix_s(compilerpath+' -c '+mof+' -o '+mocf)
    end  
  else
     for i=1:size(files,'*')  // loop on .mo files
       mof = files(i); 
       mocf = strsubst(mof,'.mo','.moc')
       mocf_info = fileinfo(mocf); 
       recompile = %f ; 
       if mocf_info == [] then 
	 recompile = %t ;
       else 
	  mof_info = fileinfo(mof); 
	  if ( mof_info(6) > mocf_info(6)) then 
	    recompile = %t ; 
	  end
       end
       if recompile == %t then 
	 if verbose then 
	   write(%io(2),' '+names(i) + '.mo must be recompiled');
	 end
	 if execstr('unix_s(compilerpath+'' -c ''+mof+'' -o ''+mocf)','errcatch')<>0 then
	   write(%io(2),' '+names(i) + '.mo cannot be compiled');
	end
       end  
     end
  end
  
  // list the c files (extern functions)
  files=listfiles(path+'*.c',%f)
  names = basename(files,%f);
  if files==[] | files== "" then 
    return ;
  end
  libname='lib'+basename(part(path,1:length(path)-1))
  make=ilib_gen_Make(libname,['',''],names,[],path+'Makelib',%f,'','','') 
  make=strsubst(make,'.mak','')
  if execstr('ilib_compile(libname,make,names);','errcatch')<>0 then
    write(%io(2),'Problem while building library '+libname)
  end
 
endfunction

