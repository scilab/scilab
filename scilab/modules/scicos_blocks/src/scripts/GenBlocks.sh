#!/bin/sh
#
#  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
#  Copyright (C) INRIA - Scilab Project - Sylvestre Ledru
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# See the file ./license.txt
#
# Generates block.h file given file Fblocknames and Cblocknames
# Fblocknames  contains list of Fortran Blocks 
# Cblocknames  contains list of C Blocks 
# Copyright INRIA
if [ $# -ne 4 ]; then
	echo "Wrong syntax. Syntax is : $0 Fortran_Block_Names C_Block_Names CPP_Block_Names Outputfile_h"
	exit 1
fi
Fin=$1
Cin=$2
CPPin=$3
fout=$4

echo "#ifndef __SCICOS_BLOCKS__ " > $fout 
echo "#define __SCICOS_BLOCKS__ " >> $fout 
echo "#include \"scicos.h\" " >> $fout
echo "/******* Copyright INRIA *************/" >> $fout 
echo "/******* Please do not edit (file automatically generated) *************/" >> $fout 

links=`cat $Fin`
for i in $links
	do ( echo "extern void F2C($i) (ARGS_scicos);"  >> $fout ;); done

links=`cat $Cin`
for i in $links
	do ( echo "extern void $i (ARGS_scicos);"  >> $fout ;); done

links=`cat $CPPin`
for i in $links
	do ( echo "extern void $i (ARGS_scicos);"  >> $fout ;); done

echo " " >> $fout 
echo "OpTab tabsim[] ={" >> $fout
rm -f $fout-temp$$

links=`cat $Fin`
for i in $links
	do  (  echo "{\"$i\",(ScicosF) F2C($i)}," >> $fout-temp$$ ;); done ;

links=`cat $Cin`
for i in $links
	do  (  echo "{\"$i\",(ScicosF) $i}," >> $fout-temp$$ ;); done ;

links=`cat $CPPin`
for i in $links
	do  (  echo "{\"$i\",(ScicosF4) $i}," >> $fout-temp$$ ;); done ;

sort $fout-temp$$ >> $fout; 
echo "{(char *) 0, (ScicosF) 0}};" >> $fout ;

x=`cat $Fin $Cin $CPPin | wc -l `;
echo " " >> $fout 
echo "int ntabsim=" $x ";" >> $fout ;
echo "#endif " >> $fout;
rm -f $fout-temp$$
echo "/***********************************/" >> $fout 

