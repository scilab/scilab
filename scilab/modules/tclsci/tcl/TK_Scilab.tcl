
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA 1998 - Bertrand Guiheneuf
# Copyright (C) INRIA 2004 - Serge Steer
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
