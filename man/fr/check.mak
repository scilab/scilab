XMLINTCHECK="..\check.bat"
DIRECTORY=java arma control dcd elementary fileio functions graphics gui identification linear metanet mtlb nonlinear polynomials programming pvm robust signal sound statistics strings tdcs tksci translation translation utilities sparse

all :: $(DIRECTORY)

java:  
	@echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
arma:  
	@echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
control:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
dcd:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..	
elementary:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
fileio:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
functions:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
graphics:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
gui:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
identification:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
linear:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
metanet:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
mtlb:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
nonlinear:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
polynomials:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
programming:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
pvm:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
robust:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
signal:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
sound:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
statistics:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
strings:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
tdcs:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
tksci:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
translation:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
utilities:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..
sparse:
  @echo Checking $@
	@cd $@
	@$(XMLINTCHECK)
	@cd ..

	