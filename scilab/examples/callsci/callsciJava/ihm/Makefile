SOURCES=*.java

SCIDIR=/usr/local/scilab

JAVAC=javac -d $(SCIDIR)/examples/callsci/callsciJava/classes -classpath \
	$(SCIDIR)/examples/callsci/callsciJava/classes -g 

all:
	$(JAVAC)  $(SOURCES)

clean:
	$(RM) -f $(SCIDIR)/examples/callsci/callsciJava/classes/*.class


