function loadmatfile(fil)
//loads all the variables in a matlab 5 binary file into Scilab
//
//This function has been developped following the "MAT-File Format" description:
//www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf 

//Copyright INRIA
//Author Serge Steer    
  ReadmiMatrix; //load the matfile functions 

  //--file opening
  fd=open_matfile(fil)
 
  //--read the file header
  [head,version,endian]=matfile_header(fd)
  
  //--set constants
  exec(LoadMatConstants,-1);

  //--loop on the stored variables
  Name='';Names=[];Matrices=list()
  while %t 
    ierr=execstr('[Matrix,Name]=ReadmiMatrix(fd)','errcatch') //read next variable
    if ierr<>0 then mclose(fd),disp(lasterror()),return,end 
    if meof(fd) then  break,end //EOF reached 
    Names=[Names,Name];Matrices($+1)=Matrix
  end
  //--file closing
  mclose(fd)
  
  //return variables in the calling context
  execstr('['+strcat(Names,',')+']=resume(Matrices(:))')
endfunction

