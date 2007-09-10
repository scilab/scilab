# Scipad texts in French
# in sync at least with v6.127


# commons
::msgcat::mcset fr "Cancel" "Annuler"
::msgcat::mcset fr "Cance&l" "Annu&ler"
::msgcat::mcset fr "OK" "OK"
::msgcat::mcset fr "All files" "Tous les fichiers"
::msgcat::mcset fr "Add/Change" "Ajouter/Modifier"
::msgcat::mcset fr "Add/Chan&ge" "Ajouter/&Modifier"
::msgcat::mcset fr "&Remove" "&Supprimer"
::msgcat::mcset fr "Rem&ove" "Supprime&r"
::msgcat::mcset fr "Untitled" "SansTitre"
::msgcat::mcset fr "Close" "Fermer"
::msgcat::mcset fr "&Close" "&Fermer"
::msgcat::mcset fr "Error " "Erreur "
::msgcat::mcset fr "The file" "Le fichier"


# buffernavigation.tcl
::msgcat::mcset fr "Hide" "Cacher"
::msgcat::mcset fr "Close this tile, keep content" "Fermer cette vue, conserver le contenu"
::msgcat::mcset fr "Close this tile" "Fermer cette vue"
::msgcat::mcset fr "Goto Line?" "Aller à la ligne ?"
::msgcat::mcset fr "Go to:" "Aller à : "
::msgcat::mcset fr "&logical line" "la ligne &logique"
::msgcat::mcset fr "&physical line" "la ligne &physique"
::msgcat::mcset fr "in &function" "de la &fonction"
::msgcat::mcset fr "in &current file" "du fichier &courant"
::msgcat::mcset fr "Outside of function definition" "Hors de la définition de la fonction"


# colorize.tcl
::msgcat::mcset fr "Wait seconds while recolorizing file" "Patientez un instant le temps de la colorisation"
::msgcat::mcset fr "You can't do that while colorization is in progress.\
                    Please try again when finished." \
                   "Action interdite durant la colorisation. Veuillez réessayer plus tard."
::msgcat::mcset fr "Command forbidden during colorization" "Commande interdite durant la colorisation"


# completion.tcl
::msgcat::mcset fr "No possible completion" "Aucune terminaison possible"
::msgcat::mcset fr "possible completions" "terminaisons possibles"
::msgcat::mcset fr "Possible completions" "Terminaisons possibles"


# db_argsmanagement.tcl
::msgcat::mcset fr "Add" "Ajouter"
::msgcat::mcset fr "This variable can be watched but cannot be edited!" \
                   "Cette variable peut être espionnée mais ne peut pas être modifiée !"
::msgcat::mcset fr "A new watch variable will be created." \
                   "Une nouvelle variable espion va être créée."
::msgcat::mcset fr "Non editable variable" "Variable non éditable"
::msgcat::mcset fr "Variable:" "Variable :"
::msgcat::mcset fr "Expression:" "Expression :"
::msgcat::mcset fr "Value:" "Valeur :"
::msgcat::mcset fr "&Get current value from Scilab" "Obtenir la valeur &actuelle dans Scilab"
::msgcat::mcset fr "Non watchable variable" "Variable non espionnable"
::msgcat::mcset fr "Sorry, variable" "Désolé, la variable"
    ::msgcat::mcset fr "is special and cannot be watched." "est spéciale et ne peut pas être espionnée."


# db_bptsmanagement.tcl
::msgcat::mcset fr "No breakpoint here!" "Pas de point d'arrêt ici !"


# db_configureexec.tcl
::msgcat::mcset fr "Configure execution" "Configurer l'exécution"
::msgcat::mcset fr "Function name:" "Nom de la fonction :"
::msgcat::mcset fr "&Obtain" "&Obtenir"
::msgcat::mcset fr "Input arguments:" "Arguments d'entrée :"
::msgcat::mcset fr "The selected function has not been found in the opened files, maybe it was deleted since last configuration of the debugger.\nUse button \"" \
                   "La fonction sélectionnée n'a pas pu être trouvée dans les fichiers ouverts, peut-être a-t-elle été supprimée depuis la précédente configuration du débuggueur.\nUtiliser le bouton \""
    ::msgcat::mcset fr "\" to refresh the list of available functions." \
                       "\" pour rafraîchir la liste des fonctions disponibles."
::msgcat::mcset fr "Function name or input arguments do not match definition of the function" \
                    "Le nom de la fonction ou ses arguments ne correspondent pas à la définition de la fonction"
    ::msgcat::mcset fr "in the file!\n\nCheck function\
                        name and arguments (names, order) in the configuration dialog.\
                        \nArguments order can be changed using drag and drop with\
                        right mouse button in the arguments listbox." \
                       "dans le fichier!\n\n\
                        Vérifier le nom de la fonction et ses arguments (nom, ordre\
                        d'apparition) dans la boîte de dialogue de configuration.\n\
                        L'ordre des arguments peut être modifié par glisser/déposer\
                        avec le bouton droit de la souris."
::msgcat::mcset fr "Error in selected function name or arguments" "Erreur sur la fonction sélectionnée ou ses arguments"
::msgcat::mcset fr "At least one input argument does not match definition of the function" \
                    "Au moins un argument ne correspond pas à la définition de la fonction"
    ::msgcat::mcset fr "in the file!\n\nCheck function\
                        arguments (their names) in the configuration dialog."\
                       "dans le fichier!\n\n\
                        Vérifier les arguments de la fonction (leur noms)\
                        dans la boîte de dialogue de configuration."
::msgcat::mcset fr "Error in selected function arguments" "Erreur sur les arguments de la fonction sélectionnée"
::msgcat::mcset fr "Main level code found" "Présence de code hors fonction"
::msgcat::mcset fr "Scipad ancillary file" "Fichier de sous-routines de Scipad"
::msgcat::mcset fr "The current file contains only Scipad ancillaries that cannot be debugged in Scipad. Please select another buffer." \
                   "Le fichier actuel ne contient que des sous-routines utilisées par Scipad, et qui ne peuvent pas être débugguées dans Scipad. Veuillez choisir un autre fichier."
::msgcat::mcset fr "This file contains main level code.\nDebug as a single .sce file?" \
                   "Ce fichier contient du code hors définition de fonction.\nDébugguer en tant qu'unique entité .sce ?"
::msgcat::mcset fr "No function found" "Aucune fonction trouvée"
::msgcat::mcset fr "This file contains no function and will be treated" \
                   " Ce fichier ne contient aucune fonction et sera traité"
    ::msgcat::mcset fr "as a single .sce file.\nStart debugging?" "en tant qu'unique entité .sce.\nCommencer le débug ?"
::msgcat::mcset fr "You must enable colorization to be able to launch the debugger!" \
                   "Vous devez autoriser la colorisation pour lancer le débuggueur !"
::msgcat::mcset fr "The debugger needs colorization" \
                   "Le débuggueur a besoin de la colorisation"
::msgcat::mcset fr "Warning!\n\n \
                 A function missing the closing \"endfunction\" has been found in the currently opened files.\n \
                 Such syntax is now obsolete.\n\n \
                 Please terminate function " \
                   " Attention !\n\n \
                 Une fonction sans \"endfunction\" a été trouvée dans les fichiers actuellement ouverts.\n \
                 Cette syntaxe est obsolète.\n\n \
                 Veuillez terminer la fonction "
::msgcat::mcset fr " and try again." " et essayer une nouvelle fois."
::msgcat::mcset fr "Note: This message might also be triggered by a string\n \
                 quoted with single quotes and containing the word \"function\",\n \
                 when \"Colorize \'strings\'\" is unchecked in the Options menu." \
                   "Nota : Ce message peut aussi être dû à une chaîne de caractères\n \
                 délimitée par des guillemets simples et contenant le mot \"function\",\n \
                 lorsque \"Colorer \'chaîne\'\" n'est pas coché dans le menu Options."
::msgcat::mcset fr "This known bug can be worked around simply by checking \"Colorize \'&strings\'\"." \
                   "Ce bug connu peut être évité simplement en cochant l'option \"Colorer \'chaîne\'\"."
::msgcat::mcset fr "Unterminated function definition found" "Fonction non terminée identifiée"
::msgcat::mcset fr "Warning!\n\n \
                 One or more function is defined more than once in the currently opened files.\n \
                 The debugger cannot cope with multiple copies of functions.\n \
                 The following duplicates were detected:\n" \
                   "Attention !\n\n \
                 Une ou plusieurs fonctions sont définies plus d'une fois dans les fichiers actuellement ouverts.\n \
                 Le débuggueur ne supporte pas les copies multiples de fonctions.\n \
                 Les doublons suivants ont été détectés :\n"
::msgcat::mcset fr "\n--> Definition of function " "\n--> Définition de la fonction "
::msgcat::mcset fr " found in:\n" " trouvée dans :\n"
::msgcat::mcset fr "Duplicate function definitions found" "Fonctions en doublon identifiées"


# db_debugsession.tcl
::msgcat::mcset fr "Too many breakpointed functions" \
                   "Trop de fonctions contenant des points d'arrêt"
::msgcat::mcset fr "You have currently" \
                   "Vos fichiers ouverts définissent actuellement"
    ::msgcat::mcset fr "functions in your opened files." "fonctions."
    ::msgcat::mcset fr "Scilab supports a maximum of" "Scilab supporte au maximum"
    ::msgcat::mcset fr "possible breakpointed functions (see help setbpt)." \
                       "functions avec points d'arrêts (voir help setbpt)."
    ::msgcat::mcset fr "Step-by-step hence cannot be performed." \
                       "Le mode pas à pas n'est en conséquence pas possible."
    ::msgcat::mcset fr "This command will actually run to the next breakpoint." \
                       "Cette commande va atteindre le prochain point d'arrêt."
    ::msgcat::mcset fr "Break hence cannot be performed." \
                       "La commande Stop ne peut pas être exécutée."
::msgcat::mcset fr "Too many breakpoints" "Trop de points d'arrêt"
::msgcat::mcset fr "Executing this command would require to set" \
                   "Exécuter cette commande nécessiterait d'insérer"
    ::msgcat::mcset fr "breakpoints in your opened files." \
                       "points d'arrêt dans vos fichiers ouverts."
    ::msgcat::mcset fr "possible breakpoints in Scilab (see help setbpt)." \
                       "points d'arrêt (voir help setbpt)."
::msgcat::mcset fr "Cursor must be in a function" \
                   "Le curseur doit être dans une fonction"
::msgcat::mcset fr "This command cannot succeed if the function where the debug just stopped is not opened in Scipad!" \
                   "Le fichier dans lequel le débug vient de stopper doit être ouvert pour exécuter cette commande !"
::msgcat::mcset fr "Cannot run to return point" \
                   "Impossible d'exécuter jusqu'au point de retour"
::msgcat::mcset fr "Warning!\nThe line where the debugger just stopped contains a return statement (either endfunction, return or resume) but this statement is not the first one on the line." \
                   "Attention !\nLa ligne sur laquelle le débuggueur vient de s'arrêter contient une instruction de retour (endfunction ou bien return ou bien resume), mais cette instruction n'est pas la première sur la ligne."
::msgcat::mcset fr "As a consequence the debugger cannot predict if the next debug step will really jump outside of the current function, which is what you just asked the debugger to do." \
                   "Par conséquent le débuggueur ne peut pas prédire si le prochain pas de débug va faire sortir de la fonction en cours (i.e. ce que vous venez de demander de réaliser)."
::msgcat::mcset fr "To avoid this unresolvable case, it is recommended to place the return statement alone on its line." \
                   "Pour éviter ce cas indécidable, il est recommandé de placer chaque instruction de retour seule sur sa ligne."
::msgcat::mcset fr "Do you want to see this warning again during this Scipad session?" \
                   "Voulez-vous voir ce message à nouveau durant cette session Scipad?"
::msgcat::mcset fr "Unresolvable case found while running to return point" \
                   "Cas indécidable identifié durant l'exécution jusqu'au point de retour"
::msgcat::mcset fr "No effect - The debugged file is not stuck" \
                   "Aucun effet - Le fichier en débug n'est pas bloqué"


# db_init.tcl
::msgcat::mcset fr "Please wait..." "Veuillez patienter..."


# db_states.tcl
::msgcat::mcset fr "Currently no debug session" "Pas de session de débug en cours"
::msgcat::mcset fr "Ready to start debug" "Prêt pour le débug"
::msgcat::mcset fr "Debug in progress" "Débug en cours"
::msgcat::mcset fr "Scilab is back at the main level now." \
                   "Scilab est de retour au niveau principal."


# db_watch.tcl
::msgcat::mcset fr "Watch" "Espions"
::msgcat::mcset fr "Watch variables:" "Variables espions :"
::msgcat::mcset fr "A&uto (local variables)" "A&uto (variables locales)"
::msgcat::mcset fr "Au&to watch globals too" "Espionner aussi les &globales"
::msgcat::mcset fr "Generic expressions:" "Expressions génériques :"
::msgcat::mcset fr "Call stack:" "Pile des appels :"
::msgcat::mcset fr "Always on to&p" "Toujours à l'&avant-plan"
::msgcat::mcset fr "Hide watch &variables" "Cacher les &variables"
::msgcat::mcset fr "Hide e&xpressions" "Cacher les e&xpressions"
::msgcat::mcset fr "Hide call &stack" "Cacher la &pile d'appel"
::msgcat::mcset fr "&Dock" "&Intégrer"
::msgcat::mcset fr "&Add" "Ajout&er"
::msgcat::mcset fr "&Filter errors" "Fi&ltrer les erreurs"
::msgcat::mcset fr "Breakpoints" "Points d'arrêt"
::msgcat::mcset fr "Breakpointed functions" "Fonctions avec breakpoints"
::msgcat::mcset fr "Scilab busy indicator" "Indicateur d'occupation de Scilab"
::msgcat::mcset fr "Debugger state indicator" "Indicateur de l'état du débuggueur"
::msgcat::mcset fr "Undefined elements are not legal in variable" "Les éléments indéfinis sont illégaux pour la variable"
::msgcat::mcset fr ".\nThis variable will not be updated in Scilab." ".\nCette variable ne sera pas mise à jour dans Scilab"
::msgcat::mcset fr "Illegal undefined element found" "Elément indéfini illégal identifié"
::msgcat::mcset fr "evaluates to:" "est évalué en:"
::msgcat::mcset fr "produces error" "résulte en une erreur"
::msgcat::mcset fr "Scipad cannot find source code for the selected function.\nPlease open the adequate file manually and try again." \
                   "Scipad ne trouve pas le code source de la fonction sélectionnée.\nVeuillez ouvrir le fichier manuellement et réessayer."
::msgcat::mcset fr "Missing source code" "Code source manquant"


# filecommands.tcl
::msgcat::mcset fr "New script" "Nouveau script"
::msgcat::mcset fr "New view on the same file" "Nouvelle vue sur ce fichier"
::msgcat::mcset fr "The contents of" "Voulez-vous enregistrer les modifications apportées à"
    ::msgcat::mcset fr "may have changed, do you wish to save your changes?" "?"
::msgcat::mcset fr "Save Confirm?" "Confirmer sauver ?"
::msgcat::mcset fr "Open source of..." "Ouvrir le code source de..."
::msgcat::mcset fr "Open source of:" "Ouvrir le source de :"
::msgcat::mcset fr "Weird drag&drop" "Glisser&déposer inattendu"
::msgcat::mcset fr "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                    "n'est ni un nom de fichier valide ni un nom de répertoire\
                     valide. Soit vous avez tenté de glisser un objet d'un \
                     type différent, soit c'est un bug du mécanisme de \
                     glisser&déposer."
::msgcat::mcset fr "File not found" "Fichier introuvable"
::msgcat::mcset fr "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "n'existe plus. Voulez-vous créer un nouveau fichier\
                    portant ce nom ?"
::msgcat::mcset fr "Open file" "Ouvrir fichier"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
    ::msgcat::mcset fr "is already opened! Save the current opened file to another name and reopen it from disk!" "est déjà ouvert! Sauvez-le sous un autre nom et rouvrez-le à partir du disque !"
::msgcat::mcset fr "File" "Fichier"
    ::msgcat::mcset fr "saved" "sauvegardé"
::msgcat::mcset fr "cannot be written!" "est en lecture seule !"
::msgcat::mcset fr "The contents of " "Le contenu de"
    ::msgcat::mcset fr "has changed on disk, save it anyway?" "a changé sur le disque, êtes-vous sûr de vouloir le sauvegarder ?"
    ::msgcat::mcset fr "has been modified outside of Scipad. Do you want to reload it?" \
                       "a été modifié hors de Scipad. Voulez-vous le recharger à partir du disque ?"
::msgcat::mcset fr "File has changed!" "Le fichier a changé"
::msgcat::mcset fr "Save as" "Enregistrer sous"
::msgcat::mcset fr "Scilab files" "Fichiers Scilab"
::msgcat::mcset fr "Scicos files" "Fichiers Scicos"
::msgcat::mcset fr "XML files" "Fichiers XML"
::msgcat::mcset fr "Wait seconds while loading and colorizing file" "Patientez un instant le temps du chargement et de la colorisation"
::msgcat::mcset fr "The contents of this file may have changed, do you wish to save your changes?" "Voulez-vous enregistrer les modifications apportées à ce fichier ?"
::msgcat::mcset fr "Revert" "Revenir à la version enregistrée de"
    ::msgcat::mcset fr "to saved?" "?"
::msgcat::mcset fr "Revert Confirm?" "Confirmer revenir ?"
::msgcat::mcset fr "Unreadable file" "Fichier illisible!"
::msgcat::mcset fr "exists but is not readable!" "existe mais ne peut pas être lu!"
::msgcat::mcset fr "This file is rather large and colorization might take some time.\n\nColorize anyway?" \
                   "Ce fichier est de grande taille et sa colorisation peut être longue.\n\nColorer tout de même?"
::msgcat::mcset fr "File size warning" "Avertissement relatif à la taille du fichier"
::msgcat::mcset fr "Windows shortcut file" "Fichier raccourci Windows"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
    ::msgcat::mcset fr "is a Windows shortcut and will not be opened!" \
                       "est un raccourci Windows et ne sera pas ouvert !"


# findreplace.tcl
::msgcat::mcset fr "Dialog box already open!" "Boîte de dialogue déjà ouverte!"
::msgcat::mcset fr "The string" "La chaîne"
    ::msgcat::mcset fr "could not be found in" "n'a pu être trouvée dans"
    ::msgcat::mcset fr "could not be found in any of the opened files !" "n'a pu être trouvée dans aucun des fichiers ouverts !"
::msgcat::mcset fr "replacements done" "remplacements effectués"
::msgcat::mcset fr "Find" "Rechercher"
::msgcat::mcset fr "No match found in the selection for" "La chaîne"
    ::msgcat::mcset fr "\nWould you like to look for it in the entire text?" "n'a pu être trouvée dans la sélection.\nVoulez-vous rechercher dans la totalité du texte ?"
::msgcat::mcset fr "You are searching for an empty string!" "La chaîne à rechercher est vide!"
::msgcat::mcset fr "Re&place" "&Remplacer"
::msgcat::mcset fr "Find what:" "Rechercher :"
::msgcat::mcset fr "Replace with:" "Remplacer par :"
::msgcat::mcset fr "Find &Next" "Rechercher &suivant"
::msgcat::mcset fr "Replace &All" "Remplacer &tout"
::msgcat::mcset fr "Direction" "Sens"
::msgcat::mcset fr "&Upwards" "Vers le &haut"
::msgcat::mcset fr "&Downwards" "Vers le &bas" 
::msgcat::mcset fr "Text types" "Types de texte"
::msgcat::mcset fr "Find options" "Options de recherche"
::msgcat::mcset fr "Match &whole word only" "Mot &entier seulement"
::msgcat::mcset fr "Match &case" "Respecter la &casse"
::msgcat::mcset fr "&Regular expression" "E&xpression régulière"
::msgcat::mcset fr "In all &opened files" "Dans tous les fichiers &ouverts"
::msgcat::mcset fr "In &selection only" "Da&ns la sélection seulement"
::msgcat::mcset fr "In typed te&xt" "Dans des t&ypes de texte"
::msgcat::mcset fr "In a director&y" "Dans un réper&toire"
::msgcat::mcset fr "Find in files search options" "Options de recherche dans un répertoire"
::msgcat::mcset fr "Directory r&ecurse search" "Recherche réc&ursive"
::msgcat::mcset fr "Include &hidden files" "Inclure les fichiers c&achés"
::msgcat::mcset fr "In files/file types:" "Dans les fichiers/types :"
::msgcat::mcset fr "In directory:" "Dans le répertoire :"
::msgcat::mcset fr "Search for file names" "Rechercher dans les noms des fichiers"
::msgcat::mcset fr "You have given an empty search string. Are you sure you want to search among file names?" \
                   "La chaîne à rechercher est vide. Etes-vous sûr de vouloir rechercher parmi les noms des fichiers ?"
::msgcat::mcset fr "Wrapped around" "Rebouclé"
::msgcat::mcset fr "Back at the first file searched!" "Revenu au premier fichier !"
::msgcat::mcset fr "No more matches" "Pas d'autre correspondance"
::msgcat::mcset fr "The string to regexp search for cannot be compiled by Tcl.\nCheck again, maybe certain characters should be escaped." \
                   "L'expression régulière entrée est invalide (impossible à compiler).\nVérifiez, il se peut que le caractère d'échappement \\ doive précéder certains caractères."
::msgcat::mcset fr "Any single character" "Un caractère quelconque"
::msgcat::mcset fr "Zero or more" "Zéro ou plus"
::msgcat::mcset fr "One or more" "Un ou plus"
::msgcat::mcset fr "Zero or one" "Zéro ou un"
::msgcat::mcset fr "Any one character in the set" "Un caractère quelconque de l'ensemble"
::msgcat::mcset fr "Any one character not in the set" "Un caractère quelconque hors de l'ensemble"
::msgcat::mcset fr "Or" "Ou"
::msgcat::mcset fr "Beginning of line" "Début de ligne"
::msgcat::mcset fr "End of line" "Fin de ligne"
::msgcat::mcset fr "Beginning of word" "Début de mot"
::msgcat::mcset fr "End of word" "Fin de mot"
::msgcat::mcset fr "A letter" "Une lettre"
::msgcat::mcset fr "A decimal digit" "Un chiffre décimal"
::msgcat::mcset fr "An alphanumeric (letter or digit)" "Un caractère alphanumérique (lettre ou chiffre)"
::msgcat::mcset fr "A space or tab character" "Un espace ou une tabulation"
::msgcat::mcset fr "A rational number" "Un nombre rationel"
::msgcat::mcset fr "A floating point number" "Un nombre en virgule flottante"
::msgcat::mcset fr "A Scilab comment" "Un commentaire Scilab"
::msgcat::mcset fr "A Scilab name" "Un nom Scilab"
::msgcat::mcset fr "A Scilab continued line (with dots)" "Une ligne continuée Scilab (avec points de suite)"


# findinfiles.tcl
::msgcat::mcset fr "Directory access issue" "Erreur d'accès à un répertoire"
::msgcat::mcset fr "The directory you specified cannot be reached. The search won't bring any result." \
                   "Scipad ne peut pas accéder au répertoire spécifié. La recherche ne donnera aucun résultat."
::msgcat::mcset fr "Some directories or files could not be open and were ignored during search.\n\
                    You might miss read access to them.\n\n\
                    The following ones were ignored" \
                   "Certains répertoires ou fichiers n'ont pas pu être ouverts et ont été ignorés durant la recherche.\n\
                    Vous n'avez peut-être pas les droits de lecture sur eux.\n\n\
                    Les éléments suivants ont été ignorés"
::msgcat::mcset fr "Ignored files" "Fichiers ignorés"
::msgcat::mcset fr "Searching in file:" "Recherche dans :"
::msgcat::mcset fr "End of file search:" "Fin de la recherche:"
::msgcat::mcset fr "matches found" "correspondances trouvées"
::msgcat::mcset fr "searched files" "fichiers examinés"
::msgcat::mcset fr "Constructing file list:" "Construction de la liste des fichiers:"
::msgcat::mcset fr "&Previous" "&Précédent"
::msgcat::mcset fr "&Next" "&Suivant"
::msgcat::mcset fr "Scipad search results" "Résultats de recherche Scipad"


# fonts.tcl
::msgcat::mcset fr "Choose font" "Choisir une police"
::msgcat::mcset fr "Family" "Police"
::msgcat::mcset fr "Style" "Style"
::msgcat::mcset fr "&Bold" "&Gras"
::msgcat::mcset fr "&Italic" "&Italique"
::msgcat::mcset fr "&Underline" "&Souligné"
::msgcat::mcset fr "Size" "Taille"
::msgcat::mcset fr "&Defaults" "Par &défaut"
::msgcat::mcset fr "Use as..." "Utiliser pour..."
::msgcat::mcset fr "&Text font" "Police de &texte"
::msgcat::mcset fr "&Menu font" "Police de &menu"
::msgcat::mcset fr "Sample" "Exemple"
::msgcat::mcset fr "Font size" "Police taille"
::msgcat::mcset fr "Revert fonts" "Revenir aux polices par défaut"
::msgcat::mcset fr "Are you sure you want to revert all fonts to defaults?" \
                   "Voulez-vous vraiment revenir aux polices par défaut ?"


# FormatStringsForWatch.sci
::msgcat::mcset fr " what you try to watch is not supported by the debugger" \
                   " ce que vous essayez d'espionner n'est pas supporté par le débuggueur"
::msgcat::mcset fr " what you try to watch is of type " \
                   " ce que vous tentez d'espionner est de type "
    ::msgcat::mcset fr " - this type is not supported by the debugger" \
                       " - ce type n'est pas supporté par le débuggueur"
::msgcat::mcset fr "Type:" "Type :"
::msgcat::mcset fr "(unsupported)" "(non supporté)"
::msgcat::mcset fr "size:" "taille :"
::msgcat::mcset fr "real" "réel"
::msgcat::mcset fr "complex" "complexe"
::msgcat::mcset fr "elements" "éléments"
::msgcat::mcset fr "graphic handle" "handle de graphique"
::msgcat::mcset fr "uncompiled function" "fonction non compilée"
::msgcat::mcset fr "compiled function" "fonction compilée"
::msgcat::mcset fr "library" "librairie"
::msgcat::mcset fr "pointer" "pointeur"
::msgcat::mcset fr "size implicit index" "indice de taille implicite"
::msgcat::mcset fr "primitive" "primitive"


# FormatWhereForWatch.sci
::msgcat::mcset fr "Breakpoint" "Point d'arrêt"
::msgcat::mcset fr "called at line" "appelé à la ligne "
::msgcat::mcset fr "of exec file" "dans un fichier exec"
::msgcat::mcset fr "of execstr instruction" "dans une instruction execstr"
::msgcat::mcset fr "called under pause" "appelé sous pause"
::msgcat::mcset fr "of macro" "de la macro"


# helps.tcl
::msgcat::mcset fr "About" "A propos"
::msgcat::mcset fr aboutme_message \
                   "\n Créé par Joseph Acosta, joeja@mindspring.com.\n\
                    Modifié par le Consortium Scilab.\n\
                    Amélioré par Enrico Segre 2003-2006.\n\
                    Améliorations diverses, dont outils de débug, par François Vogel 2004-2007.\n\
                    Traductions :\n\
                     . Suédois, Gunnar Lindholm 2005\n\
                     . Norvégien, Torbjørn Pettersen 2005\n\
                     . Allemand, Dirk Reusch 2005\n\
                     . Polonais, Bartosz Kosiorek 2005\n\
                     . Espagnol, Juan Nicolás Quesada 2006\n\
                     . Chinois, Yung-Jang Lee 2007\n\
                     . Danois, Kim B. Hansen 2007."


# infomessages.tcl
::msgcat::mcset fr "Line:" "Ligne:"
    ::msgcat::mcset fr "Column:" "Colonne:"
::msgcat::mcset fr "Line" "Ligne"
    ::msgcat::mcset fr "in" "de"
::msgcat::mcset fr "Logical line:" "Ligne logique:"
::msgcat::mcset fr " \[ReadOnly\]" " \[LectureSeule\]"
::msgcat::mcset fr " (modified)" " (modifié)"


# inputtext.tcl
::msgcat::mcset fr "No corresponding <" "Aucun <"
    ::msgcat::mcset fr "> found!" "> correspondant trouvé !"
::msgcat::mcset fr "Code editing is not allowed during debug!" "Impossible de modifier le code durant le débug !"


# menues.tcl
::msgcat::mcset fr "&File" "&Fichier"
::msgcat::mcset fr "&Edit" "&Edition"
::msgcat::mcset fr "&Search" "&Rechercher"
::msgcat::mcset fr "E&xecute" "E&xécuter"
::msgcat::mcset fr "&Debug" "&Débug"
::msgcat::mcset fr "S&cheme" "&Langage"
::msgcat::mcset fr "&Options" "&Options"
::msgcat::mcset fr "&Windows" "Fe&nêtres"
::msgcat::mcset fr "&Help" "&Aide"

    # menues.tcl - File menu
    ::msgcat::mcset fr "&New" "&Nouveau"
    ::msgcat::mcset fr "&Open..." "&Ouvrir..."
    ::msgcat::mcset fr "Open &in new" "O&uvrir dans une nouvelle fenêtre"
        ::msgcat::mcset fr "&horizontal tile" "&horizontale"
        ::msgcat::mcset fr "&vertical tile" "&verticale"
    ::msgcat::mcset fr "Op&en source of..." "Ouvrir le &code source de..."
    ::msgcat::mcset fr "&Save" "&Enregistrer"
    ::msgcat::mcset fr "Save &as..." "En&registrer sous..."
    ::msgcat::mcset fr "Save a&ll" "Enregistrer &tout"
    ::msgcat::mcset fr "&Revert..." "Re&venir..."
    ::msgcat::mcset fr "Import &Matlab file..."  "Importer fichier &Matlab..."
    ::msgcat::mcset fr "Create help s&keleton..." "Créer un &squelette d'aide XML..."
    ::msgcat::mcset fr "Compile as &help page" "Compiler comme fichier d'&aide"
    ::msgcat::mcset fr "Open &function source" "Ouvrir le source &de la fonction"
    ::msgcat::mcset fr "Print Se&tup" "Mise en pa&ge"
    ::msgcat::mcset fr "&Print" "&Imprimer"
    ::msgcat::mcset fr "&Close file" "&Fermer ce fichier"
    ::msgcat::mcset fr "E&xit" "&Quitter"

    # menues.tcl - Edit menu
    ::msgcat::mcset fr "&Undo" "&Annuler"
    ::msgcat::mcset fr "&Redo" "&Répéter"
    ::msgcat::mcset fr "Cu&t" "Co&uper"
    ::msgcat::mcset fr "B&lock cut" "Couper (mode bl&oc)"
    ::msgcat::mcset fr "&Copy" "&Copier"
    ::msgcat::mcset fr "&Paste" "Co&ller"
    ::msgcat::mcset fr "&Block paste" "Coller (mode &bloc)"
    ::msgcat::mcset fr "&Delete" "&Effacer"
    ::msgcat::mcset fr "Select &All" "Sélectionner &tout"
    ::msgcat::mcset fr "Insert &Time/Date" "Insérer Heure/Date"
    ::msgcat::mcset fr "Co&mment selection" "Co&mmenter la sélection"
    ::msgcat::mcset fr "U&ncomment selection" "&Décommenter la sélection"
    ::msgcat::mcset fr "&Indent selection" "&Indenter la sélection"
    ::msgcat::mcset fr "Unin&dent selection" "Dé&sindenter la sélection"

    # menues.tcl - Search menu
    ::msgcat::mcset fr "&Find..." "&Rechercher..."
    ::msgcat::mcset fr "Find &Next" "Rechercher &suivant"
    ::msgcat::mcset fr "&Replace..." "Rem&placer..."
    ::msgcat::mcset fr "&Goto Line..." "&Atteindre..."

    # menues.tcl - Exec menu
    ::msgcat::mcset fr "&Load into Scilab" "&Charger dans Scilab"
    ::msgcat::mcset fr "&Evaluate selection" "&Evaluer la sélection"

    # menues.tcl - Debug menu
    ::msgcat::mcset fr "&Insert/Remove breakpoint" "&Insérer/Supprimer un point d'arrêt"
    ::msgcat::mcset fr "Remove &all breakpoints" "Supprimer &tous les points d'arrêt"
    ::msgcat::mcset fr "&Configure execution..." "Con&figurer l'exécution..."
    ::msgcat::mcset fr "Go to next b&reakpoint" "Aller jusq&u'au prochain point d'arrêt"
    ::msgcat::mcset fr "&Step by step" "&Pas à pas"
    ::msgcat::mcset fr "Step &into" "Pas à pas &détaillé"
    ::msgcat::mcset fr "Step o&ver" "Pas à pas &principal"
    ::msgcat::mcset fr "Step &out" "Pas à pas &sortant"
    ::msgcat::mcset fr "Run to re&turn point" "Exécuter jusqu'au point de &retour"
    ::msgcat::mcset fr "Run to c&ursor" "Exécuter jusqu'au cur&seur"
    ::msgcat::mcset fr "G&o on ignoring any breakpoint" "&Continuer sans aucun point d'arrêt"
    ::msgcat::mcset fr "Show &watch" "Fenêtre &watch"
    ::msgcat::mcset fr "&Break" "&Stop"
    ::msgcat::mcset fr "Cance&l debug" "&Annuler le débug"

#    ::msgcat::mcset fr "language scheme" "schéma langage"
    # menues.tcl - Scheme menu
    ::msgcat::mcset fr "S&cilab" "S&cilab"
    ::msgcat::mcset fr "&XML" "&XML"
    ::msgcat::mcset fr "&none" "&aucun"
    ::msgcat::mcset fr "&Colorize" "&Colorer"

    # menues.tcl - Options menu
    ::msgcat::mcset fr "&Fonts..." "&Polices..."
    ::msgcat::mcset fr "&Colors" "&Couleurs"
    ::msgcat::mcset fr "Colori&ze" "C&olorer"
        ::msgcat::mcset fr "&Always" "&Toujours"
        ::msgcat::mcset fr "Ask for &big files" "&Demander pour les gros fichiers"
        ::msgcat::mcset fr "&Never" "&Jamais"
        ::msgcat::mcset fr "Colorize \'&strings\'" "Colorer \'&chaîne\'"
    ::msgcat::mcset fr "Line n&umbers" "N&uméros de ligne"
        ::msgcat::mcset fr "&Hide" "&Masqués"
        ::msgcat::mcset fr "&Left aligned" "Alignés à &gauche"
        ::msgcat::mcset fr "&Right aligned" "Alignés à &droite"
    ::msgcat::mcset fr "Show c&ontinued lines" "Surligner les li&gnes incomplètes"
    ::msgcat::mcset fr "Word &wrap" "&Retour à la ligne automatique"
    ::msgcat::mcset fr "&Double-click behavior" "Comportement du &double-clic"
        ::msgcat::mcset fr "&Linux" "&Linux"
        ::msgcat::mcset fr "Windo&ws" "Windo&ws"
        ::msgcat::mcset fr "&Scilab" "&Scilab"
    ::msgcat::mcset fr "&Tabs and indentation" "Tabs et &indentation"
        ::msgcat::mcset fr "Tab inserts &spaces" "Tab insère des &espaces"
        ::msgcat::mcset fr "&Indentation spaces" "&Dimension de l'indentation (espaces)"
        ::msgcat::mcset fr "&Tab size (characters)" "&Taille d'une tabulation (caractères)"
        ::msgcat::mcset fr "Use &keyword indentation" "Indentation sur &mot-clés"
    ::msgcat::mcset fr "Com&pletion" "Ter&minaison"
        ::msgcat::mcset fr "&Tab" "&Tab"
        ::msgcat::mcset fr "&Control-Tab" "&Control-Tab"
        ::msgcat::mcset fr "&Alt-Tab" "&Alt-Tab"
        ::msgcat::mcset fr "&Shift-Tab" "&Shift-Tab"
        ::msgcat::mcset fr "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
        ::msgcat::mcset fr "S&hift-Control-Tab" "S&hift-Control-Tab"
        ::msgcat::mcset fr "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
        ::msgcat::mcset fr "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
    ::msgcat::mcset fr "File&names" "&Noms de fichiers"
        ::msgcat::mcset fr "&Full path" "&Complets"
        ::msgcat::mcset fr "Full path if &ambiguous" "Complets si &ambigus"
        ::msgcat::mcset fr "&Unambiguous pruned path" "&Raccourcis non ambigus"
    ::msgcat::mcset fr  "Windows menu &sorting" "&Tri dans le menu Fenêtres"
        ::msgcat::mcset fr "&File opening order" "Ordre d'ouverture des &fichiers"
        ::msgcat::mcset fr "&Alphabetically" "&Alphabétique"
        ::msgcat::mcset fr "&Most recently used" "Plus &récemment utilisé"
    ::msgcat::mcset fr "&Recent files" "&Fichiers récents"
    ::msgcat::mcset fr "&Backup files depth" "Profond&eur de sauvegarde"
    ::msgcat::mcset fr "&Locale" "&Langue"
    ::msgcat::mcset fr "Execution &errors" "E&rreurs d'exécution"
        ::msgcat::mcset fr "In Scilab &shell only" "Dans la fenêtre &Scilab seulement"
        ::msgcat::mcset fr "Copied in a &message box" "Recopiées dans une &boîte de dialogue"

    # menues.tcl - Windows menu
    ::msgcat::mcset fr "&Maximize" "&Maximiser"
    ::msgcat::mcset fr "&Split" "&Diviser"
    ::msgcat::mcset fr "S&plit (side by side)" "Diviser (&côte à côte)"
    ::msgcat::mcset fr "Sp&lit file" "D&iviser le fichier"
    ::msgcat::mcset fr "Spl&it file (side by side)" "Diviser le fichier (cô&te à côte)"
    ::msgcat::mcset fr "Tile all &vertically" "Aligner &verticalement"
    ::msgcat::mcset fr "Tile all &horizontally" "Aligner &horizontalement"
    ::msgcat::mcset fr "Space sashes &evenly" "Homo&généiser"
    ::msgcat::mcset fr "Opened Files" "Fichiers ouverts"

    # menues.tcl - Help menu
    ::msgcat::mcset fr "&Help..." "&Aide..."
    ::msgcat::mcset fr "&What's?..." "&Qu'est-ce ?..."
    ::msgcat::mcset fr "&About" "A &propos"
    ::msgcat::mcset fr "Adding &translations..." "&Traduire Scipad..."
        ::msgcat::mcset fr "Adding Scipad Translations..." "Ajouter une nouvelle traduction de Scipad..."
    ::msgcat::mcset fr "&Bugs \& wishlist" "&Bugs \& demandes"
        ::msgcat::mcset fr "Scipad Known bugs" "Bugs connus de Scipad"
    ::msgcat::mcset fr "Change&log" "&Modifications"
        ::msgcat::mcset fr "Changes in the Scipad codebase" "Liste des modifications dans le code source de Scipad"


# popupmenus.tcl
::msgcat::mcset fr "Open the source of" "Ouvrir le source de"
::msgcat::mcset fr "AddWatch" "Ajouter %s comme espion"
::msgcat::mcset fr "AddGenExp" "Ajouter %s comme expression générique"
::msgcat::mcset fr "Jump to" "Sauter à"


# print.tcl
::msgcat::mcset fr "Print Setup" "Mise en page"
::msgcat::mcset fr "Print Command: " "Commande d'impression : "


# scilabexec.tcl
::msgcat::mcset fr "No point in loading an empty file!" "Charger un fichier vide ne présente pas d'intérêt!"
::msgcat::mcset fr "Silent file save failed" "Enregistrement silencieux impossible"
::msgcat::mcset fr "File could not be saved in a temporary location.\nOverwrite original file?" \
                   "Le fichier n'a pas pu être sauvé dans une destination temporaire.\nEcraser le fichier original ?"
::msgcat::mcset fr "Exec done" "Exécution effectuée"
::msgcat::mcset fr "Matlab files" "Fichiers Matlab"
::msgcat::mcset fr "Matlab file to convert" "Fichier Matlab à importer"
::msgcat::mcset fr "Scilab is converting, please hold on..." \
                   "Scilab est en train de convertir, patientez SVP..."
::msgcat::mcset fr "Matlab file import" "Import d'un fichier Matlab"
::msgcat::mcset fr "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi" \
                   "La conversion du ficher a échoué, voyez dans la fenêtre\
                    Scilab pour plus d'informations -- Veuillez communiquer le\
                    message d'erreur et le fichier Matlab incriminé à\
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"
::msgcat::mcset fr "Please select destination path for the xml source of the help file" "Veuillez choisir le chemin de destination pour le source xml du fichier d'aide"
::msgcat::mcset fr "Older version of" "Il existe une version plus ancienne de"
    ::msgcat::mcset fr "found!" "!"
::msgcat::mcset fr "An old version of" "Une ancienne version de"
    ::msgcat::mcset fr "already exists: open the old file instead?" "existe déjà : Ouvrir le fichier existant ?"
::msgcat::mcset fr "Existing file" "Fichier existant"
::msgcat::mcset fr "New skeleton" "Nouveau squelette d'aide"
::msgcat::mcset fr "ScilabEval command cannot be passed to Scilab!" "Une commande ScilabEval ne peut pas être passée à Scilab!"
::msgcat::mcset fr impossibleScilabEval_message \
                   "ATTENTION : Une commande n'a pas pu être passée à Scilab et a été ignorée.\n\n \
                   La raison probable est que vous n'avez pas les droits d'écriture dans le répertoire TMPDIR de Scilab. \
                   Un comportement imprévisible peut se produire.\n\n \
                   La commande en cause est:\n\n-->"
::msgcat::mcset fr impossibleScilabEval_message2 \
                   "ATTENTION : Une commande n'a pas pu être passée à Scilab et a été ignorée \
                   parce qu'elle est trop longue.\n\n \
                   Un comportement imprévisible peut se produire.\n\n \
                   Si vous êtes en cours de débug, essayez de supprimer des variables espionnées.\n\n \
                   La commande en cause est:\n\n-->"
::msgcat::mcset fr "(end of command skipped)" "(fin de la commande supprimée)"
::msgcat::mcset fr "Scilab working" "Scilab occupé"
::msgcat::mcset fr "Scilab is working, please wait for the prompt to execute this command!" \
                   "Scilab est occupé, veuillez attendre le prompt pour exécuter cette commande !"
::msgcat::mcset fr "Scilab is working, wait for the prompt to load file" \
                   "Scilab est occupé, attendez le prompt pour charger le fichier"
::msgcat::mcset fr "Scilab is working, wait for the prompt to execute the selection." \
                   "Scilab est occupé, attendez le prompt pour exécuter la sélection."
::msgcat::mcset fr "Scilab is working, wait for the prompt to convert a Matlab file." \
                   "Scilab est occupé, attendez le prompt pour importer un fichier Matlab."
::msgcat::mcset fr "Scilab is working, wait for the prompt to compile the help file." \
                   "Scilab est occupé, attendez le prompt pour compiler la page d'aide."
::msgcat::mcset fr "Scilab is working, wait for the prompt to issue a debugger command." \
                   "Scilab est occupé, attendez le prompt pour effectuer des commandes de débug."
::msgcat::mcset fr "Scilab execution error" "Erreur Scilab durant l'exécution"
::msgcat::mcset fr "The shell reported an error while trying to execute " "Le shell a rencontré une erreur en tentant d'exécuter "
    ::msgcat::mcset fr ": error " ": erreur "
    ::msgcat::mcset fr "at line " "ligne "
    ::msgcat::mcset fr " of " " de "
::msgcat::mcset fr "Execution aborted!" "Exécution arrêtée!"


# scipad.tcl
::msgcat::mcset fr "NOT CONNECTED TO SCILAB" "PAS DE CONNEXION A SCILAB"


# whichfun.tcl
::msgcat::mcset fr "Being at line" "Etant à la ligne"
    ::msgcat::mcset fr ", function" ", la fonction"
    ::msgcat::mcset fr "begins at" "débute en"
    ::msgcat::mcset fr ", and there are" ", et il y a"
    ::msgcat::mcset fr "continued lines, i.e. we are at line" "lignes multiples, i.e. nous sommes à la ligne"
    ::msgcat::mcset fr "of" "de"
::msgcat::mcset fr "Function" "Fonction"
    ::msgcat::mcset fr "line" "ligne"
::msgcat::mcset fr "The cursor is not currently inside a function body." "Le curseur ne se trouve pas à l'intérieur d'une fonction."
::msgcat::mcset fr "Which function?" "Quelle fonction ?"


#preference variables description
::msgcat::mcset fr "BGCOLOR" "Arrière-plan de la zone d'édition"
::msgcat::mcset fr "BGLNMARGCOLOR" "Arrière-plan des numéros de ligne"
::msgcat::mcset fr "FGCOLOR" "Texte brut"
::msgcat::mcset fr "FGLNMARGCOLOR" "Texte des numéros de ligne"
::msgcat::mcset fr "CURCOLOR" "Curseur"
::msgcat::mcset fr "PARCOLOR" "Parenthèses ()"
::msgcat::mcset fr "BRAKCOLOR" "Crochets \\\[\\\]"
::msgcat::mcset fr "BRACCOLOR" "Accolades {}"
::msgcat::mcset fr "PUNCOLOR"  "Ponctuation"
::msgcat::mcset fr "INTFCOLOR" "Fonctions internes"
::msgcat::mcset fr "COMMCOLOR" "Commandes Scilab"
::msgcat::mcset fr "SCICCOLOR" "Fonctions Scicos"
::msgcat::mcset fr "OPCOLOR"   "Opérateurs"
::msgcat::mcset fr "QTXTCOLOR"  "Chaîne de caractères"
::msgcat::mcset fr "REMCOLOR" "Commentaires"
::msgcat::mcset fr "XMLCOLOR" "Balises XML"
::msgcat::mcset fr "NUMCOLOR" "Nombres"
::msgcat::mcset fr "SELCOLOR" "Sélection"
::msgcat::mcset fr "BREAKPOINTCOLOR" "Point d'arrêt"
::msgcat::mcset fr "LFUNCOLOR" "Fonctions de librairie"
::msgcat::mcset fr "PDEFCOLOR" "Variables prédéfinies"
::msgcat::mcset fr "FOUNDTEXTCOLOR" "Texte trouvé"
::msgcat::mcset fr "CONTLINECOLOR" "Ligne incomplète"
::msgcat::mcset fr "USERFUNCOLOR" "Fonctions utilisateur"


#overriding tcl-core dialogs (many are actually already found in 
#  TKDIR/msgs/fr.msg)
::msgcat::mcset fr "&Directory:" "&Répertoire :"
::msgcat::mcset fr "File &name:" "&Nom de fichier :"
::msgcat::mcset fr "Files of &type:" "Fichiers de &type :"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
::msgcat::mcset fr "&Open" "&Ouvrir"
::msgcat::mcset fr "Open" "Ouvrir"
::msgcat::mcset fr "Save As" "Enregistrer sous"
::msgcat::mcset fr "&Cancel" "&Annuler"
::msgcat::mcset fr "&Selection:" "&Sélection :"
::msgcat::mcset fr "&Yes" "&Oui"
::msgcat::mcset fr "&No" "&Non"
::msgcat::mcset fr "&Red" "&Rouge"
::msgcat::mcset fr "&Green" "&Vert"
::msgcat::mcset fr "&Blue" "&Bleu"
::msgcat::mcset fr "Directory \"%1\$s\" does not exist."\
        "Le répertoire \"%1\$s\" n'existe pas."
