//------------------------------------
// Allan CORNET INRIA 2007
//------------------------------------
function ret=check_versions(varargin)
  [lhs,rhs]=argn(0);
  ret=%T;
  modules=getmodules();
  szmodules=size(modules);

  for i=1:1:szmodules(1)
    file_to_check=SCI+'/modules/'+modules(i)+'/VERSION.xml';
    printf('checking : '+modules(i)+'\n');
    if (fileinfo(file_to_check)<>[]) then
      try
        if MSDOS then
          stat = unix_w(SCI+"\modules\helptools\bin\xmllint\xmllint --noout --valid "+file_to_check);
        else
    	    stat = unix_w("xmllint --noout --valid "+file_to_check);
        end
      catch
        ret=%F;
        printf('\n Please check : '+file_to_check+'\n\n');
      end
    else
      printf('\n file doesn''t exist : '+file_to_check+'\n\n');
    end
  end
  
endfunction 
 