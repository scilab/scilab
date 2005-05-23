SCIDIR=../../..
SOURCES=BadDataArgumentException.java Scilab.java SciReal.java
CLASSREP=$(SCIDIR)/bin
DEST=$(CLASSREP)/javasci
CLASSES=$(DEST)/Scilab.class $(DEST)/BadDataArgumentException.class $(DEST)/SciReal.class

JAVAC=javac -d $(CLASSREP) -classpath $(CLASSREP)

all:: $(CLASSES)
	@echo building java classes
$(DEST)/Scilab.class: Scilab.java
	@$(JAVAC)  $<	

$(DEST)/SciReal.class: SciReal.java
	@$(JAVAC)  $<	

$(DEST)/BadDataArgumentException.class: BadDataArgumentException.java
	@$(JAVAC)  $<	


cleanC::
	@$(RM) -f $(DEST)/*.class


