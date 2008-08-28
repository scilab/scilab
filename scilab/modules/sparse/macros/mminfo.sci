// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA Serge Steer <Serge.Steer@inria.fr>
// ...
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [rows,cols,entries,rep,field,symm,comments] = mminfo(filename)
// 
//  function  [rows, cols, entries, rep, field, symmetry] = mminfo(filename)
// 
//      Reads the contents of the Matrix Market file ''filename''
//      and extracts size and storage information.
// 
//      In the case of coordinate matrices, entries refers to the
//      number of coordinate entries stored in the file.  The number
//      of non-zero entries in the final matrix cannot be determined
//      until the data is read (and symmetrized, if necessary).
// 
//      In the case of array matrices, entries is the product
//      rows*cols, regardless of whether symmetry was used to
//      store the matrix efficiently.
// 
//Author S. Steer, Copyright INRIA
  if type(filename)==10 then 
    mmfile = mopen(filename,"r");
  else
    mmfile=filename,
  end
  header = mgetl(mmfile,1);
  if header==[] then error("Empty file."),end;

  T=tokens(header)
  if size(T,'*')<5 then
    error('Invalid header line. It should be :%%MatrixMarket matrix representation field symmetry')
  end;
  head0=T(1)
  head1=convstr(T(2),'l')
  rep=convstr(T(3),'l')
  field=convstr(T(4),'l')
  symm=convstr(T(5),'l')

  if head0<>"%%MatrixMarket" then
    error("Not a valid MatrixMarket header.")
  end;
  if head1<>"matrix" then
    error("MatrixMarket "+head1+" file not yet handled")
  end;

  // Read through comments, ignoring them
  comments=[]
  Line = mgetl(mmfile,1);
  while Line<>[]&part(Line,1)=='%' 
     comments=[comments;Line]
    Line = mgetl(mmfile,1);
  end;

  // Read size information, then branch according to
  // sparse or dense format

  if rep=="coordinate" then ninfo=3,else  ninfo=2,end
  
  fmt=strcat('%d'+emptystr(1,ninfo))
  //  read matrix given in sparse coordinate matrix format
  sizeinfo = msscanf(Line,fmt);count=size(sizeinfo,'*')
  while count==0
    Line = mgetl(mmfile,1);
    if Line==[] then error("End-of-file reached before size information was found."),end
    sizeinfo = msscanf(Line,fmt);count=size(sizeinfo,'*')

  end;
  if count<>ninfo then error("Invalid size specification line."),end

  rows = sizeinfo(1);
  cols = sizeinfo(2);
  if ninfo==3 then //sparse
    entries = sizeinfo(3);
  else
    entries = rows*cols;
  end
    
  if type(filename)==10 then mclose(mmfile);end
  if argn(1)==1&type(filename)==10 then
    mprintf("%s\n",["=============================================================="
		    "Information about MatrixMarket file :"+filename;
		    ""
		    comments
		    "storage: "+rep
		    "entry type: "+field
		    "symmetry: "+symm
		   "=============================================================="])
  end
  // Done.
endfunction
