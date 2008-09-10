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
# in sync at least with v6.155


# commons
::msgcat::mcset fr_fr "Cancel" "Annuler"
::msgcat::mcset fr_fr "Cance&l" "Annu&ler"
::msgcat::mcset fr_fr "OK" "OK"
::msgcat::mcset fr_fr "All files" "Tous les fichiers"
::msgcat::mcset fr_fr "Add/Change" "Ajouter/Modifier"
::msgcat::mcset fr_fr "Add/Chan&ge" "Ajouter/&Modifier"
::msgcat::mcset fr_fr "&Remove" "&Supprimer"
::msgcat::mcset fr_fr "Rem&ove" "Supprime&r"
::msgcat::mcset fr_fr "Untitled" "SansTitre"
::msgcat::mcset fr_fr "Close" "Fermer"
::msgcat::mcset fr_fr "&Close" "&Fermer"
::msgcat::mcset fr_fr "Error " "Erreur "
::msgcat::mcset fr_fr "The file" "Le fichier"
::msgcat::mcset fr_fr "Function" "Fonction"
::msgcat::mcset fr_fr "Line" "Ligne"


# buffernavigation.tcl
::msgcat::mcset fr_fr "Hide" "Cacher"
::msgcat::mcset fr_fr "Close this tile, keep content" "Fermer cette vue, conserver le contenu"
::msgcat::mcset fr_fr "Close this tile" "Fermer cette vue"
::msgcat::mcset fr_fr "Goto Line?" "Aller à la ligne ?"
::msgcat::mcset fr_fr "Go to:" "Aller à : "
::msgcat::mcset fr_fr "&logical line" "la ligne &logique"
::msgcat::mcset fr_fr "&physical line" "la ligne &physique"
::msgcat::mcset fr_fr "in &function" "de la &fonction"
::msgcat::mcset fr_fr "in &current file" "du fichier &courant"
::msgcat::mcset fr_fr "Outside of function definition" "Hors de la définition de la fonction"


# colorize.tcl
::msgcat::mcset fr_fr "Wait seconds while recolorizing file" "Patientez un instant le temps de la colorisation"
::msgcat::mcset fr_fr "You can't do that while colorization is in progress.\
                    Please try again when finished." \
                   "Action interdite durant la colorisation. Veuillez réessayer plus tard."
::msgcat::mcset fr_fr "Command forbidden during colorization" "Commande interdite durant la colorisation"


# completion.tcl
::msgcat::mcset fr_fr "No possible completion" "Aucune terminaison possible"
::msgcat::mcset fr_fr "possible completions" "terminaisons possibles"
::msgcat::mcset fr_fr "Possible completions" "Terminaisons possibles"


# db_argsmanagement.tcl
::msgcat::mcset fr_fr "Add" "Ajouter"
::msgcat::mcset fr_fr "This variable can be watched but cannot be edited!" \
                   "Cette variable peut être espionnée mais ne peut pas être modifiée !"
::msgcat::mcset fr_fr "A new watch variable will be created." \
                   "Une nouvelle variable espion va être créée."
::msgcat::mcset fr_fr "Non editable variable" "Variable non éditable"
::msgcat::mcset fr_fr "Variable:" "Variable :"
::msgcat::mcset fr_fr "Expression:" "Expression :"
::msgcat::mcset fr_fr "Value:" "Valeur :"
::msgcat::mcset fr_fr "&Get current value from Scilab" "Obtenir la valeur &actuelle dans Scilab"
::msgcat::mcset fr_fr "Non watchable variable" "Variable non espionnable"
::msgcat::mcset fr_fr "Sorry, variable" "Désolé, la variable"
    ::msgcat::mcset fr_fr "is special and cannot be watched." "est spéciale et ne peut pas être espionnée."


# db_bptsgui.tcl
::msgcat::mcset fr_fr "Breakpoints properties" "Propriétés des points d'arrêt"
::msgcat::mcset fr_fr "Enabled" "Activé"
::msgcat::mcset fr_fr "Expression" "Expression"
::msgcat::mcset fr_fr "Hit if" "Accédé si"
::msgcat::mcset fr_fr istrue "est vraie"
::msgcat::mcset fr_fr haschanged "a changé"
::msgcat::mcset fr_fr "Watched" "Espionné"
::msgcat::mcset fr_fr "Hit count" "Nb d'accès"
::msgcat::mcset fr_fr "Break if" "Stoppe si"
::msgcat::mcset fr_fr "Threshold" "Seuil"
::msgcat::mcset fr_fr always "toujours"
::msgcat::mcset fr_fr equalto "est égal à"
::msgcat::mcset fr_fr multipleof "est un multiple de"
::msgcat::mcset fr_fr greaterthan "est supérieur à"
::msgcat::mcset fr_fr lessthan "est inférieur à"
::msgcat::mcset fr_fr "Reset hit" "RàZ accès"
::msgcat::mcset fr_fr "Breakpoints vocabulary" "Vocabulaire des points d'arrêt"
::msgcat::mcset fr_fr "A breakpoint is identified by its location, i.e. its position (a line number) in a function." \
                   "Un point d'arrêt est identifié par sa position, i.e. son numéro de ligne au sein d'une fonction."
::msgcat::mcset fr_fr "When the execution process encounters an enabled breakpoint, then this breakpoint is said to be reached." \
                   "Lorsque l'exécution du code rencontre un point d'arrêt activé, alors ce point d'arrêt est dit atteint."
::msgcat::mcset fr_fr "When the breakpoint is reached and its associated conditional expression is true (or changed),\
                    then the breakpoint is said to be hit." \
                   "Lorsqu'un point d'arrêt est atteint et que son expression conditionnelle est vraie (ou modifiée),\
                    alors le point d'arrêt est dit accédé."
::msgcat::mcset fr_fr "The hit count is the number of times the breakpoint has been hit." \
                   "Le nombre d'accès est le nombre de fois que le point d'arrêt a été accédé."
::msgcat::mcset fr_fr "Execution will stop at a breakpoint if the hit count satisfies the selectable break condition." \
                   "L'exécution s'arrêtera sur un point d'arrêt si le nombre d'accès à ce point d'arrêt satisfait la condition d'arrêt."


# db_bptsmanagement.tcl
::msgcat::mcset fr_fr "No breakpoint here!" "Pas de point d'arrêt ici !"
::msgcat::mcset fr_fr "Delete all breakpoints" "Supprimer tous les points d'arrêt"
::msgcat::mcset fr_fr "Are you sure you want to delete all the existing breakpoints?" \
                   "Etes-vous sûr de vouloir supprimer tous les points d'arrêt ?"
::msgcat::mcset fr_fr "All breakpoints deleted" "Points d'arrêt supprimés"
::msgcat::mcset fr_fr "No action taken" "Aucun changement effectué"
::msgcat::mcset fr_fr "No breakpoint!" "Aucun point d'arrêt !"


# db_configureexec.tcl
::msgcat::mcset fr_fr "Configure execution" "Configurer l'exécution"
::msgcat::mcset fr_fr "Function name:" "Nom de la fonction :"
::msgcat::mcset fr_fr "&Obtain" "&Obtenir"
::msgcat::mcset fr_fr "Input arguments:" "Arguments d'entrée :"
::msgcat::mcset fr_fr "The selected function has not been found in the opened files, maybe it was deleted since last configuration of the debugger.\nUse button \"" \
                   "La fonction sélectionnée n'a pas pu être trouvée dans les fichiers ouverts, peut-être a-t-elle été supprimée depuis la précédente configuration du débuggueur.\nUtiliser le bouton \""
    ::msgcat::mcset fr_fr "\" to refresh the list of available functions." \
                       "\" pour rafraîchir la liste des fonctions disponibles."
::msgcat::mcset fr_fr "Function name or input arguments do not match definition of the function" \
                    "Le nom de la fonction ou ses arguments ne correspondent pas à la définition de la fonction"
    ::msgcat::mcset fr_fr "in the file!\n\nCheck function\
                        name and arguments (names, order) in the configuration dialog.\
                        \nArguments order can be changed using drag and drop with\
                        right mouse button in the arguments listbox." \
                       "dans le fichier!\n\n\
                        Vérifier le nom de la fonction et ses arguments (nom, ordre\
                        d'apparition) dans la boîte de dialogue de configuration.\n\
                        L'ordre des arguments peut être modifié par glisser/déposer\
                        avec le bouton droit de la souris."
::msgcat::mcset fr_fr "Error in selected function name or arguments" "Erreur sur la fonction sélectionnée ou ses arguments"
::msgcat::mcset fr_fr "At least one input argument does not match definition of the function" \
                    "Au moins un argument ne correspond pas à la définition de la fonction"
    ::msgcat::mcset fr_fr "in the file!\n\nCheck function\
                        arguments (their names) in the configuration dialog."\
                       "dans le fichier!\n\n\
                        Vérifier les arguments de la fonction (leur noms)\
                        dans la boîte de dialogue de configuration."
::msgcat::mcset fr_fr "Error in selected function arguments" "Erreur sur les arguments de la fonction sélectionnée"
::msgcat::mcset fr_fr "Main level code found" "Présence de code hors fonction"
::msgcat::mcset fr_fr "Scipad ancillary file" "Fichier de sous-routines de Scipad"
::msgcat::mcset fr_fr "The current file contains only Scipad ancillaries that cannot be debugged in Scipad." \
                   "Le fichier actuel ne contient que des sous-routines utilisées par Scipad, et qui ne peuvent pas être débugguées dans Scipad."
::msgcat::mcset fr_fr "Reserved function names are:\n" "Les noms réservés sont:\n"
::msgcat::mcset fr_fr "See help scipad for further details." "Veuillez consulter l'aide (help scipad) pour plus de détails."
::msgcat::mcset fr_fr "This file contains main level code.\nDebug as a single .sce file?" \
                   "Ce fichier contient du code hors définition de fonction.\nDébugguer en tant qu'unique entité .sce ?"
::msgcat::mcset fr_fr "No function found" "Aucune fonction trouvée"
::msgcat::mcset fr_fr "This file contains no function and will be treated" \
                   " Ce fichier ne contient aucune fonction et sera traité"
    ::msgcat::mcset fr_fr "as a single .sce file.\nStart debugging?" "en tant qu'unique entité .sce.\nCommencer le débug ?"
::msgcat::mcset fr_fr "You must enable colorization to be able to launch the debugger!" \
                   "Vous devez autoriser la colorisation pour lancer le débuggueur !"
::msgcat::mcset fr_fr "The debugger needs colorization" \
                   "Le débuggueur a besoin de la colorisation"
::msgcat::mcset fr_fr "Warning!\n\n \
                 A function missing the closing \"endfunction\" has been found in the currently opened files.\n \
                 Such syntax is now obsolete.\n\n \
                 Please terminate function " \
                   " Attention !\n\n \
                 Une fonction sans \"endfunction\" a été trouvée dans les fichiers actuellement ouverts.\n \
                 Cette syntaxe est obsolète.\n\n \
                 Veuillez terminer la fonction "
::msgcat::mcset fr_fr " and try again." " et essayer une nouvelle fois."
::msgcat::mcset fr_fr "Note: This message might also be triggered by a string\n \
                 quoted with single quotes and containing the word \"function\",\n \
                 when \"Colorize \'strings\'\" is unchecked in the Options menu." \
                   "Nota : Ce message peut aussi être dû à une chaîne de caractères\n \
                 délimitée par des guillemets simples et contenant le mot \"function\",\n \
                 lorsque \"Colorer \'chaîne\'\" n'est pas coché dans le menu Options."
::msgcat::mcset fr_fr "This known bug can be worked around simply by checking \"Colorize \'&strings\'\"." \
                   "Ce bug connu peut être évité simplement en cochant l'option \"Colorer \'chaîne\'\"."
::msgcat::mcset fr_fr "Unterminated function definition found" "Fonction non terminée identifiée"
::msgcat::mcset fr_fr "Warning!\n\n \
                 One or more function is defined more than once in the currently opened files.\n \
                 The debugger cannot cope with multiple copies of functions.\n \
                 The following duplicates were detected:\n" \
                   "Attention !\n\n \
                 Une ou plusieurs fonctions sont définies plus d'une fois dans les fichiers actuellement ouverts.\n \
                 Le débuggueur ne supporte pas les copies multiples de fonctions.\n \
                 Les doublons suivants ont été détectés :\n"
::msgcat::mcset fr_fr "\n--> Definition of function " "\n--> Définition de la fonction "
::msgcat::mcset fr_fr " found in:\n" " trouvée dans :\n"
::msgcat::mcset fr_fr "Duplicate function definitions found" "Fonctions en doublon identifiées"


# db_debugsession.tcl
::msgcat::mcset fr_fr "Too many breakpointed functions" \
                   "Trop de fonctions contenant des points d'arrêt"
::msgcat::mcset fr_fr "You have currently" \
                   "Vos fichiers ouverts définissent actuellement"
    ::msgcat::mcset fr_fr "functions in your opened files." "fonctions."
    ::msgcat::mcset fr_fr "Scilab supports a maximum of" "Scilab supporte au maximum"
    ::msgcat::mcset fr_fr "possible breakpointed functions (see help setbpt)." \
                       "functions avec points d'arrêts (voir help setbpt)."
    ::msgcat::mcset fr_fr "Step-by-step hence cannot be performed." \
                       "Le mode pas à pas n'est en conséquence pas possible."
    ::msgcat::mcset fr_fr "This command will actually run to the next breakpoint." \
                       "Cette commande va atteindre le prochain point d'arrêt."
    ::msgcat::mcset fr_fr "Break hence cannot be performed." \
                       "La commande Stop ne peut pas être exécutée."
::msgcat::mcset fr_fr "Too many breakpoints" "Trop de points d'arrêt"
::msgcat::mcset fr_fr "Executing this command would require to set" \
                   "Exécuter cette commande nécessiterait d'insérer"
    ::msgcat::mcset fr_fr "breakpoints in your opened files." \
                       "points d'arrêt dans vos fichiers ouverts."
    ::msgcat::mcset fr_fr "possible breakpoints in Scilab (see help setbpt)." \
                       "points d'arrêt (voir help setbpt)."
::msgcat::mcset fr_fr "Cursor must be in a function" \
                   "Le curseur doit être dans une fonction"
::msgcat::mcset fr_fr "This command cannot succeed if the function where the debug just stopped is not opened in Scipad!" \
                   "Le fichier dans lequel le débug vient de stopper doit être ouvert pour exécuter cette commande !"
::msgcat::mcset fr_fr "Cannot run to return point" \
                   "Impossible d'exécuter jusqu'au point de retour"
::msgcat::mcset fr_fr "Warning!\nThe line where the debugger just stopped contains a return statement (either endfunction, return or resume) but this statement is not the first one on the line." \
                   "Attention !\nLa ligne sur laquelle le débuggueur vient de s'arrêter contient une instruction de retour (endfunction ou bien return ou bien resume), mais cette instruction n'est pas la première sur la ligne."
::msgcat::mcset fr_fr "As a consequence the debugger cannot predict if the next debug step will really jump outside of the current function, which is what you just asked the debugger to do." \
                   "Par conséquent le débuggueur ne peut pas prédire si le prochain pas de débug va faire sortir de la fonction en cours (i.e. ce que vous venez de demander de réaliser)."
::msgcat::mcset fr_fr "To avoid this unresolvable case, it is recommended to place the return statement alone on its line." \
                   "Pour éviter ce cas indécidable, il est recommandé de placer chaque instruction de retour seule sur sa ligne."
::msgcat::mcset fr_fr "Do you want to see this warning again during this Scipad session?" \
                   "Voulez-vous voir ce message à nouveau durant cette session Scipad?"
::msgcat::mcset fr_fr "Unresolvable case found while running to return point" \
                   "Cas indécidable identifié durant l'exécution jusqu'au point de retour"
::msgcat::mcset fr_fr "No effect - The debugged file is not stuck" \
                   "Aucun effet - Le fichier en débug n'est pas bloqué"


# db_init.tcl
::msgcat::mcset fr_fr "Please wait..." "Veuillez patienter..."


# db_states.tcl
::msgcat::mcset fr_fr "Currently no debug session" "Pas de session de débug en cours"
::msgcat::mcset fr_fr "Ready to start debug" "Prêt pour le débug"
::msgcat::mcset fr_fr "Debug in progress" "Débug en cours"
::msgcat::mcset fr_fr "Scilab is back at the main level now." \
                   "Scilab est de retour au niveau principal."


# db_watch.tcl
::msgcat::mcset fr_fr "Watch" "Espions"
::msgcat::mcset fr_fr "Watch variables:" "Variables espions :"
::msgcat::mcset fr_fr "A&uto (local variables)" "A&uto (variables locales)"
::msgcat::mcset fr_fr "Au&to watch globals too" "Espionner aussi les &globales"
::msgcat::mcset fr_fr "Generic expressions:" "Expressions génériques :"
::msgcat::mcset fr_fr "Call stack:" "Pile des appels :"
::msgcat::mcset fr_fr "Always on to&p" "Toujours à l'&avant-plan"
::msgcat::mcset fr_fr "Hide watch &variables" "Cacher les &variables"
::msgcat::mcset fr_fr "Hide e&xpressions" "Cacher les e&xpressions"
::msgcat::mcset fr_fr "Hide call &stack" "Cacher la &pile d'appel"
::msgcat::mcset fr_fr "&Dock" "&Intégrer"
::msgcat::mcset fr_fr "&Add" "Ajout&er"
::msgcat::mcset fr_fr "&Filter errors" "Fi&ltrer les erreurs"
::msgcat::mcset fr_fr "Breakpoints" "Points d'arrêt"
::msgcat::mcset fr_fr "Breakpointed functions" "Fonctions avec breakpoints"
::msgcat::mcset fr_fr "Scilab busy indicator" "Indicateur d'occupation de Scilab"
::msgcat::mcset fr_fr "Debugger state indicator" "Indicateur de l'état du débuggueur"
::msgcat::mcset fr_fr "Undefined elements are not legal in variable" "Les éléments indéfinis sont illégaux pour la variable"
::msgcat::mcset fr_fr ".\nThis variable will not be updated in Scilab." ".\nCette variable ne sera pas mise à jour dans Scilab"
::msgcat::mcset fr_fr "Illegal undefined element found" "Elément indéfini illégal identifié"
::msgcat::mcset fr_fr "evaluates to:" "est évalué en:"
::msgcat::mcset fr_fr "produces error" "résulte en une erreur"
::msgcat::mcset fr_fr "Scipad cannot find source code for the selected function.\nPlease open the adequate file manually and try again." \
                   "Scipad ne trouve pas le code source de la fonction sélectionnée.\nVeuillez ouvrir le fichier manuellement et réessayer."
::msgcat::mcset fr_fr "Missing source code" "Code source manquant"


# filecommands.tcl
::msgcat::mcset fr_fr "New script" "Nouveau script"
::msgcat::mcset fr_fr "New view on the same file" "Nouvelle vue sur ce fichier"
::msgcat::mcset fr_fr "The contents of" "Voulez-vous enregistrer les modifications apportées à"
    ::msgcat::mcset fr_fr "may have changed, do you wish to save your changes?" "?"
::msgcat::mcset fr_fr "Save Confirm?" "Confirmer sauver ?"
::msgcat::mcset fr_fr "Open source of..." "Ouvrir le code source de..."
::msgcat::mcset fr_fr "Open source of:" "Ouvrir le source de :"
::msgcat::mcset fr_fr "Weird drag&drop" "Glisser&déposer inattendu"
::msgcat::mcset fr_fr "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                    "n'est ni un nom de fichier valide ni un nom de répertoire\
                     valide. Soit vous avez tenté de glisser un objet d'un \
                     type différent, soit c'est un bug du mécanisme de \
                     glisser&déposer."
::msgcat::mcset fr_fr "File not found" "Fichier introuvable"
::msgcat::mcset fr_fr "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "n'existe plus. Voulez-vous créer un nouveau fichier\
                    portant ce nom ?"
::msgcat::mcset fr_fr "Open file" "Ouvrir fichier"
#::msgcat::mcset fr_fr "The file" "Le fichier" ;# defined in the "commons" section
    ::msgcat::mcset fr_fr "is already opened! Save the current opened file to another name and reopen it from disk!" "est déjà ouvert! Sauvez-le sous un autre nom et rouvrez-le à partir du disque !"
::msgcat::mcset fr_fr "File" "Fichier"
    ::msgcat::mcset fr_fr "saved" "sauvegardé"
::msgcat::mcset fr_fr "cannot be written!" "est en lecture seule !"
::msgcat::mcset fr_fr "The contents of " "Le contenu de"
    ::msgcat::mcset fr_fr "has changed on disk, save it anyway?" "a changé sur le disque, êtes-vous sûr de vouloir le sauvegarder ?"
    ::msgcat::mcset fr_fr "has been modified outside of Scipad. Do you want to reload it?" \
                       "a été modifié hors de Scipad. Voulez-vous le recharger à partir du disque ?"
::msgcat::mcset fr_fr "File has changed!" "Le fichier a changé"
::msgcat::mcset fr_fr "Save as" "Enregistrer sous"
::msgcat::mcset fr_fr "Scilab files" "Fichiers Scilab"
::msgcat::mcset fr_fr "Scicos files" "Fichiers Scicos"
::msgcat::mcset fr_fr "XML files" "Fichiers XML"
::msgcat::mcset fr_fr "Wait seconds while loading and colorizing file" "Patientez un instant le temps du chargement et de la colorisation"
::msgcat::mcset fr_fr "The contents of this file may have changed, do you wish to save your changes?" "Voulez-vous enregistrer les modifications apportées à ce fichier ?"
::msgcat::mcset fr_fr "Revert" "Revenir à la version enregistrée de"
    ::msgcat::mcset fr_fr "to saved?" "?"
::msgcat::mcset fr_fr "Revert Confirm?" "Confirmer revenir ?"
::msgcat::mcset fr_fr "Unreadable file" "Fichier illisible!"
::msgcat::mcset fr_fr "exists but is not readable!" "existe mais ne peut pas être lu!"
::msgcat::mcset fr_fr "This file is rather large and colorization might take some time.\n\nColorize anyway?" \
                   "Ce fichier est de grande taille et sa colorisation peut être longue.\n\nColorer tout de même?"
::msgcat::mcset fr_fr "File size warning" "Avertissement relatif à la taille du fichier"
::msgcat::mcset fr_fr "Windows shortcut file" "Fichier raccourci Windows"
#::msgcat::mcset fr_fr "The file" "Le fichier" ;# defined in the "commons" section
    ::msgcat::mcset fr_fr "is a Windows shortcut and will not be opened!" \
                       "est un raccourci Windows et ne sera pas ouvert !"


# findreplace.tcl
::msgcat::mcset fr_fr "Dialog box already open!" "Boîte de dialogue déjà ouverte!"
::msgcat::mcset fr_fr "The string" "La chaîne"
    ::msgcat::mcset fr_fr "could not be found in" "n'a pu être trouvée dans"
    ::msgcat::mcset fr_fr "could not be found in any of the opened files !" "n'a pu être trouvée dans aucun des fichiers ouverts !"
::msgcat::mcset fr_fr "replacements done" "remplacements effectués"
::msgcat::mcset fr_fr "Find" "Rechercher"
::msgcat::mcset fr_fr "No match found in the selection for" "La chaîne"
    ::msgcat::mcset fr_fr "\nWould you like to look for it in the entire text?" "n'a pu être trouvée dans la sélection.\nVoulez-vous rechercher dans la totalité du texte ?"
::msgcat::mcset fr_fr "You are searching for an empty string!" "La chaîne à rechercher est vide!"
::msgcat::mcset fr_fr "Re&place" "&Remplacer"
::msgcat::mcset fr_fr "Find what:" "Rechercher :"
::msgcat::mcset fr_fr "Replace with:" "Remplacer par :"
::msgcat::mcset fr_fr "Find &Next" "Rechercher &suivant"
::msgcat::mcset fr_fr "Replace &All" "Remplacer &tout"
::msgcat::mcset fr_fr "Direction" "Sens"
::msgcat::mcset fr_fr "&Upwards" "Vers le &haut"
::msgcat::mcset fr_fr "&Downwards" "Vers le &bas" 
::msgcat::mcset fr_fr "Text types" "Types de texte"
::msgcat::mcset fr_fr "Find options" "Options de recherche"
::msgcat::mcset fr_fr "Match &whole word only" "Mot &entier seulement"
::msgcat::mcset fr_fr "Match &case" "Respecter la &casse"
::msgcat::mcset fr_fr "&Regular expression" "E&xpression régulière"
::msgcat::mcset fr_fr "In all &opened files" "Dans tous les fichiers &ouverts"
::msgcat::mcset fr_fr "In &selection only" "Da&ns la sélection seulement"
::msgcat::mcset fr_fr "In typed te&xt" "Dans des t&ypes de texte"
::msgcat::mcset fr_fr "In a director&y" "Dans un réper&toire"
::msgcat::mcset fr_fr "Find in files search options" "Options de recherche dans un répertoire"
::msgcat::mcset fr_fr "Directory r&ecurse search" "Recherche réc&ursive"
::msgcat::mcset fr_fr "Include &hidden files" "Inclure les fichiers c&achés"
::msgcat::mcset fr_fr "In files/file types:" "Dans les fichiers/types :"
::msgcat::mcset fr_fr "In directory:" "Dans le répertoire :"
::msgcat::mcset fr_fr "Search for file names" "Rechercher dans les noms des fichiers"
::msgcat::mcset fr_fr "You have given an empty search string. Are you sure you want to search among file names?" \
                   "La chaîne à rechercher est vide. Etes-vous sûr de vouloir rechercher parmi les noms des fichiers ?"
::msgcat::mcset fr_fr "Wrapped around" "Rebouclé"
::msgcat::mcset fr_fr "Back at the first file searched!" "Revenu au premier fichier !"
::msgcat::mcset fr_fr "No more matches" "Pas d'autre correspondance"
::msgcat::mcset fr_fr "The string to regexp search for cannot be compiled by Tcl.\nCheck again, maybe certain characters should be escaped." \
                   "L'expression régulière entrée est invalide (impossible à compiler).\nVérifiez, il se peut que le caractère d'échappement \\ doive précéder certains caractères."
::msgcat::mcset fr_fr "Any single character" "Un caractère quelconque"
::msgcat::mcset fr_fr "Zero or more" "Zéro ou plus"
::msgcat::mcset fr_fr "One or more" "Un ou plus"
::msgcat::mcset fr_fr "Zero or one" "Zéro ou un"
::msgcat::mcset fr_fr "Any one character in the set" "Un caractère quelconque de l'ensemble"
::msgcat::mcset fr_fr "Any one character not in the set" "Un caractère quelconque hors de l'ensemble"
::msgcat::mcset fr_fr "Or" "Ou"
::msgcat::mcset fr_fr "Beginning of line" "Début de ligne"
::msgcat::mcset fr_fr "End of line" "Fin de ligne"
::msgcat::mcset fr_fr "Beginning of word" "Début de mot"
::msgcat::mcset fr_fr "End of word" "Fin de mot"
::msgcat::mcset fr_fr "A letter" "Une lettre"
::msgcat::mcset fr_fr "A decimal digit" "Un chiffre décimal"
::msgcat::mcset fr_fr "An alphanumeric (letter or digit)" "Un caractère alphanumérique (lettre ou chiffre)"
::msgcat::mcset fr_fr "A space or tab character" "Un espace ou une tabulation"
::msgcat::mcset fr_fr "A rational number" "Un nombre rationel"
::msgcat::mcset fr_fr "A floating point number" "Un nombre en virgule flottante"
::msgcat::mcset fr_fr "A Scilab comment" "Un commentaire Scilab"
::msgcat::mcset fr_fr "A Scilab name" "Un nom Scilab"
::msgcat::mcset fr_fr "A Scilab continued line (with dots)" "Une ligne continuée Scilab (avec points de suite)"


# findinfiles.tcl
::msgcat::mcset fr_fr "Directory access issue" "Erreur d'accès à un répertoire"
::msgcat::mcset fr_fr "The directory you specified cannot be reached. The search won't bring any result." \
                   "Scipad ne peut pas accéder au répertoire spécifié. La recherche ne donnera aucun résultat."
::msgcat::mcset fr_fr "Some directories or files could not be open and were ignored during search.\n\
                    You might miss read access to them.\n\n\
                    The following ones were ignored" \
                   "Certains répertoires ou fichiers n'ont pas pu être ouverts et ont été ignorés durant la recherche.\n\
                    Vous n'avez peut-être pas les droits de lecture sur eux.\n\n\
                    Les éléments suivants ont été ignorés"
::msgcat::mcset fr_fr "Ignored files" "Fichiers ignorés"
::msgcat::mcset fr_fr "Searching in file:" "Recherche dans :"
::msgcat::mcset fr_fr "End of file search:" "Fin de la recherche:"
::msgcat::mcset fr_fr "matches found" "correspondances trouvées"
::msgcat::mcset fr_fr "searched files" "fichiers examinés"
::msgcat::mcset fr_fr "Constructing file list:" "Construction de la liste des fichiers:"
::msgcat::mcset fr_fr "&Previous" "&Précédent"
::msgcat::mcset fr_fr "&Next" "&Suivant"
::msgcat::mcset fr_fr "Scipad search results" "Résultats de recherche Scipad"


# fonts.tcl
::msgcat::mcset fr_fr "Choose font" "Choisir une police"
::msgcat::mcset fr_fr "Family" "Police"
::msgcat::mcset fr_fr "Style" "Style"
::msgcat::mcset fr_fr "&Bold" "&Gras"
::msgcat::mcset fr_fr "&Italic" "&Italique"
::msgcat::mcset fr_fr "&Underline" "&Souligné"
::msgcat::mcset fr_fr "Size" "Taille"
::msgcat::mcset fr_fr "&Defaults" "Par &défaut"
::msgcat::mcset fr_fr "Use as..." "Utiliser pour..."
::msgcat::mcset fr_fr "&Text font" "Police de &texte"
::msgcat::mcset fr_fr "&Menu font" "Police de &menu"
::msgcat::mcset fr_fr "Sample" "Exemple"
::msgcat::mcset fr_fr "Font size" "Police taille"
::msgcat::mcset fr_fr "Revert fonts" "Revenir aux polices par défaut"
::msgcat::mcset fr_fr "Are you sure you want to revert all fonts to defaults?" \
                   "Voulez-vous vraiment revenir aux polices par défaut ?"


# FormatStringsForWatch.sci
::msgcat::mcset fr_fr " what you try to watch is not supported by the debugger" \
                   " ce que vous essayez d'espionner n'est pas supporté par le débuggueur"
::msgcat::mcset fr_fr " what you try to watch is of type " \
                   " ce que vous tentez d'espionner est de type "
    ::msgcat::mcset fr_fr " - this type is not supported by the debugger" \
                       " - ce type n'est pas supporté par le débuggueur"
::msgcat::mcset fr_fr "Type:" "Type :"
::msgcat::mcset fr_fr "(unsupported)" "(non supporté)"
::msgcat::mcset fr_fr "size:" "taille :"
::msgcat::mcset fr_fr "real" "réel"
::msgcat::mcset fr_fr "complex" "complexe"
::msgcat::mcset fr_fr "elements" "éléments"
::msgcat::mcset fr_fr "graphic handle" "handle de graphique"
::msgcat::mcset fr_fr "uncompiled function" "fonction non compilée"
::msgcat::mcset fr_fr "compiled function" "fonction compilée"
::msgcat::mcset fr_fr "library" "librairie"
::msgcat::mcset fr_fr "pointer" "pointeur"
::msgcat::mcset fr_fr "size implicit index" "indice de taille implicite"
::msgcat::mcset fr_fr "primitive" "primitive"


# FormatWhereForWatch.sci
::msgcat::mcset fr_fr "Breakpoint" "Point d'arrêt"
::msgcat::mcset fr_fr "called at line" "appelé à la ligne "
::msgcat::mcset fr_fr "of exec file" "dans un fichier exec"
::msgcat::mcset fr_fr "of execstr instruction" "dans une instruction execstr"
::msgcat::mcset fr_fr "called under pause" "appelé sous pause"
::msgcat::mcset fr_fr "of macro" "de la macro"


# helps.tcl
::msgcat::mcset fr_fr "About" "A propos"
::msgcat::mcset fr_fr aboutme_message \
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
::msgcat::mcset fr_fr Scipad_is_released_under_GPL "SciPad est un logiciel sous licence GPL"


# infomessages.tcl
::msgcat::mcset fr_fr "Line:" "Ligne:"
    ::msgcat::mcset fr_fr "Column:" "Colonne:"
#::msgcat::mcset fr_fr "Line" "Ligne" ; # now common
    ::msgcat::mcset fr_fr "in" "de"
::msgcat::mcset fr_fr "Logical line:" "Ligne logique:"
::msgcat::mcset fr_fr " \[ReadOnly\]" " \[LectureSeule\]"
::msgcat::mcset fr_fr " (modified)" " (modifié)"


# inputtext.tcl
::msgcat::mcset fr_fr "No corresponding <" "Aucun <"
    ::msgcat::mcset fr_fr "> found!" "> correspondant trouvé !"
::msgcat::mcset fr_fr "Code editing is not allowed during debug!" "Impossible de modifier le code durant le débug !"


# menues.tcl
::msgcat::mcset fr_fr "&File" "&Fichier"
::msgcat::mcset fr_fr "&Edit" "&Edition"
::msgcat::mcset fr_fr "&Search" "&Rechercher"
::msgcat::mcset fr_fr "E&xecute" "E&xécuter"
::msgcat::mcset fr_fr "&Debug" "&Débug"
::msgcat::mcset fr_fr "S&cheme" "&Langage"
::msgcat::mcset fr_fr "&Options" "&Options"
::msgcat::mcset fr_fr "&Windows" "Fe&nêtres"
::msgcat::mcset fr_fr "&Help" "&Aide"

    # menues.tcl - File menu
    ::msgcat::mcset fr_fr "&New" "&Nouveau"
    ::msgcat::mcset fr_fr "&Open..." "&Ouvrir..."
    ::msgcat::mcset fr_fr "Open &in new" "O&uvrir dans une nouvelle fenêtre"
        ::msgcat::mcset fr_fr "&horizontal tile" "&horizontale"
        ::msgcat::mcset fr_fr "&vertical tile" "&verticale"
    ::msgcat::mcset fr_fr "Op&en source of..." "Ouvrir le &code source de..."
    ::msgcat::mcset fr_fr "&Save" "&Enregistrer"
    ::msgcat::mcset fr_fr "Save &as..." "En&registrer sous..."
    ::msgcat::mcset fr_fr "Save a&ll" "Enregistrer &tout"
    ::msgcat::mcset fr_fr "&Revert..." "Re&venir..."
    ::msgcat::mcset fr_fr "Import &Matlab file..."  "Importer fichier &Matlab..."
    ::msgcat::mcset fr_fr "Create help s&keleton..." "Créer un &squelette d'aide XML..."
    ::msgcat::mcset fr_fr "Compile as &help page" "Compiler comme fichier d'&aide"
    ::msgcat::mcset fr_fr "Open &function source" "Ouvrir le source &de la fonction"
    ::msgcat::mcset fr_fr "Print Se&tup" "Mise en pa&ge"
    ::msgcat::mcset fr_fr "&Print" "&Imprimer"
    ::msgcat::mcset fr_fr "&Close file" "&Fermer ce fichier"
    ::msgcat::mcset fr_fr "E&xit" "&Quitter"

    # menues.tcl - Edit menu
    ::msgcat::mcset fr_fr "&Undo" "&Annuler"
    ::msgcat::mcset fr_fr "&Redo" "&Répéter"
    ::msgcat::mcset fr_fr "Cu&t" "Co&uper"
    ::msgcat::mcset fr_fr "B&lock cut" "Couper (mode bl&oc)"
    ::msgcat::mcset fr_fr "&Copy" "&Copier"
    ::msgcat::mcset fr_fr "&Paste" "Co&ller"
    ::msgcat::mcset fr_fr "&Block paste" "Coller (mode &bloc)"
    ::msgcat::mcset fr_fr "&Delete" "&Effacer"
    ::msgcat::mcset fr_fr "Select &All" "Sélectionner &tout"
    ::msgcat::mcset fr_fr "Insert &Time/Date" "Insérer Heure/Date"
    ::msgcat::mcset fr_fr "Co&mment selection" "Co&mmenter la sélection"
    ::msgcat::mcset fr_fr "U&ncomment selection" "&Décommenter la sélection"
    ::msgcat::mcset fr_fr "&Indent selection" "&Indenter la sélection"
    ::msgcat::mcset fr_fr "Unin&dent selection" "Dé&sindenter la sélection"

    # menues.tcl - Search menu
    ::msgcat::mcset fr_fr "&Find..." "&Rechercher..."
    ::msgcat::mcset fr_fr "Find &Next" "Rechercher &suivant"
    ::msgcat::mcset fr_fr "&Replace..." "Rem&placer..."
    ::msgcat::mcset fr_fr "&Goto Line..." "&Atteindre..."

    # menues.tcl - Exec menu
    ::msgcat::mcset fr_fr "&Load into Scilab" "&Charger dans Scilab"
    ::msgcat::mcset fr_fr "&Evaluate selection" "&Evaluer la sélection"

    # menues.tcl - Debug menu
    ::msgcat::mcset fr_fr "&Insert/Remove breakpoint" "&Insérer/Supprimer un point d'arrêt"
    ::msgcat::mcset fr_fr "&Edit breakpoints" "&Editer les points d'arrêt"
    ::msgcat::mcset fr_fr "Remove &all breakpoints" "Supprimer &tous les points d'arrêt"
    ::msgcat::mcset fr_fr "&Configure execution..." "Con&figurer l'exécution..."
    ::msgcat::mcset fr_fr "Go to next b&reakpoint" "Aller jusq&u'au prochain point d'arrêt"
    ::msgcat::mcset fr_fr "&Step by step" "&Pas à pas"
    ::msgcat::mcset fr_fr "Step &into" "Pas à pas &détaillé"
    ::msgcat::mcset fr_fr "Step o&ver" "Pas à pas &principal"
    ::msgcat::mcset fr_fr "Step &out" "Pas à pas &sortant"
    ::msgcat::mcset fr_fr "Run to re&turn point" "Exécuter jusqu'au point de &retour"
    ::msgcat::mcset fr_fr "Run to c&ursor" "Exécuter jusqu'au cur&seur"
    ::msgcat::mcset fr_fr "G&o on ignoring any breakpoint" "&Continuer sans aucun point d'arrêt"
    ::msgcat::mcset fr_fr "Show &watch" "Fenêtre &watch"
    ::msgcat::mcset fr_fr "&Break" "&Stop"
    ::msgcat::mcset fr_fr "Cance&l debug" "&Annuler le débug"

#    ::msgcat::mcset fr_fr "language scheme" "schéma langage"
    # menues.tcl - Scheme menu
    ::msgcat::mcset fr_fr "S&cilab" "S&cilab"
    ::msgcat::mcset fr_fr "&XML" "&XML"
    ::msgcat::mcset fr_fr "&none" "&aucun"
    ::msgcat::mcset fr_fr "&Colorize" "&Colorer"

    # menues.tcl - Options menu
    ::msgcat::mcset fr_fr "&Fonts..." "&Polices..."
    ::msgcat::mcset fr_fr "&Colors" "&Couleurs"
    ::msgcat::mcset fr_fr "Colori&ze" "C&olorer"
        ::msgcat::mcset fr_fr "&Always" "&Toujours"
        ::msgcat::mcset fr_fr "Ask for &big files" "&Demander pour les gros fichiers"
        ::msgcat::mcset fr_fr "&Never" "&Jamais"
        ::msgcat::mcset fr_fr "Colorize \'&strings\'" "Colorer \'&chaîne\'"
        ::msgcat::mcset fr_fr "Colorize &user functions" "Colorer les fonctions &utilisateur"
        ::msgcat::mcset fr_fr "Show c&ontinued lines" "Surligner les li&gnes incomplètes"
    ::msgcat::mcset fr_fr "Line n&umbers" "N&uméros de ligne"
        ::msgcat::mcset fr_fr "&Hide" "&Masqués"
        ::msgcat::mcset fr_fr "&Left aligned" "Alignés à &gauche"
        ::msgcat::mcset fr_fr "&Right aligned" "Alignés à &droite"
    ::msgcat::mcset fr_fr "Word &wrap" "&Retour à la ligne automatique"
    ::msgcat::mcset fr_fr "&Double-click behavior" "Comportement du &double-clic"
        ::msgcat::mcset fr_fr "&Linux" "&Linux"
        ::msgcat::mcset fr_fr "Windo&ws" "Windo&ws"
        ::msgcat::mcset fr_fr "&Scilab" "&Scilab"
    ::msgcat::mcset fr_fr "&Tabs and indentation" "Tabs et &indentation"
        ::msgcat::mcset fr_fr "Tab inserts &spaces" "Tab insère des &espaces"
        ::msgcat::mcset fr_fr "&Indentation spaces" "&Dimension de l'indentation (espaces)"
        ::msgcat::mcset fr_fr "&Tab size (characters)" "&Taille d'une tabulation (caractères)"
        ::msgcat::mcset fr_fr "Use &keyword indentation" "Indentation sur &mot-clés"
    ::msgcat::mcset fr_fr "Com&pletion" "Ter&minaison"
        ::msgcat::mcset fr_fr "&Tab" "&Tab"
        ::msgcat::mcset fr_fr "&Control-Tab" "&Control-Tab"
        ::msgcat::mcset fr_fr "&Alt-Tab" "&Alt-Tab"
        ::msgcat::mcset fr_fr "&Shift-Tab" "&Shift-Tab"
        ::msgcat::mcset fr_fr "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
        ::msgcat::mcset fr_fr "S&hift-Control-Tab" "S&hift-Control-Tab"
        ::msgcat::mcset fr_fr "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
        ::msgcat::mcset fr_fr "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
    ::msgcat::mcset fr_fr "File&names" "&Noms de fichiers"
        ::msgcat::mcset fr_fr "&Full path" "&Complets"
        ::msgcat::mcset fr_fr "Full path if &ambiguous" "Complets si &ambigus"
        ::msgcat::mcset fr_fr "&Unambiguous pruned path" "&Raccourcis non ambigus"
    ::msgcat::mcset fr_fr  "Windows menu &sorting" "&Tri dans le menu Fenêtres"
        ::msgcat::mcset fr_fr "&File opening order" "Ordre d'ouverture des &fichiers"
        ::msgcat::mcset fr_fr "&Alphabetically" "&Alphabétique"
        ::msgcat::mcset fr_fr "&Most recently used" "Plus &récemment utilisé"
    ::msgcat::mcset fr_fr "&Recent files" "&Fichiers récents"
    ::msgcat::mcset fr_fr "&Backup files depth" "Profond&eur de sauvegarde"
    ::msgcat::mcset fr_fr "&Locale" "&Langue"
    ::msgcat::mcset fr_fr "Execution &errors" "E&rreurs d'exécution"
        ::msgcat::mcset fr_fr "In Scilab &shell only" "Dans la fenêtre &Scilab seulement"
        ::msgcat::mcset fr_fr "Copied in a &message box" "Recopiées dans une &boîte de dialogue"

    # menues.tcl - Windows menu
    ::msgcat::mcset fr_fr "&Maximize" "&Maximiser"
    ::msgcat::mcset fr_fr "&Split" "&Diviser"
    ::msgcat::mcset fr_fr "S&plit (side by side)" "Diviser (&côte à côte)"
    ::msgcat::mcset fr_fr "Sp&lit file" "D&iviser le fichier"
    ::msgcat::mcset fr_fr "Spl&it file (side by side)" "Diviser le fichier (cô&te à côte)"
    ::msgcat::mcset fr_fr "Tile all &vertically" "Aligner &verticalement"
    ::msgcat::mcset fr_fr "Tile all &horizontally" "Aligner &horizontalement"
    ::msgcat::mcset fr_fr "Space sashes &evenly" "Homo&généiser"
    ::msgcat::mcset fr_fr "Opened Files" "Fichiers ouverts"

    # menues.tcl - Help menu
    ::msgcat::mcset fr_fr "&Help..." "&Aide..."
    ::msgcat::mcset fr_fr "&What's?..." "&Qu'est-ce ?..."
    ::msgcat::mcset fr_fr "&About" "A &propos"
    ::msgcat::mcset fr_fr "Adding &translations..." "&Traduire Scipad..."
        ::msgcat::mcset fr_fr "Adding Scipad Translations..." "Ajouter une nouvelle traduction de Scipad..."
    ::msgcat::mcset fr_fr "&Bugs \& wishlist" "&Bugs \& demandes"
        ::msgcat::mcset fr_fr "Scipad Known bugs" "Bugs connus de Scipad"
    ::msgcat::mcset fr_fr "Change&log" "&Modifications"
        ::msgcat::mcset fr_fr "Changes in the Scipad codebase" "Liste des modifications dans le code source de Scipad"


# popupmenus.tcl
::msgcat::mcset fr_fr "Open the source of" "Ouvrir le source de"
::msgcat::mcset fr_fr "AddWatch" "Ajouter %s comme espion"
::msgcat::mcset fr_fr "AddGenExp" "Ajouter %s comme expression générique"
::msgcat::mcset fr_fr "Jump to" "Sauter à"


# print.tcl
::msgcat::mcset fr_fr "Print Setup" "Mise en page"
::msgcat::mcset fr_fr "Print Command: " "Commande d'impression : "


# scilabexec.tcl
::msgcat::mcset fr_fr "No point in loading an empty file!" "Charger un fichier vide ne présente pas d'intérêt!"
::msgcat::mcset fr_fr "Silent file save failed" "Enregistrement silencieux impossible"
::msgcat::mcset fr_fr "File could not be saved in a temporary location.\nOverwrite original file?" \
                   "Le fichier n'a pas pu être sauvé dans une destination temporaire.\nEcraser le fichier original ?"
::msgcat::mcset fr_fr "Exec done" "Exécution effectuée"
::msgcat::mcset fr_fr "Matlab files" "Fichiers Matlab"
::msgcat::mcset fr_fr "Matlab file to convert" "Fichier Matlab à importer"
::msgcat::mcset fr_fr "Scilab is converting, please hold on..." \
                   "Scilab est en train de convertir, patientez SVP..."
::msgcat::mcset fr_fr "Matlab file import" "Import d'un fichier Matlab"
::msgcat::mcset fr_fr "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to" \
                   "La conversion du ficher a échoué, voyez dans la fenêtre\
                    Scilab pour plus d'informations -- Veuillez communiquer le\
                    message d'erreur et le fichier Matlab incriminé "
::msgcat::mcset fr_fr "Please select destination path for the xml source of the help file" "Veuillez choisir le chemin de destination pour le source xml du fichier d'aide"
::msgcat::mcset fr_fr "Older version of" "Il existe une version plus ancienne de"
    ::msgcat::mcset fr_fr "found!" "!"
::msgcat::mcset fr_fr "An old version of" "Une ancienne version de"
    ::msgcat::mcset fr_fr "already exists: open the old file instead?" "existe déjà : Ouvrir le fichier existant ?"
::msgcat::mcset fr_fr "Existing file" "Fichier existant"
::msgcat::mcset fr_fr "New skeleton" "Nouveau squelette d'aide"
::msgcat::mcset fr_fr "This feature is no longer available in Scilab 5 due to bug " "Cette fonction n'est plus disponible dans Scilab 5 à cause du bug "
::msgcat::mcset fr_fr "See details at " "Plus d'infos: "
::msgcat::mcset fr_fr "See also:" "Voir aussi :"
::msgcat::mcset fr_fr "Note: Scilab 4.x is offering this feature." "Nota: Scilab 4.x propose cette fonctionnalité."
::msgcat::mcset fr_fr "A solution is to use a backported version of Scipad inside one of these environments, for instance:" "Une solution est d'utiliser une version dite \"backport\" de Scipad à l'intérieur d'un de ces environnements, par exemple :"
::msgcat::mcset fr_fr "Feature missing from Scilab 5" "Fonctionnalité manquante dans Scilab 5"
::msgcat::mcset fr_fr "ScilabEval command cannot be passed to Scilab!" "Une commande ScilabEval ne peut pas être passée à Scilab!"
::msgcat::mcset fr_fr impossibleScilabEval_message \
                   "ATTENTION : Une commande n'a pas pu être passée à Scilab et a été ignorée.\n\n \
                   La raison probable est que vous n'avez pas les droits d'écriture dans le répertoire TMPDIR de Scilab. \
                   Un comportement imprévisible peut se produire.\n\n \
                   La commande en cause est:\n\n-->"
::msgcat::mcset fr_fr impossibleScilabEval_message2 \
                   "ATTENTION : Une commande n'a pas pu être passée à Scilab et a été ignorée \
                   parce qu'elle est trop longue.\n\n \
                   Un comportement imprévisible peut se produire.\n\n \
                   Si vous êtes en cours de débug, essayez de supprimer des variables espionnées.\n\n \
                   La commande en cause est:\n\n-->"
::msgcat::mcset fr_fr "(end of command skipped)" "(fin de la commande supprimée)"
::msgcat::mcset fr_fr "Scilab working" "Scilab occupé"
::msgcat::mcset fr_fr "Scilab is working, please wait for the prompt to execute this command!" \
                   "Scilab est occupé, veuillez attendre le prompt pour exécuter cette commande !"
::msgcat::mcset fr_fr "Scilab is working, wait for the prompt to load file" \
                   "Scilab est occupé, attendez le prompt pour charger le fichier"
::msgcat::mcset fr_fr "Scilab is working, wait for the prompt to execute the selection." \
                   "Scilab est occupé, attendez le prompt pour exécuter la sélection."
::msgcat::mcset fr_fr "Scilab is working, wait for the prompt to convert a Matlab file." \
                   "Scilab est occupé, attendez le prompt pour importer un fichier Matlab."
::msgcat::mcset fr_fr "Scilab is working, wait for the prompt to compile the help file." \
                   "Scilab est occupé, attendez le prompt pour compiler la page d'aide."
::msgcat::mcset fr_fr "Scilab is working, wait for the prompt to issue a debugger command." \
                   "Scilab est occupé, attendez le prompt pour effectuer des commandes de débug."
::msgcat::mcset fr_fr "Scilab execution error" "Erreur Scilab durant l'exécution"
::msgcat::mcset fr_fr "The shell reported an error while trying to execute " "Le shell a rencontré une erreur en tentant d'exécuter "
    ::msgcat::mcset fr_fr ": error " ": erreur "
    ::msgcat::mcset fr_fr "at line " "ligne "
    ::msgcat::mcset fr_fr " of " " de "
::msgcat::mcset fr_fr "Execution aborted!" "Exécution arrêtée!"


# scipad.tcl
::msgcat::mcset fr_fr "NOT CONNECTED TO SCILAB" "PAS DE CONNEXION A SCILAB"


# whichfun.tcl
::msgcat::mcset fr_fr "Being at line" "Etant à la ligne"
    ::msgcat::mcset fr_fr ", function" ", la fonction"
    ::msgcat::mcset fr_fr "begins at" "débute en"
    ::msgcat::mcset fr_fr ", and there are" ", et il y a"
    ::msgcat::mcset fr_fr "continued lines, i.e. we are at line" "lignes multiples, i.e. nous sommes à la ligne"
    ::msgcat::mcset fr_fr "of" "de"
#::msgcat::mcset fr_fr "Function" "Fonction" ; # now global
    ::msgcat::mcset fr_fr "line" "ligne"
::msgcat::mcset fr_fr "The cursor is not currently inside a function body." "Le curseur ne se trouve pas à l'intérieur d'une fonction."
::msgcat::mcset fr_fr "Which function?" "Quelle fonction ?"


#preference variables description
::msgcat::mcset fr_fr "BGCOLOR" "Arrière-plan de la zone d'édition"
::msgcat::mcset fr_fr "BGLNMARGCOLOR" "Arrière-plan des numéros de ligne"
::msgcat::mcset fr_fr "FGCOLOR" "Texte brut"
::msgcat::mcset fr_fr "FGLNMARGCOLOR" "Texte des numéros de ligne"
::msgcat::mcset fr_fr "CURCOLOR" "Curseur"
::msgcat::mcset fr_fr "PARCOLOR" "Parenthèses ()"
::msgcat::mcset fr_fr "BRAKCOLOR" "Crochets \\\[\\\]"
::msgcat::mcset fr_fr "BRACCOLOR" "Accolades {}"
::msgcat::mcset fr_fr "PUNCOLOR"  "Ponctuation"
::msgcat::mcset fr_fr "INTFCOLOR" "Fonctions internes"
::msgcat::mcset fr_fr "COMMCOLOR" "Commandes Scilab"
::msgcat::mcset fr_fr "SCICCOLOR" "Fonctions Scicos"
::msgcat::mcset fr_fr "OPCOLOR"   "Opérateurs"
::msgcat::mcset fr_fr "QTXTCOLOR"  "Chaîne de caractères"
::msgcat::mcset fr_fr "REMCOLOR" "Commentaires"
::msgcat::mcset fr_fr "XMLCOLOR" "Balises XML"
::msgcat::mcset fr_fr "NUMCOLOR" "Nombres"
::msgcat::mcset fr_fr "SELCOLOR" "Sélection"
::msgcat::mcset fr_fr "BREAKPOINTCOLOR" "Point d'arrêt"
::msgcat::mcset fr_fr "LFUNCOLOR" "Fonctions de librairie"
::msgcat::mcset fr_fr "PDEFCOLOR" "Variables prédéfinies"
::msgcat::mcset fr_fr "FOUNDTEXTCOLOR" "Texte trouvé"
::msgcat::mcset fr_fr "CONTLINECOLOR" "Ligne incomplète"
::msgcat::mcset fr_fr "USERFUNCOLOR" "Fonctions utilisateur"


#overriding tcl-core dialogs (many are actually already found in 
#  TKDIR/msgs/fr.msg)
::msgcat::mcset fr_fr "&Directory:" "&Répertoire :"
::msgcat::mcset fr_fr "File &name:" "&Nom de fichier :"
::msgcat::mcset fr_fr "Files of &type:" "Fichiers de &type :"
#::msgcat::mcset fr_fr "&Save" "&Enregistrer" # already included
::msgcat::mcset fr_fr "&Open" "&Ouvrir"
::msgcat::mcset fr_fr "Open" "Ouvrir"
::msgcat::mcset fr_fr "Save As" "Enregistrer sous"
::msgcat::mcset fr_fr "&Cancel" "&Annuler"
::msgcat::mcset fr_fr "&Selection:" "&Sélection :"
::msgcat::mcset fr_fr "&Yes" "&Oui"
::msgcat::mcset fr_fr "&No" "&Non"
::msgcat::mcset fr_fr "&Red" "&Rouge"
::msgcat::mcset fr_fr "&Green" "&Vert"
::msgcat::mcset fr_fr "&Blue" "&Bleu"
::msgcat::mcset fr_fr "Directory \"%1\$s\" does not exist."\
        "Le répertoire \"%1\$s\" n'existe pas."
