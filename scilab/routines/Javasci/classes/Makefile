
SOURCES=BadDataArgumentException.java Scilab.java SciReal.java

SCIDIR=../../..
CLASSREP=$(SCIDIR)/bin/Javasci/javasci

JAVAC=javac -d $(SCIDIR)/bin/Javasci -classpath $(SCIDIR)/bin/Javasci -g 

all: $(CLASSREP)

$(CLASSREP): $(SOURCES) 
	$(JAVAC)  $(SOURCES)

clean:
	$(RM) -f $(CLASSREP)/*.class


