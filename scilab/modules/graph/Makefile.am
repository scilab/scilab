#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2010 - Allan CORNET
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
#

#### Target ######
modulename=graph


#### graph : Conf files ####
libscigraph_la_rootdir = $(mydatadir)
libscigraph_la_root_DATA =  license.txt 


#### graph : init scripts ####
libscigraph_la_etcdir = $(mydatadir)/etc
libscigraph_la_etc_DATA = etc/graph.quit \
				etc/graph.start

include $(top_srcdir)/Makefile.incl.am

if GUI
if XCOS
USEANT=1
endif
endif
