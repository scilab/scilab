// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA Serge Steer <Serge.Steer@inria.fr>
// ...
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function [A,rows,cols,entries,rep,field,symm] = mmread(filename)
// function  [A] = mmread(filename)
// 
// function  [A,rows,cols,entries,rep,field,symm] = mmread(filename)
// 
//      Reads the contents of the Matrix Market file ''filename''
//      into the matrix ''A''.  ''A'' will be either sparse or full,
//      depending on the Matrix Market format indicated by
//      ''coordinate'' (coordinate sparse storage), or
//      ''array'' (dense array storage).  The data will be duplicated
//      as appropriate if symmetry is indicated in the header.
// 
//      Optionally, size information about the matrix can be 
//      obtained by using the return values rows, cols, and
//      entries, where entries is the number of nonzero entries
//      in the final matrix. Type information can also be retrieved
//      using the optional return values rep (representation), field,
//      and symm (symmetry).
//Author S. Steer, Copyright INRIA
  mmfile = mopen(filename,"r");
  [rows, cols, entries, rep, field, symm] = mminfo(mmfile)
  if rep=='coordinate' then //sparse matrix
    if field=="real" then // real valued entries:
      T = mfscanf(-1,mmfile,"%d %d %f\n");
      if size(T(:,1),1)<>entries then error("Not enough data in file");end;
      A = sparse(T(:,1:2),  T(:,3), [rows ,cols]);
    elseif field=="complex" then // complex valued entries:
      T = mfscanf(-1,mmfile,"%d %d %f %f\n");
      if size(T(:,1),1)<>entries then error("Not enough data in file");end;
      A = sparse(T(:,1:2),  T(:,3)+%i*T(:,4), [rows ,cols]);
    elseif field=="pattern" then // pattern matrix (no values given):
      T = mfscanf(-1,mmfile,"%d %d\n");
      if size(T(:,1),1)<>entries then error("Not enough data in file");end;
      A = sparse(T(:,1:2),  ones(entries,1), [rows ,cols]);
    end
  elseif rep=="array" then //   dense  array (column major) format
    if field=="real" then // real valued entries:
      A = mfscanf(-1,mmfile,"%f\n");
      if or(symm==['symmetric','hermitian','skew-symmetric']) then
	for j=1:cols-1,
	  currenti = j*rows;
	  A = [A(1:currenti); zeros(j,1);A(currenti+1:length(A))];
	end
      elseif symm<>"general" then
	error("Unrecognized symmetry:"+symm+..
	      "Recognized choices: symmetric,hermitian,skew-symmetric,general")
      end;
      A = matrix(A,rows,cols);
    elseif field=="complex" then // complx valued entries:
      A = mfscanf(-1,mmfile,"%f %f\n");
      A=A(:,1)+%i*A(:,2)
      if or(symm==['symmetric','hermitian','skew-symmetric']) then
	for j=1:cols-1,
	  currenti = j*rows;
	  A = [A(1:currenti); zeros(j,1);A(currenti+1:length(A))];
	end
      elseif symm<>"general" then
	error("Unrecognized symmetry:"+symm+..
	      "Recognized choices: symmetric,hermitian,skew-symmetric,general")
      end;
      A = matrix(A,rows,cols);
    end;
  elseif field=="pattern" then // pattern (makes no sense for dense)
    error("Pattern matrix type invalid for array storage format.");
  else // Unknown matrix type
    error("Invalid matrix type specification. Check header against MM documentation.");
  end;

  // 
  // If symmetric, skew-symmetric or Hermitian, duplicate lower
  // triangular part and modify entries as appropriate:
  // 
  if symm=="symmetric" then
    A = A + A.' - diag(diag(A));
    entries = nnz(A);
  elseif symm=="hermitian" then
    A = A + A' - diag(diag(A));
    entries = nnz(A);
  elseif symm=="skew-symmetric" then
    A = A - tril(A,-1).';
    entries = nnz(A);
  end;

  mclose(mmfile);
  // Done.
endfunction
