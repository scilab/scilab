rem Path to Scilab data
set SCI="C:\Program Files\scilab-5.3.0"
rem Java API
set CLASSPATH=%SCI%/modules/javasci/jar/org.scilab.modules.javasci.jar;%SCI%/modules/types/jar/org.scilab.modules.types.jar
rem Path to native libs
set LIB_PATH="%SCI%\bin"

javac -cp %CLASSPATH% Example1.java
java -cp %CLASSPATH%;. -Djava.library.path=%LIB_PATH% Example1
