# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

modulename=atoms

#### atoms : Conf files ####
libsciatoms_la_rootdir = $(mydatadir)
libsciatoms_la_root_DATA =  license.txt 

#### atoms : init scripts ####
libsciatoms_la_etcdir = $(mydatadir)/etc
libsciatoms_la_etc_DATA = etc/atoms.quit etc/atoms.start etc/repositories

#### atoms : XML gui ####
libsciatoms_la_guidir = $(mydatadir)/gui
libsciatoms_la_gui_DATA = gui/atomsGui.xml

#### atoms : internal functions ####
MACROSDIRSEXT = macros/atoms_internals macros/atoms_gui

#### atoms : icons files ####
#
# Generated with:
#    $ find images/ -type f -printf "%p \\\\\n" -name *.svg -or -name *.gif -or -name *.jpg -or -name *.png |sort -u
libsciatoms_la_iconsdir=$(mydatadir)/
nobase_libsciatoms_la_icons_DATA = images/icons/16x16/emblems/emblem-default.png \
images/icons/16x16/status/dialog-information.png \
images/icons/16x16/status/software-update-available.png \
images/icons/16x16/status/software-update-installed.png \
images/icons/16x16/status/software-update-notinstalled.png \
images/icons/16x16/status/software-update-notuptodate.png

include $(top_srcdir)/Makefile.incl.am
