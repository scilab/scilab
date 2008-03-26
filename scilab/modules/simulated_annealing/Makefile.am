# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Pierre MARECHAL
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

#### Target ######
modulename=simulated_annealing

#### simulated_annealing : Conf files ####
libsimulated_annealing_la_rootdir = $(mydatadir)
libsimulated_annealing_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### simulated_annealing : init scripts ####
libsimulated_annealing_la_etcdir = $(mydatadir)/etc
libsimulated_annealing_la_etc_DATA = etc/simulated_annealing.quit etc/simulated_annealing.start

#### simulated_annealing : examples files ####
libsimulated_annealing_la_examplesdir = $(mydatadir)/examples
libsimulated_annealing_la_examples_DATA = \
examples/neigh_func_ising2d.sci \
examples/SAdemo.sce \
examples/SAIsing2ddemo.sce

include $(top_srcdir)/Makefile.incl.am
