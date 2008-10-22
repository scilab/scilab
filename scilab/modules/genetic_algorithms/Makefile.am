# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Pierre MARECHAL
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

#### Target ######
modulename=genetic_algorithms

#### genetic_algorithms : Conf files ####
libgenetic_algorithms_la_rootdir = $(mydatadir)
libgenetic_algorithms_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### genetic_algorithms : init scripts ####
libgenetic_algorithms_la_etcdir = $(mydatadir)/etc
libgenetic_algorithms_la_etc_DATA = etc/genetic_algorithms.quit etc/genetic_algorithms.start

#### genetic_algorithms : examples files ####
libgenetic_algorithms_la_examplesdir = $(mydatadir)/examples
libgenetic_algorithms_la_examples_DATA = \
examples/crossover_ga_ising2d.sci \
examples/GAdemo.sce \
examples/GAIsing2ddemo.sce \
examples/init_ga_ising2d.sci \
examples/MOGAdemo.sce \
examples/mutation_ga_ising2d.sci \
examples/NSGA2demo.sce \
examples/NSGAdemo.sce

libgenetic_algorithms_la_examplesisingdir = $(mydatadir)/examples/Ising
libgenetic_algorithms_la_examplesising_DATA = \
examples/Ising/init_ising2d.sci \
examples/Ising/init_potts2d.sci \
examples/Ising/ising2d.sci \
examples/Ising/plot_ising2d.sci \
examples/Ising/plot_potts2d.sci

include $(top_srcdir)/Makefile.incl.am
