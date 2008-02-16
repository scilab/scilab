// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006 - INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


fd=mopen('Makefile','wt');
mputl('SOURCES=*.java',fd);
mputl(' ',fd);
mputl('SCIDIR='+SCI,fd);
mputl(' ',fd);
mputl('JAVAC=javac -deprecation -d $(SCIDIR)/bin -classpath $(SCIDIR)/bin -g ',fd);
mputl(' ',fd);
mputl(' ',fd);
mputl('all:',fd);
mputl(ascii(9)+'$(JAVAC) $(SOURCES)',fd);
mputl(' ',fd);
mputl('clean:',fd);
mputl(ascii(9)+'$(RM) -f $(SCIDIR)/bin/*.class',fd);
mputl(' ',fd);	
mputl(' ',fd);
mclose(fd);
