// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

parent=pvm_parent();
if parent<0 then
  return
end

while %t  //Infinite loop
  [buf,info] = pvm_recv(parent, -1)//get new variable or instruction
	disp(info)
  if info<0 then break,end
  
  if type(buf)==10 then //an instruction
    if execstr(buf,'errcatch') then break,end//execute it
    write(%io(2),'Instruction: '+buf+' done')
  else //a variable
    %var=buf //preserve it in %var
    disp('Variable received')
  end
end
