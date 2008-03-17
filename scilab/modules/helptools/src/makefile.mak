all:: check build

build:: makehelp


check :
	@..\..\..\bin\scilex -nb -nwni -nouserstartup -l fr -e check_help();exit;
	@..\..\..\bin\scilex -nb -nwni -nouserstartup -l en -e check_help();exit;


makehelp :
	@..\..\..\bin\scilex -nb -l en -nouserstartup -nwni -e xmltojar();exit;
	@..\..\..\bin\scilex -nb -l fr -nouserstartup -nwni -e xmltojar();exit;
	
	
distclean::	
clean::
	@..\..\..\bin\scilex -nb -l en -nouserstartup -nwni -e clean_help();exit;
	@..\..\..\bin\scilex -nb -l fr -nouserstartup -nwni -e clean_help();exit;
