function getd(path)
// get all .sci files in the specified directory
// Copyright INRIA/Enpc 
  
  [lhs,rhs]=argn(0)
  if rhs<=0 then path='./',end

  // convert path according to MSDOS flag 
  // and with env var substitutions 
  path=pathconvert(path,%t,%t);
  
  // list the sci files 
  lst=listfiles(path+'*.sci',%f)

  if lst==[] | lst== "" then 
    error('I cannot find any sci files in '+path);
    return ;
  end

  nold=size(who('get'),'*')
  prot=funcprot();funcprot(0)

  for k=1:size(lst,'*'); getf(lst(k));  end

  funcprot(prot)
  new=who('get')
  new=new(1:(size(new,'*')-nold-2))
  if new<>[] then
    execstr('['+strcat(new,',')+']=resume('+strcat(new,',')+')')
  end
endfunction

