function matfile2sci(mat_file_path,result_file_path)
// Translate a Matlab 5 MAT file into a Scilab file
// 
// mat_file_path : path of the Matlab MAT file
// result_file_path : path of the generated Scilab file  
//
//This function has been developped following the "MAT-File Format" description:
//www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf 

//Copyright INRIA
//Author Serge Steer  
  

  ReadmiMatrix=ReadmiMatrix; //load the matfile functions 

  //--file opening
  fdi=open_matfile(mat_file_path)
  fdo=mopen(result_file_path,'w') 
  
  //--read the file header
  [head,version,endian]=matfile_header(fdi)
  
  //--set constants
  exec(LoadMatConstants,-1);

  //--loop on the stored variables
  while %t 
    //[Matrix,Name]=ReadmiMatrix(fdi) //read next variable
    ierr=execstr('[Matrix,Name]=ReadmiMatrix(fdi)','errcatch') //read next variable
    if ierr<>0 then mclose(fdi),mclose(fdo),return,end 
    if meof(fdi) then  break,end //EOF reached 
    execstr(Name +'= Matrix; save(fdo,'+Name+')')
  end
  //--file closing
  mclose(fdi);mclose(fdo)
endfunction

