
SOURCES=BadDataArgumentException.java Matrix.java MatrixUtilities.java

SCIDIR=../../..
CLASSREP=$(SCIDIR)/bin/Javasci/javasci/

JAVAC=javac -d $(SCIDIR)/bin/Javasci -classpath $(SCIDIR)/bin/Javasci -g 

all: $(CLASSREP)/Matrix.class

$(CLASSREP)/Matrix.class: $(SOURCES) 
	$(JAVAC)  $(SOURCES)

clean:
	$(RM) -f $(CLASSREP)/*.class


