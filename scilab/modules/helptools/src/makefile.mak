all:: check build

build:: makehelp


check :
	@..\..\..\bin\scilex -nb -l fr -e check_help();exit;
	@..\..\..\bin\scilex -nb -l en -e check_help();exit;


makehelp :
	@..\..\..\bin\scilex -nb -l en -nouserstartup -nwni -e xmltohtml();exit;
	@..\..\..\bin\scilex -nb -l fr -nouserstartup -nwni -e xmltohtml();exit;
	
	
distclean::	
clean::
	@..\..\..\bin\scilex -nb -l en -nouserstartup -nwni -e clean_help();exit;
	@..\..\..\bin\scilex -nb -l fr -nouserstartup -nwni -e clean_help();exit;
