# Scipad texts in Danish
# in sync at least with v5.75


# commons
::msgcat::mcset da "Cancel" "Annuller"
::msgcat::mcset da "Cance&l" "&Annuller"
::msgcat::mcset da "OK" "OK"
::msgcat::mcset da "Scilab working" "Scilab arbejder"
::msgcat::mcset da "All files" "Alle filer"
::msgcat::mcset da "Add/Change" "Tilføj/Rediger"
::msgcat::mcset da "Untitled" "Unavngivet"
::msgcat::mcset da "Close" "Luk"
::msgcat::mcset da "&Close" "&Luk"


# buffernavigation.tcl
::msgcat::mcset da "Goto Line?" "Gå til linie?"
::msgcat::mcset da "Go to:" "Gå til: "
::msgcat::mcset da "&logical line" "&logisk linie"
::msgcat::mcset da "&physical line" "&fysisk linie"
::msgcat::mcset da "in &function" "i f&unktion"
::msgcat::mcset da "in &current file" "i &aktuel fil"
::msgcat::mcset da "Outside of function definition" "Uden for funktions&definition"


# colorize.tcl
::msgcat::mcset da "Wait seconds while recolorizing file" "Vent et øjeblik under filens farvelægning"
::msgcat::mcset da "You can't do that while colorization is in progress.\
                    Please try again when finished." \
                   "Det kan du ikke gøre under farvelægningen.\
                    Prøv venligst igen bagefter."
::msgcat::mcset da "Command forbidden during colorization" "Kommando forbudt under farvelægning"


# completion.tcl
::msgcat::mcset da "No possible completion" "Ingen mulig afslutning"
::msgcat::mcset da "possible completions" "mulige afslutninger"
::msgcat::mcset da "Possible completions" "Mulige afslutninger"


# db_argsmanagement.tcl
::msgcat::mcset da "Variable:" "Variabel:"
::msgcat::mcset da "Value:" "Værdi:"
::msgcat::mcset da "Get current value from Scilab" "Hent aktuel værdi fra Scilab"


# db_configureexec.tcl
::msgcat::mcset da "Configure execution" "Konfigurer udførelse"
::msgcat::mcset da "Function name:" "Funktionsnavn:"
::msgcat::mcset da "Obtain" "Hent"
::msgcat::mcset da "Input arguments:" "Input-argumenter:"
::msgcat::mcset da "The selected function has not been found in the opened files, maybe it was deleted since last configuration of the debugger.\nUse button \"" \
                   "Den valgte funktion blev ikke fundet i de åbnede filer, den er måske blevet slettet siden den sidste konfiguration af debuggeren.\nBrug knappen \""
    ::msgcat::mcset da "\" to refresh the list of available functions." \
                       "\" for at opdatere listen over tilgængelige funktioner."
::msgcat::mcset da "Function name or input arguments do not match definition of the function" \
                    "Funktionsnavn eller input-argumenter matcher ikke funktionens definition"
    ::msgcat::mcset da "in the file!\n\nCheck function\
                        name and arguments (names, order) in the configuration dialog.\
                        \nArguments order can be changed using drag and drop with\
                        right mouse button in the arguments listbox." \
                       "i filen!\n\n\Tjek funktionens\
                        navn og argumenter (navne, rækkefølge) i konfigurationsdialogen.\
                        \nArgumenternes rækkefølge kan ændres ved træk/slip med\
                        højre museknap i argumenternes listebox."
::msgcat::mcset da "Error in selected function name or arguments" "Fejl i valgt funktionsnavn eller argumenter"
::msgcat::mcset da "Debugger is not yet developed for sce scripts!" "Debuggeren er endnu ikke udviklet til sce-scripts!"


# db_debugsession.tcl
::msgcat::mcset da "Scilab is working, wait for the prompt to issue a debugger command." "Scilab arbejder, vent på prompten for at bruge en debugger-kommando."
::msgcat::mcset da "Cursor position is out of reach!" "Markørposition uden for rækkevidde!"
::msgcat::mcset da "Cursor must be in a function" "Markøren skal være i en funktion"
::msgcat::mcset da "No effect - The debugged file is not stuck" "Ingen effekt - Den debuggede fil er ikke blokeret"
::msgcat::mcset da "Scilab execution error" "Fejl i Scilabs udførelse"
::msgcat::mcset da "The shell reported an error while trying to execute " "Shellen rapporterede en fejl under forsøget på udførelse "
    ::msgcat::mcset da " at line " " i linie "
    ::msgcat::mcset da " of " " af "
::msgcat::mcset da "Execution aborted!" "Udførelse afbrudt!"


# db_init.tcl
::msgcat::mcset da "Please wait..." "Vent venligst..."


# db_states.tcl
::msgcat::mcset da "Currently no debug session" "Ingen aktuel debug-session"
::msgcat::mcset da "Ready to start debug" "Klar til start af debug"
::msgcat::mcset da "Debug in progress" "Debug i gang"


# db_watch.tcl
::msgcat::mcset da "Watch" "Overvåg"
::msgcat::mcset da "Watch variables:" "Overvågede variable:"
::msgcat::mcset da "Call stack:" "Call stack:"
::msgcat::mcset da "Show watch variables" "Vis overvågede variable"
::msgcat::mcset da "Hide watch variables" "Skjul overvågede variable"
::msgcat::mcset da "Show call stack" "Vis call stack"
::msgcat::mcset da "Hide call stack" "Skjul kaldbunken"
::msgcat::mcset da "Undefined elements are not legal in variable" "Udefinerede elementer er ulovlige i variable"
::msgcat::mcset da ".\nThis variable will not be updated in Scilab." ".\nDenne variabel bliver ikke opdateret i Scilab"
::msgcat::mcset da "Illegal undefined element found" "Ulovlig udefineret element fundet"


# filecommands.tcl
::msgcat::mcset da "New Script" "Nyt script"
::msgcat::mcset da "The contents of" "Vil du gemme dine ændringer i"
    ::msgcat::mcset da "may have changed, do you wish to save your changes?" "?"
::msgcat::mcset da "Save Confirm?" "Bekræft gem?"
::msgcat::mcset da "Open source of..." "Åbn kilden til..."
::msgcat::mcset da "Open source of:" "Åbn kilden til:"
::msgcat::mcset da "Weird drag&drop" "Underlig træk&slip"
::msgcat::mcset da "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                    "er hverken et gyldigt filnavn eller en gyldig\
                     destination. Du trækker enten et objekt af en anden type\
                     eller du har ramt en bug i træk&slip-mekanismen."
::msgcat::mcset da "File not found" "Fil ikke fundet"
::msgcat::mcset da "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "eksisterer ikke mere. Vil du oprette en\
                    tom fil med det samme navn?"
::msgcat::mcset da "Open file" "Åbn fil"
::msgcat::mcset da "The file" "Filen"
    ::msgcat::mcset da "is already opened! Save the current opened file to another name and reopen it from disk!" "er allerede åbnet! Gem den aktuelle åbnede fil under et andenavn og genåbn den fra disken!"
::msgcat::mcset da "File" "Fil"
    ::msgcat::mcset da "saved" "gemt"
::msgcat::mcset da "cannot be written!" "kan ikke skrives!"
::msgcat::mcset da "The contents of " "Indholdet af"
    ::msgcat::mcset da "has changed on disk, save it anyway?" "er ændret på disken, gem den alligevel?"
    ::msgcat::mcset da "has been modified outside of Scipad. Do you want to reload it?" \
                       "er blevet ændret uden for Scipad. Vil du genindlæse den?"
::msgcat::mcset da "File has changed!" "Filen er ændret"
::msgcat::mcset da "Save as" "Gem som"
::msgcat::mcset da "Scilab files" "Scilab-filer"
::msgcat::mcset da "Scicos files" "Scicos-filer"
::msgcat::mcset da "XML files" "XML-filerne"
::msgcat::mcset da "Wait seconds while loading and colorizing file" "Vent et øjeblik under indlæsning og farvelægning af filen"
::msgcat::mcset da "The contents of this file may have changed, do you wish to save your changes?" "Indholdet af filen er måske ændret, vil du gemme dine ændringer?"
::msgcat::mcset da "Revert" "Før tilbage til versionen gemt i"
    ::msgcat::mcset da "to saved?" "?"
::msgcat::mcset da "Revert Confirm?" "Bekræft tilbageførsel?"
::msgcat::mcset da "Unreadable file" "Ulæselig fil"
::msgcat::mcset da "exists but is not readable!" "men er ikke læselig!"


# findreplace.tcl
::msgcat::mcset da "The string" "Strengen"
    ::msgcat::mcset da "could not be found in" "kunne ikke findes i"
    ::msgcat::mcset da "could not be found in any of the opened files !" "kunne ikke findes i nogen af de åbnede filer!"
::msgcat::mcset da "replacements done" "erstatninger udført"
::msgcat::mcset da "Find" "Find"
::msgcat::mcset da "No match found in the selection for" "Strengen"
    ::msgcat::mcset da "\nWould you like to look for it in the entire text?" "blev ikke fundet i markeringen.\nVil du søge efter dem i hele teksten?"
::msgcat::mcset da "You are searching for an empty string!" "Du søger en tom streng!"
::msgcat::mcset da "Re&place" "&Erstat"
::msgcat::mcset da "Find what:" "Find:"
::msgcat::mcset da "Replace with:" "Erstat med:"
::msgcat::mcset da "Find &Next" "Find &Næste"
::msgcat::mcset da "Replace &All" "Erstat &alle"
::msgcat::mcset da "&Upwards" "&Opad"
::msgcat::mcset da "&Downwards" "Ne&dad" 
::msgcat::mcset da "Match &whole word only" "Match kun &hele ord"
::msgcat::mcset da "Match &case" "Forskel på &store og små bogstaver"
::msgcat::mcset da "&Regular expression" "A&lmindeligt udtryk"
::msgcat::mcset da "In all &opened files" "I alle åbnede &filer"
::msgcat::mcset da "In &selection only" "Kun i &markeringen"
::msgcat::mcset da "In a director&y" "I en des&tination"
::msgcat::mcset da "Directory r&ecurse search" "&Rekursiv destinationssøgning"
::msgcat::mcset da "In files/file types:" "I filerne/filtyperne:"
::msgcat::mcset da "In directory:" "I destinationen:"
::msgcat::mcset da "Wrapped around" "Ombrudt"
::msgcat::mcset da "Back at the first file searched!" "Tilbage til den først søgte fil!"
::msgcat::mcset da "No more matches" "Ikke flere resultater"
::msgcat::mcset da "The string to regexp search for cannot be compiled by Tcl.\nCheck again, maybe certain characters should be escaped." \
                   "Søgestrengen er ugyldig (umulig at kompilere med Tcl).\nTjek igen, måske skal visse tegn undgås."


# findinfiles.tcl
::msgcat::mcset da "Directory access issue" "Problem med adgang til destination"
::msgcat::mcset da "The directory you specified cannot be reached. The search won't bring any result." \
                   "Destinationen du valgte kan ikke nås. Søgningen giver intet resultat."
::msgcat::mcset da "Some directories or files could not be open and will be ignored during search.\n\
                    You might miss read access to them.\n\n\
                    The following ones will be ignored" \
                   "Visse destinationer eller filer kunne ikke åbnes og ignoreres under søgningen.\n\
                    Du kan miste læseadgang til dem.\n\n\
                    De følgende elementer ignoreres"
::msgcat::mcset da "Ignored files" "Ignorerede filer"
::msgcat::mcset da "Searching in file:" "Søger i filen:"
::msgcat::mcset da "End of file search:" "Søgningens afslutning:"
::msgcat::mcset da "matches found" "fundne resultater"
::msgcat::mcset da "Constructing file list:" "Opretter filliste:"
::msgcat::mcset da "&Previous" "&Forrige"
::msgcat::mcset da "&Next" "&Næste"
::msgcat::mcset da "Scipad search results" "Søgeresultater i Scipad"


# helps.tcl
::msgcat::mcset da "About" "Om"
::msgcat::mcset da aboutme_message \
                   "\n Skabt af Joseph Acosta, joeja@mindspring.com.\n\
                    Modificeret af Scilab Consortium.\n\
                    Revideret af Enrico Segre 2003-2005.\n\
                    Diverse forbedringer, særligt værktøjer til debug,\
                    af François Vogel 2004-2005.\n\
                    Svensk oversættelse af Gunnar Lindholm 2005.\n\
                    Norsk oversættelse af Torbjørn Pettersen 2005.\n\
                    Tysk oversættelse af Dirk Reusch 2005.\n\
                    Polsk oversættelse af Bartosz Kosiorek 2005.\n\
                    Spansk oversættelse af Nicolás Quesada 2006.\n\
                    Dansk oversættelse af Kim B. Hansen 2006."

# infomessages.tcl
::msgcat::mcset da "Line:" "Linie:"
    ::msgcat::mcset da "Column:" "Kolonne:"
::msgcat::mcset da "Line" "Linie"
    ::msgcat::mcset da "in" "i"
::msgcat::mcset da "Logical line:" "Logisk linie:"
::msgcat::mcset da " \[ReadOnly\]" " \[Skrivebeskyttet\]"
::msgcat::mcset da " (modified)" " (ændret)"


# inputtext.tcl
::msgcat::mcset da "No corresponding <" "Intet <"
    ::msgcat::mcset da "> found!" "> resultat fundet!"
::msgcat::mcset da "Code editing is not allowed during debug!" "Redigering af kode er ikke tilladt under debug!"


# menues.tcl
::msgcat::mcset da "&File" "&Fil"
::msgcat::mcset da "&Edit" "&Rediger"
::msgcat::mcset da "&Search" "&Søg"
::msgcat::mcset da "E&xecute" "&Kør"
::msgcat::mcset da "&Debug" "&Debug"
::msgcat::mcset da "S&cheme" "S&prog"
::msgcat::mcset da "&Options" "&Indstillinger"
::msgcat::mcset da "&Windows" "&Vinduer"
::msgcat::mcset da "&Help" "&Hjælp"

    # menues.tcl - File menu
    ::msgcat::mcset da "&New" "&Ny"
    ::msgcat::mcset da "&Open..." "&Åbn..."
    ::msgcat::mcset da "Open &in new" "Åbn i &nyt vindue"
        ::msgcat::mcset da "&horizontal tile" "&vandret"
        ::msgcat::mcset da "&vertical tile" "&lodret"
    ::msgcat::mcset da "Op&en source of..." "Åbn &kilden til..."
    ::msgcat::mcset da "&Save" "&Gem"
    ::msgcat::mcset da "Save &as..." "Gem &som..."
    ::msgcat::mcset da "&Revert..." "&Tilbagefør..."
    ::msgcat::mcset da "Import &Matlab file..."  "Importer &Matlab-fil..."
    ::msgcat::mcset da "Create help s&keleton..." "Opret &hjælpeskelet..."
    ::msgcat::mcset da "Compile as &help page" "Komp&iler som hjælpeside"
    ::msgcat::mcset da "Open &function source" "Åbn &funktionskilde"
    ::msgcat::mcset da "Print Se&tup" "P&rintopsætning"
    ::msgcat::mcset da "&Print" "&Print"
    ::msgcat::mcset da "E&xit" "F&orlad"

    # menues.tcl - Edit menu
    ::msgcat::mcset da "&Undo" "&Fortryd"
    ::msgcat::mcset da "&Redo" "&Annuller fortryd"
    ::msgcat::mcset da "Cu&t" "&Klip"
    ::msgcat::mcset da "&Copy" "K&opier"
    ::msgcat::mcset da "&Paste" "&Sæt ind"
    ::msgcat::mcset da "&Delete" "S&let"
    ::msgcat::mcset da "Select &All" "&Vælg alt"
    ::msgcat::mcset da "Insert &Time/Date" "&Indsæt Tid/Dato"
    ::msgcat::mcset da "Co&mment selection" "Kommenter &markeringen"
    ::msgcat::mcset da "U&ncomment selection" "Afkommenter ma&rkeringen"
    ::msgcat::mcset da "&Indent selection" "In&dryk markering"
    ::msgcat::mcset da "Unin&dent selection" " &Udryk markeringen"

    # menues.tcl - Search menu
    ::msgcat::mcset da "&Find..." "&Find..."
    ::msgcat::mcset da "Find &Next" "Find &næste"
    ::msgcat::mcset da "&Replace..." "&Erstat..."
    ::msgcat::mcset da "&Goto Line..." "&Gå til linie..."

    # menues.tcl - Exec menu
    ::msgcat::mcset da "&Load into Scilab" "&Indlæs i Scilab"
    ::msgcat::mcset da "&Evaluate selection" "&Evaluer markering"

    # menues.tcl - Debug menu
    ::msgcat::mcset da "&Insert/Remove breakpoint" "&Indsæt/Fjern stoppunkt"
    ::msgcat::mcset da "Remove &all breakpoints" "&Fjern alle stoppunkter"
    ::msgcat::mcset da "&Configure execution..." "&Konfigurer udførelse..."
    ::msgcat::mcset da "Go to next b&reakpoint" "&Gå til næste stoppunkt"
    ::msgcat::mcset da "&Step by step" "&Trin for trin"
    ::msgcat::mcset da "Step &into" "Træd &ind i"
    ::msgcat::mcset da "Step o&ver" "Træd &forbi"
    ::msgcat::mcset da "Step &out" "Træd &ud"
    ::msgcat::mcset da "Run to c&ursor" "Kør til &markør"
    ::msgcat::mcset da "G&o on ignoring any breakpoint" "F&ortsæt med at ignorere ethvert stoppunkt"
    ::msgcat::mcset da "Show &watch" "Vis o&vervågning"
    ::msgcat::mcset da "&Break" "&Stop"
    ::msgcat::mcset da "Cance&l debug" "&Annuller debug"

#    ::msgcat::mcset da "language scheme" "sprogskema"
    # menues.tcl - Scheme menu
    ::msgcat::mcset da "S&cilab" "&Scilab"
    ::msgcat::mcset da "&XML" "&XML"
    ::msgcat::mcset da "&none" "&intet"

    # menues.tcl - Options menu
    ::msgcat::mcset da "&Font size" "&Skriftstørrelse"
        ::msgcat::mcset da "&micro" "&mikro"
        ::msgcat::mcset da "&small" "&lille"
        ::msgcat::mcset da "m&edium" "m&edium"
        ::msgcat::mcset da "&large" "&stor"
    ::msgcat::mcset da "&Colors" "&Farver"
    ::msgcat::mcset da "Colorize \'&strings\'" "Farvelæg \'stre&nge\'"
    ::msgcat::mcset da "Show c&ontinued lines" "Vis fortsatte &linier"
    ::msgcat::mcset da "Word &Wrap" "Om&bryd tekst"
    ::msgcat::mcset da "&Tabs and indentation" "Fane&r og indrykning"
        ::msgcat::mcset da "Tab inserts &spaces" "&Tab indsætter mellemrum"
        ::msgcat::mcset da "&Indentation spaces" "&Størrelse af indrykning"
    ::msgcat::mcset da "Com&pletion" "&Afslutning"
        ::msgcat::mcset da "&Tab" "&Tab"
        ::msgcat::mcset da "&Control-Tab" "&Control-Tab"
        ::msgcat::mcset da "&Alt-Tab" "Alt-Ta&b"
        ::msgcat::mcset da "&Shift-Tab" "&Shift-Tab"
        ::msgcat::mcset da "C&ontrol-Alt-Tab" "&Control-Alt-Tab"
        ::msgcat::mcset da "S&hift-Control-Tab" "S&hift-Control-Tab"
        ::msgcat::mcset da "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
        ::msgcat::mcset da "Shi&ft-Control-Alt-Tab" "Shif&t-Control-Alt-Tab"
    ::msgcat::mcset da "File&names" "Fil&navne"
        ::msgcat::mcset da "&Full path" "&Fuld sti"
        ::msgcat::mcset da "Full path if &ambiguous" "Fuld sti hvis &tvetydig"
        ::msgcat::mcset da "&Unambiguous pruned path" "&Utvetydigt beskåret sti"
    ::msgcat::mcset da "&Recent files" "&Seneste filer"
    ::msgcat::mcset da "&Backup files depth" "Dybde af &backup-filer"
    ::msgcat::mcset da "&Locale" "S&prog"

    # menues.tcl - Windows menu
    ::msgcat::mcset da "&Maximize" "&Maksimer"
    ::msgcat::mcset da "&Split" "&Opdel"
    ::msgcat::mcset da "S&plit (side by side)" "Opdel (&side om side)"
    ::msgcat::mcset da "Tile all &vertically" "Arranger &lodret"
    ::msgcat::mcset da "Tile all &horizontally" "Arranger &vandret"
    ::msgcat::mcset da "Space sashes &evenly" "&Ens mellemrum mellem vinduer"
    ::msgcat::mcset da "Opened Files" "Åbnede filer"

    # menues.tcl - Help menu
    ::msgcat::mcset da "&Help..." "&Hjælp"
    ::msgcat::mcset da "&What's?..." "H&vad er?..."
    ::msgcat::mcset da "&About" "&Om"
    ::msgcat::mcset da "Adding &translations..." "&Tilføj oversættelser..."
        ::msgcat::mcset da "Adding Scipad Translations..." "Tilføj Scipad-oversættelser..."
    ::msgcat::mcset da "&Bugs \& wishlist" "&Bugs \& ønskeliste"
        ::msgcat::mcset da "Scipad Known bugs" "Kende Scipad-bugs"
    ::msgcat::mcset da "Change&log" "&Ændringer"
        ::msgcat::mcset da "Changes in the Scipad codebase" "Ændringer i Scipad-kodebasen"


# popupmenus.tcl
::msgcat::mcset da "Open the source of" "Åbn kilden til"
::msgcat::mcset da "AddWatch" "Tilføj til overvågning"


# print.tcl
::msgcat::mcset da "Print Setup" "Printopsætning"
::msgcat::mcset da "Print Command: " "Printkommando: "


# scilabexec.tcl
::msgcat::mcset da "No point in loading an empty file!" "Ingen mening i at indlæse en tom fil!"
::msgcat::mcset da "Scilab is working, wait for the prompt to load file" "Scilab arbejder, vent på prompten for at indlæse filen"
::msgcat::mcset da "Exec done" "Udførelse gennemført"
::msgcat::mcset da "Scilab is working, wait for the prompt to execute the selection." "Scilab arbejder, vent på prompten for at køre markeringen"
::msgcat::mcset da "Scilab is working, wait for the prompt to convert a Matlab file." "Scilab arbejder, vent på prompten for at konvertere en Matlab-fil."
::msgcat::mcset da "Matlab files" "Matlab-filer"
::msgcat::mcset da "Matlab file to convert" "Matlab-filer til konvertering"
::msgcat::mcset da "Scilab is converting, please hold on..." "Scilab konverterer, vent venligst..."
::msgcat::mcset da "Matlab file import" "Matlab-filimport"
::msgcat::mcset da "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi" \
                   "Konverteringen af filen mislykkedes, se Scilab-vinduet\
                    for detaljer -- Rapporter evt. fejlteksten og den\
                    fejlende Matlab-fil til\
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"
::msgcat::mcset da "Please select destination path for the xml source of the help file" "Vælg venligst destinationsstien til hjælpfilens xml-kilde"
::msgcat::mcset da "Older version of" "Ældre version af"
    ::msgcat::mcset da "found!" "fundet!"
::msgcat::mcset da "An old version of" "En gammel version af"
    ::msgcat::mcset da "already exists: open the old file instead?" "findes allerede: Åbn den gamle fil i stedet?"
::msgcat::mcset da "Existing file" "Eksisterende fil"
::msgcat::mcset da "New skeleton" "Nyt skelet"
::msgcat::mcset da "Scilab is working, wait for the prompt to compile the help file." "Scilab arbejder, vent på prompten for at kompilere hjælpfilen."
::msgcat::mcset da "ScilabEval command cannot be passed to Scilab!" "En ScilabEval-kommando kan ikke leveres til Scilab!"
::msgcat::mcset da impossibleScilabEval_message \
				   "ADVARSEL: En kommando kunne ikke leveres til Scilab og blev ignoreret.\n\n \
				   Du har højst sandsynligt ikke skriveadgang til Scilabs TMPDIR. \
				   Der kan ske uforudsigelige resultater.\n\n \
				   Den fejlende kommando var:\n\n-->"


# scipad.tcl
::msgcat::mcset da "NOT CONNECTED TO SCILAB" "IKKE FORBUNDET TIL SCILAB"


# textarea.tcl
::msgcat::mcset da "Font size" "Skriftstørrelse"


# whichfun.tcl
::msgcat::mcset da "Being at line" "Befindende sig på linie"
    ::msgcat::mcset da ", function" ", funktionen"
    ::msgcat::mcset da "begins at" "begynder på"
    ::msgcat::mcset da ", and there are" ", og der ere"
    ::msgcat::mcset da "continued lines, i.e. we are at line" "fortsatte linier, altså er vi på linie"
    ::msgcat::mcset da "of" "af"
::msgcat::mcset da "Function" "Funktion"
    ::msgcat::mcset da "line" "linie"
::msgcat::mcset da "The cursor is not currently inside a function body." "Markøren er p.t. ikke inde i en funktions indre."
::msgcat::mcset da "Which function?" "Hvilken funktion?"


#preference variables description
::msgcat::mcset da "BGCOLOR" "Baggrund"
::msgcat::mcset da "FGCOLOR" "Brødtekst"
::msgcat::mcset da "CURCOLOR" "Markør"
::msgcat::mcset da "PARCOLOR" "Parenteser ()"
::msgcat::mcset da "BRAKCOLOR" "Klammer \\\[\\\]"
::msgcat::mcset da "BRACCOLOR" "Tuborgparenteser {}"
::msgcat::mcset da "PUNCOLOR"  "Tegnsætning"
::msgcat::mcset da "INTFCOLOR" "Indre funktioner"
::msgcat::mcset da "COMMCOLOR" "Kommandoer"
::msgcat::mcset da "SCICCOLOR" "Scicos-funtioner"
::msgcat::mcset da "OPCOLOR"   "Operatorer"
::msgcat::mcset da "QTXTCOLOR"  "Strengtekst"
::msgcat::mcset da "REMCOLOR" "Kommentarer"
::msgcat::mcset da "XMLCOLOR" "XML-tags"
::msgcat::mcset da "NUMCOLOR" "Numre"
::msgcat::mcset da "SELCOLOR" "Markering"
::msgcat::mcset da "BREAKPOINTCOLOR" "Stoppunktbaggrund"
::msgcat::mcset da "LFUNCOLOR" "Bibliotek-funktioner"
::msgcat::mcset da "PDEFCOLOR" "Prædefinerede variable"
::msgcat::mcset da "FOUNDTEXTCOLOR" "Funden tekst"
::msgcat::mcset da "CONTLINECOLOR" "Fortsat linie"


#overriding tcl-core dialogs (many are actually already found in 
#  TKDIR/msgs/fr.msg)
::msgcat::mcset da "&Directory:" "&Destination:"
::msgcat::mcset da "File &name:" "&Filnavn:"
::msgcat::mcset da "Files of &type:" "Filer af &type:"
#::msgcat::mcset da "&Save" "&Gem" # already included
::msgcat::mcset da "&Open" "&Åbn"
::msgcat::mcset da "Open" "Åbn"
::msgcat::mcset da "Save As" "Gem som"
::msgcat::mcset da "&Cancel" "&Annuller"
::msgcat::mcset da "&Selection:" "&Markering:"
::msgcat::mcset da "&Yes" "&Ja"
::msgcat::mcset da "&No" "&Nej"
::msgcat::mcset da "&Red" "&Rød"
::msgcat::mcset da "&Green" "G&røn"
::msgcat::mcset da "&Blue" "&Blå"
::msgcat::mcset da "Directory \"%1\$s\" does not exist."\
        "Destinationen \"%1\$s\" findes ikke."
