//------------------------------------
// Allan CORNET INRIA 2007
//------------------------------------
function ret=check_localization()
 ret=%T;
 modules=getmodules();
 szmodules=size(modules);

 filesxml=['messages.xml','menus.xml','errors.xml'];

 for i=1:1:szmodules(1)
   bOK=%T;
   directory_to_check=SCI+'/modules/'+modules(i)+'/languages/'+getlanguage()+'/';
   printf('checking : '+modules(i)+'\n');
   for k2=1:1:3
     try
     if MSDOS then
		   stat = unix_w(SCI+"\modules\helptools\bin\xmllint\xmllint --noout --valid "+directory_to_check+filesxml(k2));
     else
    	 stat = unix_w("xmllint --noout --valid "+directory_to_check+filesxml(k2));
     end
     catch
       bOK=%F;
     end
   end
   if (~bOK) then
     printf('\n Please check directory : '+directory_to_check+'\n\n');
     ret=%F;
   end
 end	
endfunction
//------------------------------------
