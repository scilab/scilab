SOURCES=*.java

SCIDIR=/usr/local/scilab

JAVAC=javac -d $(SCIDIR)/bin/Javasci -classpath \
	$(SCIDIR)/bin/Javasci -g 

all:
	$(JAVAC)  $(SOURCES)

clean:
	$(RM) -f $(SCIDIR)/bin/Javasci/*.class


