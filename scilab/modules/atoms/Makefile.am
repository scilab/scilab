# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

modulename=atoms

#### atoms : Conf files ####
libsciatoms_la_rootdir = $(mydatadir)
libsciatoms_la_root_DATA =  license.txt version.xml

#### atoms : init scripts ####
libsciatoms_la_etcdir = $(mydatadir)/etc
libsciatoms_la_etc_DATA = etc/atoms.quit etc/atoms.start etc/repositories

#### atoms : internal functions ####
MACROSDIRSEXT = macros/atoms_internals macros/atoms_gui

#### atoms : icons files ####
libsciatoms_la_iconsdir=$(mydatadir)/
nobase_libsciatoms_la_icons_DATA = images/icons/installed.png \
images/icons/notinstalled.png \
images/icons/notuptodate.png \
images/icons/software-update-available.png \
images/icons/dialog-information.png \
images/icons/emblem-default.png \
images/icons/user-home.png

include $(top_srcdir)/Makefile.incl.am
