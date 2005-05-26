SCIDIR=../../..
SOURCES=BadDataArgumentException.java Scilab.java SciReal.java
CLASSREP=$(SCIDIR)/bin
DEST=$(CLASSREP)/javasci
CLASSES=$(DEST)/Scilab.class $(DEST)/BadDataArgumentException.class $(DEST)/SciReal.class

JAVAC=javac -d $(CLASSREP) -classpath $(CLASSREP)

all:: $(CLASSES)

$(DEST)/Scilab.class: Scilab.java
	@$(JAVAC)  Scilab.java

$(DEST)/SciReal.class: SciReal.java
	@$(JAVAC)  SciReal.java

$(DEST)/BadDataArgumentException.class: BadDataArgumentException.java
	@$(JAVAC)  BadDataArgumentException.java


cleanC::
	@$(RM) -f $(DEST)/*.class


