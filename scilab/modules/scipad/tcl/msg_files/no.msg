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
# Scipad texts in norwegian
# in sync at least with v6.129
# by Torbjørn Pettersen, 5/9/2007

# commons
::msgcat::mcset no "Cancel" "Avbryt"
::msgcat::mcset no "Cance&l" "Avbry&t"
::msgcat::mcset no "OK" "OK"
::msgcat::mcset no "All files" "Alle filer"
::msgcat::mcset no "Add/Change" "Legg til/Endre"
::msgcat::mcset no "Add/Chan&ge" "Legg til/&Endre"
::msgcat::mcset no "&Remove" "&Fjern"
::msgcat::mcset no "Rem&ove" "Fjern"
::msgcat::mcset no "Untitled" "Navnløs"
::msgcat::mcset no "Close" "Lukk"
::msgcat::mcset no "&Close" "&Lukk"
::msgcat::mcset no "Error " "Feil "
::msgcat::mcset no "The file" "Filen"


# buffernavigation.tcl
::msgcat::mcset no "Hide" "Skjul"
::msgcat::mcset no "Close this tile, keep content" "Lukk, men behold innhold"
::msgcat::mcset no "Close this tile" "Lukk"
::msgcat::mcset no "Goto Line?" "Gå til rad ?"
::msgcat::mcset no "Go to:" "Gå til : "
::msgcat::mcset no "&logical line" "&logisk linje"
::msgcat::mcset no "&physical line" "&fysisk linje"
::msgcat::mcset no "in &function" "i &funksjon"
::msgcat::mcset no "in &current file" "i &aktiv fil"
::msgcat::mcset no "Outside of function definition"\
                   "Utenfor funksjonsdefinisjon"


# colorize.tcl
::msgcat::mcset no "Wait seconds while recolorizing file"\
                   "Vent et øyeblikk mens filen fargelegges"
::msgcat::mcset no "You can't do that while colorization is in progress.\
                    Please try again when finished."\
                   "Du kan ikke gjøre det mens fargelegging pågår. Vennligst\
                    prøv igjen når fargelegging er  avsluttet"
::msgcat::mcset no "Command forbidden during colorization"\
                   "Forbudt kommando under fargelegging"


# completion.tcl
::msgcat::mcset no "No possible completion" "Ingen mulig avslutning"
::msgcat::mcset no "possible completions" "mulige avsluttninger"
::msgcat::mcset no "Possible completions" "Mulige avsluttninger"


# db_argsmanagement.tcl
::msgcat::mcset no "Add" "Legg til"
::msgcat::mcset no "This variable can be watched but cannot be edited!"\
                   "Denne variabel kan overvåkes men ikke editeres!"
::msgcat::mcset no "A new watch variable will be created."\
                   "En ny overvåkingsvariabel blir opprettet"
::msgcat::mcset no "Non editable variable" "Ikke editerbar variabel"
::msgcat::mcset no "Variable:" "Variabel :"
::msgcat::mcset no "Expression:" "Uttrykk :"
::msgcat::mcset no "Value:" "Verdi :"
::msgcat::mcset no "&Get current value from Scilab"\
                   "Hent aktuell &verdi fra Scilab"
::msgcat::mcset no "Non watchable variable" "Ikke overvåkbar variabel"
::msgcat::mcset no "Sorry, variable" "Beklager, variabelen"
::msgcat::mcset no "is special and cannot be watched."\
                   "er spesiell og kan ikke overvåkes."


# db_bptsmanagement.tcl
::msgcat::mcset no "No breakpoint here!" "Ingen stoppsignal her!"


# db_configureexec.tcl
::msgcat::mcset no "Configure execution" "Konfigurer kjøring"
::msgcat::mcset no "Function name:" "Funksjonsnavn :"
::msgcat::mcset no "&Obtain" "&Hent"
::msgcat::mcset no "Input arguments:" "Input argumenter :"
::msgcat::mcset no "The selected function has not been found in the opened\
                    files, maybe it was deleted since last configuration of\
                    the debugger.\nUse button \""\
                   "Den valgte funksjonen har ikke blitt funnet i de åpne\
                    filene, kanskje er den slettet siden siste konfigurering\
                    av debuggeren.\nBruk knappen \""
::msgcat::mcset no "\" to refresh the list of available functions."\
                   " for å oppdatere listen over tilgjengelige funksjoner."
::msgcat::mcset no "Function name or input arguments do not match definition\
                    of the function"\
                   "Funksjonsnavn eller input argument passer ikke med\
                    funksjonsdefinisjonen"
::msgcat::mcset no "in the file!\n\nCheck function name and arguments\
                    (names, order) in the configuration dialog. \nArguments\
                    order can be changed using drag and drop with right\
                    mouse button in the arguments listbox."\
                   "i filen!\n\n Kontroller funksjonsnavnet och argument\
                    (navn, rekkefølge) i konfigurasjonsdialogen.\n\
                    Rekkefølge av argumenter kan endres ved dra och slipp\
                    med høyre musknapp i ruten med argument."
::msgcat::mcset no "Error in selected function name or arguments"\
                   "Feil i valgt funksjonsnavn eller argument"
::msgcat::mcset no "At least one input argument does not match definition of\
                    the function"\
                   "Minst ett input argument passer ikke med \
                    funksjonsdefinisjonen"
::msgcat::mcset no "in the file!\n\nCheck function arguments (their names)\
                    in the configuration dialog."\
                   "i filen!\n\nSjekk funksjonsargumentene (navnene) i\
                    konfigurasonsdialogen."
::msgcat::mcset no "Error in selected function arguments"\
                   "Feil i argumenter til valgt funksjon"
::msgcat::mcset no "Main level code found" "Hovednivåkode funnet"
::msgcat::mcset no "Scipad ancillary file" "Scipad hjelpefil"
::msgcat::mcset no "The current file contains only Scipad ancillaries that\
                    cannot be debugged in Scipad."\
                   "Den valgte filen inneholder bare Scipad objekter som \
                    ikke kan feilsøkes i Scipad."
::msgcat::mcset no "This file contains main level code.\nDebug as a single\
                    .sce file?"\
                   "Denne filen inneholder topp nivå kode. \nFeilsøk som en\
                    frittstående .sce fil?"
::msgcat::mcset no "No function found" "Fant ingen funksjon"
::msgcat::mcset no "This file contains no function and will be treated"\
                   "Denne filen inneholder ingen funksjoner og behandles"
::msgcat::mcset no "as a single .sce file.\nStart debugging?"\
                   "som en frittstående .sce fil.\nStart  feilsøking ?"
::msgcat::mcset no "You must enable colorization to be able to launch the\
                    debugger!"\
                   "Du må tillate fargelegging for å starte \
                    feilsøkingsverktøyet !"
::msgcat::mcset no "The debugger needs colorization"\
                   "Feilsøker behøver fargelegging"
::msgcat::mcset no "Warning!\n\n  A function missing the closing\
                    \"endfunction\" has been found in the currently opened\
                    files.\n  Such syntax is now obsolete.\n\n  Please\
                    terminate function "\
                   "Advarsel!\n\n  En funksjon som mangler \"endfunction\"\
                    er funnet i den aktive filen. \n  Denne syntaksen er nå\
                    avleggs.\n\n  Vennligst avslutt funksjonen "
::msgcat::mcset no " and try again." " og forsøk pånytt."
::msgcat::mcset no "Note: This message might also be triggered by a string\n \
                    quoted with single quotes and containing the word\
                    \"function\",\n  when \"Colorize \'strings\'\" is\
                    unchecked in the Options menu."\
                   "Merk: Denne meldingen kan også være forårsaket av  en\
                    streng med dobbelte quotes som inneholder ordene \
                    \"function\",\n  når  \"Fargelegg \'strenger\'\" ikke er\
                    haket ut i  menyen Options."
::msgcat::mcset no "This known bug can be worked around simply by checking\
                    \"Colorize \'&strings\'\"."\
                   "Dette er en kjent feil som kan unngås ved å markere \
                    \"Fargelegg \'stringer\'\"."
::msgcat::mcset no "Unterminated function definition found"\
                   "Uavsluttet definisjon av funksjon er funnet"
::msgcat::mcset no "Warning!\n\n  One or more function is defined more than\
                    once in the currently opened files.\n  The debugger\
                    cannot cope with multiple copies of functions.\n  The\
                    following duplicates were detected:\n"\
                   "Advarsel!\n\n  En eller flere funksjoner er definert me\
                    enn en gang i filene som er åpne.\n"
::msgcat::mcset no "\n--> Definition of function " "\n--> Funksjonsdefinisjon "
::msgcat::mcset no " found in:\n" "funnet i:\n"
::msgcat::mcset no "Duplicate function definitions found"\
                   "Dobbel funksjonsdefinisjon funnet"


# db_debugsession.tcl
::msgcat::mcset no "Too many breakpointed functions"\
                   "For mange funksjoner med stoppsignal"
::msgcat::mcset no "You have currently" "Du har for øyeblikket"
::msgcat::mcset no "functions in your opened files." "funksjoner."
::msgcat::mcset no "Scilab supports a maximum of" "Scilag støtte maksimalt"
::msgcat::mcset no "possible breakpointed functions (see help setbpt)."\
                   "mulige funksjoner med stoppsignal (se help setbpt)."
::msgcat::mcset no "Step-by-step hence cannot be performed."\
                   "Steg-for-steg feilsøk kan derfor utføres."
::msgcat::mcset no "This command will actually run to the next breakpoint."\
                   "Denne kommandoen vil kjøre til neste  stoppsignal."
::msgcat::mcset no "Break hence cannot be performed."\
                   "Stopp kan derfor ikke utføres."
::msgcat::mcset no "Too many breakpoints" "For mange stoppsignal"
::msgcat::mcset no "Executing this command would require to set"\
                   "Kjøring av denne kommandoen vil kreve at det settes"
::msgcat::mcset no "breakpoints in your opened files."\
                   "stoppsignal i åpne filer."
::msgcat::mcset no "possible breakpoints in Scilab (see help setbpt)."\
                   "mulige stoppsignal i Scilab (se help setbpt)."
::msgcat::mcset no "Cursor must be in a function"\
                   "Kursor må være innenfor en funksjon"
::msgcat::mcset no "This command cannot succeed if the function where the\
                    debug just stopped is not opened in Scipad!"\
                   "Denne kommandoen kan ikke utføres hvis funksjonen hvor\
                    feilsøkingen nettopp stoppet ikke er tilgjengelig i Scilab!"
::msgcat::mcset no "Cannot run to return point"\
                   "Kan ikke kjøre fram til return punktet."
::msgcat::mcset no "Warning!\nThe line where the debugger just stopped\
                    contains a return statement (either endfunction, return\
                    or resume) but this statement is not the first one on\
                    the line."\
                   "Advarsel!\nLinjen hvor feilsøkeren nettopp stoppet \
                    inneholder en return settning (enten endfunction, return\
                    eller resume) men denne kommandoen er ikke den første \
                    instruksjonen på linje."
::msgcat::mcset no "As a consequence the debugger cannot predict if the next\
                    debug step will really jump outside of the current\
                    function, which is what you just asked the debugger to\
                    do."\
                   "Som en følge kan ikke feilsøkeren forutse om neste \
                    feilsøkingstrinn vil hoppe ut av den aktive funksjonen,\
                    som er det du nettopp har bedt feilsøkeren om å gjøre."
::msgcat::mcset no "To avoid this unresolvable case, it is recommended to\
                    place the return statement alone on its line."\
                   "For å unngå denne uavklarete situasjonen, anbefales  det\
                    å plassere return kommandoen alene på en linje."
::msgcat::mcset no "Do you want to see this warning again during this Scipad\
                    session?"\
                   "Ønsker du å se denne advarselen igjen i løpet av denne \
                    Scipad sesjonen?"
::msgcat::mcset no "Unresolvable case found while running to return point"\
                   "Uavklart situasjon oppstod under kjøring til return\
                    punktet."
::msgcat::mcset no "No effect - The debugged file is not stuck"\
                   "Ingen effekt - den feilsøkte filen er ikke låst"


# db_init.tcl
::msgcat::mcset no "Please wait..." "Vennligst vent..."


# db_states.tcl
::msgcat::mcset no "Currently no debug session"\
                   "Foreløpig ingen feilsøkingssesjon"
::msgcat::mcset no "Ready to start debug" "Klar for å begynne feilsøking"
::msgcat::mcset no "Debug in progress" "Feilsøking pågår"
::msgcat::mcset no "Scilab is back at the main level now."\
                   "Scilab er nå tilbake i hovednivået."


# db_watch.tcl
::msgcat::mcset no "Watch" "Overvåk"
::msgcat::mcset no "Watch variables:" "Overvåk variabler :"
::msgcat::mcset no "A&uto (local variables)" "A&uto (lokale variable)"
::msgcat::mcset no "Au&to watch globals too" "Au&to overvåk globale variable"
::msgcat::mcset no "Generic expressions:" "Generisk uttrykk:"
::msgcat::mcset no "Call stack:" "Call stack:"
::msgcat::mcset no "Always on to&p" "Alltid på topp"
::msgcat::mcset no "Hide watch &variables" "Skjul overvåkede &variabler"
::msgcat::mcset no "Hide e&xpressions" "Skjul uttrykk"
::msgcat::mcset no "Hide call &stack" "Skjul call &stack"
::msgcat::mcset no "&Dock" "Integrer"
::msgcat::mcset no "&Add" "Legg til"
::msgcat::mcset no "&Filter errors" "Filtrer feil"
::msgcat::mcset no "Breakpoints" "Stoppsignal"
::msgcat::mcset no "Breakpointed functions" "Funksjoner med stoppsignal"
::msgcat::mcset no "Scilab busy indicator" "Scilab opptattindikator"
::msgcat::mcset no "Debugger state indicator"\
                   "Tilstandsindikator for feilsøker"
::msgcat::mcset no "Undefined elements are not legal in variable"\
                   "Udefinerte elementer er ikke lovlig i variabel"
::msgcat::mcset no ".\nThis variable will not be updated in Scilab."\
                   ".\nDenne variabelen vil ikke bli oppdatert i Scilab"
::msgcat::mcset no "Illegal undefined element found"\
                   "Ulovlig udefinert element funnet"
::msgcat::mcset no "evaluates to:" "evaluerer til:"
::msgcat::mcset no "produces error" "resulterer i feilmelding"
::msgcat::mcset no "Scipad cannot find source code for the selected\
                    function.\nPlease open the adequate file manually and\
                    try again."\
                   "Scipad kan ikke finne kildekoden til den valgte funksjon\
                    .\nVennligst åpne den riktige filen manuelt og  forsøk\
                    igjen."
::msgcat::mcset no "Missing source code" "Manglende kildekode"


# filecommands.tcl
::msgcat::mcset no "New script" "Nytt skript"
::msgcat::mcset no "New view on the same file" "Nytt bilde av den samme filen"
::msgcat::mcset no "The contents of" "Innholdet av"
::msgcat::mcset no "may have changed, do you wish to save your changes?"\
                   "kan være endret, vil de lagre endringene?"
::msgcat::mcset no "Save Confirm?" "Bekreft lagring?"
::msgcat::mcset no "Open source of..." "Åpne kildekode til..."
::msgcat::mcset no "Open source of:" "Åpne kildekode til:"
::msgcat::mcset no "Weird drag&drop" "Merkelig drag&drop"
::msgcat::mcset no "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or you\
                    hit a bug of the dnd mechanism."\
                   "er verken et gyldig filnavn eller katalog. Enten drar du\
                    et object av en annen type, eller er det feil i\
                    dra-og-slipp funksjonaliteten."
::msgcat::mcset no "File not found" "Filen finnes ikke"
::msgcat::mcset no "does not exist anymore. Do you want to create an empty\
                    file with the same name?"\
                   "finnes ikke lenger. Vil du opprette en tom fil med samme\
                    navn?"
::msgcat::mcset no "Open file" "Åpne fil"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset no "is already opened! Save the current opened file to\
                    another name and reopen it from disk!"\
                   "er allerede åpen! Lagre aktiv åpen fil under et annet\
                    navn og åpne den fra disk!"
::msgcat::mcset no "File" "Fil"
::msgcat::mcset no "saved" "lagret"
::msgcat::mcset no "cannot be written!" "kan ikke skrives!"
::msgcat::mcset no "The contents of " "Innholdet i"
::msgcat::mcset no "has changed on disk, save it anyway?"\
                   "er endret på disken, lagre den uansett?"
::msgcat::mcset no "has been modified outside of Scipad. Do you want to\
                    reload it?"\
                   "har blitt modifisert utenfor Scipad. Ønsker du å lese\
                    den inn på nytt?"
::msgcat::mcset no "File has changed!" "Filen er endret!"
::msgcat::mcset no "Save as" "Lagre som"
::msgcat::mcset no "Scilab files" "Scilabfiler"
::msgcat::mcset no "Scicos files" "Scicosfiler"
::msgcat::mcset no "XML files" "XML filer"
::msgcat::mcset no "Wait seconds while loading and colorizing file"\
                   "Vent mens filen lastest og fargelegges"
::msgcat::mcset no "The contents of this file may have changed, do you wish\
                    to save your changes?"\
                   "Innholdet i filen kan være endret, ønsker du å lagre\
                    endringene dine?"
::msgcat::mcset no "Revert" "Gå tilbake"
::msgcat::mcset no "to saved?" "til tidligere versjon?"
::msgcat::mcset no "Revert Confirm?" "Bekreft lasting av tidligere versjon?"
::msgcat::mcset no "Unreadable file" "Ikke lesbar fil!"
::msgcat::mcset no "exists but is not readable!"\
                   "eksisterer men er ikke lesbar!"
::msgcat::mcset no "This file is rather large and colorization might take\
                    some time.\n\nColorize anyway?"\
                   "Denne filen er temmelig stor og fargelegging kan ta  en\
                    stund.\n\nFargelegg allikevel?"
::msgcat::mcset no "File size warning" "Advarsel om filstørrelse"
::msgcat::mcset no "Windows shortcut file" "Windows snarveifil"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset no "is a Windows shortcut and will not be opened!"\
                   "er en Windwos snarvei og vil ikke bli åpnet!"


# findreplace.tcl
::msgcat::mcset no "Dialog box already open!" "Dialogboks er allerede åpen!"
::msgcat::mcset no "The string" "Strengen"
::msgcat::mcset no "could not be found in" "ble ikke funnet"
::msgcat::mcset no "could not be found in any of the opened files !"\
                   "ble ikke funnet i noen av de åpne filene!"
::msgcat::mcset no "replacements done" "erstattninger utført"
::msgcat::mcset no "Find" "Søk"
::msgcat::mcset no "No match found in the selection for"\
                   "Ingen treff i den markerte delen av"
::msgcat::mcset no "\nWould you like to look for it in the entire text?"\
                   "\nVil du søke gjennom hele teksten?"
::msgcat::mcset no "You are searching for an empty string!"\
                   "Du søker etter en tom streng!"
::msgcat::mcset no "Re&place" "&Erstatt"
::msgcat::mcset no "Find what:" "Søk etter :"
::msgcat::mcset no "Replace with:" "Erstatt med :"
::msgcat::mcset no "Find &Next" "Finn &neste"
::msgcat::mcset no "Replace &All" "Erstatt &alle"
::msgcat::mcset no "Direction" "Rettning"
::msgcat::mcset no "&Upwards" "&Oppover"
::msgcat::mcset no "&Downwards" "&Nedover"
::msgcat::mcset no "Text types" "Tekst typer"
::msgcat::mcset no "Find options" "Søke opsjoner"
::msgcat::mcset no "Match &whole word only" "Søk kun etter &hele ord"
::msgcat::mcset no "Match &case" "Skill på store og små bokstaver"
::msgcat::mcset no "&Regular expression" "&Regulært uttrykk (regexp)"
::msgcat::mcset no "In all &opened files" "I alle åpne &filer"
::msgcat::mcset no "In &selection only" "Kun i &valgt område"
::msgcat::mcset no "In typed te&xt" "I skrevet tekst"
::msgcat::mcset no "In a director&y" "I en &katalog"
::msgcat::mcset no "Find in files search options" "Finn i filers søkeopsjoner"
::msgcat::mcset no "Directory r&ecurse search" "Søk &rekursivt i katalog"
::msgcat::mcset no "Include &hidden files" "Inkluder skulte filer"
::msgcat::mcset no "In files/file types:" "I filer/fil typer:"
::msgcat::mcset no "In directory:" "I katalog:"
::msgcat::mcset no "Search for file names" "Søk etter filnavn"
::msgcat::mcset no "You have given an empty search string. Are you sure you\
                    want to search among file names?"\
                   "Du har oppgitt en tom søkestreng. Er du sikker på at  du\
                    vil søke blant filnavnene?"
::msgcat::mcset no "Wrapped around" "Wrapped around"
::msgcat::mcset no "Back at the first file searched!"\
                   "Tilbake ved første fil som ble gjennomsøkt!"
::msgcat::mcset no "No more matches" "Ingen flere søketreff"
::msgcat::mcset no "The string to regexp search for cannot be compiled by\
                    Tcl.\nCheck again, maybe certain characters should be\
                    escaped."\
                   "Tekststrengen til regexp søk kan ikke kompileres av\
                    Tcl.\nSjekk på nytt, kanskje noen karakterer  behøver\
                    \""
::msgcat::mcset no "Any single character" "Noen enkeltstående karakterer"
::msgcat::mcset no "Zero or more" "Null eller flere"
::msgcat::mcset no "One or more" "En eller flere"
::msgcat::mcset no "Zero or one" "Null eller en"
::msgcat::mcset no "Any one character in the set"\
                   "En vilkårlig karakter i settet"
::msgcat::mcset no "Any one character not in the set"\
                   "En vilkårlig karakter som ikke er i settet"
::msgcat::mcset no "Or" "Eller"
::msgcat::mcset no "Beginning of line" "Begynnelsen av linje"
::msgcat::mcset no "End of line" "Slutten av linje"
::msgcat::mcset no "Beginning of word" "Begynnelse av ord"
::msgcat::mcset no "End of word" "Slutten av ord"
::msgcat::mcset no "A letter" "En bokstav"
::msgcat::mcset no "A decimal digit" "Ett desimaltall"
::msgcat::mcset no "An alphanumeric (letter or digit)"\
                   "Ett alfanumerisk symbol (bokstav eller tall)"
::msgcat::mcset no "A space or tab character" "Et mellomrom eller tab karakter"
::msgcat::mcset no "A rational number" "Et reelt tall"
::msgcat::mcset no "A floating point number" "Et flyttall"
::msgcat::mcset no "A Scilab comment" "En Scilab kommentar"
::msgcat::mcset no "A Scilab name" "Et Scilab navn"
::msgcat::mcset no "A Scilab continued line (with dots)"\
                   "Fortsettelse av en Scilab linje (med punkter)"


# findinfiles.tcl
::msgcat::mcset no "Directory access issue" "Problem med adgang til katalog"
::msgcat::mcset no "The directory you specified cannot be reached. The\
                    search won't bring any result."\
                   "Angitt katalog kan ikke nås. Søket vil ikke gi resultat"
::msgcat::mcset no "Some directories or files could not be open and were\
                    ignored during search.\n You might miss read access to\
                    them.\n\n The following ones were ignored"\
                   "Noen kataloger eller filer kan ikke åpnes og vil ikke\
                    tas med i søk.\n Kanskje du mangler lesetilgang til\
                    dem.\n\n Følgende vil ikke bli tatt med."
::msgcat::mcset no "Ignored files" "Utelatte filer"
::msgcat::mcset no "Searching in file:" "Søker i fil:"
::msgcat::mcset no "End of file search:" "Slutt på filsøk:"
::msgcat::mcset no "matches found" "antall søketreff"
::msgcat::mcset no "searched files" "gjennomsøkte filer"
::msgcat::mcset no "Constructing file list:" "Genererer liste over filer:"
::msgcat::mcset no "&Previous" "&Foregående"
::msgcat::mcset no "&Next" "&Neste"
::msgcat::mcset no "Scipad search results" "Søkeresultater fra Scipad"


# fonts.tcl
::msgcat::mcset no "Choose font" "Velg font"
::msgcat::mcset no "Family" "Familie"
::msgcat::mcset no "Style" "Stil"
::msgcat::mcset no "&Bold" "Uthevet"
::msgcat::mcset no "&Italic" "Kursiv"
::msgcat::mcset no "&Underline" "Understreket"
::msgcat::mcset no "Size" "Størrelse"
::msgcat::mcset no "&Defaults" "Default"
::msgcat::mcset no "Use as..." "Bruk som..."
::msgcat::mcset no "&Text font" "Tekstfont"
::msgcat::mcset no "&Menu font" "Menyfont"
::msgcat::mcset no "Sample" "Eksempel"
::msgcat::mcset no "Font size" "Fontstørrelse"
::msgcat::mcset no "Revert fonts" "Reverser fonter"
::msgcat::mcset no "Are you sure you want to revert all fonts to defaults?"\
                   "Er du sikker på at du ønsker å reversere alle fonter til\
                    default?"


# FormatStringsForWatch.sci
::msgcat::mcset no " what you try to watch is not supported by the debugger"\
                   " det du forsøker å overvåke er ikke støttet  i feilsøkeren"
::msgcat::mcset no " what you try to watch is of type "\
                   " det du forsøker å overvåke er av type "
::msgcat::mcset no " - this type is not supported by the debugger"\
                   " - denne typen er ikke støttet av debuggeren"
::msgcat::mcset no "Type:" "Type :"
::msgcat::mcset no "(unsupported)" "(ikke støttet)"
::msgcat::mcset no "size:" "størrelse:"
::msgcat::mcset no "real" "reelt"
::msgcat::mcset no "complex" "komplekst"
::msgcat::mcset no "elements" "elementer"
::msgcat::mcset no "graphic handle" "grafikkpeker"
::msgcat::mcset no "uncompiled function" "ukompilert funksjon"
::msgcat::mcset no "compiled function" "kompilert funksjon"
::msgcat::mcset no "library" "bibliotek"
::msgcat::mcset no "pointer" "peker"
::msgcat::mcset no "size implicit index" "størrelsesimplisitt indeks"
::msgcat::mcset no "primitive" "primitive"


# FormatWhereForWatch.sci
::msgcat::mcset no "Breakpoint" "Stoppsignal"
::msgcat::mcset no "called at line" "kallet på linje "
::msgcat::mcset no "of exec file" "i exec fil"
::msgcat::mcset no "of execstr instruction" "i execstr instruksjon"
::msgcat::mcset no "called under pause" "kallet under pause"
::msgcat::mcset no "of macro" "fra makro"


# helps.tcl
::msgcat::mcset no "About" "Om"
::msgcat::mcset no "aboutme_message"\
                   "\n Lagd av Joseph Acosta, joeja@mindspring.com.\n\
                    Modifisert av Scilab Constortium.\n Revidert av Enrico\
                    Segre 2003-2006.\n Diverse forbedringer, spesielt\
                    feilsøkingsverktøy av FranÃ§ois Vogel 2004-2008."


# infomessages.tcl
::msgcat::mcset no "Line:" "Rad:"
::msgcat::mcset no "Column:" "Kolonne:"
::msgcat::mcset no "Line" "Rad"
::msgcat::mcset no "in" "i"
::msgcat::mcset no "Logical line:" "Logisk linje:"
::msgcat::mcset no " \[ReadOnly\]" " \[Skrivebeskyttet\]"
::msgcat::mcset no " (modified)" " (endret)"


# inputtext.tcl
::msgcat::mcset no "No corresponding <" "Ingen tilsvarende <"
::msgcat::mcset no "> found!" "> funnet!"
::msgcat::mcset no "Code editing is not allowed during debug!"\
                   "Editering av kode ikke tillatt under feilsøking!"


# menues.tcl
::msgcat::mcset no "&File" "&Fil"
::msgcat::mcset no "&Edit" "R&ediger"
::msgcat::mcset no "&Search" "&Søk og erstatt"
::msgcat::mcset no "E&xecute" "&Kjør"
::msgcat::mcset no "&Debug" "Fei&lsøk"
::msgcat::mcset no "S&cheme" "S&pråk"
::msgcat::mcset no "&Options" "&Innstillinger"
::msgcat::mcset no "&Windows" "&Vindu"
::msgcat::mcset no "&Help" "&Hjelp"

# menues.tcl - File menu
::msgcat::mcset no "&New" "&Ny"
::msgcat::mcset no "&Open..." "Åpne..."
::msgcat::mcset no "Open &in new" "Åpne &i nytt"
::msgcat::mcset no "&horizontal tile" "stable &horisontalt"
::msgcat::mcset no "&vertical tile" "stable &vertikalt"
::msgcat::mcset no "Op&en source of..." "&Åpne kildekoden til ..."
::msgcat::mcset no "&Save" "&Lagre"
::msgcat::mcset no "Save &as..." "Lagre &som..."
::msgcat::mcset no "Save a&ll" "Lagre &alle"
::msgcat::mcset no "&Revert..." "&Angre..."
::msgcat::mcset no "Import &Matlab file..." "Importer &Matlab fil..."
::msgcat::mcset no "Create help s&keleton..." "Lag &hjelpmal..."
::msgcat::mcset no "Compile as &help page" "Kompiler som &hjelpfil"
::msgcat::mcset no "Open &function source" "Åpne &funksjonskode"
::msgcat::mcset no "Print Se&tup" "Skriver &oppsett"
::msgcat::mcset no "&Print" "&Skriv"
::msgcat::mcset no "&Close file" "Lukk fil"
::msgcat::mcset no "E&xit" "Avs&lutt"

# menues.tcl - Edit menu
::msgcat::mcset no "&Undo" "Angre"
::msgcat::mcset no "&Redo" "&Gjenta"
::msgcat::mcset no "Cu&t" "Klipp &ut"
::msgcat::mcset no "B&lock cut" "Klipp ut blokk"
::msgcat::mcset no "&Copy" "Kopier"
::msgcat::mcset no "&Paste" "&Lim inn"
::msgcat::mcset no "&Block paste" "Lim inn blokk"
::msgcat::mcset no "&Delete" "&Slett"
::msgcat::mcset no "Select &All" "&Velg alt"
::msgcat::mcset no "Insert &Time/Date" "Sett inn dato og &tid"
::msgcat::mcset no "Co&mment selection" "&Kommenter valgt område"
::msgcat::mcset no "U&ncomment selection" "&Avkommenter valgt område"
::msgcat::mcset no "&Indent selection" "Rykk &inn valgt område"
::msgcat::mcset no "Unin&dent selection" "Rykk &tilbake valgt område"

# menues.tcl - Search menu
::msgcat::mcset no "&Find..." "&Søk..."
::msgcat::mcset no "Find &Next" "Finn &neste"
::msgcat::mcset no "&Replace..." "E&rstatt..."
::msgcat::mcset no "&Goto Line..." "&Gå til linje..."

# menues.tcl - Exec menu
::msgcat::mcset no "&Load into Scilab" "&Last inn i Scilab"
::msgcat::mcset no "&Evaluate selection" "&Evaluer markering"

# menues.tcl - Debug menu
::msgcat::mcset no "&Insert/Remove breakpoint" "&Legg til/Fjern stoppsignal"
::msgcat::mcset no "Remove &all breakpoints" "&Fjern alle stoppsignal"
::msgcat::mcset no "&Configure execution..." "Konfigurer kjøring..."
::msgcat::mcset no "Go to next b&reakpoint" "Gå til neste stoppsignal"
::msgcat::mcset no "&Step by step" "Steg for steg"
::msgcat::mcset no "Step &into" "Steg &inn i"
::msgcat::mcset no "Step o&ver" "Steg &over"
::msgcat::mcset no "Step &out" "Steg &ut av"
::msgcat::mcset no "Run to re&turn point" "Kjør til return punkt"
::msgcat::mcset no "Run to c&ursor" "Kjør fram til kursor"
::msgcat::mcset no "G&o on ignoring any breakpoint"\
                   "&Fortsett og ignorer stoppsignal"
::msgcat::mcset no "Show &watch" "&Vis overvåket variabel"
::msgcat::mcset no "&Break" "&Stopp"
::msgcat::mcset no "Cance&l debug" "&Avbryt feilsøking"

#    ::msgcat::mcset fr "language scheme" "schÃ©ma langage"
# menues.tcl - Scheme menu
::msgcat::mcset no "S&cilab" "S&cilab"
::msgcat::mcset no "&XML" "&XML"
::msgcat::mcset no "&none" "&ingenting"
::msgcat::mcset no "&Colorize" "Fargelegg"

# menues.tcl - Options menu
::msgcat::mcset no "&Fonts..." "Skrifttyper..."
::msgcat::mcset no "&Colors" "&Farger"
::msgcat::mcset no "Colori&ze" "Fargelegg"
::msgcat::mcset no "&Always" "Alltid"
::msgcat::mcset no "Ask for &big files" "Spør for store filer"
::msgcat::mcset no "&Never" "Aldri"
::msgcat::mcset no "Colorize \'&strings\'" "Fargelegg \'&strenger\'"
::msgcat::mcset no "Line n&umbers" "Linjenummer"
::msgcat::mcset no "&Hide" "Skjul"
::msgcat::mcset no "&Left aligned" "Venstrejustert"
::msgcat::mcset no "&Right aligned" "Høyrejustert"
::msgcat::mcset no "Show c&ontinued lines" "Vis forlengede linjer"
::msgcat::mcset no "Word &wrap" "&Bryt lange linjer"
::msgcat::mcset no "&Double-click behavior" "Oppførsel ved dobbeltklikk"
::msgcat::mcset no "&Linux" "&Linux"
::msgcat::mcset no "Windo&ws" "Windo&ws"
::msgcat::mcset no "&Scilab" "&Scilab"
::msgcat::mcset no "&Tabs and indentation" "Tabulering og &innrykk"
::msgcat::mcset no "Tab inserts &spaces" "Tabulering til mellomrom"
::msgcat::mcset no "&Indentation spaces" "&Innrykksmellomrom"
::msgcat::mcset no "&Tab size (characters)" "Tabstørrelse (antall karakterer)"
::msgcat::mcset no "Use &keyword indentation" "Bruk innrykk ift nøkkelord"
::msgcat::mcset no "Com&pletion" "Fu&llfør"
::msgcat::mcset no "&Tab" "&Tab"
::msgcat::mcset no "&Control-Tab" "&Control-Tab"
::msgcat::mcset no "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset no "&Shift-Tab" "&Shift-Tab"
::msgcat::mcset no "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
::msgcat::mcset no "S&hift-Control-Tab" "S&hift-Control-Tab"
::msgcat::mcset no "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
::msgcat::mcset no "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
::msgcat::mcset no "File&names" "Fil&navn"
::msgcat::mcset no "&Full path" "&Komplett filsti"
::msgcat::mcset no "Full path if &ambiguous" "Komplett filsti hvis &tvetydig"
::msgcat::mcset no "&Unambiguous pruned path" "&Tvetydig forkortet sti"
::msgcat::mcset no "Windows menu &sorting" "Sortering av filnavn"
::msgcat::mcset no "&File opening order" "Rekkefølge for filåpning"
::msgcat::mcset no "&Alphabetically" "Alfabetisk"
::msgcat::mcset no "&Most recently used" "Sist brukte"
::msgcat::mcset no "&Recent files" "Tidligere redigerte filer"
::msgcat::mcset no "&Backup files depth"\
                   "Sikkerhetskopidybde (antall .bak filer)"
::msgcat::mcset no "&Locale" "&Språk"
::msgcat::mcset no "Execution &errors" "Kjørefeil"
::msgcat::mcset no "In Scilab &shell only" "Kun i Scilab skall"
::msgcat::mcset no "Copied in a &message box" "Kopiert i en meldingsboks"

# menues.tcl - Windows menu
::msgcat::mcset no "&Maximize" "&Maksimer"
::msgcat::mcset no "&Split" "&Splitt"
::msgcat::mcset no "S&plit (side by side)" "S&plitt (side om side)"
::msgcat::mcset no "Sp&lit file" "Splitt fil"
::msgcat::mcset no "Spl&it file (side by side)" "Splitt fil (side ved side)"
::msgcat::mcset no "Tile all &vertically" "Stable alle &vertikalt"
::msgcat::mcset no "Tile all &horizontally" "Stable alle &horisontalt"
::msgcat::mcset no "Space sashes &evenly" "Fordel &jevnt"
::msgcat::mcset no "Opened Files" "Åpne filer"

# menues.tcl - Help menu
::msgcat::mcset no "&Help..." "&Hjelp..."
::msgcat::mcset no "&What's?..." "&Hva er?..."
::msgcat::mcset no "&About" "&Om"
::msgcat::mcset no "Adding &translations..." "Bidra med oversettelser..."
::msgcat::mcset no "Adding Scipad Translations..."\
                   "Bidra med oversettelser av Scipad:"
::msgcat::mcset no "&Bugs \& wishlist" "Scipad bugs and wishlist"
::msgcat::mcset no "Scipad Known bugs" "Kjente feil i Scipad"
::msgcat::mcset no "Change&log" "Endringslogg"
::msgcat::mcset no "Changes in the Scipad codebase" "Endringer i koden til Scipad"


# popupmenus.tcl
::msgcat::mcset no "Open the source of" "Åpne kildekode til"
::msgcat::mcset no "AddWatch" "Legg til overvåket variabel"
::msgcat::mcset no "AddGenExp" "AddGemExp"
::msgcat::mcset no "Jump to" "Hopp til"


# print.tcl
::msgcat::mcset no "Print Setup" "Utskriftsoppsett"
::msgcat::mcset no "Print Command: " "Utskriftskommando : "


# scilabexec.tcl
::msgcat::mcset no "No point in loading an empty file!"\
                   "Ingen vits å laste en tom fil!"
::msgcat::mcset no "Silent file save failed" "Feil under stille lagring av fil"
::msgcat::mcset no "File could not be saved in a temporary\
                    location.\nOverwrite original file?"\
                   "Fil kunne ikke lagres på midlertidig plass. \nOverskriv\
                    original fil?"
::msgcat::mcset no "Exec done" "Kjøring klar"
::msgcat::mcset no "Matlab files" "Matlabfiler"
::msgcat::mcset no "Matlab file to convert" "Oversett Matlab fil"
::msgcat::mcset no "Scilab is converting, please hold on..."\
                   "Scilab oversetter, vennligst vent..."
::msgcat::mcset no "Matlab file import" "Importer Matlab fil"
::msgcat::mcset no "Conversion of the file failed, see the Scilab window for\
                    details -- Perhaps report the error text and the\
                    offending Matlab file to"\
                   "Oversettelsen misslyktes, se i Scilab vinduet  etter\
                    detaljer. Kanskje skal du rapportere feilmeldingen og\
                    relevante deler av Matlab filen til"
::msgcat::mcset no "Please select destination path for the xml source of the\
                    help file"\
                   "Vennligst velg sti til xml kode til hjelpfilen"
::msgcat::mcset no "Older version of" "Eldre versjon av"
::msgcat::mcset no "found!" "funnet!"
::msgcat::mcset no "An old version of" "En gammel version av"
::msgcat::mcset no "already exists: open the old file instead?"\
                   "finnes allerede: Åpne den gamle filen istedenfor?"
::msgcat::mcset no "Existing file" "Filen finnes"
::msgcat::mcset no "New skeleton" "Ny mal"
::msgcat::mcset no "ScilabEval command cannot be passed to Scilab!"\
                   "ScilabEval kommando kan ikke sendes til Scilab!"
::msgcat::mcset no "impossibleScilabEval_message" "umulig ScilabEval melding"
::msgcat::mcset no "impossibleScilabEval_message2"\
                   "ADVARSEL: En kommando kunne ikke sendes til Scilab\
                    og er ignorert fordi den er for lang.\n\n \
                    Uforutsigbare resultat kan forekomme.\n\n Hvis du \
                    driver feils?king, fors?k ? fjern overv?kede\
                    variable.\n\n  Den ugyldige kommandoen var:\n\n-->" 
::msgcat::mcset no "(end of command skipped)" "(slutten av kommando utelatt)"
::msgcat::mcset no "Scilab working" "Scilab arbeider"
::msgcat::mcset no "Scilab is working, please wait for the prompt to execute\
                    this command!"\
                   "Scilab arbeider, vent for å utføre denne kommandoen!"
::msgcat::mcset no "Scilab is working, wait for the prompt to load file"\
                   "Scilab arbeider, vent på kursor for å laste filen"
::msgcat::mcset no "Scilab is working, wait for the prompt to execute the\
                    selection."\
                   "Scilab arbeider, vent på å kjøre valgt tekst."
::msgcat::mcset no "Scilab is working, wait for the prompt to convert a\
                    Matlab file."\
                   "Scilab arbeider, vent på at Matlab filen oversettes."
::msgcat::mcset no "Scilab is working, wait for the prompt to compile the\
                    help file."\
                   "Scilab arbeider, vent på at hjelpfilen skal kompileres."
::msgcat::mcset no "Scilab is working, wait for the prompt to issue a\
                    debugger command."\
                   "Scilab arbeider, vent på kursor."
::msgcat::mcset no "Scilab execution error" "Kjørefeil i Scilab"
::msgcat::mcset no "The shell reported an error while trying to execute "\
                   "Skallet rapporterte en feil under kjøring "
::msgcat::mcset no ": error " ": feil "
::msgcat::mcset no "at line " "på rad "
::msgcat::mcset no " of " " i "
::msgcat::mcset no "Execution aborted!" "Kjøring avbrutt!"


# scipad.tcl
::msgcat::mcset no "NOT CONNECTED TO SCILAB" "IKKE KOBLET TIL SCILAB"


# whichfun.tcl
::msgcat::mcset no "Being at line" "Begynner på linje"
::msgcat::mcset no ", function" ", funksjonen"
::msgcat::mcset no "begins at" "begynner på"
::msgcat::mcset no ", and there are" ", og det er "
::msgcat::mcset no "continued lines, i.e. we are at line"\
                   "flere rader, d.v.s. vi er på linje"
::msgcat::mcset no "of" "i"
::msgcat::mcset no "Function" "Funksjon"
::msgcat::mcset no "line" "rad"
::msgcat::mcset no "The cursor is not currently inside a function body."\
                   "Kursor er ikke innenfor noen funksjonskode."
::msgcat::mcset no "Which function?" "Hvilken funksjon?"


#preference variables description
::msgcat::mcset no "BGCOLOR" "Bakgrunnsfarge"
::msgcat::mcset no "BGLNMARGCOLOR" "Bakgrunnsfarge for linjenummer"
::msgcat::mcset no "FGCOLOR" "Tekstfarge"
::msgcat::mcset no "FGLNMARGCOLOR" "Farge for linjenummer"
::msgcat::mcset no "CURCOLOR" "Kursor"
::msgcat::mcset no "PARCOLOR" "Parenteser ()"
::msgcat::mcset no "BRAKCOLOR" "Hakeparenteser \\\[\\\]"
::msgcat::mcset no "BRACCOLOR" "Myke parenteser {}"
::msgcat::mcset no "PUNCOLOR" "Punkter"
::msgcat::mcset no "INTFCOLOR" "Interne funksjoner"
::msgcat::mcset no "COMMCOLOR" "Scilab kommandoer"
::msgcat::mcset no "SCICCOLOR" "Scicos funksjoner"
::msgcat::mcset no "OPCOLOR" "Operatorer"
::msgcat::mcset no "QTXTCOLOR" "Tekster"
::msgcat::mcset no "REMCOLOR" "Kommentarer"
::msgcat::mcset no "XMLCOLOR" "XML"
::msgcat::mcset no "NUMCOLOR" "Tall"
::msgcat::mcset no "SELCOLOR" "Markering"
::msgcat::mcset no "BREAKPOINTCOLOR" "Farge for stoppsignal"
::msgcat::mcset no "LFUNCOLOR" "Biblioteksrutiner"
::msgcat::mcset no "PDEFCOLOR" "Predefinierte variabler"
::msgcat::mcset no "FOUNDTEXTCOLOR" "Funnet tekst"
::msgcat::mcset no "CONTLINECOLOR" "Ufullstendige linjer"
::msgcat::mcset no "USERFUNCOLOR" "Farge for brukerfunksjoner"


#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset no "&Directory:" "&Katalog :"
::msgcat::mcset no "File &name:" "Fil&navn:"
::msgcat::mcset no "Files of &type:" "Filer av &type:"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
::msgcat::mcset no "&Open" "&Åpne"
::msgcat::mcset no "Open" "Åpne"
::msgcat::mcset no "Save As" "Lagre som"
::msgcat::mcset no "&Cancel" "Avbryt"
::msgcat::mcset no "&Selection:" "&Valg :"
::msgcat::mcset no "&Yes" "&Ja"
::msgcat::mcset no "&No" "&Nei"
::msgcat::mcset no "&Red" "&Rød"
::msgcat::mcset no "&Green" "&Grønn"
::msgcat::mcset no "&Blue" "&Blå"
::msgcat::mcset no "Directory \"%1\$s\" does not exist."\
                   "Katalog \"%1\$s\" eksisterer ikke."
