# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009 - DIGITEO
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

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
