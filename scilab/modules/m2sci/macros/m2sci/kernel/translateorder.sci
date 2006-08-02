function  [transorder]=translateorder(transorder,funtxt,overfunname)
//  Copyright INRIA
//
//  TRANSLATEORDER Recursive function  
//  Determinates a translate order of the M-files found in Paths (used by "translatepaths" function)
//  Ouput-Input 
//  -transorder : a vector containing the M-files names which are arranged in order to respect an priority order of translation
//  Input 
//  -funtxt : a vector which contains the name of a M-file found in the Paths (its first component: funtxt(1)), and the called functions by this file (the others components : funtxt(2:$)) 
//  -overfunname : a vector which contains the files names being passed like argument of     "translateorder" function

// the file is already in the list 
if or(transorder==funtxt(1)) then
return
end

if size(funtxt,"*")>1 then
  for i=2:size(funtxt,1)
    // the called function is already in the list
    if find(funtxt(i)==transorder)<>[] then
      continue
    // the called function is already passed in argument of "translateorder" function (a loop)
    elseif find(funtxt(i)==overfunname)<>[] then
     disp([overfunname;funtxt(i)],"loop : ")
     //return
      continue
    // order the under level, also we call translateorder (recursive)
    else
      overfunname($+1)=funtxt(1)
      execstr("[transorder]=translateorder(transorder,"+funtxt(i)+"vect,overfunname)")  
    end
  end
      transorder($+1)=funtxt(1)
else
     // no called function by the M-file, also we put the M-file name in the transorder vector
      transorder($+1)=funtxt(1)
end

endfunction
 


