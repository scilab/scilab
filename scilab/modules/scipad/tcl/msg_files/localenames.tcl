# locale names (common)
# note that to avoid encoding issues, all unicode characters which are beyond 
# the basic ASCII encoding are safely expressed in \u form 
# (see http://wiki.tcl.tk/656)
# If I would have used UTF-8, extended characters would be read correctly
# only if the LANG environment supported UTF8.
::msgcat::mcset $lang "da locale (da)" "Dansk (da)"
::msgcat::mcset $lang "de locale (de)" "Deutsch (de)"
::msgcat::mcset $lang "fr locale (fr)" "Fran\u00E7ais (fr)"
::msgcat::mcset $lang "eng locale (eng)" "English (eng)"
::msgcat::mcset $lang "es locale (es)" "Espa\u00F1ol (es)"
::msgcat::mcset $lang "it locale (it)" "Italiano (it)"
::msgcat::mcset $lang "no locale (no)" "Norsk (no)"
::msgcat::mcset $lang "pl locale (pl)" "Polski (pl)"
::msgcat::mcset $lang "se locale (se)" "Svenska (se)"
::msgcat::mcset $lang "zh_tw locale (zh_tw)" "\u4E2D\u6587\u7E41\u9AD4 (zh_tw)"
::msgcat::mcset $lang "zh_cn locale (zh_cn)" "\u4e2d\u6587\u7c21\u4f53 (zh_cn)"
