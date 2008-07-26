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

# Scipad texts in Danish by Kim B. Hansen
# in sync at least with v5.75


# commons
::msgcat::mcset da_DK "Cancel" "Annuller"
::msgcat::mcset da_DK "Cance&l" "&Annuller"
::msgcat::mcset da_DK "OK" "OK"
::msgcat::mcset da_DK "Scilab working" "Scilab arbejder"
::msgcat::mcset da_DK "All files" "Alle filer"
::msgcat::mcset da_DK "Add/Change" "Tilføj/Rediger"
::msgcat::mcset da_DK "Untitled" "Unavngivet"
::msgcat::mcset da_DK "Close" "Luk"
::msgcat::mcset da_DK "&Close" "&Luk"


# buffernavigation.tcl
::msgcat::mcset da_DK "Goto Line?" "Gå til linie?"
::msgcat::mcset da_DK "Go to:" "Gå til: "
::msgcat::mcset da_DK "&logical line" "&logisk linie"
::msgcat::mcset da_DK "&physical line" "&fysisk linie"
::msgcat::mcset da_DK "in &function" "i f&unktion"
::msgcat::mcset da_DK "in &current file" "i &aktuel fil"
::msgcat::mcset da_DK "Outside of function definition" "Uden for funktions&definition"


# colorize.tcl
::msgcat::mcset da_DK "Wait seconds while recolorizing file" "Vent et øjeblik under filens farvelægning"
::msgcat::mcset da_DK "You can't do that while colorization is in progress.\
                    Please try again when finished." \
                   "Det kan du ikke gøre under farvelægningen.\
                    Prøv venligst igen bagefter."
::msgcat::mcset da_DK "Command forbidden during colorization" "Kommando forbudt under farvelægning"


# completion.tcl
::msgcat::mcset da_DK "No possible completion" "Ingen mulig afslutning"
::msgcat::mcset da_DK "possible completions" "mulige afslutninger"
::msgcat::mcset da_DK "Possible completions" "Mulige afslutninger"


# db_argsmanagement.tcl
::msgcat::mcset da_DK "Variable:" "Variabel:"
::msgcat::mcset da_DK "Value:" "Værdi:"
::msgcat::mcset da_DK "Get current value from Scilab" "Hent aktuel værdi fra Scilab"


# db_configureexec.tcl
::msgcat::mcset da_DK "Configure execution" "Konfigurer udførelse"
::msgcat::mcset da_DK "Function name:" "Funktionsnavn:"
::msgcat::mcset da_DK "Obtain" "Hent"
::msgcat::mcset da_DK "Input arguments:" "Input-argumenter:"
::msgcat::mcset da_DK "The selected function has not been found in the opened files, maybe it was deleted since last configuration of the debugger.\nUse button \"" \
                   "Den valgte funktion blev ikke fundet i de åbnede filer, den er måske blevet slettet siden den sidste konfiguration af debuggeren.\nBrug knappen \""
    ::msgcat::mcset da_DK "\" to refresh the list of available functions." \
                       "\" for at opdatere listen over tilgængelige funktioner."
::msgcat::mcset da_DK "Function name or input arguments do not match definition of the function" \
                    "Funktionsnavn eller input-argumenter matcher ikke funktionens definition"
    ::msgcat::mcset da_DK "in the file!\n\nCheck function\
                        name and arguments (names, order) in the configuration dialog.\
                        \nArguments order can be changed using drag and drop with\
                        right mouse button in the arguments listbox." \
                       "i filen!\n\n\Tjek funktionens\
                        navn og argumenter (navne, rækkefølge) i konfigurationsdialogen.\
                        \nArgumenternes rækkefølge kan ændres ved træk/slip med\
                        højre museknap i argumenternes listebox."
::msgcat::mcset da_DK "Error in selected function name or arguments" "Fejl i valgt funktionsnavn eller argumenter"
::msgcat::mcset da_DK "Debugger is not yet developed for sce scripts!" "Debuggeren er endnu ikke udviklet til sce-scripts!"


# db_debugsession.tcl
::msgcat::mcset da_DK "Scilab is working, wait for the prompt to issue a debugger command." "Scilab arbejder, vent på prompten for at bruge en debugger-kommando."
::msgcat::mcset da_DK "Cursor position is out of reach!" "Markørposition uden for rækkevidde!"
::msgcat::mcset da_DK "Cursor must be in a function" "Markøren skal være i en funktion"
::msgcat::mcset da_DK "No effect - The debugged file is not stuck" "Ingen effekt - Den debuggede fil er ikke blokeret"
::msgcat::mcset da_DK "Scilab execution error" "Fejl i Scilabs udførelse"
::msgcat::mcset da_DK "The shell reported an error while trying to execute " "Shellen rapporterede en fejl under forsøget på udførelse "
    ::msgcat::mcset da_DK " at line " " i linie "
    ::msgcat::mcset da_DK " of " " af "
::msgcat::mcset da_DK "Execution aborted!" "Udførelse afbrudt!"


# db_init.tcl
::msgcat::mcset da_DK "Please wait..." "Vent venligst..."


# db_states.tcl
::msgcat::mcset da_DK "Currently no debug session" "Ingen aktuel debug-session"
::msgcat::mcset da_DK "Ready to start debug" "Klar til start af debug"
::msgcat::mcset da_DK "Debug in progress" "Debug i gang"


# db_watch.tcl
::msgcat::mcset da_DK "Watch" "Overvåg"
::msgcat::mcset da_DK "Watch variables:" "Overvågede variable:"
::msgcat::mcset da_DK "Call stack:" "Call stack:"
::msgcat::mcset da_DK "Show watch variables" "Vis overvågede variable"
::msgcat::mcset da_DK "Hide watch variables" "Skjul overvågede variable"
::msgcat::mcset da_DK "Show call stack" "Vis call stack"
::msgcat::mcset da_DK "Hide call stack" "Skjul kaldbunken"
::msgcat::mcset da_DK "Undefined elements are not legal in variable" "Udefinerede elementer er ulovlige i variable"
::msgcat::mcset da_DK ".\nThis variable will not be updated in Scilab." ".\nDenne variabel bliver ikke opdateret i Scilab"
::msgcat::mcset da_DK "Illegal undefined element found" "Ulovlig udefineret element fundet"


# filecommands.tcl
::msgcat::mcset da_DK "New Script" "Nyt script"
::msgcat::mcset da_DK "The contents of" "Vil du gemme dine ændringer i"
    ::msgcat::mcset da_DK "may have changed, do you wish to save your changes?" "?"
::msgcat::mcset da_DK "Save Confirm?" "Bekræft gem?"
::msgcat::mcset da_DK "Open source of..." "Åbn kilden til..."
::msgcat::mcset da_DK "Open source of:" "Åbn kilden til:"
::msgcat::mcset da_DK "Weird drag&drop" "Underlig træk&slip"
::msgcat::mcset da_DK "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                    "er hverken et gyldigt filnavn eller en gyldig\
                     destination. Du trækker enten et objekt af en anden type\
                     eller du har ramt en bug i træk&slip-mekanismen."
::msgcat::mcset da_DK "File not found" "Fil ikke fundet"
::msgcat::mcset da_DK "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "eksisterer ikke mere. Vil du oprette en\
                    tom fil med det samme navn?"
::msgcat::mcset da_DK "Open file" "Åbn fil"
::msgcat::mcset da_DK "The file" "Filen"
    ::msgcat::mcset da_DK "is already opened! Save the current opened file to another name and reopen it from disk!" "er allerede åbnet! Gem den aktuelle åbnede fil under et andenavn og genåbn den fra disken!"
::msgcat::mcset da_DK "File" "Fil"
    ::msgcat::mcset da_DK "saved" "gemt"
::msgcat::mcset da_DK "cannot be written!" "kan ikke skrives!"
::msgcat::mcset da_DK "The contents of " "Indholdet af"
    ::msgcat::mcset da_DK "has changed on disk, save it anyway?" "er ændret på disken, gem den alligevel?"
    ::msgcat::mcset da_DK "has been modified outside of Scipad. Do you want to reload it?" \
                       "er blevet ændret uden for Scipad. Vil du genindlæse den?"
::msgcat::mcset da_DK "File has changed!" "Filen er ændret"
::msgcat::mcset da_DK "Save as" "Gem som"
::msgcat::mcset da_DK "Scilab files" "Scilab-filer"
::msgcat::mcset da_DK "Scicos files" "Scicos-filer"
::msgcat::mcset da_DK "XML files" "XML-filerne"
::msgcat::mcset da_DK "Wait seconds while loading and colorizing file" "Vent et øjeblik under indlæsning og farvelægning af filen"
::msgcat::mcset da_DK "The contents of this file may have changed, do you wish to save your changes?" "Indholdet af filen er måske ændret, vil du gemme dine ændringer?"
::msgcat::mcset da_DK "Revert" "Før tilbage til versionen gemt i"
    ::msgcat::mcset da_DK "to saved?" "?"
::msgcat::mcset da_DK "Revert Confirm?" "Bekræft tilbageførsel?"
::msgcat::mcset da_DK "Unreadable file" "Ulæselig fil"
::msgcat::mcset da_DK "exists but is not readable!" "men er ikke læselig!"


# findreplace.tcl
::msgcat::mcset da_DK "The string" "Strengen"
    ::msgcat::mcset da_DK "could not be found in" "kunne ikke findes i"
    ::msgcat::mcset da_DK "could not be found in any of the opened files !" "kunne ikke findes i nogen af de åbnede filer!"
::msgcat::mcset da_DK "replacements done" "erstatninger udført"
::msgcat::mcset da_DK "Find" "Find"
::msgcat::mcset da_DK "No match found in the selection for" "Strengen"
    ::msgcat::mcset da_DK "\nWould you like to look for it in the entire text?" "blev ikke fundet i markeringen.\nVil du søge efter dem i hele teksten?"
::msgcat::mcset da_DK "You are searching for an empty string!" "Du søger en tom streng!"
::msgcat::mcset da_DK "Re&place" "&Erstat"
::msgcat::mcset da_DK "Find what:" "Find:"
::msgcat::mcset da_DK "Replace with:" "Erstat med:"
::msgcat::mcset da_DK "Find &Next" "Find &Næste"
::msgcat::mcset da_DK "Replace &All" "Erstat &alle"
::msgcat::mcset da_DK "&Upwards" "&Opad"
::msgcat::mcset da_DK "&Downwards" "Ne&dad" 
::msgcat::mcset da_DK "Match &whole word only" "Match kun &hele ord"
::msgcat::mcset da_DK "Match &case" "Forskel på &store og små bogstaver"
::msgcat::mcset da_DK "&Regular expression" "A&lmindeligt udtryk"
::msgcat::mcset da_DK "In all &opened files" "I alle åbnede &filer"
::msgcat::mcset da_DK "In &selection only" "Kun i &markeringen"
::msgcat::mcset da_DK "In a director&y" "I en des&tination"
::msgcat::mcset da_DK "Directory r&ecurse search" "&Rekursiv destinationssøgning"
::msgcat::mcset da_DK "In files/file types:" "I filerne/filtyperne:"
::msgcat::mcset da_DK "In directory:" "I destinationen:"
::msgcat::mcset da_DK "Wrapped around" "Ombrudt"
::msgcat::mcset da_DK "Back at the first file searched!" "Tilbage til den først søgte fil!"
::msgcat::mcset da_DK "No more matches" "Ikke flere resultater"
::msgcat::mcset da_DK "The string to regexp search for cannot be compiled by Tcl.\nCheck again, maybe certain characters should be escaped." \
                   "Søgestrengen er ugyldig (umulig at kompilere med Tcl).\nTjek igen, måske skal visse tegn undgås."


# findinfiles.tcl
::msgcat::mcset da_DK "Directory access issue" "Problem med adgang til destination"
::msgcat::mcset da_DK "The directory you specified cannot be reached. The search won't bring any result." \
                   "Destinationen du valgte kan ikke nås. Søgningen giver intet resultat."
::msgcat::mcset da_DK "Some directories or files could not be open and will be ignored during search.\n\
                    You might miss read access to them.\n\n\
                    The following ones will be ignored" \
                   "Visse destinationer eller filer kunne ikke åbnes og ignoreres under søgningen.\n\
                    Du kan miste læseadgang til dem.\n\n\
                    De følgende elementer ignoreres"
::msgcat::mcset da_DK "Ignored files" "Ignorerede filer"
::msgcat::mcset da_DK "Searching in file:" "Søger i filen:"
::msgcat::mcset da_DK "End of file search:" "Søgningens afslutning:"
::msgcat::mcset da_DK "matches found" "fundne resultater"
::msgcat::mcset da_DK "Constructing file list:" "Opretter filliste:"
::msgcat::mcset da_DK "&Previous" "&Forrige"
::msgcat::mcset da_DK "&Next" "&Næste"
::msgcat::mcset da_DK "Scipad search results" "Søgeresultater i Scipad"


# helps.tcl
::msgcat::mcset da_DK "About" "Om"
::msgcat::mcset da_DK aboutme_message \
                   "\n Skabt af Joseph Acosta, joeja@mindspring.com.\n\
                    Modificeret af Scilab Consortium.\n\
                    Revideret af Enrico Segre 2003-2005.\n\
                    Diverse forbedringer, særligt værktøjer til debug,\
                    af François Vogel 2004-2008.\n\
                    Svensk oversættelse af Gunnar Lindholm 2005.\n\
                    Norsk oversættelse af Torbjørn Pettersen 2005.\n\
                    Tysk oversættelse af Dirk Reusch 2005.\n\
                    Polsk oversættelse af Bartosz Kosiorek 2005.\n\
                    Spansk oversættelse af Nicolás Quesada 2006.\n\
                    Dansk oversættelse af Kim B. Hansen 2006."

# infomessages.tcl
::msgcat::mcset da_DK "Line:" "Linie:"
    ::msgcat::mcset da_DK "Column:" "Kolonne:"
::msgcat::mcset da_DK "Line" "Linie"
    ::msgcat::mcset da_DK "in" "i"
::msgcat::mcset da_DK "Logical line:" "Logisk linie:"
::msgcat::mcset da_DK " \[ReadOnly\]" " \[Skrivebeskyttet\]"
::msgcat::mcset da_DK " (modified)" " (ændret)"


# inputtext.tcl
::msgcat::mcset da_DK "No corresponding <" "Intet <"
    ::msgcat::mcset da_DK "> found!" "> resultat fundet!"
::msgcat::mcset da_DK "Code editing is not allowed during debug!" "Redigering af kode er ikke tilladt under debug!"


# menues.tcl
::msgcat::mcset da_DK "&File" "&Fil"
::msgcat::mcset da_DK "&Edit" "&Rediger"
::msgcat::mcset da_DK "&Search" "&Søg"
::msgcat::mcset da_DK "E&xecute" "&Kør"
::msgcat::mcset da_DK "&Debug" "&Debug"
::msgcat::mcset da_DK "S&cheme" "S&prog"
::msgcat::mcset da_DK "&Options" "&Indstillinger"
::msgcat::mcset da_DK "&Windows" "&Vinduer"
::msgcat::mcset da_DK "&Help" "&Hjælp"

    # menues.tcl - File menu
    ::msgcat::mcset da_DK "&New" "&Ny"
    ::msgcat::mcset da_DK "&Open..." "&Åbn..."
    ::msgcat::mcset da_DK "Open &in new" "Åbn i &nyt vindue"
        ::msgcat::mcset da_DK "&horizontal tile" "&vandret"
        ::msgcat::mcset da_DK "&vertical tile" "&lodret"
    ::msgcat::mcset da_DK "Op&en source of..." "Åbn &kilden til..."
    ::msgcat::mcset da_DK "&Save" "&Gem"
    ::msgcat::mcset da_DK "Save &as..." "Gem &som..."
    ::msgcat::mcset da_DK "&Revert..." "&Tilbagefør..."
    ::msgcat::mcset da_DK "Import &Matlab file..."  "Importer &Matlab-fil..."
    ::msgcat::mcset da_DK "Create help s&keleton..." "Opret &hjælpeskelet..."
    ::msgcat::mcset da_DK "Compile as &help page" "Komp&iler som hjælpeside"
    ::msgcat::mcset da_DK "Open &function source" "Åbn &funktionskilde"
    ::msgcat::mcset da_DK "Print Se&tup" "P&rintopsætning"
    ::msgcat::mcset da_DK "&Print" "&Print"
    ::msgcat::mcset da_DK "E&xit" "F&orlad"

    # menues.tcl - Edit menu
    ::msgcat::mcset da_DK "&Undo" "&Fortryd"
    ::msgcat::mcset da_DK "&Redo" "&Annuller fortryd"
    ::msgcat::mcset da_DK "Cu&t" "&Klip"
    ::msgcat::mcset da_DK "&Copy" "K&opier"
    ::msgcat::mcset da_DK "&Paste" "&Sæt ind"
    ::msgcat::mcset da_DK "&Delete" "S&let"
    ::msgcat::mcset da_DK "Select &All" "&Vælg alt"
    ::msgcat::mcset da_DK "Insert &Time/Date" "&Indsæt Tid/Dato"
    ::msgcat::mcset da_DK "Co&mment selection" "Kommenter &markeringen"
    ::msgcat::mcset da_DK "U&ncomment selection" "Afkommenter ma&rkeringen"
    ::msgcat::mcset da_DK "&Indent selection" "In&dryk markering"
    ::msgcat::mcset da_DK "Unin&dent selection" " &Udryk markeringen"

    # menues.tcl - Search menu
    ::msgcat::mcset da_DK "&Find..." "&Find..."
    ::msgcat::mcset da_DK "Find &Next" "Find &næste"
    ::msgcat::mcset da_DK "&Replace..." "&Erstat..."
    ::msgcat::mcset da_DK "&Goto Line..." "&Gå til linie..."

    # menues.tcl - Exec menu
    ::msgcat::mcset da_DK "&Load into Scilab" "&Indlæs i Scilab"
    ::msgcat::mcset da_DK "&Evaluate selection" "&Evaluer markering"

    # menues.tcl - Debug menu
    ::msgcat::mcset da_DK "&Insert/Remove breakpoint" "&Indsæt/Fjern stoppunkt"
    ::msgcat::mcset da_DK "Remove &all breakpoints" "&Fjern alle stoppunkter"
    ::msgcat::mcset da_DK "&Configure execution..." "&Konfigurer udførelse..."
    ::msgcat::mcset da_DK "Go to next b&reakpoint" "&Gå til næste stoppunkt"
    ::msgcat::mcset da_DK "&Step by step" "&Trin for trin"
    ::msgcat::mcset da_DK "Step &into" "Træd &ind i"
    ::msgcat::mcset da_DK "Step o&ver" "Træd &forbi"
    ::msgcat::mcset da_DK "Step &out" "Træd &ud"
    ::msgcat::mcset da_DK "Run to c&ursor" "Kør til &markør"
    ::msgcat::mcset da_DK "G&o on ignoring any breakpoint" "F&ortsæt med at ignorere ethvert stoppunkt"
    ::msgcat::mcset da_DK "Show &watch" "Vis o&vervågning"
    ::msgcat::mcset da_DK "&Break" "&Stop"
    ::msgcat::mcset da_DK "Cance&l debug" "&Annuller debug"

#    ::msgcat::mcset da_DK "language scheme" "sprogskema"
    # menues.tcl - Scheme menu
    ::msgcat::mcset da_DK "S&cilab" "&Scilab"
    ::msgcat::mcset da_DK "&XML" "&XML"
    ::msgcat::mcset da_DK "&none" "&intet"

    # menues.tcl - Options menu
    ::msgcat::mcset da_DK "&Font size" "&Skriftstørrelse"
        ::msgcat::mcset da_DK "&micro" "&mikro"
        ::msgcat::mcset da_DK "&small" "&lille"
        ::msgcat::mcset da_DK "m&edium" "m&edium"
        ::msgcat::mcset da_DK "&large" "&stor"
    ::msgcat::mcset da_DK "&Colors" "&Farver"
    ::msgcat::mcset da_DK "Colorize \'&strings\'" "Farvelæg \'stre&nge\'"
    ::msgcat::mcset da_DK "Show c&ontinued lines" "Vis fortsatte &linier"
    ::msgcat::mcset da_DK "Word &Wrap" "Om&bryd tekst"
    ::msgcat::mcset da_DK "&Tabs and indentation" "Fane&r og indrykning"
        ::msgcat::mcset da_DK "Tab inserts &spaces" "&Tab indsætter mellemrum"
        ::msgcat::mcset da_DK "&Indentation spaces" "&Størrelse af indrykning"
    ::msgcat::mcset da_DK "Com&pletion" "&Afslutning"
        ::msgcat::mcset da_DK "&Tab" "&Tab"
        ::msgcat::mcset da_DK "&Control-Tab" "&Control-Tab"
        ::msgcat::mcset da_DK "&Alt-Tab" "Alt-Ta&b"
        ::msgcat::mcset da_DK "&Shift-Tab" "&Shift-Tab"
        ::msgcat::mcset da_DK "C&ontrol-Alt-Tab" "&Control-Alt-Tab"
        ::msgcat::mcset da_DK "S&hift-Control-Tab" "S&hift-Control-Tab"
        ::msgcat::mcset da_DK "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
        ::msgcat::mcset da_DK "Shi&ft-Control-Alt-Tab" "Shif&t-Control-Alt-Tab"
    ::msgcat::mcset da_DK "File&names" "Fil&navne"
        ::msgcat::mcset da_DK "&Full path" "&Fuld sti"
        ::msgcat::mcset da_DK "Full path if &ambiguous" "Fuld sti hvis &tvetydig"
        ::msgcat::mcset da_DK "&Unambiguous pruned path" "&Utvetydigt beskåret sti"
    ::msgcat::mcset da_DK "&Recent files" "&Seneste filer"
    ::msgcat::mcset da_DK "&Backup files depth" "Dybde af &backup-filer"
    ::msgcat::mcset da_DK "&Locale" "S&prog"

    # menues.tcl - Windows menu
    ::msgcat::mcset da_DK "&Maximize" "&Maksimer"
    ::msgcat::mcset da_DK "&Split" "&Opdel"
    ::msgcat::mcset da_DK "S&plit (side by side)" "Opdel (&side om side)"
    ::msgcat::mcset da_DK "Tile all &vertically" "Arranger &lodret"
    ::msgcat::mcset da_DK "Tile all &horizontally" "Arranger &vandret"
    ::msgcat::mcset da_DK "Space sashes &evenly" "&Ens mellemrum mellem vinduer"
    ::msgcat::mcset da_DK "Opened Files" "Åbnede filer"

    # menues.tcl - Help menu
    ::msgcat::mcset da_DK "&Help..." "&Hjælp"
    ::msgcat::mcset da_DK "&What's?..." "H&vad er?..."
    ::msgcat::mcset da_DK "&About" "&Om"
    ::msgcat::mcset da_DK "Adding &translations..." "&Tilføj oversættelser..."
        ::msgcat::mcset da_DK "Adding Scipad Translations..." "Tilføj Scipad-oversættelser..."
    ::msgcat::mcset da_DK "&Bugs \& wishlist" "&Bugs \& ønskeliste"
        ::msgcat::mcset da_DK "Scipad Known bugs" "Kende Scipad-bugs"
    ::msgcat::mcset da_DK "Change&log" "&Ændringer"
        ::msgcat::mcset da_DK "Changes in the Scipad codebase" "Ændringer i Scipad-kodebasen"


# popupmenus.tcl
::msgcat::mcset da_DK "Open the source of" "Åbn kilden til"
::msgcat::mcset da_DK "AddWatch" "Tilføj til overvågning"


# print.tcl
::msgcat::mcset da_DK "Print Setup" "Printopsætning"
::msgcat::mcset da_DK "Print Command: " "Printkommando: "


# scilabexec.tcl
::msgcat::mcset da_DK "No point in loading an empty file!" "Ingen mening i at indlæse en tom fil!"
::msgcat::mcset da_DK "Scilab is working, wait for the prompt to load file" "Scilab arbejder, vent på prompten for at indlæse filen"
::msgcat::mcset da_DK "Exec done" "Udførelse gennemført"
::msgcat::mcset da_DK "Scilab is working, wait for the prompt to execute the selection." "Scilab arbejder, vent på prompten for at køre markeringen"
::msgcat::mcset da_DK "Scilab is working, wait for the prompt to convert a Matlab file." "Scilab arbejder, vent på prompten for at konvertere en Matlab-fil."
::msgcat::mcset da_DK "Matlab files" "Matlab-filer"
::msgcat::mcset da_DK "Matlab file to convert" "Matlab-filer til konvertering"
::msgcat::mcset da_DK "Scilab is converting, please hold on..." "Scilab konverterer, vent venligst..."
::msgcat::mcset da_DK "Matlab file import" "Matlab-filimport"
::msgcat::mcset da_DK "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to" \
                   "Konverteringen af filen mislykkedes, se Scilab-vinduet\
                    for detaljer -- Rapporter evt. fejlteksten og den\
                    fejlende Matlab-fil til"
::msgcat::mcset da_DK "Please select destination path for the xml source of the help file" "Vælg venligst destinationsstien til hjælpfilens xml-kilde"
::msgcat::mcset da_DK "Older version of" "Ældre version af"
    ::msgcat::mcset da_DK "found!" "fundet!"
::msgcat::mcset da_DK "An old version of" "En gammel version af"
    ::msgcat::mcset da_DK "already exists: open the old file instead?" "findes allerede: Åbn den gamle fil i stedet?"
::msgcat::mcset da_DK "Existing file" "Eksisterende fil"
::msgcat::mcset da_DK "New skeleton" "Nyt skelet"
::msgcat::mcset da_DK "Scilab is working, wait for the prompt to compile the help file." "Scilab arbejder, vent på prompten for at kompilere hjælpfilen."
::msgcat::mcset da_DK "ScilabEval command cannot be passed to Scilab!" "En ScilabEval-kommando kan ikke leveres til Scilab!"
::msgcat::mcset da_DK impossibleScilabEval_message \
				   "ADVARSEL: En kommando kunne ikke leveres til Scilab og blev ignoreret.\n\n \
				   Du har højst sandsynligt ikke skriveadgang til Scilabs TMPDIR. \
				   Der kan ske uforudsigelige resultater.\n\n \
				   Den fejlende kommando var:\n\n-->"


# scipad.tcl
::msgcat::mcset da_DK "NOT CONNECTED TO SCILAB" "IKKE FORBUNDET TIL SCILAB"


# textarea.tcl
::msgcat::mcset da_DK "Font size" "Skriftstørrelse"


# whichfun.tcl
::msgcat::mcset da_DK "Being at line" "Befindende sig på linie"
    ::msgcat::mcset da_DK ", function" ", funktionen"
    ::msgcat::mcset da_DK "begins at" "begynder på"
    ::msgcat::mcset da_DK ", and there are" ", og der ere"
    ::msgcat::mcset da_DK "continued lines, i.e. we are at line" "fortsatte linier, altså er vi på linie"
    ::msgcat::mcset da_DK "of" "af"
::msgcat::mcset da_DK "Function" "Funktion"
    ::msgcat::mcset da_DK "line" "linie"
::msgcat::mcset da_DK "The cursor is not currently inside a function body." "Markøren er p.t. ikke inde i en funktions indre."
::msgcat::mcset da_DK "Which function?" "Hvilken funktion?"


#preference variables description
::msgcat::mcset da_DK "BGCOLOR" "Baggrund"
::msgcat::mcset da_DK "FGCOLOR" "Brødtekst"
::msgcat::mcset da_DK "CURCOLOR" "Markør"
::msgcat::mcset da_DK "PARCOLOR" "Parenteser ()"
::msgcat::mcset da_DK "BRAKCOLOR" "Klammer \\\[\\\]"
::msgcat::mcset da_DK "BRACCOLOR" "Tuborgparenteser {}"
::msgcat::mcset da_DK "PUNCOLOR"  "Tegnsætning"
::msgcat::mcset da_DK "INTFCOLOR" "Indre funktioner"
::msgcat::mcset da_DK "COMMCOLOR" "Kommandoer"
::msgcat::mcset da_DK "SCICCOLOR" "Scicos-funtioner"
::msgcat::mcset da_DK "OPCOLOR"   "Operatorer"
::msgcat::mcset da_DK "QTXTCOLOR"  "Strengtekst"
::msgcat::mcset da_DK "REMCOLOR" "Kommentarer"
::msgcat::mcset da_DK "XMLCOLOR" "XML-tags"
::msgcat::mcset da_DK "NUMCOLOR" "Numre"
::msgcat::mcset da_DK "SELCOLOR" "Markering"
::msgcat::mcset da_DK "BREAKPOINTCOLOR" "Stoppunktbaggrund"
::msgcat::mcset da_DK "LFUNCOLOR" "Bibliotek-funktioner"
::msgcat::mcset da_DK "PDEFCOLOR" "Prædefinerede variable"
::msgcat::mcset da_DK "FOUNDTEXTCOLOR" "Funden tekst"
::msgcat::mcset da_DK "CONTLINECOLOR" "Fortsat linie"


#overriding tcl-core dialogs (many are actually already found in 
#  TKDIR/msgs/fr.msg)
::msgcat::mcset da_DK "&Directory:" "&Destination:"
::msgcat::mcset da_DK "File &name:" "&Filnavn:"
::msgcat::mcset da_DK "Files of &type:" "Filer af &type:"
#::msgcat::mcset da_DK "&Save" "&Gem" # already included
::msgcat::mcset da_DK "&Open" "&Åbn"
::msgcat::mcset da_DK "Open" "Åbn"
::msgcat::mcset da_DK "Save As" "Gem som"
::msgcat::mcset da_DK "&Cancel" "&Annuller"
::msgcat::mcset da_DK "&Selection:" "&Markering:"
::msgcat::mcset da_DK "&Yes" "&Ja"
::msgcat::mcset da_DK "&No" "&Nej"
::msgcat::mcset da_DK "&Red" "&Rød"
::msgcat::mcset da_DK "&Green" "G&røn"
::msgcat::mcset da_DK "&Blue" "&Blå"
::msgcat::mcset da_DK "Directory \"%1\$s\" does not exist."\
        "Destinationen \"%1\$s\" findes ikke."
