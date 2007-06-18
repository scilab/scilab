########### 
## Unitary tests of Scilab features
## Java tests
## INRIA
## Authors : 
## * Pierre MARECHAL
## * Allan CORNET
##
## Update Sylvestre LEDRU 2007
##
###########

SHELL = /bin/sh
SCILABBIN=$$SCI/bin/scilab
info:
	@echo "Type \"make tests\" in tests directory"
	@echo "  to test the  distribution"
	@echo " SEE THE README FILE (IMPORTANT) "

tests: 
	@if test -z "$SCI"; then \
		echo "SCI must be defined"; \
		exit 1; \
	fi; \
	if test -x "$(SCILABBIN)"; then \
		$(SCILABBIN) -nwni -nb -f JavaTests.sce; \
	else \
		echo "Cannot find file $(SCILABBIN)"; \
	fi

clean ::
	$(RM) *.dia

distclean::
	$(RM) *.dia
#javac -deprecation -d /home/sylvestre/dev/scilab5/modules/javasci/tests -cp /home/sylvestre/dev/scilab5/modules/javasci/jar/javasci.jar:. Boolean.java
#java -cp /home/sylvestre/dev/scilab5/modules/javasci/jar/javasci.jar:. -Djava.library.path=/home/sylvestre/dev/scilab5/modules/javasci/.libs/ Boolean > /home/sylvestre/dev/scilab5/modules/javasci/tests/Boolean.dia



#java -cp /home/sylvestre/dev/scilab5/modules/javasci/jar/javasci.jar:.:/home/sylvestre/dev/scilab5//modules/javasci/jar/javasci.jar:/home/sylvestre/dev/scilab5//modules/localization/jar/org.scilab.modules.localization.jar:/home/sylvestre/dev/scilab5//modules/jvm/jar/org.scilab.modules.jvm.jar:/home/sylvestre/dev/scilab5//modules/gui/jar/org.scilab.modules.gui.jar:/home/sylvestre/dev/scilab5/modules/graphics/jar/org.scilab.modules.graphics.jar:/home/sylvestre/dev/scilab5//modules/shell/jar/org.scilab.modules.shell.jar -Djava.library.path=/home/sylvestre/dev/scilab5/modules/javasci/.libs/ BooleanArray