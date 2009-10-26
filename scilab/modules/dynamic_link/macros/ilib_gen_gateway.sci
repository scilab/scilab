// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA/ENPC
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//==========================================
// ilib_gen_gateway used by ilib_build
//==========================================
// generate an interface gateway named name
// from table table taking into account 
// attributes of function i.e mex fmex or scilab interface 
// if name is a full path just extract the filename 

function gateway_filename = ilib_gen_gateway(name,tables)

  [lhs,rhs] = argn(0);
  if rhs <> 2 then
    error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "ilib_gen_gateway"));
    return
  end

  gateway_filename = '';
  k = strindex(name,['/','\']);
  if k~=[] then
    path = part(name,1:k($));
    name = part(name,k($)+1:length(name));
  else
     path='';
  end
  
  [path_name, file_name, ext_name] = fileparts(name);
  if ext_name == '.c' then
    name = path_name + file_name;
  else
    name = path_name + file_name + ext_name;
  end
  
  if typeof(tables)<>'list' then 
    tables = list(tables);
  end
  
  L = length(tables); 

  for itable = 1:L 
    // loop on a list of tables 
    if L <> 1 then 
      tname = name +string(itable);
    else 
      tname = name ;
    end
    
    table = tables(itable);
    [mt,nt] = size(table);
    
    if (nt == 2) then 
      col = "csci";
      table = [table, col(ones(mt,1))];
      nt = 3;
    end 
    
    if ( nt <> 3 ) then 
      error(msprintf(gettext("%s: Wrong size for input argument #%d: %d expected.\n"),"ilib_gen_gateway",2,3));
    end 
    [gate,names] = new_names(table); 
    t = [ '#include <mex.h> ';
          '#include <sci_gateway.h>';
          '#include <api_scilab.h>';
          'static int direct_gateway(char *fname,void F(void)) { F();return 0;};';
          'extern Gatefunc ' + names(:) + ';';
          'static GenericTable Tab[]={';
          '  {'+ gate(:)+','+ names(:)+',""'+table(:,1)+'""},';
          '};';
          ' ';
          'int C2F(' + tname + ')()';
          '{';
          '  Rhs = Max(0, Rhs);';
          '  if (*(Tab[Fin-1].f) != NULL) '
          '  {';
          '     pvApiCtx->pstName = (char*)Tab[Fin-1].name;';
          '    (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);';
          '  }';
          '  return 0;';
          '}'];

	  gateway_filename = path + tname + '.c';
    // first chek if we already have a gateway 
    [fd,ierr] = mopen(gateway_filename);
    if ierr == 0 then
      mclose(fd);
      t1 = mgetl(gateway_filename);
      if t1 <> t then 
	      mputl(t, gateway_filename);    
      end
    else
       // file does not exist we create it 
       mputl(t, gateway_filename) ;   
    end
    
    if ilib_verbose() > 1 then
      disp(t);
    end
  end
endfunction
//==========================================



//==========================================
// new_names only used by ilib_gen_gateway
//==========================================
function [gate,names] = new_names(table) 
  // change names according to types 
  [mt,nt] = size(table);
  gate = "mex_gateway"; 
  gate = gate(ones(mt,1)); 
  names = " "; 
  names = names(ones(mt,1)); 
  for i = 1:mt 
    select table(i,3) 
     case 'cmex' then 
       names(i) = "mex_" + table(i,2) ;
     case 'fmex' then 
       gate(i) = "(Myinterfun)fortran_mex_gateway" ;
       names(i) = "C2F(mex" + table(i,2) + ")" ;
     case 'Fmex' then 
       gate(i) = "(Myinterfun)fortran_mex_gateway" ;
       names(i) = "C2F(mex" + table(i,2) + ")" ;
     case 'csci'  then
       if isdef('WITHOUT_AUTO_PUTLHSVAR') then
         if (WITHOUT_AUTO_PUTLHSVAR == %T) then
           gate(i) = "(Myinterfun)sci_gateway_without_putlhsvar" ;
         else
           gate(i) = "(Myinterfun)sci_gateway" ;
         end
       else
         gate(i) = "(Myinterfun)sci_gateway" ;
       end
       names(i) = table(i,2) ;
     case 'fsci'  then 
       if isdef('WITHOUT_AUTO_PUTLHSVAR') then
         if (WITHOUT_AUTO_PUTLHSVAR == %T) then
           gate(i) = "(Myinterfun)sci_gateway_without_putlhsvar" ;
         else
           gate(i) = "(Myinterfun)sci_gateway" ;
         end
       else
         gate(i) = "(Myinterfun)sci_gateway" ;
       end
       names(i) = "C2F(" + table(i,2) + ")" ;
     case 'direct'  then 
       gate(i) = "(Myinterfun)direct_gateway" ;
       names(i) = "C2F(" + table(i,2) + ")" ;
    else 
      error(999,"Wrong interface type " + table(i,3)); 
    end 
  end 
endfunction
//==========================================
