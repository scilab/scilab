# Scipad texts in French
# in sync at least with v5.75


# commons
::msgcat::mcset fr "Cancel" "Annuler"
::msgcat::mcset fr "Cance&l" "Annu&ler"
::msgcat::mcset fr "OK" "OK"
::msgcat::mcset fr "Scilab working" "Scilab occupé"
::msgcat::mcset fr "All files" "Tous les fichiers"
::msgcat::mcset fr "Add/Change" "Ajouter/Modifier"
::msgcat::mcset fr "Untitled" "SansTitre"
::msgcat::mcset fr "Close" "Fermer"
::msgcat::mcset fr "&Close" "&Fermer"


# buffernavigation.tcl
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
::msgcat::mcset fr "Variable:" "Variable :"
::msgcat::mcset fr "Value:" "Valeur :"
::msgcat::mcset fr "Get current value from Scilab" "Obtenir la valeur actuelle dans Scilab"


# db_configureexec.tcl
::msgcat::mcset fr "Configure execution" "Configurer l'exécution"
::msgcat::mcset fr "Function name:" "Nom de la fonction :"
::msgcat::mcset fr "Obtain" "Obtenir"
::msgcat::mcset fr "Input arguments:" "Arguments d'entrée :"
::msgcat::mcset fr "Remove" "Supprimer"
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
::msgcat::mcset fr "Debugger is not yet developed for sce scripts!" "Débug de scripts .sce non implémenté!"


# db_debugsession.tcl
::msgcat::mcset fr "Scilab is working, wait for the prompt to issue a debugger command." "Scilab est occupé, attendez le prompt pour effectuer des commandes de débug."
::msgcat::mcset fr "Cursor position is out of reach!" "Position du curseur hors d'atteinte!"
::msgcat::mcset fr "Cursor must be in a function" "Le curseur doit être dans une fonction"
::msgcat::mcset fr "No effect - The debugged file is not stuck" "Aucun effet - Le fichier en débug n'est pas bloqué"
::msgcat::mcset fr "Scilab execution error" "Erreur Scilab durant l'exécution"
::msgcat::mcset fr "The shell reported an error while trying to execute " "Le shell a rencontré une erreur en tentant d'exécuter "
    ::msgcat::mcset fr " at line " " ligne "
    ::msgcat::mcset fr " of " " de "
::msgcat::mcset fr "Execution aborted!" "Exécution arrêtée!"


# db_init.tcl
::msgcat::mcset fr "Please wait..." "Veuillez patienter..."


# db_states.tcl
::msgcat::mcset fr "Currently no debug session" "Pas de session de débug en cours"
::msgcat::mcset fr "Ready to start debug" "Prêt pour le débug"
::msgcat::mcset fr "Debug in progress" "Débug en cours"


# db_watch.tcl
::msgcat::mcset fr "Watch" "Espions"
::msgcat::mcset fr "Watch variables:" "Variables espions :"
::msgcat::mcset fr "Remove" "Supprimer"
::msgcat::mcset fr "Call stack:" "Pile des appels:"
::msgcat::mcset fr "Show watch variables" "Montrer les variables"
::msgcat::mcset fr "Hide watch variables" "Cacher les variables"
::msgcat::mcset fr "Show call stack" "Montrer la pile d'appel"
::msgcat::mcset fr "Hide call stack" "Cacher la pile d'appel"
::msgcat::mcset fr "Undefined elements are not legal in variable" "Les éléments indéfinis sont illégaux pour la variable"
::msgcat::mcset fr ".\nThis variable will not be updated in Scilab." ".\nCette variable ne sera pas mise à jour dans Scilab"
::msgcat::mcset fr "Illegal undefined element found" "Elément indéfini illégal identifié"


# filecommands.tcl
::msgcat::mcset fr "New Script" "Nouveau script"
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
::msgcat::mcset fr "The file" "Le fichier"
    ::msgcat::mcset fr "is already opened! Save the current opened file to another name and reopen it from disk!" "est déjà ouvert! Sauvez-le sous un autre nom et rouvrez-le à partir du disque !"
::msgcat::mcset fr "File" "Fichier"
    ::msgcat::mcset fr "saved" "sauvegardé"
::msgcat::mcset fr "cannot be written!" "est en lecture seule!"
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


# findreplace.tcl
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
::msgcat::mcset fr "&Upwards" "Vers le &haut"
::msgcat::mcset fr "&Downwards" "Vers le &bas" 
::msgcat::mcset fr "Match &whole word only" "Mot &entier seulement"
::msgcat::mcset fr "Match &case" "Respecter la &casse"
::msgcat::mcset fr "&Regular expression" "E&xpression régulière"
::msgcat::mcset fr "In all &opened files" "Dans tous les fichiers &ouverts"
::msgcat::mcset fr "In &selection only" "Da&ns la sélection seulement"
::msgcat::mcset fr "In a director&y" "Dans un réper&toire"
::msgcat::mcset fr "Directory r&ecurse search" "Recherche ré&cursive"
::msgcat::mcset fr "In files/file types:" "Dans les fichiers/types :"
::msgcat::mcset fr "In directory:" "Dans le répertoire :"
::msgcat::mcset fr "Wrapped around" "Rebouclé"
::msgcat::mcset fr "Back at the first file searched!" "Revenu au premier fichier !"
::msgcat::mcset fr "No more matches" "Pas d'autre correspondance"
::msgcat::mcset fr "The string to regexp search for cannot be compiled by Tcl.\nCheck again, maybe certain characters should be escaped." \
                   "L'expression régulière entrée est invalide (impossible à compiler).\nVérifiez, il se peut que le caractère d'échappement \\ doive précéder certains caractères."


# findinfiles.tcl
::msgcat::mcset fr "Directory access issue" "Erreur d'accès à un répertoire"
::msgcat::mcset fr "The directory you specified cannot be reached. The search won't bring any result." \
                   "Scipad ne peut pas accéder au répertoire spécifié. La recherche ne donnera aucun résultat."
::msgcat::mcset fr "Some directories or files could not be open and will be ignored during search.\n\
                    You might miss read access to them.\n\n\
                    The following ones will be ignored" \
                   "Certains répertoires ou fichiers n'ont pas pu être ouverts et seront ignorés durant la recherche.\n\
                    Vous n'avez peut-être pas les droits de lecture sur eux.\n\n\
                    Les éléments suivants seront ignorés"
::msgcat::mcset fr "Ignored files" "Fichiers ignorés"
::msgcat::mcset fr "Searching in file:" "Recherche dans :"
::msgcat::mcset fr "End of file search:" "Fin de la recherche:"
::msgcat::mcset fr "matches found" "correspondances trouvées"
::msgcat::mcset fr "Constructing file list:" "Construction de la liste des fichiers:"
::msgcat::mcset fr "&Previous" "&Précédent"
::msgcat::mcset fr "&Next" "&Suivant"
::msgcat::mcset fr "Scipad search results" "Résultats de recherche Scipad"


# helps.tcl
::msgcat::mcset fr "About" "A propos"
::msgcat::mcset fr aboutme_message \
                   "\n Créé par Joseph Acosta, joeja@mindspring.com.\n\
                    Modifié par le Consortium Scilab.\n\
                    Amélioré par Enrico Segre 2003-2005.\n\
                    Améliorations diverses, dont outils de débug, par François Vogel 2004-2005.\n\
                    Traduction en Suédois par Gunnar Lindholm 2005.\n\
                    Traduction en Norvégien par Torbjørn Pettersen 2005.\n\
                    Traduction en Allemand par Dirk Reusch 2005.\n\
                    Traduction en Polonais par Bartosz Kosiorek 2005.\n\
                    Traduction en Espagnol par Juan Nicolás Quesada 2006."


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
    ::msgcat::mcset fr "&Revert..." "Re&venir..."
    ::msgcat::mcset fr "Import &Matlab file..."  "Importer fichier &Matlab..."
    ::msgcat::mcset fr "Create help s&keleton..." "Créer un &squelette d'aide XML..."
    ::msgcat::mcset fr "Compile as &help page" "Compiler comme fichier d'&aide"
    ::msgcat::mcset fr "Open &function source" "Ouvrir le source &de la fonction"
    ::msgcat::mcset fr "Print Se&tup" "Mise en pa&ge"
    ::msgcat::mcset fr "&Print" "&Imprimer"
    ::msgcat::mcset fr "E&xit" "&Quitter"

    # menues.tcl - Edit menu
    ::msgcat::mcset fr "&Undo" "&Annuler"
    ::msgcat::mcset fr "&Redo" "&Répéter"
    ::msgcat::mcset fr "Cu&t" "Co&uper"
    ::msgcat::mcset fr "&Copy" "&Copier"
    ::msgcat::mcset fr "&Paste" "Co&ller"
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
    ::msgcat::mcset fr "Step &into" "Pas à pas &principal"
    ::msgcat::mcset fr "Step o&ver" "Pas à pas &détaillé"
    ::msgcat::mcset fr "Step &out" "Pas à pas &sortant"
    ::msgcat::mcset fr "Run to c&ursor" "Executer jusqu'au cur&seur"
    ::msgcat::mcset fr "G&o on ignoring any breakpoint" "&Continuer sans aucun point d'arrêt"
    ::msgcat::mcset fr "Show &watch" "Fenêtre &watch"
    ::msgcat::mcset fr "&Break" "&Stop"
    ::msgcat::mcset fr "Cance&l debug" "&Annuler le débug"

#    ::msgcat::mcset fr "language scheme" "schéma langage"
    # menues.tcl - Scheme menu
    ::msgcat::mcset fr "S&cilab" "S&cilab"
    ::msgcat::mcset fr "&XML" "&XML"
    ::msgcat::mcset fr "&none" "&aucun"

    # menues.tcl - Options menu
    ::msgcat::mcset fr "&Font size" "&Taille de police"
        ::msgcat::mcset fr "&micro" "&micro"
        ::msgcat::mcset fr "&small" "&petit"
        ::msgcat::mcset fr "m&edium" "m&oyen"
        ::msgcat::mcset fr "&large" "&grand"
    ::msgcat::mcset fr "&Colors" "&Couleurs"
    ::msgcat::mcset fr "Colorize \'&strings\'" "C&olorer \'chaîne\'"
    ::msgcat::mcset fr "Show c&ontinued lines" "Afficher les li&gnes incomplètes"
    ::msgcat::mcset fr "Word &Wrap" "&Retour à la ligne automatique"
    ::msgcat::mcset fr "&Tabs and indentation" "Tabs et &indentation"
        ::msgcat::mcset fr "Tab inserts &spaces" "Tab insère des &espaces"
        ::msgcat::mcset fr "&Indentation spaces" "&Dimension de l'indentation"
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
    ::msgcat::mcset fr "&Recent files" "&Fichiers récents"
    ::msgcat::mcset fr "&Backup files depth" "&Profondeur de sauvegarde"
    ::msgcat::mcset fr "&Locale" "&Langue"
        # locale names (common)
        ::msgcat::mcset fr "de locale (de)" "Deutsch (de)"
        ::msgcat::mcset fr "fr locale (fr)" "Français (fr)"
        ::msgcat::mcset fr "eng locale (eng)" "English (eng)"
        ::msgcat::mcset fr "es locale (es)" "Español (es)"
        ::msgcat::mcset fr "it locale (it)" "Italiano (it)"
        ::msgcat::mcset fr "no locale (no)" "Norsk (no)"
        ::msgcat::mcset fr "pl locale (pl)" "Polski (pl)"
        ::msgcat::mcset fr "se locale (se)" "Svenska (se)"

    # menues.tcl - Windows menu
    ::msgcat::mcset fr "&Maximize" "&Maximiser"
    ::msgcat::mcset fr "&Split" "&Diviser"
    ::msgcat::mcset fr "S&plit (side by side)" "Diviser (&côte à côte)"
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


# print.tcl
::msgcat::mcset fr "Print Setup" "Mise en page"
::msgcat::mcset fr "Print Command: " "Commande d'impression : "


# scilabexec.tcl
::msgcat::mcset fr "No point in loading an empty file!" "Charger un fichier vide ne présente pas d'intérêt!"
::msgcat::mcset fr "Scilab is working, wait for the prompt to load file" "Scilab est occupé, attendez le prompt pour charger le fichier"
::msgcat::mcset fr "Exec done" "Exécution effectuée"
::msgcat::mcset fr "Scilab is working, wait for the prompt to execute the selection." "Scilab est occupé, attendez le prompt pour exécuter la sélection."
::msgcat::mcset fr "Scilab is working, wait for the prompt to convert a Matlab file." "Scilab est occupé, attendez le prompt pour importer un fichier Matlab."
::msgcat::mcset fr "Matlab files" "Fichiers Matlab"
::msgcat::mcset fr "Matlab file to convert" "Fichier Matlab à importer"
::msgcat::mcset fr "Scilab is converting, please hold on..." "Scilab est en train de convertir, patientez SVP..."
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
::msgcat::mcset fr "Scilab is working, wait for the prompt to compile the help file." "Scilab est occupé, attendez le prompt pour compiler la page d'aide."
::msgcat::mcset fr "ScilabEval command cannot be passed to Scilab!" "Une commande ScilabEval ne peut pas être passée à Scilab!"
::msgcat::mcset fr impossibleScilabEval_message \
                   "ATTENTION : Une commande n'a pas pu être passée à Scilab et a été ignorée.\n\n \
                   La raison probable est que vous n'avez pas les droits d'écriture dans le répertoire TMPDIR de Scilab. \
                   Un comportement imprévisible peut se produire.\n\n \
                   La commande en cause est:\n\n-->"


# scipad.tcl
::msgcat::mcset fr "NOT CONNECTED TO SCILAB" "PAS DE CONNEXION A SCILAB"


# textarea.tcl
::msgcat::mcset fr "Font size" "Police taille"


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
::msgcat::mcset fr "BGCOLOR" "Arrière-plan"
::msgcat::mcset fr "FGCOLOR" "Texte brut"
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
