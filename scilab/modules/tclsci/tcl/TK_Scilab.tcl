
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA 1998 - Bertrand Guiheneuf
# Copyright (C) INRIA 2004 - Serge Steer
# 
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#

############################################
# TCLSCI Gui Interface facility for Scilab
############################################

if {[info exists SciPath]} { set env(SCIPATH) $SciPath };
############################################
proc bgerror { m } {
puts " Error: $m";
}
############################################
# the root object is the first tk object
set root .;
############################################
wm withdraw $root;
wm title $root "Tk Scilab root Window";
pack [ label .l ];
destroy .l;
############################################
