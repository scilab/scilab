# A sample Makefile to build a Javasci v2-based application
# Path to Scilab data
SCI = /path/to/scilab/share/scilab/
# Java API
CLASSPATH = $(SCI)/modules/javasci/jar/org.scilab.modules.javasci.jar:$(SCI)/modules/types/jar/org.scilab.modules.types.jar
# Path to native libs
LIB_PATH = $(SCI)/../../lib/scilab/
# Scilab binary: /path/to/scilab/lib/scilab/
# with the source version: $(SCI)/modules/javasci/.libs/:$(SCI)/modules/.libs/
all:
	javac -cp $(CLASSPATH) Example1.java
	java -cp $(CLASSPATH):. -DSCI=$(SCI) -Djava.library.path=$(LIB_PATH) Example1