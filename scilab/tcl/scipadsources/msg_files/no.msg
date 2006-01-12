# Scipad texts in norwegian
# in sync at least with v5.73

# commons
# commons
::msgcat::mcset no "Cancel" "Avbryt"
::msgcat::mcset no "Cance&l" "Avbry&t"
::msgcat::mcset no "OK" "OK"
::msgcat::mcset no "Scilab working" "Scilab arbeider"
::msgcat::mcset no "All files" "Alle filer"
::msgcat::mcset no "Add/Change" "Legg til/Endre"
::msgcat::mcset no "Untitled" "Navnløs"
::msgcat::mcset no "Close" "Lukk"
::msgcat::mcset no "&Close" "&Lukk"


# buffernavigation.tcl
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
                   "Du kan ikke gjøre det mens fargelegging pågår.\
                    Vennligst prøv igjen når fargelegging er \
                    avsluttet"
::msgcat::mcset no "Command forbidden during colorization"\
                   "Forbudt kommando under fargelegging"


# completion.tcl
::msgcat::mcset no "No possible completion" "Ingen mulig avslutning"
::msgcat::mcset no "possible completions" "mulige avsluttninger"
::msgcat::mcset no "Possible completions" "Mulige avsluttninger"


# db_argsmanagement.tcl
::msgcat::mcset no "Variable:" "Variabel :"
::msgcat::mcset no "Value:" "Verdi :"
::msgcat::mcset no "Get current value from Scilab"\
                   "Hent aktuell verdi fra Scilab"


# db_configureexec.tcl
::msgcat::mcset no "Configure execution" "Konfigurer kjøring"
::msgcat::mcset no "Function name:" "Funksjonsnavn :"
::msgcat::mcset no "Obtain" "Hent"
::msgcat::mcset no "Input arguments:" "Input argumenter :"
::msgcat::mcset no "Remove" "Fjern"
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
::msgcat::mcset no "Debugger is not yet developed for sce scripts!"\
                   "Det finnes ingen debugger for sce kode!"


# db_debugsession.tcl
::msgcat::mcset no "Scilab is working, wait for the prompt to issue a\
                    debugger command."\
                   "Scilab arbeider, vent på kursor."
::msgcat::mcset no "Cursor position is out of reach!"\
                   "Kursor er utenfor rekkevidde!"
::msgcat::mcset no "Cursor must be in a function"\
                   "Kursor må være plassert i en funktion"
::msgcat::mcset no "No effect - The debugged file is not stuck"\
                   "No effect - The debugged file is not stuck"
::msgcat::mcset no "Scilab execution error" "Kjørefeil i Scilab"
::msgcat::mcset no "The shell reported an error while trying to execute "\
                   "Skallet rapporterte en feil under kjøring "
::msgcat::mcset no " at line " " på rad "
::msgcat::mcset no " of " " i "
::msgcat::mcset no "Execution aborted!" "Kjøring avbrutt!"


# db_init.tcl
::msgcat::mcset no "Please wait..." "Vennligst vent..."


# db_states.tcl
::msgcat::mcset no "Currently no debug session"\
                   "Foreløpig ingen feilsøkingssesjon"
::msgcat::mcset no "Ready to start debug" "Klar for å begynne feilsøking"
::msgcat::mcset no "Debug in progress" "Feilsøking pågår"


# db_watch.tcl
::msgcat::mcset no "Watch" "Overvåk"
::msgcat::mcset no "Watch variables:" "Overvåk variabler :"
::msgcat::mcset no "Remove" "Fjern"
::msgcat::mcset no "Call stack:" "Call stack:"
::msgcat::mcset no "Show watch variables" "Vis overvåkede variabler"
::msgcat::mcset no "Hide watch variables" "Skjul overvåkede variabler"
::msgcat::mcset no "Show call stack" "Vis call stack"
::msgcat::mcset no "Hide call stack" "Skjul call stack"
::msgcat::mcset no "Undefined elements are not legal in variable"\
                   "Udefinerte elementer er ikke lovlig i variabel"
::msgcat::mcset no ".\nThis variable will not be updated in Scilab."\
                   ".\nDenne variabelen vil ikke bli oppdatert i Scilab"
::msgcat::mcset no "Illegal undefined element found"\
                   "Ulovlig udefinert element funnet"


# filecommands.tcl
::msgcat::mcset no "New Script" "Nytt skript"
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
::msgcat::mcset no "The file" "Filen"
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


# findreplace.tcl
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
::msgcat::mcset no "&Upwards" "&Oppover"
::msgcat::mcset no "&Downwards" "&Nedover"
::msgcat::mcset no "Match &whole word only" "Søk kun etter &hele ord"
::msgcat::mcset no "Match &case" "Skill på store og små bokstaver"
::msgcat::mcset no "&Regular expression" "&Regulært uttrykk (regexp)"
::msgcat::mcset no "In all &opened files" "I alle åpne &filer"
::msgcat::mcset no "In &selection only" "Kun i &valgt område"
::msgcat::mcset no "In a director&y" "I en &katalog"
::msgcat::mcset no "Directory r&ecurse search" "Søk &rekursivt i katalog"
::msgcat::mcset no "In files/file types:" "I filer/fil typer:"
::msgcat::mcset no "In directory:" "I katalog:"
::msgcat::mcset no "Wrapped around" "Wrapped around"
::msgcat::mcset no "Back at the first file searched!"\
                   "Tilbake ved første fil som ble gjennomsøkt!"
::msgcat::mcset no "No more matches" "Ingen flere søketreff"
::msgcat::mcset no "The string to regexp search for cannot be compiled by\
                    Tcl.\nCheck again, maybe certain characters should be\
                    escaped."\
                   "Tekststrengen til regexp søk kan ikke kompileres av\
                    Tcl.\nSjekk på nytt, kanskje noen karakterer  behøver\
                    \\"


# findinfiles.tcl
::msgcat::mcset no "Directory access issue" "Problem med adgang til katalog"
::msgcat::mcset no "The directory you specified cannot be reached. The\
                    search won't bring any result."\
                   "Angitt katalog kan ikke nås. Søket vil ikke gi resultat"
::msgcat::mcset no "Some directories or files could not be open and will be\
                    ignored during search.\n You might miss read access to\
                    them.\n\n The following ones will be ignored"\
                   "Noen kataloger eller filer kan ikke åpnes  og vil ikke\
                    tas med i søk.\n Kanskje du mangler lesetilgang til\
                    dem.\n\n Følgende vil ikke bli tatt med."
::msgcat::mcset no "Ignored files" "Utelatte filer"
::msgcat::mcset no "Searching in file:" "Søker i fil:"
::msgcat::mcset no "End of file search:" "Slutt på filsøk:"
::msgcat::mcset no "matches found" "antall søketreff"
::msgcat::mcset no "Constructing file list:" "Genererer liste over filer:"
::msgcat::mcset no "&Previous" "&Foregående"
::msgcat::mcset no "&Next" "&Neste"
::msgcat::mcset no "Scipad search results" "Søkeresultater fra Scipad"


# helps.tcl
::msgcat::mcset no "About" "Om"
::msgcat::mcset no "aboutme_message"\
                   "\n Lagd av Joseph Acosta, joeja@mindspring.com.\n\
                    Modifisert av Scilab Constortium.\n Revidert av Enrico\
                    Segre 2003-2005.\n Diverse forbedringer, spesielt\
                    feilsøkingsverktøy av François Vogel 2004-2005."


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
::msgcat::mcset no "&Revert..." "&Angre..."
::msgcat::mcset no "Import &Matlab file..." "Importer &Matlab fil..."
::msgcat::mcset no "Create help s&keleton..." "Lag &hjelpmal..."
::msgcat::mcset no "Compile as &help page" "Kompiler som &hjelpfil"
::msgcat::mcset no "Open &function source" "Åpne &funksjonskode"
::msgcat::mcset no "Print Se&tup" "Skriver &oppsett"
::msgcat::mcset no "&Print" "&Skriv"
::msgcat::mcset no "E&xit" "Avs&lutt"

# menues.tcl - Edit menu
::msgcat::mcset no "&Undo" "Angre"
::msgcat::mcset no "&Redo" "&Gjenta"
::msgcat::mcset no "Cu&t" "Klipp &ut"
::msgcat::mcset no "&Copy" "Kopier"
::msgcat::mcset no "&Paste" "&Lim inn"
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
::msgcat::mcset no "Go to next b&reakpoint" "Gå til neste stoppested"
::msgcat::mcset no "&Step by step" "Steg for steg"
::msgcat::mcset no "Step &into" "Steg &inn i"
::msgcat::mcset no "Step o&ver" "Steg &over"
::msgcat::mcset no "Step &out" "Steg &ut av"
::msgcat::mcset no "Run to c&ursor" "Kjør fram til kursor"
::msgcat::mcset no "G&o on ignoring any breakpoint"\
                   "&Fortsett og ignorer stoppsignal"
::msgcat::mcset no "Show &watch" "&Vis overvåket variabel"
::msgcat::mcset no "&Break" "&Stopp"
::msgcat::mcset no "Cance&l debug" "&Avbryt feilsøking"

#    ::msgcat::mcset fr "language scheme" "schéma langage"
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
::msgcat::mcset no "Colorize \'&strings\'" "Fargelegg \'&strenger\'"
::msgcat::mcset no "Show c&ontinued lines" "Vis forlengede linjer"
::msgcat::mcset no "Word &Wrap" "&Brekk linjer"
::msgcat::mcset no "&Tabs and indentation" "Tabulering og &innrykk"
::msgcat::mcset no "Tab inserts &spaces" "Tabulering til mellomrom"
::msgcat::mcset no "&Indentation spaces" "&Innrykksmellomrom"
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
::msgcat::mcset no "&Recent files" "Tidligere redigerte filer"
::msgcat::mcset no "&Backup files depth" "Sikkerhetskopidybde (antall .bak filer)"
::msgcat::mcset no "&Locale" "&Språk"
# locale names (common)
::msgcat::mcset no "de locale (de)" "Deutsch (de)"
::msgcat::mcset no "fr locale (fr)" "Français (fr)"
::msgcat::mcset no "eng locale (eng)" "English (eng)"
::msgcat::mcset no "es locale (es)" "Español (es)"
::msgcat::mcset no "it locale (it)" "Italiano (it)"
::msgcat::mcset no "no locale (no)" "Norsk (no)"
::msgcat::mcset no "pl locale (pl)" "Polski (pl)"
::msgcat::mcset no "se locale (se)" "Svenska (se)"

# menues.tcl - Windows menu
::msgcat::mcset no "&Maximize" "&Maksimer"
::msgcat::mcset no "&Split" "&Splitt"
::msgcat::mcset no "S&plit (side by side)" "S&plitt (side om side)"
::msgcat::mcset no "Tile all &vertically" "Stable alle &vertikalt"
::msgcat::mcset no "Tile all &horizontally" "Stable alle &horisontalt"
::msgcat::mcset no "Space sashes &evenly" "Fordel &jevnt"
::msgcat::mcset no "Opened Files" "Åpne filer"

# menues.tcl - Help menu
::msgcat::mcset no "&Help..." "&Hjelp..."
::msgcat::mcset no "&What's?..." "&Hva er?..."
::msgcat::mcset no "&About" "&Om"
::msgcat::mcset no "Adding &translations..." "Legger til oversettelser..."
::msgcat::mcset no "Adding Scipad Translations..."\
                   "Legger til Scipad oversettelser:"
::msgcat::mcset no "&Bugs \& wishlist" "Feil \& ønskeliste"
::msgcat::mcset no "Scipad Known bugs" "Kjente Scipad feil"
::msgcat::mcset no "Change&log" "Endringslogg"
::msgcat::mcset no "Changes in the Scipad codebase"\
                   "Endringer I Scipad kode"


# popupmenus.tcl
::msgcat::mcset no "Open the source of" "Åpne kildekode til"
::msgcat::mcset no "AddWatch" "Legg til overvåket variabel"


# print.tcl
::msgcat::mcset no "Print Setup" "Utskriftsoppsett"
::msgcat::mcset no "Print Command: " "Utskriftskommando : "


# scilabexec.tcl
::msgcat::mcset no "No point in loading an empty file!"\
                   "Ingen vits å laste en tom fil!"
::msgcat::mcset no "Scilab is working, wait for the prompt to load file"\
                   "Scilab arbeider, vent på kursor for å laste filen"
::msgcat::mcset no "Exec done" "Kjøring klar"
::msgcat::mcset no "Scilab is working, wait for the prompt to execute the\
                    selection."\
                   "Scilab arbeider, vent på å kjøre valgt tekst."
::msgcat::mcset no "Scilab is working, wait for the prompt to convert a\
                    Matlab file."\
                   "Scilab arbeider, vent på at Matlab filen oversettes."
::msgcat::mcset no "Matlab files" "Matlabfiler"
::msgcat::mcset no "Matlab file to convert" "Oversett Matlab fil"
::msgcat::mcset no "Scilab is converting, please hold on..."\
                   "Scilab oversetter, vennligst vent..."
::msgcat::mcset no "Matlab file import" "Importer Matlab fil"
::msgcat::mcset no "Conversion of the file failed, see the Scilab window for\
                    details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"\
                   "Oversettelsen misslyktes, se i Scilab vinduet  etter\
                    detaljer. Kanskje skal du rapportere feilmeldingen og\
                    					relevante deler av Matlab filen til\
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"
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
::msgcat::mcset no "Scilab is working, wait for the prompt to compile the\
                    help file."\
                   "Scilab arbeider, vent på at hjelpfilen skal kompileres."
::msgcat::mcset no "ScilabEval command cannot be passed to Scilab!"\
                   "ScilabEval kommando kan ikke sendes til Scilab!"
::msgcat::mcset no "impossibleScilabEval_message" "umulig ScilabEval melding"


# scipad.tcl
::msgcat::mcset no "NOT CONNECTED TO SCILAB" "IKKE KOBLET TIL SCILAB"


# textarea.tcl
::msgcat::mcset no "Font size" "Fontstørrelse"


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
::msgcat::mcset no "FGCOLOR" "Tekstfarge"
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
::msgcat::mcset no "BREAKPOINTCOLOR" "Stoppestedsfarge"
::msgcat::mcset no "LFUNCOLOR" "Biblioteksrutiner"
::msgcat::mcset no "PDEFCOLOR" "Predefinierte variabler"
::msgcat::mcset no "FOUNDTEXTCOLOR" "Funnet tekst"
::msgcat::mcset no "CONTLINECOLOR" "Ufullstendige linjer"


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
