#file menu
menu $pad.filemenu.files -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu  add cascade -label "File" -underline 0 \
	-menu $pad.filemenu.files
    $pad.filemenu.files add command -label "New" -underline 0 \
                   -command "filesetasnewmat" -accelerator Ctrl+n
    $pad.filemenu.files add command -label "Open" -underline 0 \
                   -command "filetoopen $textareacur" -accelerator Ctrl+o
    $pad.filemenu.files add command -label "Save" -underline 0 \
                   -command "filetosavecur" -accelerator Ctrl+s
    $pad.filemenu.files add command -label "Save As" -underline 5 \
                   -command "filesaveascur" -accelerator Ctrl+S
    $pad.filemenu.files add separator
    $pad.filemenu.files add command -label "Import Matlab file..." \
	-underline 7 -command "importmatlab" -accelerator F4
    $pad.filemenu.files add separator
    if {"$tcl_platform(platform)" == "unix"} {
	$pad.filemenu.files add command -label "Print Setup" -underline 8 \
                   -command "printseupselection" -accelerator Ctrl+P
	$pad.filemenu.files add command -label "Print" -underline 0 \
                   -command "selectprint $textareacur" -accelerator Ctrl+p
	$pad.filemenu.files add separator
    }
    $pad.filemenu.files add command -label "Close" -underline 0 \
	-command "closecur" -accelerator Ctrl+w
    $pad.filemenu.files add command -label "Exit" -underline 1 \
	-command "exitapp" -accelerator Ctrl+q
} else {
    $pad.filemenu  add cascade -label "Fichier" -underline 0 \
	-menu $pad.filemenu.files
    $pad.filemenu.files add command -label "Nouveau" -underline 0 \
	-command "filesetasnewmat" -accelerator Ctrl+n
    $pad.filemenu.files add command -label "Ouvrir" -underline 0 \
	-command "filetoopen $textareacur" -accelerator Ctrl+o
    $pad.filemenu.files add command -label "Enregistrer" -underline 0 \
	-command "filetosavecur" -accelerator Ctrl+s
    $pad.filemenu.files add command -label "Enregistrer sous" -underline 2 \
	-command "filesaveascur" -accelerator Ctrl+S
    $pad.filemenu.files add separator
    $pad.filemenu.files add command -label "Importer fichier Matlab..." \
                   -underline 17 -command "importmatlab" -accelerator F4
    $pad.filemenu.files add separator
    if {"$tcl_platform(platform)" == "unix"} {
	$pad.filemenu.files add command -label "Mise en page" -underline 8 \
	    -command "printseupselection" -accelerator Ctrl+P
	$pad.filemenu.files add command -label "Imprimer" -underline 0 \
	    -command "selectprint $textareacur" -accelerator Ctrl+p
	$pad.filemenu.files add separator
    }
    $pad.filemenu.files add command -label "Fermer" -underline 0 \
	-command "closecur" -accelerator Ctrl+w
    $pad.filemenu.files add command -label "Quitter" -underline 0 \
	-command "exitapp" -accelerator Ctrl+q 
}

#edit menu
menu $pad.filemenu.edit -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Edit" -underline 0 \
	-menu $pad.filemenu.edit
    $pad.filemenu.edit add command -label "Undo" -underline 0 \
	-command " undo_menu_proc" -accelerator Ctrl+z
    $pad.filemenu.edit add command -label "Redo" -underline 0 \
	-command "redo_menu_proc" -accelerator Ctrl+Z
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Cut" -underline 2 \
	-command "cuttext" -accelerator Ctrl+x
    $pad.filemenu.edit add command -label "Copy" -underline 0 \
	-command "copytext" -accelerator Ctrl+c
    $pad.filemenu.edit add command -label "Paste" -underline 0 \
	-command "pastetext" -accelerator Ctrl+v
    $pad.filemenu.edit add command -label "Delete" -underline 0 \
	-command "deletetext" -accelerator Del
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Select All" -underline 7 \
	-command "selectall" -accelerator Ctrl+/
#    $pad.filemenu.edit add command -label "Time/Date" -underline 5 \
#	-command "printtime"
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Comment selection" -underline 3 \
	-command "CommentSel" -accelerator Ctrl+m
    $pad.filemenu.edit add command -label "Uncomment selection" -underline 1 \
	-command "UnCommentSel" -accelerator Ctrl+M
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Indent selection" -underline 0 \
	-command "IndentSel" -accelerator Ctrl+d
    $pad.filemenu.edit add command -label "Unindent selection" -underline 4 \
	-command "UnIndentSel" -accelerator Ctrl+D
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add check -label "Word Wrap" -underline 5 \
	-command "wraptext"
} else {
    $pad.filemenu add cascade -label "Edition" -underline 0 \
	-menu $pad.filemenu.edit
    $pad.filemenu.edit add command -label "Annuler" -underline 0 \
	-command " undo_menu_proc" -accelerator Ctrl+z
    $pad.filemenu.edit add command -label "Répéter" -underline 0 \
	-command "redo_menu_proc" -accelerator Ctrl+Z
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Couper" -underline 2 \
	-command "cuttext" -accelerator Ctrl+x
    $pad.filemenu.edit add command -label "Copier" -underline 0 \
	-command "copytext" -accelerator Ctrl+c
    $pad.filemenu.edit add command -label "Coller" -underline 2 \
	-command "pastetext" -accelerator Ctrl+v
    $pad.filemenu.edit add command -label "Effacer" -underline 0 \
	-command "deletetext" -accelerator Del
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Sélectionner tout" -underline 13 \
	-command "selectall" -accelerator Ctrl+/
#    $pad.filemenu.edit add command -label "Time/Date" -underline 5 \
#      -command "printtime"
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Commenter la sélection" -underline 3\
	-command "CommentSel" -accelerator Ctrl+m
    $pad.filemenu.edit add command -label "Décommenter la sélection" -underline 0\
	-command "UnCommentSel" -accelerator Ctrl+M
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Indenter la sélection" -underline 0\
	-command "IndentSel" -accelerator Ctrl+d
    $pad.filemenu.edit add command -label "Désindenter la sélection" \
        -underline 1 -command "UnIndentSel" -accelerator Ctrl+D
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add check -label "Retour à la ligne automatique" \
	-underline 12 -command "wraptext"
} 

#search menu
menu $pad.filemenu.search -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Search" -underline 0 \
	-menu $pad.filemenu.search 
    $pad.filemenu.search add command -label "Find" -underline 0 \
	-command "findtext find" -accelerator Ctrl+f
    $pad.filemenu.search add command -label "Find Next" -underline 5 \
	-command "findnext find" -accelerator F3
    $pad.filemenu.search add command -label "Replace" -underline 0 \
	-command "findtext replace" -accelerator Ctrl+r
# add new menu option include by Matthieu PHILIPPE from gotoline.pth 21/11/2001
    $pad.filemenu.search add command -label "Goto Line" -underline 0 \
	-command "gotoline" -accelerator Ctrl+g
} else {
    $pad.filemenu add cascade -label "Rechercher" -underline 0 \
	-menu $pad.filemenu.search 
    $pad.filemenu.search add command -label "Rechercher" \
	-underline 0 -command "findtext find" -accelerator Ctrl+f
    $pad.filemenu.search add command -label "Rechercher suivant" \
	-underline 11 -command "findnext find" -accelerator F3
    $pad.filemenu.search add command -label "Remplacer" -underline 3 \
	-command "findtext replace" -accelerator Ctrl+r
# add new menu option include by Matthieu PHILIPPE from gotoline.pth 21/11/2001
    $pad.filemenu.search add command -label "Atteindre" -underline 0 \
	-command "gotoline" -accelerator Ctrl+g
} 

# added by matthieu PHILIPPE dec 11th 2001
# window menu
if {$lang == "eng"} {
    menu $pad.filemenu.wind -tearoff 1 -title "Opened Files" -font $menuFont
    $pad.filemenu add cascade -label "Windows" -underline 0 \
	-menu $pad.filemenu.wind
} else {
    menu $pad.filemenu.wind -tearoff 1 -title "Fichiers ouverts" \
	-font $menuFont
    $pad.filemenu add cascade -label "Fenêtres" -underline 0 \
	-menu $pad.filemenu.wind
}
$pad.filemenu.wind add radiobutton \
    -label "$listoffile("$pad.textarea",filename)"\
    -value $winopened -variable radiobuttonvalue \
    -command "montretext $pad.textarea"

# options menu
if {$lang == "eng"} {
    menu $pad.filemenu.options -tearoff 1 -font $menuFont
    $pad.filemenu add cascade -label "Options" -underline 0 \
	-menu $pad.filemenu.options
    $pad.filemenu.options add command -label "font size" -foreground red 
    $pad.filemenu.options add radiobutton -label "micro" -value 10 \
	-variable FontSize -command "setfontscipad 10" -underline 0
    $pad.filemenu.options add radiobutton -label "small" -value 12 \
	-variable FontSize -command "setfontscipad 12" -underline 0
    $pad.filemenu.options add radiobutton -label "medium" -value 14 \
	-variable FontSize -command "setfontscipad 14" -underline 1
    $pad.filemenu.options add radiobutton -label "large" -value 18\
	-variable FontSize -command "setfontscipad 18" -underline 0
#FV 27/05/04, changed for a submenu in cascade (nicer, isn't it?)
#     menu $pad.filemenu.options.fontsize -tearoff 0 -font $menuFont
#     $pad.filemenu.options add cascade -label "font size" -menu $pad.filemenu.options.fontsize
#     $pad.filemenu.options.fontsize add radiobutton -label "micro" -value 10 \
# 	-variable FontSize -command "setfontscipad 10" -underline 0
#     $pad.filemenu.options.fontsize add radiobutton -label "small" -value 12 \
# 	-variable FontSize -command "setfontscipad 12" -underline 0
#     $pad.filemenu.options.fontsize add radiobutton -label "medium" -value 14 \
# 	-variable FontSize -command "setfontscipad 14" -underline 1
#     $pad.filemenu.options.fontsize add radiobutton -label "large" -value 18\
# 	-variable FontSize -command "setfontscipad 18" -underline 0
    $pad.filemenu.options add command -label "language scheme" -foreground red
#FV 27/05/04, changed for a submenu in cascade (nicer, isn't it?)
#    menu $pad.filemenu.options.scheme -tearoff 0 -font $menuFont
#    $pad.filemenu.options add cascade -label "language scheme" -menu $pad.filemenu.options.scheme
} else {
    menu $pad.filemenu.options -tearoff 1 -font $menuFont
    $pad.filemenu add cascade -label "Options" -underline 0 \
	-menu $pad.filemenu.options
    $pad.filemenu.options add command -label "taille de police" -foreground red
    $pad.filemenu.options add radiobutton -label "micro" -value 10 \
	-variable FontSize -command "setfontscipad 10" -underline 0
    $pad.filemenu.options add radiobutton -label "petit" -value 12 \
	-variable FontSize -command "setfontscipad 12" -underline 0
    $pad.filemenu.options add radiobutton -label "moyen" -value 14  \
	-variable FontSize -command "setfontscipad 14" -underline 1
    $pad.filemenu.options add radiobutton -label "grand" -value 18 \
	-variable FontSize -command "setfontscipad 18" -underline 0
#FV 27/05/04, changed for a submenu in cascade (nicer, isn't it?)
#     menu $pad.filemenu.options.fontsize -tearoff 0 -font $menuFont
#     $pad.filemenu.options add cascade -label "taille de police" -menu $pad.filemenu.options.fontsize
#     $pad.filemenu.options.fontsize add radiobutton -label "micro" -value 10 \
# 	-variable FontSize -command "setfontscipad 10" -underline 0
#     $pad.filemenu.options.fontsize add radiobutton -label "petit" -value 12 \
# 	-variable FontSize -command "setfontscipad 12" -underline 0
#     $pad.filemenu.options.fontsize add radiobutton -label "moyen" -value 14  \
# 	-variable FontSize -command "setfontscipad 14" -underline 1
#     $pad.filemenu.options.fontsize add radiobutton -label "grand" -value 18 \
# 	-variable FontSize -command "setfontscipad 18" -underline 0
    $pad.filemenu.options add command -label "schéma langage" -foreground red
#FV 27/05/04, changed for a submenu in cascade (nicer, isn't it?)
#    menu $pad.filemenu.options.scheme -tearoff 0 -font $menuFont
#    $pad.filemenu.options add cascade -label "schéma langage" -menu $pad.filemenu.options.scheme
}
#ES 27/5/04
$pad.filemenu.options add radiobutton -label "Scilab" -underline 1 \
     -command {changelanguage "scilab"} -variable Scheme -value "scilab"
$pad.filemenu.options add radiobutton -label "XML" -underline 0 \
     -command {changelanguage "xml"} -variable Scheme -value "xml"
$pad.filemenu.options add radiobutton -label "none" -underline 0 \
        -command {changelanguage "none"} -variable Scheme -value "none"
#FV 27/05/04, changed for a submenu in cascade (nicer, isn't it?)
# $pad.filemenu.options.scheme add radiobutton -label "Scilab" \
#      -command {changelanguage "scilab"} -variable Scheme -value "scilab"
# $pad.filemenu.options.scheme add radiobutton -label "XML" \
#      -command {changelanguage "xml"} -variable Scheme -value "xml"
# $pad.filemenu.options.scheme add radiobutton -label "none" \
#         -command {changelanguage "none"} -variable Scheme -value "none"

# exec menu
menu $pad.filemenu.exec -tearoff 1 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Execute" -underline 1 \
	-menu $pad.filemenu.exec
    $pad.filemenu.exec add command -label "Load into Scilab" -underline 0\
	-command "execfile" -accelerator Ctrl+l
    $pad.filemenu.exec add command -label "Evaluate selection" -underline 0\
	-command "execselection" -accelerator Ctrl+y
} else {
    $pad.filemenu add cascade -label "Exécuter" -underline 1 \
	-menu $pad.filemenu.exec
    $pad.filemenu.exec add command -label "Charger dans Scilab" -underline 0\
	-command "execfile" -accelerator Ctrl+l
    $pad.filemenu.exec add command -label "Evaluer la sélection" -underline 0\
	-command "execselection" -accelerator Ctrl+y
}

#debug menu
#menu $pad.filemenu.debug -tearoff 1 -font $menuFont
#if {$lang == "eng"} {
#    $pad.filemenu add cascade -label "Debug" -underline 0 \
#	-menu $pad.filemenu.debug
#    $pad.filemenu.debug add command -label "Insert/Remove breakpoint" \
#      -underline 0 -command "insertremove_bp" -accelerator F9
#    $pad.filemenu.debug add command -label "Remove all breakpoints" \
#      -underline 7 -command "removeall_bp" -accelerator Ctrl+F9
#    $pad.filemenu.debug add separator
#    $pad.filemenu.debug add command -label "Configure execution" \
#      -underline 0 -command "configurefoo_bp" -accelerator F10
#    $pad.filemenu.debug add separator
#    $pad.filemenu.debug add command -label "Start execution with breakpoints" \
#      -underline 0 -command "execfile_bp" -accelerator Ctrl+F11
#    $pad.filemenu.debug add command -label "Go on up to next breakpoint" \
#      -underline 12 -command "resume_bp" -accelerator F11
#    $pad.filemenu.debug add command -label "Insert/Remove breakpoint during debug" \
#      -underline 18 -command "insertremovedebug_bp" -accelerator Shift+F11
#    $pad.filemenu.debug add command -label "Step by step" \
#      -underline 0 -command "stepbystep_bp" -accelerator F12 -state disabled
#    $pad.filemenu.debug add command -label "Go on ignoring any breakpoint" \
#      -underline 1 -command "goonwo_bp" -accelerator Shift+F12
#    $pad.filemenu.debug add command -label "Display call stack" \
#      -underline 6 -command "dispcallstack_bp"
#    $pad.filemenu.debug add separator
#    $pad.filemenu.debug add command -label "Show watch" \
#      -underline 5 -command "showwatch_bp" -accelerator Ctrl+F12
#    $pad.filemenu.debug add separator
#    $pad.filemenu.debug add command -label "Cancel debug" \
#      -underline 5 -command "canceldebug_bp"
#    $pad.filemenu.debug add command -label "Remove all breakpoints in Scilab" \
#      -underline 0 -command "removescilab_bp with_output"
#} else {
#    $pad.filemenu add cascade -label "Débug" -underline 0 \
#	-menu $pad.filemenu.debug
#    $pad.filemenu.debug add command -label "Insérer/Supprimer un point d'arrêt" \
#      -underline 0 -command "insertremove_bp" -accelerator F9
#    $pad.filemenu.debug add command -label "Supprimer tous les points d'arrêt" \
#      -underline 10 -command "removeall_bp" -accelerator Ctrl+F9
#    $pad.filemenu.debug add separator
#    $pad.filemenu.debug add command -label "Configurer l'exécution" \
#      -underline 3 -command "configurefoo_bp" -accelerator F10
#    $pad.filemenu.debug add separator
#    $pad.filemenu.debug add command -label "Démarrer l'exécution avec les points d'arrêt" \
#      -underline 0 -command "execfile_bp" -accelerator Ctrl+F11
#    $pad.filemenu.debug add command -label "Continuer jusqu'au prochain point d'arrêt" \
#      -underline 10 -command "resume_bp" -accelerator F11
#    $pad.filemenu.debug add command -label "Insérer/Supprimer un point d'arrêt pendant le debug" \
#      -underline 40 -command "insertremovedebug_bp" -accelerator Shift+F11
#    $pad.filemenu.debug add command -label "Pas à pas" \
#      -underline 0 -command "stepbystep_bp" -accelerator F12 -state disabled
#    $pad.filemenu.debug add command -label "Continuer sans aucun point d'arrêt" \
#      -underline 0 -command "goonwo_bp" -accelerator Shift+F12
#    $pad.filemenu.debug add command -label "Montrer la pile des appels" \
#      -underline 0 -command "dispcallstack_bp"
#    $pad.filemenu.debug add separator
#    $pad.filemenu.debug add command -label "Fenêtre watch" \
#      -underline 8 -command "showwatch_bp" -accelerator Ctrl+F12
#    $pad.filemenu.debug add separator
#    $pad.filemenu.debug add command -label "Annuler le débug" \
#      -underline 0 -command "canceldebug_bp"
#    $pad.filemenu.debug add command -label "Supprimer tous les points d'arrêt dans Scilab" \
#      -underline 0 -command "removescilab_bp with_output"
#}

# help menu
# FV 13/05/04, -accelerator Shift+F1 uncommented
menu $pad.filemenu.help -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Help" -underline 0 \
	-menu $pad.filemenu.help
    $pad.filemenu.help add command -label "About" -underline 0 \
	-command "aboutme" -accelerator Shift+F1
    $pad.filemenu.help add command -label "Help" -underline 0 \
        -command "helpme" -accelerator F1
    $pad.filemenu.help add command -label "What's?" -underline 0 \
	-command "helpword" -accelerator Ctrl+F1
} else {
    $pad.filemenu add cascade -label "Aide" -underline 0 \
	-menu $pad.filemenu.help
    $pad.filemenu.help add command -label "A propos" -underline 1 \
	-command "aboutme" -accelerator Shift+F1
    $pad.filemenu.help add command -label "Aide" -underline 0 \
        -command "helpme" -accelerator F1
    $pad.filemenu.help add command -label "Qu'est-ce ?" -underline 0 \
	-command "helpword" -accelerator Ctrl+F1
}

# now make the menu visible
$pad configure -menu $pad.filemenu 
####
##ES: remember fontsize
setfontscipad $FontSize
