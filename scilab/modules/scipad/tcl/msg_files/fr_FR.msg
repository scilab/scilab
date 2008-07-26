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

# Scipad texts in French by Francois Vogel
# in sync at least with v6.151


# commons
::msgcat::mcset fr_FR "Cancel" "Annuler"
::msgcat::mcset fr_FR "Cance&l" "Annu&ler"
::msgcat::mcset fr_FR "OK" "OK"
::msgcat::mcset fr_FR "All files" "Tous les fichiers"
::msgcat::mcset fr_FR "Add/Change" "Ajouter/Modifier"
::msgcat::mcset fr_FR "Add/Chan&ge" "Ajouter/&Modifier"
::msgcat::mcset fr_FR "&Remove" "&Supprimer"
::msgcat::mcset fr_FR "Rem&ove" "Supprime&r"
::msgcat::mcset fr_FR "Untitled" "SansTitre"
::msgcat::mcset fr_FR "Close" "Fermer"
::msgcat::mcset fr_FR "&Close" "&Fermer"
::msgcat::mcset fr_FR "Error " "Erreur "
::msgcat::mcset fr_FR "The file" "Le fichier"
::msgcat::mcset fr_FR "Function" "Fonction"
::msgcat::mcset fr_FR "Line" "Ligne"


# buffernavigation.tcl
::msgcat::mcset fr_FR "Hide" "Cacher"
::msgcat::mcset fr_FR "Close this tile, keep content" "Fermer cette vue, conserver le contenu"
::msgcat::mcset fr_FR "Close this tile" "Fermer cette vue"
::msgcat::mcset fr_FR "Goto Line?" "Aller à la ligne ?"
::msgcat::mcset fr_FR "Go to:" "Aller à : "
::msgcat::mcset fr_FR "&logical line" "la ligne &logique"
::msgcat::mcset fr_FR "&physical line" "la ligne &physique"
::msgcat::mcset fr_FR "in &function" "de la &fonction"
::msgcat::mcset fr_FR "in &current file" "du fichier &courant"
::msgcat::mcset fr_FR "Outside of function definition" "Hors de la définition de la fonction"


# colorize.tcl
::msgcat::mcset fr_FR "Wait seconds while recolorizing file" "Patientez un instant le temps de la colorisation"
::msgcat::mcset fr_FR "You can't do that while colorization is in progress.\
                    Please try again when finished." \
                   "Action interdite durant la colorisation. Veuillez réessayer plus tard."
::msgcat::mcset fr_FR "Command forbidden during colorization" "Commande interdite durant la colorisation"


# completion.tcl
::msgcat::mcset fr_FR "No possible completion" "Aucune terminaison possible"
::msgcat::mcset fr_FR "possible completions" "terminaisons possibles"
::msgcat::mcset fr_FR "Possible completions" "Terminaisons possibles"


# db_argsmanagement.tcl
::msgcat::mcset fr_FR "Add" "Ajouter"
::msgcat::mcset fr_FR "This variable can be watched but cannot be edited!" \
                   "Cette variable peut être espionnée mais ne peut pas être modifiée !"
::msgcat::mcset fr_FR "A new watch variable will be created." \
                   "Une nouvelle variable espion va être créée."
::msgcat::mcset fr_FR "Non editable variable" "Variable non éditable"
::msgcat::mcset fr_FR "Variable:" "Variable :"
::msgcat::mcset fr_FR "Expression:" "Expression :"
::msgcat::mcset fr_FR "Value:" "Valeur :"
::msgcat::mcset fr_FR "&Get current value from Scilab" "Obtenir la valeur &actuelle dans Scilab"
::msgcat::mcset fr_FR "Non watchable variable" "Variable non espionnable"
::msgcat::mcset fr_FR "Sorry, variable" "Désolé, la variable"
    ::msgcat::mcset fr_FR "is special and cannot be watched." "est spéciale et ne peut pas être espionnée."


# db_bptsgui.tcl
::msgcat::mcset fr_FR "Breakpoints properties" "Propriétés des points d'arrêt"
::msgcat::mcset fr_FR "Enabled" "Activé"
::msgcat::mcset fr_FR "Expression" "Expression"
::msgcat::mcset fr_FR "Hit if" "Accédé si"
::msgcat::mcset fr_FR istrue "est vraie"
::msgcat::mcset fr_FR haschanged "a changé"
::msgcat::mcset fr_FR "Watched" "Espionné"
::msgcat::mcset fr_FR "Hit count" "Nb d'accès"
::msgcat::mcset fr_FR "Break if" "Stoppe si"
::msgcat::mcset fr_FR "Threshold" "Seuil"
::msgcat::mcset fr_FR always "toujours"
::msgcat::mcset fr_FR equalto "est égal à"
::msgcat::mcset fr_FR multipleof "est un multiple de"
::msgcat::mcset fr_FR greaterthan "est supérieur à"
::msgcat::mcset fr_FR lessthan "est inférieur à"
::msgcat::mcset fr_FR "Reset hit" "RàZ accès"
::msgcat::mcset fr_FR "Breakpoints vocabulary" "Vocabulaire des points d'arrêt"
::msgcat::mcset fr_FR "A breakpoint is identified by its location, i.e. its position (a line number) in a function." \
                   "Un point d'arrêt est identifié par sa position, i.e. son numéro de ligne au sein d'une fonction."
::msgcat::mcset fr_FR "When the execution process encounters an enabled breakpoint, then this breakpoint is said to be reached." \
                   "Lorsque l'exécution du code rencontre un point d'arrêt activé, alors ce point d'arrêt est dit atteint."
::msgcat::mcset fr_FR "When the breakpoint is reached and its associated conditional expression is true (or changed),\
                    then the breakpoint is said to be hit." \
                   "Lorsqu'un point d'arrêt est atteint et que son expression conditionnelle est vraie (ou modifiée),\
                    alors le point d'arrêt est dit accédé."
::msgcat::mcset fr_FR "The hit count is the number of times the breakpoint has been hit." \
                   "Le nombre d'accès est le nombre de fois que le point d'arrêt a été accédé."
::msgcat::mcset fr_FR "Execution will stop at a breakpoint if the hit count satisfies the selectable break condition." \
                   "L'exécution s'arrêtera sur un point d'arrêt si le nombre d'accès à ce point d'arrêt satisfait la condition d'arrêt."


# db_bptsmanagement.tcl
::msgcat::mcset fr_FR "No breakpoint here!" "Pas de point d'arrêt ici !"
::msgcat::mcset fr_FR "Delete all breakpoints" "Supprimer tous les points d'arrêt"
::msgcat::mcset fr_FR "Are you sure you want to delete all the existing breakpoints?" \
                   "Etes-vous sûr de vouloir supprimer tous les points d'arrêt ?"
::msgcat::mcset fr_FR "All breakpoints deleted" "Points d'arrêt supprimés"
::msgcat::mcset fr_FR "No action taken" "Aucun changement effectué"
::msgcat::mcset fr_FR "No breakpoint!" "Aucun point d'arrêt !"


# db_configureexec.tcl
::msgcat::mcset fr_FR "Configure execution" "Configurer l'exécution"
::msgcat::mcset fr_FR "Function name:" "Nom de la fonction :"
::msgcat::mcset fr_FR "&Obtain" "&Obtenir"
::msgcat::mcset fr_FR "Input arguments:" "Arguments d'entrée :"
::msgcat::mcset fr_FR "The selected function has not been found in the opened files, maybe it was deleted since last configuration of the debugger.\nUse button \"" \
                   "La fonction sélectionnée n'a pas pu être trouvée dans les fichiers ouverts, peut-être a-t-elle été supprimée depuis la précédente configuration du débuggueur.\nUtiliser le bouton \""
    ::msgcat::mcset fr_FR "\" to refresh the list of available functions." \
                       "\" pour rafraîchir la liste des fonctions disponibles."
::msgcat::mcset fr_FR "Function name or input arguments do not match definition of the function" \
                    "Le nom de la fonction ou ses arguments ne correspondent pas à la définition de la fonction"
    ::msgcat::mcset fr_FR "in the file!\n\nCheck function\
                        name and arguments (names, order) in the configuration dialog.\
                        \nArguments order can be changed using drag and drop with\
                        right mouse button in the arguments listbox." \
                       "dans le fichier!\n\n\
                        Vérifier le nom de la fonction et ses arguments (nom, ordre\
                        d'apparition) dans la boîte de dialogue de configuration.\n\
                        L'ordre des arguments peut être modifié par glisser/déposer\
                        avec le bouton droit de la souris."
::msgcat::mcset fr_FR "Error in selected function name or arguments" "Erreur sur la fonction sélectionnée ou ses arguments"
::msgcat::mcset fr_FR "At least one input argument does not match definition of the function" \
                    "Au moins un argument ne correspond pas à la définition de la fonction"
    ::msgcat::mcset fr_FR "in the file!\n\nCheck function\
                        arguments (their names) in the configuration dialog."\
                       "dans le fichier!\n\n\
                        Vérifier les arguments de la fonction (leur noms)\
                        dans la boîte de dialogue de configuration."
::msgcat::mcset fr_FR "Error in selected function arguments" "Erreur sur les arguments de la fonction sélectionnée"
::msgcat::mcset fr_FR "Main level code found" "Présence de code hors fonction"
::msgcat::mcset fr_FR "Scipad ancillary file" "Fichier de sous-routines de Scipad"
::msgcat::mcset fr_FR "The current file contains only Scipad ancillaries that cannot be debugged in Scipad." \
                   "Le fichier actuel ne contient que des sous-routines utilisées par Scipad, et qui ne peuvent pas être débugguées dans Scipad."
::msgcat::mcset fr_FR "Reserved function names are:\n" "Les noms réservés sont:\n"
::msgcat::mcset fr_FR "See help scipad for further details." "Veuillez consulter l'aide (help scipad) pour plus de détails."
::msgcat::mcset fr_FR "This file contains main level code.\nDebug as a single .sce file?" \
                   "Ce fichier contient du code hors définition de fonction.\nDébugguer en tant qu'unique entité .sce ?"
::msgcat::mcset fr_FR "No function found" "Aucune fonction trouvée"
::msgcat::mcset fr_FR "This file contains no function and will be treated" \
                   " Ce fichier ne contient aucune fonction et sera traité"
    ::msgcat::mcset fr_FR "as a single .sce file.\nStart debugging?" "en tant qu'unique entité .sce.\nCommencer le débug ?"
::msgcat::mcset fr_FR "You must enable colorization to be able to launch the debugger!" \
                   "Vous devez autoriser la colorisation pour lancer le débuggueur !"
::msgcat::mcset fr_FR "The debugger needs colorization" \
                   "Le débuggueur a besoin de la colorisation"
::msgcat::mcset fr_FR "Warning!\n\n \
                 A function missing the closing \"endfunction\" has been found in the currently opened files.\n \
                 Such syntax is now obsolete.\n\n \
                 Please terminate function " \
                   " Attention !\n\n \
                 Une fonction sans \"endfunction\" a été trouvée dans les fichiers actuellement ouverts.\n \
                 Cette syntaxe est obsolète.\n\n \
                 Veuillez terminer la fonction "
::msgcat::mcset fr_FR " and try again." " et essayer une nouvelle fois."
::msgcat::mcset fr_FR "Note: This message might also be triggered by a string\n \
                 quoted with single quotes and containing the word \"function\",\n \
                 when \"Colorize \'strings\'\" is unchecked in the Options menu." \
                   "Nota : Ce message peut aussi être dû à une chaîne de caractères\n \
                 délimitée par des guillemets simples et contenant le mot \"function\",\n \
                 lorsque \"Colorer \'chaîne\'\" n'est pas coché dans le menu Options."
::msgcat::mcset fr_FR "This known bug can be worked around simply by checking \"Colorize \'&strings\'\"." \
                   "Ce bug connu peut être évité simplement en cochant l'option \"Colorer \'chaîne\'\"."
::msgcat::mcset fr_FR "Unterminated function definition found" "Fonction non terminée identifiée"
::msgcat::mcset fr_FR "Warning!\n\n \
                 One or more function is defined more than once in the currently opened files.\n \
                 The debugger cannot cope with multiple copies of functions.\n \
                 The following duplicates were detected:\n" \
                   "Attention !\n\n \
                 Une ou plusieurs fonctions sont définies plus d'une fois dans les fichiers actuellement ouverts.\n \
                 Le débuggueur ne supporte pas les copies multiples de fonctions.\n \
                 Les doublons suivants ont été détectés :\n"
::msgcat::mcset fr_FR "\n--> Definition of function " "\n--> Définition de la fonction "
::msgcat::mcset fr_FR " found in:\n" " trouvée dans :\n"
::msgcat::mcset fr_FR "Duplicate function definitions found" "Fonctions en doublon identifiées"


# db_debugsession.tcl
::msgcat::mcset fr_FR "Too many breakpointed functions" \
                   "Trop de fonctions contenant des points d'arrêt"
::msgcat::mcset fr_FR "You have currently" \
                   "Vos fichiers ouverts définissent actuellement"
    ::msgcat::mcset fr_FR "functions in your opened files." "fonctions."
    ::msgcat::mcset fr_FR "Scilab supports a maximum of" "Scilab supporte au maximum"
    ::msgcat::mcset fr_FR "possible breakpointed functions (see help setbpt)." \
                       "functions avec points d'arrêts (voir help setbpt)."
    ::msgcat::mcset fr_FR "Step-by-step hence cannot be performed." \
                       "Le mode pas à pas n'est en conséquence pas possible."
    ::msgcat::mcset fr_FR "This command will actually run to the next breakpoint." \
                       "Cette commande va atteindre le prochain point d'arrêt."
    ::msgcat::mcset fr_FR "Break hence cannot be performed." \
                       "La commande Stop ne peut pas être exécutée."
::msgcat::mcset fr_FR "Too many breakpoints" "Trop de points d'arrêt"
::msgcat::mcset fr_FR "Executing this command would require to set" \
                   "Exécuter cette commande nécessiterait d'insérer"
    ::msgcat::mcset fr_FR "breakpoints in your opened files." \
                       "points d'arrêt dans vos fichiers ouverts."
    ::msgcat::mcset fr_FR "possible breakpoints in Scilab (see help setbpt)." \
                       "points d'arrêt (voir help setbpt)."
::msgcat::mcset fr_FR "Cursor must be in a function" \
                   "Le curseur doit être dans une fonction"
::msgcat::mcset fr_FR "This command cannot succeed if the function where the debug just stopped is not opened in Scipad!" \
                   "Le fichier dans lequel le débug vient de stopper doit être ouvert pour exécuter cette commande !"
::msgcat::mcset fr_FR "Cannot run to return point" \
                   "Impossible d'exécuter jusqu'au point de retour"
::msgcat::mcset fr_FR "Warning!\nThe line where the debugger just stopped contains a return statement (either endfunction, return or resume) but this statement is not the first one on the line." \
                   "Attention !\nLa ligne sur laquelle le débuggueur vient de s'arrêter contient une instruction de retour (endfunction ou bien return ou bien resume), mais cette instruction n'est pas la première sur la ligne."
::msgcat::mcset fr_FR "As a consequence the debugger cannot predict if the next debug step will really jump outside of the current function, which is what you just asked the debugger to do." \
                   "Par conséquent le débuggueur ne peut pas prédire si le prochain pas de débug va faire sortir de la fonction en cours (i.e. ce que vous venez de demander de réaliser)."
::msgcat::mcset fr_FR "To avoid this unresolvable case, it is recommended to place the return statement alone on its line." \
                   "Pour éviter ce cas indécidable, il est recommandé de placer chaque instruction de retour seule sur sa ligne."
::msgcat::mcset fr_FR "Do you want to see this warning again during this Scipad session?" \
                   "Voulez-vous voir ce message à nouveau durant cette session Scipad?"
::msgcat::mcset fr_FR "Unresolvable case found while running to return point" \
                   "Cas indécidable identifié durant l'exécution jusqu'au point de retour"
::msgcat::mcset fr_FR "No effect - The debugged file is not stuck" \
                   "Aucun effet - Le fichier en débug n'est pas bloqué"


# db_init.tcl
::msgcat::mcset fr_FR "Please wait..." "Veuillez patienter..."


# db_states.tcl
::msgcat::mcset fr_FR "Currently no debug session" "Pas de session de débug en cours"
::msgcat::mcset fr_FR "Ready to start debug" "Prêt pour le débug"
::msgcat::mcset fr_FR "Debug in progress" "Débug en cours"
::msgcat::mcset fr_FR "Scilab is back at the main level now." \
                   "Scilab est de retour au niveau principal."


# db_watch.tcl
::msgcat::mcset fr_FR "Watch" "Espions"
::msgcat::mcset fr_FR "Watch variables:" "Variables espions :"
::msgcat::mcset fr_FR "A&uto (local variables)" "A&uto (variables locales)"
::msgcat::mcset fr_FR "Au&to watch globals too" "Espionner aussi les &globales"
::msgcat::mcset fr_FR "Generic expressions:" "Expressions génériques :"
::msgcat::mcset fr_FR "Call stack:" "Pile des appels :"
::msgcat::mcset fr_FR "Always on to&p" "Toujours à l'&avant-plan"
::msgcat::mcset fr_FR "Hide watch &variables" "Cacher les &variables"
::msgcat::mcset fr_FR "Hide e&xpressions" "Cacher les e&xpressions"
::msgcat::mcset fr_FR "Hide call &stack" "Cacher la &pile d'appel"
::msgcat::mcset fr_FR "&Dock" "&Intégrer"
::msgcat::mcset fr_FR "&Add" "Ajout&er"
::msgcat::mcset fr_FR "&Filter errors" "Fi&ltrer les erreurs"
::msgcat::mcset fr_FR "Breakpoints" "Points d'arrêt"
::msgcat::mcset fr_FR "Breakpointed functions" "Fonctions avec breakpoints"
::msgcat::mcset fr_FR "Scilab busy indicator" "Indicateur d'occupation de Scilab"
::msgcat::mcset fr_FR "Debugger state indicator" "Indicateur de l'état du débuggueur"
::msgcat::mcset fr_FR "Undefined elements are not legal in variable" "Les éléments indéfinis sont illégaux pour la variable"
::msgcat::mcset fr_FR ".\nThis variable will not be updated in Scilab." ".\nCette variable ne sera pas mise à jour dans Scilab"
::msgcat::mcset fr_FR "Illegal undefined element found" "Elément indéfini illégal identifié"
::msgcat::mcset fr_FR "evaluates to:" "est évalué en:"
::msgcat::mcset fr_FR "produces error" "résulte en une erreur"
::msgcat::mcset fr_FR "Scipad cannot find source code for the selected function.\nPlease open the adequate file manually and try again." \
                   "Scipad ne trouve pas le code source de la fonction sélectionnée.\nVeuillez ouvrir le fichier manuellement et réessayer."
::msgcat::mcset fr_FR "Missing source code" "Code source manquant"


# filecommands.tcl
::msgcat::mcset fr_FR "New script" "Nouveau script"
::msgcat::mcset fr_FR "New view on the same file" "Nouvelle vue sur ce fichier"
::msgcat::mcset fr_FR "The contents of" "Voulez-vous enregistrer les modifications apportées à"
    ::msgcat::mcset fr_FR "may have changed, do you wish to save your changes?" "?"
::msgcat::mcset fr_FR "Save Confirm?" "Confirmer sauver ?"
::msgcat::mcset fr_FR "Open source of..." "Ouvrir le code source de..."
::msgcat::mcset fr_FR "Open source of:" "Ouvrir le source de :"
::msgcat::mcset fr_FR "Weird drag&drop" "Glisser&déposer inattendu"
::msgcat::mcset fr_FR "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                    "n'est ni un nom de fichier valide ni un nom de répertoire\
                     valide. Soit vous avez tenté de glisser un objet d'un \
                     type différent, soit c'est un bug du mécanisme de \
                     glisser&déposer."
::msgcat::mcset fr_FR "File not found" "Fichier introuvable"
::msgcat::mcset fr_FR "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "n'existe plus. Voulez-vous créer un nouveau fichier\
                    portant ce nom ?"
::msgcat::mcset fr_FR "Open file" "Ouvrir fichier"
#::msgcat::mcset fr_FR "The file" "Le fichier" ;# defined in the "commons" section
    ::msgcat::mcset fr_FR "is already opened! Save the current opened file to another name and reopen it from disk!" "est déjà ouvert! Sauvez-le sous un autre nom et rouvrez-le à partir du disque !"
::msgcat::mcset fr_FR "File" "Fichier"
    ::msgcat::mcset fr_FR "saved" "sauvegardé"
::msgcat::mcset fr_FR "cannot be written!" "est en lecture seule !"
::msgcat::mcset fr_FR "The contents of " "Le contenu de"
    ::msgcat::mcset fr_FR "has changed on disk, save it anyway?" "a changé sur le disque, êtes-vous sûr de vouloir le sauvegarder ?"
    ::msgcat::mcset fr_FR "has been modified outside of Scipad. Do you want to reload it?" \
                       "a été modifié hors de Scipad. Voulez-vous le recharger à partir du disque ?"
::msgcat::mcset fr_FR "File has changed!" "Le fichier a changé"
::msgcat::mcset fr_FR "Save as" "Enregistrer sous"
::msgcat::mcset fr_FR "Scilab files" "Fichiers Scilab"
::msgcat::mcset fr_FR "Scicos files" "Fichiers Scicos"
::msgcat::mcset fr_FR "XML files" "Fichiers XML"
::msgcat::mcset fr_FR "Wait seconds while loading and colorizing file" "Patientez un instant le temps du chargement et de la colorisation"
::msgcat::mcset fr_FR "The contents of this file may have changed, do you wish to save your changes?" "Voulez-vous enregistrer les modifications apportées à ce fichier ?"
::msgcat::mcset fr_FR "Revert" "Revenir à la version enregistrée de"
    ::msgcat::mcset fr_FR "to saved?" "?"
::msgcat::mcset fr_FR "Revert Confirm?" "Confirmer revenir ?"
::msgcat::mcset fr_FR "Unreadable file" "Fichier illisible!"
::msgcat::mcset fr_FR "exists but is not readable!" "existe mais ne peut pas être lu!"
::msgcat::mcset fr_FR "This file is rather large and colorization might take some time.\n\nColorize anyway?" \
                   "Ce fichier est de grande taille et sa colorisation peut être longue.\n\nColorer tout de même?"
::msgcat::mcset fr_FR "File size warning" "Avertissement relatif à la taille du fichier"
::msgcat::mcset fr_FR "Windows shortcut file" "Fichier raccourci Windows"
#::msgcat::mcset fr_FR "The file" "Le fichier" ;# defined in the "commons" section
    ::msgcat::mcset fr_FR "is a Windows shortcut and will not be opened!" \
                       "est un raccourci Windows et ne sera pas ouvert !"


# findreplace.tcl
::msgcat::mcset fr_FR "Dialog box already open!" "Boîte de dialogue déjà ouverte!"
::msgcat::mcset fr_FR "The string" "La chaîne"
    ::msgcat::mcset fr_FR "could not be found in" "n'a pu être trouvée dans"
    ::msgcat::mcset fr_FR "could not be found in any of the opened files !" "n'a pu être trouvée dans aucun des fichiers ouverts !"
::msgcat::mcset fr_FR "replacements done" "remplacements effectués"
::msgcat::mcset fr_FR "Find" "Rechercher"
::msgcat::mcset fr_FR "No match found in the selection for" "La chaîne"
    ::msgcat::mcset fr_FR "\nWould you like to look for it in the entire text?" "n'a pu être trouvée dans la sélection.\nVoulez-vous rechercher dans la totalité du texte ?"
::msgcat::mcset fr_FR "You are searching for an empty string!" "La chaîne à rechercher est vide!"
::msgcat::mcset fr_FR "Re&place" "&Remplacer"
::msgcat::mcset fr_FR "Find what:" "Rechercher :"
::msgcat::mcset fr_FR "Replace with:" "Remplacer par :"
::msgcat::mcset fr_FR "Find &Next" "Rechercher &suivant"
::msgcat::mcset fr_FR "Replace &All" "Remplacer &tout"
::msgcat::mcset fr_FR "Direction" "Sens"
::msgcat::mcset fr_FR "&Upwards" "Vers le &haut"
::msgcat::mcset fr_FR "&Downwards" "Vers le &bas" 
::msgcat::mcset fr_FR "Text types" "Types de texte"
::msgcat::mcset fr_FR "Find options" "Options de recherche"
::msgcat::mcset fr_FR "Match &whole word only" "Mot &entier seulement"
::msgcat::mcset fr_FR "Match &case" "Respecter la &casse"
::msgcat::mcset fr_FR "&Regular expression" "E&xpression régulière"
::msgcat::mcset fr_FR "In all &opened files" "Dans tous les fichiers &ouverts"
::msgcat::mcset fr_FR "In &selection only" "Da&ns la sélection seulement"
::msgcat::mcset fr_FR "In typed te&xt" "Dans des t&ypes de texte"
::msgcat::mcset fr_FR "In a director&y" "Dans un réper&toire"
::msgcat::mcset fr_FR "Find in files search options" "Options de recherche dans un répertoire"
::msgcat::mcset fr_FR "Directory r&ecurse search" "Recherche réc&ursive"
::msgcat::mcset fr_FR "Include &hidden files" "Inclure les fichiers c&achés"
::msgcat::mcset fr_FR "In files/file types:" "Dans les fichiers/types :"
::msgcat::mcset fr_FR "In directory:" "Dans le répertoire :"
::msgcat::mcset fr_FR "Search for file names" "Rechercher dans les noms des fichiers"
::msgcat::mcset fr_FR "You have given an empty search string. Are you sure you want to search among file names?" \
                   "La chaîne à rechercher est vide. Etes-vous sûr de vouloir rechercher parmi les noms des fichiers ?"
::msgcat::mcset fr_FR "Wrapped around" "Rebouclé"
::msgcat::mcset fr_FR "Back at the first file searched!" "Revenu au premier fichier !"
::msgcat::mcset fr_FR "No more matches" "Pas d'autre correspondance"
::msgcat::mcset fr_FR "The string to regexp search for cannot be compiled by Tcl.\nCheck again, maybe certain characters should be escaped." \
                   "L'expression régulière entrée est invalide (impossible à compiler).\nVérifiez, il se peut que le caractère d'échappement \\ doive précéder certains caractères."
::msgcat::mcset fr_FR "Any single character" "Un caractère quelconque"
::msgcat::mcset fr_FR "Zero or more" "Zéro ou plus"
::msgcat::mcset fr_FR "One or more" "Un ou plus"
::msgcat::mcset fr_FR "Zero or one" "Zéro ou un"
::msgcat::mcset fr_FR "Any one character in the set" "Un caractère quelconque de l'ensemble"
::msgcat::mcset fr_FR "Any one character not in the set" "Un caractère quelconque hors de l'ensemble"
::msgcat::mcset fr_FR "Or" "Ou"
::msgcat::mcset fr_FR "Beginning of line" "Début de ligne"
::msgcat::mcset fr_FR "End of line" "Fin de ligne"
::msgcat::mcset fr_FR "Beginning of word" "Début de mot"
::msgcat::mcset fr_FR "End of word" "Fin de mot"
::msgcat::mcset fr_FR "A letter" "Une lettre"
::msgcat::mcset fr_FR "A decimal digit" "Un chiffre décimal"
::msgcat::mcset fr_FR "An alphanumeric (letter or digit)" "Un caractère alphanumérique (lettre ou chiffre)"
::msgcat::mcset fr_FR "A space or tab character" "Un espace ou une tabulation"
::msgcat::mcset fr_FR "A rational number" "Un nombre rationel"
::msgcat::mcset fr_FR "A floating point number" "Un nombre en virgule flottante"
::msgcat::mcset fr_FR "A Scilab comment" "Un commentaire Scilab"
::msgcat::mcset fr_FR "A Scilab name" "Un nom Scilab"
::msgcat::mcset fr_FR "A Scilab continued line (with dots)" "Une ligne continuée Scilab (avec points de suite)"


# findinfiles.tcl
::msgcat::mcset fr_FR "Directory access issue" "Erreur d'accès à un répertoire"
::msgcat::mcset fr_FR "The directory you specified cannot be reached. The search won't bring any result." \
                   "Scipad ne peut pas accéder au répertoire spécifié. La recherche ne donnera aucun résultat."
::msgcat::mcset fr_FR "Some directories or files could not be open and were ignored during search.\n\
                    You might miss read access to them.\n\n\
                    The following ones were ignored" \
                   "Certains répertoires ou fichiers n'ont pas pu être ouverts et ont été ignorés durant la recherche.\n\
                    Vous n'avez peut-être pas les droits de lecture sur eux.\n\n\
                    Les éléments suivants ont été ignorés"
::msgcat::mcset fr_FR "Ignored files" "Fichiers ignorés"
::msgcat::mcset fr_FR "Searching in file:" "Recherche dans :"
::msgcat::mcset fr_FR "End of file search:" "Fin de la recherche:"
::msgcat::mcset fr_FR "matches found" "correspondances trouvées"
::msgcat::mcset fr_FR "searched files" "fichiers examinés"
::msgcat::mcset fr_FR "Constructing file list:" "Construction de la liste des fichiers:"
::msgcat::mcset fr_FR "&Previous" "&Précédent"
::msgcat::mcset fr_FR "&Next" "&Suivant"
::msgcat::mcset fr_FR "Scipad search results" "Résultats de recherche Scipad"


# fonts.tcl
::msgcat::mcset fr_FR "Choose font" "Choisir une police"
::msgcat::mcset fr_FR "Family" "Police"
::msgcat::mcset fr_FR "Style" "Style"
::msgcat::mcset fr_FR "&Bold" "&Gras"
::msgcat::mcset fr_FR "&Italic" "&Italique"
::msgcat::mcset fr_FR "&Underline" "&Souligné"
::msgcat::mcset fr_FR "Size" "Taille"
::msgcat::mcset fr_FR "&Defaults" "Par &défaut"
::msgcat::mcset fr_FR "Use as..." "Utiliser pour..."
::msgcat::mcset fr_FR "&Text font" "Police de &texte"
::msgcat::mcset fr_FR "&Menu font" "Police de &menu"
::msgcat::mcset fr_FR "Sample" "Exemple"
::msgcat::mcset fr_FR "Font size" "Police taille"
::msgcat::mcset fr_FR "Revert fonts" "Revenir aux polices par défaut"
::msgcat::mcset fr_FR "Are you sure you want to revert all fonts to defaults?" \
                   "Voulez-vous vraiment revenir aux polices par défaut ?"


# FormatStringsForWatch.sci
::msgcat::mcset fr_FR " what you try to watch is not supported by the debugger" \
                   " ce que vous essayez d'espionner n'est pas supporté par le débuggueur"
::msgcat::mcset fr_FR " what you try to watch is of type " \
                   " ce que vous tentez d'espionner est de type "
    ::msgcat::mcset fr_FR " - this type is not supported by the debugger" \
                       " - ce type n'est pas supporté par le débuggueur"
::msgcat::mcset fr_FR "Type:" "Type :"
::msgcat::mcset fr_FR "(unsupported)" "(non supporté)"
::msgcat::mcset fr_FR "size:" "taille :"
::msgcat::mcset fr_FR "real" "réel"
::msgcat::mcset fr_FR "complex" "complexe"
::msgcat::mcset fr_FR "elements" "éléments"
::msgcat::mcset fr_FR "graphic handle" "handle de graphique"
::msgcat::mcset fr_FR "uncompiled function" "fonction non compilée"
::msgcat::mcset fr_FR "compiled function" "fonction compilée"
::msgcat::mcset fr_FR "library" "librairie"
::msgcat::mcset fr_FR "pointer" "pointeur"
::msgcat::mcset fr_FR "size implicit index" "indice de taille implicite"
::msgcat::mcset fr_FR "primitive" "primitive"


# FormatWhereForWatch.sci
::msgcat::mcset fr_FR "Breakpoint" "Point d'arrêt"
::msgcat::mcset fr_FR "called at line" "appelé à la ligne "
::msgcat::mcset fr_FR "of exec file" "dans un fichier exec"
::msgcat::mcset fr_FR "of execstr instruction" "dans une instruction execstr"
::msgcat::mcset fr_FR "called under pause" "appelé sous pause"
::msgcat::mcset fr_FR "of macro" "de la macro"


# helps.tcl
::msgcat::mcset fr_FR "About" "A propos"
::msgcat::mcset fr_FR aboutme_message \
                   "\n Créé par Joseph Acosta, joeja@mindspring.com.\n\
                    Modifié par le Consortium Scilab.\n\
                    Amélioré par Enrico Segre 2003-2006.\n\
                    Améliorations diverses, dont outils de débug, par François Vogel 2004-2008.\n\
                    Traductions :\n\
                     . Suédois, Gunnar Lindholm 2005\n\
                     . Norvégien, Torbjørn Pettersen 2005\n\
                     . Allemand, Dirk Reusch 2005\n\
                     . Polonais, Bartosz Kosiorek 2005\n\
                     . Espagnol, Juan Nicolás Quesada 2006\n\
                     . Chinois, Yung-Jang Lee 2007\n\
                     . Danois, Kim B. Hansen 2007."
::msgcat::mcset fr_FR Scipad_is_released_under_GPL "SciPad est un logiciel sous licence GPL"


# infomessages.tcl
::msgcat::mcset fr_FR "Line:" "Ligne:"
    ::msgcat::mcset fr_FR "Column:" "Colonne:"
#::msgcat::mcset fr_FR "Line" "Ligne" ; # now common
    ::msgcat::mcset fr_FR "in" "de"
::msgcat::mcset fr_FR "Logical line:" "Ligne logique:"
::msgcat::mcset fr_FR " \[ReadOnly\]" " \[LectureSeule\]"
::msgcat::mcset fr_FR " (modified)" " (modifié)"


# inputtext.tcl
::msgcat::mcset fr_FR "No corresponding <" "Aucun <"
    ::msgcat::mcset fr_FR "> found!" "> correspondant trouvé !"
::msgcat::mcset fr_FR "Code editing is not allowed during debug!" "Impossible de modifier le code durant le débug !"


# menues.tcl
::msgcat::mcset fr_FR "&File" "&Fichier"
::msgcat::mcset fr_FR "&Edit" "&Edition"
::msgcat::mcset fr_FR "&Search" "&Rechercher"
::msgcat::mcset fr_FR "E&xecute" "E&xécuter"
::msgcat::mcset fr_FR "&Debug" "&Débug"
::msgcat::mcset fr_FR "S&cheme" "&Langage"
::msgcat::mcset fr_FR "&Options" "&Options"
::msgcat::mcset fr_FR "&Windows" "Fe&nêtres"
::msgcat::mcset fr_FR "&Help" "&Aide"

    # menues.tcl - File menu
    ::msgcat::mcset fr_FR "&New" "&Nouveau"
    ::msgcat::mcset fr_FR "&Open..." "&Ouvrir..."
    ::msgcat::mcset fr_FR "Open &in new" "O&uvrir dans une nouvelle fenêtre"
        ::msgcat::mcset fr_FR "&horizontal tile" "&horizontale"
        ::msgcat::mcset fr_FR "&vertical tile" "&verticale"
    ::msgcat::mcset fr_FR "Op&en source of..." "Ouvrir le &code source de..."
    ::msgcat::mcset fr_FR "&Save" "&Enregistrer"
    ::msgcat::mcset fr_FR "Save &as..." "En&registrer sous..."
    ::msgcat::mcset fr_FR "Save a&ll" "Enregistrer &tout"
    ::msgcat::mcset fr_FR "&Revert..." "Re&venir..."
    ::msgcat::mcset fr_FR "Import &Matlab file..."  "Importer fichier &Matlab..."
    ::msgcat::mcset fr_FR "Create help s&keleton..." "Créer un &squelette d'aide XML..."
    ::msgcat::mcset fr_FR "Compile as &help page" "Compiler comme fichier d'&aide"
    ::msgcat::mcset fr_FR "Open &function source" "Ouvrir le source &de la fonction"
    ::msgcat::mcset fr_FR "Print Se&tup" "Mise en pa&ge"
    ::msgcat::mcset fr_FR "&Print" "&Imprimer"
    ::msgcat::mcset fr_FR "&Close file" "&Fermer ce fichier"
    ::msgcat::mcset fr_FR "E&xit" "&Quitter"

    # menues.tcl - Edit menu
    ::msgcat::mcset fr_FR "&Undo" "&Annuler"
    ::msgcat::mcset fr_FR "&Redo" "&Répéter"
    ::msgcat::mcset fr_FR "Cu&t" "Co&uper"
    ::msgcat::mcset fr_FR "B&lock cut" "Couper (mode bl&oc)"
    ::msgcat::mcset fr_FR "&Copy" "&Copier"
    ::msgcat::mcset fr_FR "&Paste" "Co&ller"
    ::msgcat::mcset fr_FR "&Block paste" "Coller (mode &bloc)"
    ::msgcat::mcset fr_FR "&Delete" "&Effacer"
    ::msgcat::mcset fr_FR "Select &All" "Sélectionner &tout"
    ::msgcat::mcset fr_FR "Insert &Time/Date" "Insérer Heure/Date"
    ::msgcat::mcset fr_FR "Co&mment selection" "Co&mmenter la sélection"
    ::msgcat::mcset fr_FR "U&ncomment selection" "&Décommenter la sélection"
    ::msgcat::mcset fr_FR "&Indent selection" "&Indenter la sélection"
    ::msgcat::mcset fr_FR "Unin&dent selection" "Dé&sindenter la sélection"

    # menues.tcl - Search menu
    ::msgcat::mcset fr_FR "&Find..." "&Rechercher..."
    ::msgcat::mcset fr_FR "Find &Next" "Rechercher &suivant"
    ::msgcat::mcset fr_FR "&Replace..." "Rem&placer..."
    ::msgcat::mcset fr_FR "&Goto Line..." "&Atteindre..."

    # menues.tcl - Exec menu
    ::msgcat::mcset fr_FR "&Load into Scilab" "&Charger dans Scilab"
    ::msgcat::mcset fr_FR "&Evaluate selection" "&Evaluer la sélection"

    # menues.tcl - Debug menu
    ::msgcat::mcset fr_FR "&Insert/Remove breakpoint" "&Insérer/Supprimer un point d'arrêt"
    ::msgcat::mcset fr_FR "&Edit breakpoints" "&Editer les points d'arrêt"
    ::msgcat::mcset fr_FR "Remove &all breakpoints" "Supprimer &tous les points d'arrêt"
    ::msgcat::mcset fr_FR "&Configure execution..." "Con&figurer l'exécution..."
    ::msgcat::mcset fr_FR "Go to next b&reakpoint" "Aller jusq&u'au prochain point d'arrêt"
    ::msgcat::mcset fr_FR "&Step by step" "&Pas à pas"
    ::msgcat::mcset fr_FR "Step &into" "Pas à pas &détaillé"
    ::msgcat::mcset fr_FR "Step o&ver" "Pas à pas &principal"
    ::msgcat::mcset fr_FR "Step &out" "Pas à pas &sortant"
    ::msgcat::mcset fr_FR "Run to re&turn point" "Exécuter jusqu'au point de &retour"
    ::msgcat::mcset fr_FR "Run to c&ursor" "Exécuter jusqu'au cur&seur"
    ::msgcat::mcset fr_FR "G&o on ignoring any breakpoint" "&Continuer sans aucun point d'arrêt"
    ::msgcat::mcset fr_FR "Show &watch" "Fenêtre &watch"
    ::msgcat::mcset fr_FR "&Break" "&Stop"
    ::msgcat::mcset fr_FR "Cance&l debug" "&Annuler le débug"

#    ::msgcat::mcset fr_FR "language scheme" "schéma langage"
    # menues.tcl - Scheme menu
    ::msgcat::mcset fr_FR "S&cilab" "S&cilab"
    ::msgcat::mcset fr_FR "&XML" "&XML"
    ::msgcat::mcset fr_FR "&none" "&aucun"
    ::msgcat::mcset fr_FR "&Colorize" "&Colorer"

    # menues.tcl - Options menu
    ::msgcat::mcset fr_FR "&Fonts..." "&Polices..."
    ::msgcat::mcset fr_FR "&Colors" "&Couleurs"
    ::msgcat::mcset fr_FR "Colori&ze" "C&olorer"
        ::msgcat::mcset fr_FR "&Always" "&Toujours"
        ::msgcat::mcset fr_FR "Ask for &big files" "&Demander pour les gros fichiers"
        ::msgcat::mcset fr_FR "&Never" "&Jamais"
        ::msgcat::mcset fr_FR "Colorize \'&strings\'" "Colorer \'&chaîne\'"
        ::msgcat::mcset fr_FR "Colorize &user functions" "Colorer les fonctions &utilisateur"
        ::msgcat::mcset fr_FR "Show c&ontinued lines" "Surligner les li&gnes incomplètes"
    ::msgcat::mcset fr_FR "Line n&umbers" "N&uméros de ligne"
        ::msgcat::mcset fr_FR "&Hide" "&Masqués"
        ::msgcat::mcset fr_FR "&Left aligned" "Alignés à &gauche"
        ::msgcat::mcset fr_FR "&Right aligned" "Alignés à &droite"
    ::msgcat::mcset fr_FR "Word &wrap" "&Retour à la ligne automatique"
    ::msgcat::mcset fr_FR "&Double-click behavior" "Comportement du &double-clic"
        ::msgcat::mcset fr_FR "&Linux" "&Linux"
        ::msgcat::mcset fr_FR "Windo&ws" "Windo&ws"
        ::msgcat::mcset fr_FR "&Scilab" "&Scilab"
    ::msgcat::mcset fr_FR "&Tabs and indentation" "Tabs et &indentation"
        ::msgcat::mcset fr_FR "Tab inserts &spaces" "Tab insère des &espaces"
        ::msgcat::mcset fr_FR "&Indentation spaces" "&Dimension de l'indentation (espaces)"
        ::msgcat::mcset fr_FR "&Tab size (characters)" "&Taille d'une tabulation (caractères)"
        ::msgcat::mcset fr_FR "Use &keyword indentation" "Indentation sur &mot-clés"
    ::msgcat::mcset fr_FR "Com&pletion" "Ter&minaison"
        ::msgcat::mcset fr_FR "&Tab" "&Tab"
        ::msgcat::mcset fr_FR "&Control-Tab" "&Control-Tab"
        ::msgcat::mcset fr_FR "&Alt-Tab" "&Alt-Tab"
        ::msgcat::mcset fr_FR "&Shift-Tab" "&Shift-Tab"
        ::msgcat::mcset fr_FR "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
        ::msgcat::mcset fr_FR "S&hift-Control-Tab" "S&hift-Control-Tab"
        ::msgcat::mcset fr_FR "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
        ::msgcat::mcset fr_FR "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
    ::msgcat::mcset fr_FR "File&names" "&Noms de fichiers"
        ::msgcat::mcset fr_FR "&Full path" "&Complets"
        ::msgcat::mcset fr_FR "Full path if &ambiguous" "Complets si &ambigus"
        ::msgcat::mcset fr_FR "&Unambiguous pruned path" "&Raccourcis non ambigus"
    ::msgcat::mcset fr_FR  "Windows menu &sorting" "&Tri dans le menu Fenêtres"
        ::msgcat::mcset fr_FR "&File opening order" "Ordre d'ouverture des &fichiers"
        ::msgcat::mcset fr_FR "&Alphabetically" "&Alphabétique"
        ::msgcat::mcset fr_FR "&Most recently used" "Plus &récemment utilisé"
    ::msgcat::mcset fr_FR "&Recent files" "&Fichiers récents"
    ::msgcat::mcset fr_FR "&Backup files depth" "Profond&eur de sauvegarde"
    ::msgcat::mcset fr_FR "&Locale" "&Langue"
    ::msgcat::mcset fr_FR "Execution &errors" "E&rreurs d'exécution"
        ::msgcat::mcset fr_FR "In Scilab &shell only" "Dans la fenêtre &Scilab seulement"
        ::msgcat::mcset fr_FR "Copied in a &message box" "Recopiées dans une &boîte de dialogue"

    # menues.tcl - Windows menu
    ::msgcat::mcset fr_FR "&Maximize" "&Maximiser"
    ::msgcat::mcset fr_FR "&Split" "&Diviser"
    ::msgcat::mcset fr_FR "S&plit (side by side)" "Diviser (&côte à côte)"
    ::msgcat::mcset fr_FR "Sp&lit file" "D&iviser le fichier"
    ::msgcat::mcset fr_FR "Spl&it file (side by side)" "Diviser le fichier (cô&te à côte)"
    ::msgcat::mcset fr_FR "Tile all &vertically" "Aligner &verticalement"
    ::msgcat::mcset fr_FR "Tile all &horizontally" "Aligner &horizontalement"
    ::msgcat::mcset fr_FR "Space sashes &evenly" "Homo&généiser"
    ::msgcat::mcset fr_FR "Opened Files" "Fichiers ouverts"

    # menues.tcl - Help menu
    ::msgcat::mcset fr_FR "&Help..." "&Aide..."
    ::msgcat::mcset fr_FR "&What's?..." "&Qu'est-ce ?..."
    ::msgcat::mcset fr_FR "&About" "A &propos"
    ::msgcat::mcset fr_FR "Adding &translations..." "&Traduire Scipad..."
        ::msgcat::mcset fr_FR "Adding Scipad Translations..." "Ajouter une nouvelle traduction de Scipad..."
    ::msgcat::mcset fr_FR "&Bugs \& wishlist" "&Bugs \& demandes"
        ::msgcat::mcset fr_FR "Scipad Known bugs" "Bugs connus de Scipad"
    ::msgcat::mcset fr_FR "Change&log" "&Modifications"
        ::msgcat::mcset fr_FR "Changes in the Scipad codebase" "Liste des modifications dans le code source de Scipad"


# popupmenus.tcl
::msgcat::mcset fr_FR "Open the source of" "Ouvrir le source de"
::msgcat::mcset fr_FR "AddWatch" "Ajouter %s comme espion"
::msgcat::mcset fr_FR "AddGenExp" "Ajouter %s comme expression générique"
::msgcat::mcset fr_FR "Jump to" "Sauter à"


# print.tcl
::msgcat::mcset fr_FR "Print Setup" "Mise en page"
::msgcat::mcset fr_FR "Print Command: " "Commande d'impression : "


# scilabexec.tcl
::msgcat::mcset fr_FR "No point in loading an empty file!" "Charger un fichier vide ne présente pas d'intérêt!"
::msgcat::mcset fr_FR "Silent file save failed" "Enregistrement silencieux impossible"
::msgcat::mcset fr_FR "File could not be saved in a temporary location.\nOverwrite original file?" \
                   "Le fichier n'a pas pu être sauvé dans une destination temporaire.\nEcraser le fichier original ?"
::msgcat::mcset fr_FR "Exec done" "Exécution effectuée"
::msgcat::mcset fr_FR "Matlab files" "Fichiers Matlab"
::msgcat::mcset fr_FR "Matlab file to convert" "Fichier Matlab à importer"
::msgcat::mcset fr_FR "Scilab is converting, please hold on..." \
                   "Scilab est en train de convertir, patientez SVP..."
::msgcat::mcset fr_FR "Matlab file import" "Import d'un fichier Matlab"
::msgcat::mcset fr_FR "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to" \
                   "La conversion du ficher a échoué, voyez dans la fenêtre\
                    Scilab pour plus d'informations -- Veuillez communiquer le\
                    message d'erreur et le fichier Matlab incriminé "
::msgcat::mcset fr_FR "Please select destination path for the xml source of the help file" "Veuillez choisir le chemin de destination pour le source xml du fichier d'aide"
::msgcat::mcset fr_FR "Older version of" "Il existe une version plus ancienne de"
    ::msgcat::mcset fr_FR "found!" "!"
::msgcat::mcset fr_FR "An old version of" "Une ancienne version de"
    ::msgcat::mcset fr_FR "already exists: open the old file instead?" "existe déjà : Ouvrir le fichier existant ?"
::msgcat::mcset fr_FR "Existing file" "Fichier existant"
::msgcat::mcset fr_FR "New skeleton" "Nouveau squelette d'aide"
::msgcat::mcset fr_FR "ScilabEval command cannot be passed to Scilab!" "Une commande ScilabEval ne peut pas être passée à Scilab!"
::msgcat::mcset fr_FR impossibleScilabEval_message \
                   "ATTENTION : Une commande n'a pas pu être passée à Scilab et a été ignorée.\n\n \
                   La raison probable est que vous n'avez pas les droits d'écriture dans le répertoire TMPDIR de Scilab. \
                   Un comportement imprévisible peut se produire.\n\n \
                   La commande en cause est:\n\n-->"
::msgcat::mcset fr_FR impossibleScilabEval_message2 \
                   "ATTENTION : Une commande n'a pas pu être passée à Scilab et a été ignorée \
                   parce qu'elle est trop longue.\n\n \
                   Un comportement imprévisible peut se produire.\n\n \
                   Si vous êtes en cours de débug, essayez de supprimer des variables espionnées.\n\n \
                   La commande en cause est:\n\n-->"
::msgcat::mcset fr_FR "(end of command skipped)" "(fin de la commande supprimée)"
::msgcat::mcset fr_FR "Scilab working" "Scilab occupé"
::msgcat::mcset fr_FR "Scilab is working, please wait for the prompt to execute this command!" \
                   "Scilab est occupé, veuillez attendre le prompt pour exécuter cette commande !"
::msgcat::mcset fr_FR "Scilab is working, wait for the prompt to load file" \
                   "Scilab est occupé, attendez le prompt pour charger le fichier"
::msgcat::mcset fr_FR "Scilab is working, wait for the prompt to execute the selection." \
                   "Scilab est occupé, attendez le prompt pour exécuter la sélection."
::msgcat::mcset fr_FR "Scilab is working, wait for the prompt to convert a Matlab file." \
                   "Scilab est occupé, attendez le prompt pour importer un fichier Matlab."
::msgcat::mcset fr_FR "Scilab is working, wait for the prompt to compile the help file." \
                   "Scilab est occupé, attendez le prompt pour compiler la page d'aide."
::msgcat::mcset fr_FR "Scilab is working, wait for the prompt to issue a debugger command." \
                   "Scilab est occupé, attendez le prompt pour effectuer des commandes de débug."
::msgcat::mcset fr_FR "Scilab execution error" "Erreur Scilab durant l'exécution"
::msgcat::mcset fr_FR "The shell reported an error while trying to execute " "Le shell a rencontré une erreur en tentant d'exécuter "
    ::msgcat::mcset fr_FR ": error " ": erreur "
    ::msgcat::mcset fr_FR "at line " "ligne "
    ::msgcat::mcset fr_FR " of " " de "
::msgcat::mcset fr_FR "Execution aborted!" "Exécution arrêtée!"


# scipad.tcl
::msgcat::mcset fr_FR "NOT CONNECTED TO SCILAB" "PAS DE CONNEXION A SCILAB"


# whichfun.tcl
::msgcat::mcset fr_FR "Being at line" "Etant à la ligne"
    ::msgcat::mcset fr_FR ", function" ", la fonction"
    ::msgcat::mcset fr_FR "begins at" "débute en"
    ::msgcat::mcset fr_FR ", and there are" ", et il y a"
    ::msgcat::mcset fr_FR "continued lines, i.e. we are at line" "lignes multiples, i.e. nous sommes à la ligne"
    ::msgcat::mcset fr_FR "of" "de"
#::msgcat::mcset fr_FR "Function" "Fonction" ; # now global
    ::msgcat::mcset fr_FR "line" "ligne"
::msgcat::mcset fr_FR "The cursor is not currently inside a function body." "Le curseur ne se trouve pas à l'intérieur d'une fonction."
::msgcat::mcset fr_FR "Which function?" "Quelle fonction ?"


#preference variables description
::msgcat::mcset fr_FR "BGCOLOR" "Arrière-plan de la zone d'édition"
::msgcat::mcset fr_FR "BGLNMARGCOLOR" "Arrière-plan des numéros de ligne"
::msgcat::mcset fr_FR "FGCOLOR" "Texte brut"
::msgcat::mcset fr_FR "FGLNMARGCOLOR" "Texte des numéros de ligne"
::msgcat::mcset fr_FR "CURCOLOR" "Curseur"
::msgcat::mcset fr_FR "PARCOLOR" "Parenthèses ()"
::msgcat::mcset fr_FR "BRAKCOLOR" "Crochets \\\[\\\]"
::msgcat::mcset fr_FR "BRACCOLOR" "Accolades {}"
::msgcat::mcset fr_FR "PUNCOLOR"  "Ponctuation"
::msgcat::mcset fr_FR "INTFCOLOR" "Fonctions internes"
::msgcat::mcset fr_FR "COMMCOLOR" "Commandes Scilab"
::msgcat::mcset fr_FR "SCICCOLOR" "Fonctions Scicos"
::msgcat::mcset fr_FR "OPCOLOR"   "Opérateurs"
::msgcat::mcset fr_FR "QTXTCOLOR"  "Chaîne de caractères"
::msgcat::mcset fr_FR "REMCOLOR" "Commentaires"
::msgcat::mcset fr_FR "XMLCOLOR" "Balises XML"
::msgcat::mcset fr_FR "NUMCOLOR" "Nombres"
::msgcat::mcset fr_FR "SELCOLOR" "Sélection"
::msgcat::mcset fr_FR "BREAKPOINTCOLOR" "Point d'arrêt"
::msgcat::mcset fr_FR "LFUNCOLOR" "Fonctions de librairie"
::msgcat::mcset fr_FR "PDEFCOLOR" "Variables prédéfinies"
::msgcat::mcset fr_FR "FOUNDTEXTCOLOR" "Texte trouvé"
::msgcat::mcset fr_FR "CONTLINECOLOR" "Ligne incomplète"
::msgcat::mcset fr_FR "USERFUNCOLOR" "Fonctions utilisateur"


#overriding tcl-core dialogs (many are actually already found in 
#  TKDIR/msgs/fr.msg)
::msgcat::mcset fr_FR "&Directory:" "&Répertoire :"
::msgcat::mcset fr_FR "File &name:" "&Nom de fichier :"
::msgcat::mcset fr_FR "Files of &type:" "Fichiers de &type :"
#::msgcat::mcset fr_FR "&Save" "&Enregistrer" # already included
::msgcat::mcset fr_FR "&Open" "&Ouvrir"
::msgcat::mcset fr_FR "Open" "Ouvrir"
::msgcat::mcset fr_FR "Save As" "Enregistrer sous"
::msgcat::mcset fr_FR "&Cancel" "&Annuler"
::msgcat::mcset fr_FR "&Selection:" "&Sélection :"
::msgcat::mcset fr_FR "&Yes" "&Oui"
::msgcat::mcset fr_FR "&No" "&Non"
::msgcat::mcset fr_FR "&Red" "&Rouge"
::msgcat::mcset fr_FR "&Green" "&Vert"
::msgcat::mcset fr_FR "&Blue" "&Bleu"
::msgcat::mcset fr_FR "Directory \"%1\$s\" does not exist."\
        "Le répertoire \"%1\$s\" n'existe pas."
