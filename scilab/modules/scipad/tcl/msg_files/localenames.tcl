#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2008 - Francois Vogel
#
#  Localization files ( in tcl/msg_files/) are copyright of the 
#  individual authors, listed in the header of each file
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
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file scipad/license.txt
#

# locale names (common)
# note that to avoid encoding issues, all unicode characters which are beyond 
# the basic ASCII encoding are safely expressed in \u form 
# (see http://wiki.tcl.tk/656)
# If I would have used UTF-8, extended characters would be read correctly
# only if the LANG environment supported UTF8.
::msgcat::mcset $lang "da_dk locale" "Dansk (da_dk)"
::msgcat::mcset $lang "de_de locale" "Deutsch (de_de)"
::msgcat::mcset $lang "fr_fr locale" "Fran\u00E7ais (fr_fr)"
::msgcat::mcset $lang "en_us locale" "English (en_us)"
::msgcat::mcset $lang "es_es locale" "Espa\u00F1ol (es_es)"
::msgcat::mcset $lang "it_it locale" "Italiano (it_it)"
::msgcat::mcset $lang "no locale" "Norsk (no)"
::msgcat::mcset $lang "pl locale" "Polski (pl)"
::msgcat::mcset $lang "se locale" "Svenska (se)"
::msgcat::mcset $lang "zh_tw locale" "\u4E2D\u6587\u7E41\u9AD4 (zh_tw)"
::msgcat::mcset $lang "zh_cn locale" "\u4e2d\u6587\u7b80\u4f53 (zh_cn)"
