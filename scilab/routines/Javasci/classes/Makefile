SCIDIR=../../..
SOURCES=BadDataArgumentException.java Scilab.java SciDoubleArray.java SciDouble.java SciStringArray.java SciString.java SciComplex.java SciComplexArray.java
CLASSREP=$(SCIDIR)/bin
DEST=$(CLASSREP)/javasci
CLASSES= $(DEST)/BadDataArgumentException.class $(DEST)/Scilab.class $(DEST)/SciDoubleArray.class $(DEST)/SciDouble.class $(DEST)/SciStringArray.class $(DEST)/SciString.class $(DEST)/SciComplexArray.class $(DEST)/SciComplex.class

JAVAC=javac -d $(CLASSREP) -classpath $(CLASSREP)

all:: $(CLASSES)

$(DEST)/BadDataArgumentException.class: BadDataArgumentException.java
	@$(JAVAC)  BadDataArgumentException.java

$(DEST)/Scilab.class: Scilab.java
	@$(JAVAC)  Scilab.java

$(DEST)/SciDoubleArray.class: SciDoubleArray.java
	@$(JAVAC)  SciDoubleArray.java
	
$(DEST)/SciDouble.class: SciDouble.java
	@$(JAVAC)  SciDouble.java

$(DEST)/SciStringArray.class: SciStringArray.java
	@$(JAVAC)  SciStringArray.java
	
$(DEST)/SciString.class: SciString.java
	@$(JAVAC)  SciString.java
	
$(DEST)/SciComplexArray.class: SciComplexArray.java
	@$(JAVAC)  SciComplexArray.java
	
$(DEST)/SciComplex.class: SciComplex.java
	@$(JAVAC)  SciComplex.java

cleanC::
	@$(RM) -f $(DEST)/*.class

