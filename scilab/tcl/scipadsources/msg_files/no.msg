# Scipad texts in norwegian
# in sync at least with v3.56

# commons
::msgcat::mcset no "Cancel" "Avbryt"
::msgcat::mcset no "Cance&l" "Avbry&t"
::msgcat::mcset no "OK" "OK"
::msgcat::mcset no "Scilab working" "Scilab arbeider"
::msgcat::mcset no "All files" "Alle filer"
::msgcat::mcset no "Add/Change" "Legg til/Endre"
::msgcat::mcset no "Untitled" "Navnløs"


# buffernavigation.tcl
::msgcat::mcset no "Goto Line?" "Gå til rad ?"
::msgcat::mcset no "Goto Line: " "Gå til rad: "


# colorize.tcl
::msgcat::mcset no "Wait seconds while recolorizing file" "Vent et øyeblikk mens filen fargelegge"


# db_argsmanagement.tcl
::msgcat::mcset no "Variable:" "Variabel :"
::msgcat::mcset no "Value:" "Verdi :"


# db_configureexec.tcl
::msgcat::mcset no "Configure execution" "Konfigurer kjøring"
::msgcat::mcset no "Function name:" "Funksjonsnavn :"
::msgcat::mcset no "Obtain" "Hent"
::msgcat::mcset no "Input arguments:" "Input argumenter :"
::msgcat::mcset no "Remove" "Fjern"
::msgcat::mcset no "Check function definition: it must be on a single code line." \
                   "Kontroller funksjons definisjon: det skal være en enkel kodelinje."
::msgcat::mcset no "Function name or input arguments do not match definition of the function" \
                    "Funksjonsnavn eller input argument passer ikke med funksjonsdefinisjonen"
    ::msgcat::mcset no "in the file!\n\nCheck function\
                        name and arguments (names, order) in the configuration dialog.\
                        \nArguments order can be changed using drag and drop with\
                        right mouse button in the arguments listbox." \
                       "i filen!\n\n\
                        Kontroller funksjonsnavnet och argument (navn, rekkefølge)\
                        i konfigurasjonsdialogen.\n\
                        Rekkefølge av argumenter kan endres ved dra och slipp med\
                        høyre musknapp i ruten med argument."
::msgcat::mcset no "Error in selected function name or arguments" "Feil i valgt funksjonsnavn eller argument"
::msgcat::mcset no "Debugger is not yet developed for sce scripts!" "Det finnes ingen debugger for sce kode!"


# db_debugsession.tcl
::msgcat::mcset no "Scilab is working, wait for the prompt to issue a debugger command." "Scilab arbeider, vent på kursor."
::msgcat::mcset no "Cursor position is out of reach!" "Kursor er utenfor rekkevidde!"
::msgcat::mcset no "Cursor must be in a function" "Kursor må være plassert i en funktion"
::msgcat::mcset no "No effect - The debugged file is not stuck" "No effect - The debugged file is not stuck"
::msgcat::mcset no "Scilab execution error" "Kjørefeil i Scilab"
::msgcat::mcset no "The shell reported an error while trying to execute " "Skallet rapporterte en feil under kjøring "
    ::msgcat::mcset no " at line " " på rad "
    ::msgcat::mcset no " of " " i "
::msgcat::mcset no "Execution aborted!" "Kjøring avbrutt!"


# db_init.tcl
::msgcat::mcset no "Please wait..." "Vennligst vent..."


# db_states.tcl
::msgcat::mcset no "Currently no debug session" "Foreløpig ingen feilsøkingssesjon"
::msgcat::mcset no "Ready to start debug" "Klar for å begynne feilsøking"
::msgcat::mcset no "Debug in progress" "Feilsøking pågår"


# db_watch.tcl
::msgcat::mcset no "Watch variables:" "Overvåk variabler :"
::msgcat::mcset no "Remove" "Fjern"
::msgcat::mcset no "Call stack:" "Call stack:"
::msgcat::mcset no "Close" "Lukk"
::msgcat::mcset no "Show watch variables" "Vis overvåkede variabler"
::msgcat::mcset no "Hide watch variables" "Skjul overvåkede variabler"
::msgcat::mcset no "Show call stack" "Vis call stack"
::msgcat::mcset no "Hide call stack" "Skjul call stack"


# filecommands.tcl
::msgcat::mcset no "New Script" "Nytt skript"
::msgcat::mcset no "The contents of" "Innholdet av"
    ::msgcat::mcset no "may have changed, do you wish to save your changes?" "kan være endret, vil de lagre endringene?"
::msgcat::mcset no "Save Confirm?" "Bekreft lagring?"
::msgcat::mcset no "Weird drag&drop" "Merkelig drag&drop"
::msgcat::mcset no "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                    "er verken et gyldig filnavn eller katalog.\
                     Enten drar du et object av en annen type, eller \
                     er det feil i dra-og-slipp funksjonaliteten."
::msgcat::mcset no "File not found" "Filen finnes ikke"
::msgcat::mcset no "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "finnes ikke lenger. Vil du opprette en tom fil med samme navn?"
::msgcat::mcset no "Open file" "Åpne fil"
::msgcat::mcset no "The file" "Filen"
    ::msgcat::mcset no "is already opened! Save the current opened file to an another name and reopen it from disk!" "er allerede åpen! Lagre den åpne filen under et annet navn eller åpne den igjen!"
::msgcat::mcset no "File" "Fil"
    ::msgcat::mcset no "saved" "lagret"
::msgcat::mcset no "cannot be written!" "kan ikke skrives!"
::msgcat::mcset no "The contents of " "Innholdet i"
    ::msgcat::mcset no "has changed on disk, save it anyway?" "er endret på disken, lagre den uansett?"
::msgcat::mcset no "File has changed!" "Filen er endret!"
::msgcat::mcset no "Save as" "Lagre som"
::msgcat::mcset no "Scilab files" "Scilab filer"
::msgcat::mcset no "XML files" "XML filer"
::msgcat::mcset no "All files" "Alle filer"
::msgcat::mcset no "Wait seconds while loading and colorizing file" "Vent mens filen lastest og fargelegges"
::msgcat::mcset no "The contents of this file may have changed, do you wish to save your changes?" "Innholdet i filen kan være endret, ønsker du å lagre endringene dine?"
::msgcat::mcset no "Revert" "Gå tilbake"
    ::msgcat::mcset no "to saved?" "til tidligere versjon?"
::msgcat::mcset no "Revert Confirm?" "Bekreft lasting av tidligere versjon?"


# findreplace.tcl
::msgcat::mcset no "The string" "Strengen"
    ::msgcat::mcset no "could not be found" "ble ikke funnet"
::msgcat::mcset no "replacements done" "erstattninger utført"
::msgcat::mcset no "Find" "Søk"
::msgcat::mcset no "No match found in the selection for" "Ingen treff i den markerte delen av"
    ::msgcat::mcset no "\nWould you like to look for it in the entire text?" "\nVil du søke gjennom hele teksten?"
::msgcat::mcset no "You are searching for an empty string!" "Du søker etter en tom streng!"
::msgcat::mcset no "Re&place" "&Erstatt"
::msgcat::mcset no "Find what:" "Søk etter :"
::msgcat::mcset no "Replace with:" "Erstatt med :"
::msgcat::mcset no "Find &Next" "Finn &neste"
::msgcat::mcset no "Replace &All" "Erstatt &alle"
::msgcat::mcset no "&Upwards" "&Oppover"
::msgcat::mcset no "Downward&s" "&Nedover" 
::msgcat::mcset no "Match &case" "Skill på store og små bokstaver"
::msgcat::mcset no "&Regular expression" "&Regulært uttrykk (regexp)"


# helps.tcl
::msgcat::mcset no "About" "Om"
::msgcat::mcset no "\n Originated by Joseph Acosta, joeja@mindspring.com.\n\
                    Modified by Scilab Consortium.\n\
                    Revised by Enrico Segre 2003-2005.\n\
                    Miscellaneous improvements, particularly debug tools, by Francois Vogel 2004-2005." \
                   "\n Lagd av Joseph Acosta, joeja@mindspring.com.\n\
                    Modifisert av Scilab Constortium.\n\
                    Revidert av Enrico Segre 2003-2005.\n\
                    Diverse forbedringer, spesielt feilsøkingsverktøy av François Vogel 2004-2005."


# infomessages.tcl
::msgcat::mcset no "Line:" "Rad:"
    ::msgcat::mcset no "Column:" "Kolonne:"
::msgcat::mcset no "Line" "Rad"
    ::msgcat::mcset no "in" "i"
::msgcat::mcset no " \[ReadOnly\]" " \[Skrivebeskyttet\]"
::msgcat::mcset no " (modified)" " (endret)"


# inputtext.tcl
::msgcat::mcset no "No corresponding <" "Ingen tilsvarende <"
    ::msgcat::mcset no "> found!" "> funnet!"
::msgcat::mcset no "Code editing is not allowed during debug!" "Editering av kode ikke tillatt under feilsøking!"


# menues.tcl
::msgcat::mcset no "&File" "&Fil"
::msgcat::mcset no "&Edit" "R&ediger"
::msgcat::mcset no "&Search" "&Søk og erstatt"
::msgcat::mcset no "E&xecute" "&Kjør"
::msgcat::mcset no "&Debug" "&Feilsøk"
::msgcat::mcset no "S&cheme" "Spr&åk"
::msgcat::mcset no "&Options" "&Innstillinger"
::msgcat::mcset no "&Windows" "&Vindu"
::msgcat::mcset no "&Help" "&Hjelp"

    # menues.tcl - File menu
    ::msgcat::mcset no "&New" "&Ny"
    ::msgcat::mcset no "&Open..." "Åpne..."
    ::msgcat::mcset no "&Save" "&Lagre"
    ::msgcat::mcset no "Save &as..." "Lagre &som..."
    ::msgcat::mcset no "&Revert..." "&Angre..."
    ::msgcat::mcset no "Import &Matlab file..."  "Importer &Matlab fil..."
    ::msgcat::mcset no "Create help s&keleton..." "Lag &hjelpmal..."
    ::msgcat::mcset no "Compile as &help page" "Kompiler som &hjelpfil"
    ::msgcat::mcset no "Open &function source" "Åpne &funksjonskode"
    ::msgcat::mcset no "Print Se&tup" "Skriver &oppsett"
    ::msgcat::mcset no "&Print" "&Skriv"
    ::msgcat::mcset no "&Close" "&Lukk"
    ::msgcat::mcset no "E&xit" "Avs&lutt"

    # menues.tcl - Edit menu
    ::msgcat::mcset no "&Undo" "Angre"
    ::msgcat::mcset no "&Redo" "Gjenta"
    ::msgcat::mcset no "Cu&t" "Klipp &ut"
    ::msgcat::mcset no "&Copy" "Kopier"
    ::msgcat::mcset no "&Paste" "Lim inn"
    ::msgcat::mcset no "&Delete" "Slett"
    ::msgcat::mcset no "Select &All" "Velg &alt"
    ::msgcat::mcset no "Insert &Time/Date" "Sett inn dato og &tid"
    ::msgcat::mcset no "Co&mment selection" "Ko&mmenter valgt område"
    ::msgcat::mcset no "U&ncomment selection" "A&vkommenter valgt område"
    ::msgcat::mcset no "&Indent selection" "&Rykk &inn valgt område"
    ::msgcat::mcset no "Unin&dent selection" "&Rykk &tilbake valgt område"

    # menues.tcl - Search menu
    ::msgcat::mcset no "&Find..." "&Søk..."
    ::msgcat::mcset no "Find &Next" "Søk &neste"
    ::msgcat::mcset no "&Replace..." "E&rstatt..."
    ::msgcat::mcset no "&Goto Line..." "&Gå til linje..."

    # menues.tcl - Exec menu
    ::msgcat::mcset no "&Load into Scilab" "&Last inn i Scilab"
    ::msgcat::mcset no "&Evaluate selection" "&Evaluer markering"

    # menues.tcl - Debug menu
    ::msgcat::mcset no "&Insert/Remove breakpoint" "&Legg til/Fjern stoppested"
    ::msgcat::mcset no "Remove &all breakpoints" "&Fjern alle stoppesteder"
    ::msgcat::mcset no "&Configure execution..." "Konfigurer kjøring..."
    ::msgcat::mcset no "Go to next b&reakpoint" "Gå til neste stoppested"
    ::msgcat::mcset no "&Step by step" "Steg for steg"
    ::msgcat::mcset no "Step &into" "Steg &inn i"
    ::msgcat::mcset no "Step o&ver" "Steg &over"
    ::msgcat::mcset no "Step &out" "Steg &ut av"
    ::msgcat::mcset no "Run to c&ursor" "Kjør fram til kursor"
    ::msgcat::mcset no "G&o on ignoring any breakpoint" "&Fortsett og ignorer stoppesteder"
    ::msgcat::mcset no "Show &watch" "&Vis kontroll"
    ::msgcat::mcset no "&Break" "&Stopp"
    ::msgcat::mcset no "Cance&l debug" "&Avbryt feilsøking"

#    ::msgcat::mcset no "language scheme" "Språk"
    # menues.tcl - Scheme menu
    ::msgcat::mcset no "S&cilab" "S&cilab"
    ::msgcat::mcset no "&XML" "&XML"
    ::msgcat::mcset no "&none" "&ingenting"

    # menues.tcl - Options menu
    ::msgcat::mcset no "&Font size" "&Fontstørrelse"
        ::msgcat::mcset no "&micro" "&mikro"
        ::msgcat::mcset no "&small" "&liten"
        ::msgcat::mcset no "m&edium" "m&edium"
        ::msgcat::mcset no "&large" "&stor"
    ::msgcat::mcset no "&Colors" "&Farger"
    ::msgcat::mcset no "&Tabs and indentation" "Tabulering og &innrykk"
        ::msgcat::mcset no "Tab inserts &spaces" "Tabulering til mellomrom"
        ::msgcat::mcset no "&Indentation spaces" "&Innrykksmellomrom"
    ::msgcat::mcset no "Colorize \'&strings\'" "Fargelegg \'&strenger\'"
    ::msgcat::mcset no "Word &Wrap" "&Brekk linjer"
    ::msgcat::mcset no "File&names" "Fil&navn"
        ::msgcat::mcset no "&Full path" "&Komplett filsti"
        ::msgcat::mcset no "Full path if &ambiguous" "Full path if &ambiguous"
        ::msgcat::mcset no "&Unambiguous pruned path" "&Unambiguous pruned path"
    ::msgcat::mcset no "&Recent files" "Tidligere redigerte filer"

    # menues.tcl - Windows menu
    ::msgcat::mcset no "Opened Files" "Åpne filer"

    # menues.tcl - Help menu
    ::msgcat::mcset no "&Help..." "&Hjelp..."
    ::msgcat::mcset no "&What's?..." "&Hva er?..."
    ::msgcat::mcset no "&About" "&Om"


# popupmenus.tcl
::msgcat::mcset no "Open the source of" "Åpne kildekode til"


# print.tcl
::msgcat::mcset no "Print Command: " "Utskriftskommando : "


# scilabexec.tcl
::msgcat::mcset no "No point in loading an empty file!" "Ingen vits å laste en tom fil!"
::msgcat::mcset no "Scilab is working, wait for the prompt to load file" "Scilab arbeider, vent på kursor for å laste filen"
::msgcat::mcset no "Exec done" "Kjøring klar"
::msgcat::mcset no "Scilab is working, wait for the prompt to execute the selection." "Scilab arbeider, vent på å kjøre valgt tekst."
::msgcat::mcset no "Scilab is working, wait for the prompt to convert a Matlab file." "Scilab arbeider, vent på at Matlab filen oversettes."
::msgcat::mcset no "Matlab file to convert" "Oversett Matlab fil"
::msgcat::mcset no "Scilab is converting, please hold on..." "Scilab oversetter, vennligst vent..."
::msgcat::mcset no "Matlab file import" "Importer Matlab fil"
::msgcat::mcset no "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug/index.cgi" \
                   "Oversettelsen misslyktes, se i Scilab vinduet \
                    etter detaljer. Kanskje skal du rapportere feilmeldingen og\
					relevante deler av Matlab filen til\
                    http://scilabsoft.inria.fr/bugzilla_bug/index.cgi"
::msgcat::mcset no "Please select destination path for the xml source of the help file" "Vennligst velg sti til xml kode til hjelpfilen"
::msgcat::mcset no "Older version of" "Eldre versjon av"
    ::msgcat::mcset no "found!" "funnet!"
::msgcat::mcset no "An old version of" "En gammel version av"
    ::msgcat::mcset no "already exists: open the old file instead?" "finnes allerede: Åpne den gamle filen istedenfor?"
::msgcat::mcset no "Existing file" "Filen finnes"
::msgcat::mcset no "New skeleton" "Ny mal"
::msgcat::mcset no "Scilab is working, wait for the prompt to compile the help file." "Scilab arbeider, vent på at hjelpfilen skal kompileres."


# scipad.tcl
::msgcat::mcset no "NOT CONNECTED TO SCILAB" "IKKE KOBLET TIL SCILAB"


# textarea.tcl
::msgcat::mcset no "Font size" "Fontstørrelse"


# whichfun.tcl
::msgcat::mcset no "Being at line" "Begynner på linje"
    ::msgcat::mcset no ", function" ", funksjonen"
    ::msgcat::mcset no "begins at" "begynner på"
    ::msgcat::mcset no ", and there are" ", og det er "
    ::msgcat::mcset no "continued lines, i.e. we are at line" "flere rader, d.v.s. vi er på linje"
    ::msgcat::mcset no "of" "i"
::msgcat::mcset no "Function" "Funksjon"
    ::msgcat::mcset no "line" "rad"
::msgcat::mcset no "The cursor is not currently inside a function body." "Kursor er ikke innenfor noen funksjonskode."
::msgcat::mcset no "Which function?" "Hvilken funksjon?"

#except for "expanded" text, like preference variables description
::msgcat::mcset no "BGCOLOR" "Bakgrunnsfarge"
::msgcat::mcset no "FGCOLOR" "Tekstfarge"
::msgcat::mcset no "CURCOLOR" "Kursor"
::msgcat::mcset no "PARCOLOR" "Parenteser ()"
::msgcat::mcset no "BRAKCOLOR" "Hakeparenteser \\\[\\\]"
::msgcat::mcset no "BRACCOLOR" "Myke parenteser {}"
::msgcat::mcset no "PUNCOLOR"  "Punkter"
::msgcat::mcset no "KEYWCOLOR" "Reserverte ord"
::msgcat::mcset no "OPCOLOR"   "Operatorer"
::msgcat::mcset no "QTXTCOLOR"  "Tekster"
::msgcat::mcset no "REMCOLOR" "Kommentarer"
::msgcat::mcset no "XMLCOLOR" "XML"
::msgcat::mcset no "NUMCOLOR" "Tall"
::msgcat::mcset no "SELCOLOR" "Markering"
::msgcat::mcset no "BREAKPOINTCOLOR" "Stoppestedsfarge"
::msgcat::mcset no "LFUNCOLOR" "Biblioteksrutiner"
::msgcat::mcset no "PDEFCOLOR" "Predefinierte variabler"
::msgcat::mcset no "FOUNDTEXTCOLOR" "Funnet tekst"
