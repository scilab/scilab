//------------------------------------
// Allan CORNET INRIA 2007
//------------------------------------
function ret=check_classpath(varargin)
  [lhs,rhs]=argn(0);
  ret=%T;
  file_to_check=SCI+'/etc/classpath.xml';
  printf('checking : '+file_to_check+'\n');
  try
  if MSDOS then
    stat = unix_w(SCI+"\modules\helptools\bin\xmllint\xmllint --noout --valid "+file_to_check);
  else
    stat = unix_w("xmllint --noout --valid "+file_to_check);
  end
  catch
  ret=%F;
  end
endfunction 
//------------------------------------
 