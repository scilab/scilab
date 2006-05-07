# Scipad texts in Italian
# in sync at least with v6.30


# commons
::msgcat::mcset it "Cancel" "Annulla"
::msgcat::mcset it "Cance&l" "Annu&lla"
::msgcat::mcset it "OK" "OK"
::msgcat::mcset it "All files" "Tutti i files"
::msgcat::mcset it "Add/Change" "Aggiungi/modifica"
::msgcat::mcset it "Untitled" "SenzaTitolo"
::msgcat::mcset it "Close" "Chiudi"
::msgcat::mcset it "&Close" "&Chiudi"


# buffernavigation.tcl
::msgcat::mcset it "Goto Line?" "Vai alla linea?"
::msgcat::mcset it "Go to:" "Vai a:"
::msgcat::mcset it "&logical line" "linea &logica"
::msgcat::mcset it "&physical line" "linea fi&sica"
::msgcat::mcset it "in &function" "della &funzione"
::msgcat::mcset it "in &current file" "del file &corrente"
::msgcat::mcset it "Outside of function definition"\
                   "fuori dalla definizione della funzione"


# colorize.tcl
::msgcat::mcset it "Wait seconds while recolorizing file"\
                   "Attendere, ricolorazione in corso"
::msgcat::mcset it "You can't do that while colorization is in progress.\
                    Please try again when finished."\
                   "Non si può fare questo prima che la colorazione sia\
                    completata. Aspettare il termine della colorazione e\
                    riprovare"
::msgcat::mcset it "Command forbidden during colorization"\
                   "Comando non consentito mentre la colorazione è in corso"


# completion.tcl
::msgcat::mcset it "No possible completion" "Nessun completamento possibile"
::msgcat::mcset it "possible completions" "completamenti possibili"
::msgcat::mcset it "Possible completions" "Completamenti possibili"


# db_argsmanagement.tcl
::msgcat::mcset it "Variable:" "Variabile:"
::msgcat::mcset it "Value:" "Valore:"
::msgcat::mcset it "Get current value from Scilab"\
                   "Recupera il valore attuale in Scilab"


# db_configureexec.tcl
::msgcat::mcset it "Configure execution" "Configura l'esecuzione"
::msgcat::mcset it "Function name:" "Nome della funzione:"
::msgcat::mcset it "Obtain" "Ottieni"
::msgcat::mcset it "Input arguments:" "Argumenti di ingresso:"
::msgcat::mcset it "Remove" "Rimuovi"
::msgcat::mcset it "The selected function has not been found in the opened\
                    files, maybe it was deleted since last configuration of\
                    the debugger.\nUse button \""\
                   "la funzione selezionata non compare in nessuno dei files\
                    aperti, potrebbe essere stata cancellata dall'ultima\
                    volta che il debugger è stato configurato. Usare il\
                    tasto \""
::msgcat::mcset it "\" to refresh the list of available functions."\
                   "\" per rinfrescare la lista delle funzioni disponibili"
::msgcat::mcset it "Function name or input arguments do not match definition\
                    of the function"\
                   "Il nome della funzione o i suoi argomenti non\
                    corrispondono alla definizione della funzione"
::msgcat::mcset it "in the file!\n\nCheck function name and arguments\
                    (names, order) in the configuration dialog. \nArguments\
                    order can be changed using drag and drop with right\
                    mouse button in the arguments listbox."\
                   "nel file!\n\n Verificare il nome della funzione e gli\
                    argomenti (nome, ordine di comparsa) nella casella del\
                    dialogo di configurazione.\n L'ordine degli argomenti\
                    può essere modificato trascinando e deponendo col\
                    mouse."
::msgcat::mcset it "Error in selected function name or arguments"\
                   "Errore nella funzione selezionata o nei suoi argomenti"
::msgcat::mcset it "At least one input argument does not match definition of\
                    the function"\
                   "Uno o più argomenti non corrispondono alla definizione\
                    della funzione"
::msgcat::mcset it "in the file!\n\nCheck function arguments (their names)\
                    in the configuration dialog."\
                   "nel file!\n\nVerificare il nome della funzione e gli\
                    argomenti (nome, ordine di comparsa) nel dialogo di\
                    configurazione."
::msgcat::mcset it "Error in selected function arguments"\
                   "Errore negli argomenti della funzione selezionata"
::msgcat::mcset it "Main level code found"\
                   "Codice a livello principale"
::msgcat::mcset it "This file contains main level code.\nDebug as a single\
                    .sce file?"\
                   "Questo file contiene codice a livello principale. Eseguire\
                    il debug trattando tutto il file come un unica funzione?"
::msgcat::mcset it "No function found" "Nessuna funzione trovata"
::msgcat::mcset it "This file contains no function and will be treated"\
                   "Questo file non contiene alcuna funzione e sarà trattato"
::msgcat::mcset it "as a single .sce file.\nStart debugging?"\
                   "Come un unico file .sce. Cominciare il debug?"


# db_debugsession.tcl
::msgcat::mcset it "Cursor must be in a function"\
                   "Il cursore deve essere dentro una funzione"
::msgcat::mcset it "No effect - The debugged file is not stuck"\
                   "Senza effetto - il file in debug non è piantato"
::msgcat::mcset it "Too many breakpointed functions"\
                   "Troppe funzioni con punti di arresto"
::msgcat::mcset it "You have currently" "Ci sono al momento"
::msgcat::mcset it "functions in your opened files." "funzioni."
::msgcat::mcset it "Scilab supports a maximum of" "Scilab supporta al massimo"
::msgcat::mcset it "possible breakpointed functions (see help setbpt)."\
                   "funzioni con punti d'arresto (vedi help setbpt)."
::msgcat::mcset it "Step-by-step hence cannot be performed."\
                   "Il modo passo-passo non è pertanto possible."
::msgcat::mcset it "This command will actually run to the next breakpoint."\
                   "Questo comando andrà solo fino all'arresto successivo"
::msgcat::mcset it "Too many breakpoints" "Troppi punti d'arresto"
::msgcat::mcset it "Executing this command would require to set"\
                   "Eseguire questo comando richiederebbe di inserire"
::msgcat::mcset it "breakpoints in your opened files."\
                   "punti d'arresto nei files aperti."
::msgcat::mcset it "possible breakpoints in Scilab (see help setbpt)."\
                   "possibili punti d'arresto (vedere help setbpt)."
::msgcat::mcset it "No breakpoint here!" "Non si può mettere un arresto qui!"


# db_init.tcl
::msgcat::mcset it "Please wait..." "Attendere..."


# db_states.tcl
::msgcat::mcset it "Currently no debug session"\
                   "Nessuna sessione di debug in corso"
::msgcat::mcset it "Ready to start debug" "Pronto per il debug"
::msgcat::mcset it "Debug in progress" "Debug in corso"


# db_watch.tcl
::msgcat::mcset it "Watch" "Controllo esecuzione e variabili"
::msgcat::mcset it "Watch variables:" "Variabili osservate:"
::msgcat::mcset it "Remove" "Rimuovi"
::msgcat::mcset it "Call stack:" "stack di programma:"
::msgcat::mcset it "Hide watch variables" "Nascondi le variabili"
::msgcat::mcset it "Hide call stack" "Nascondi lo stack"
::msgcat::mcset it "Always on top" "Sempre in primo piano"
::msgcat::mcset it "Undefined elements are not legal in variable"\
                   "Gli elementi indefiniti sono illegali in questa variabile"
::msgcat::mcset it ".\nThis variable will not be updated in Scilab."\
                   ".\nQuesta variabile non sarà aggiornata in Scilab"
::msgcat::mcset it "Illegal undefined element found"\
                   "Trovato un elemento indefinito"


# filecommands.tcl
::msgcat::mcset it "New Script" "Nuovo testo"
::msgcat::mcset it "The contents of" "Il contenuto di"
::msgcat::mcset it "may have changed, do you wish to save your changes?"\
                   "potrebbe essere cambiato, registrare le modifiche?"
::msgcat::mcset it "Save Confirm?" "Conferma salvataggio?"
::msgcat::mcset it "Open source of..." "Apri la sorgente di..."
::msgcat::mcset it "Open source of:" "Apri la sorgente di:"
::msgcat::mcset it "Weird drag&drop" "Strano trascina e deponi"
::msgcat::mcset it "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or you\
                    hit a bug of the dnd mechanism."\
                   "non è un nome valido per un file. O è stato trascinato\
                    un oggetto di tipo sbagliato, o il meccanismo di d&d ha\
                    un baco."
::msgcat::mcset it "File not found" "File non trovato"
::msgcat::mcset it "does not exist anymore. Do you want to create an empty\
                    file with the same name?"\
                   "non esiste più. Creare un nuovo file collo stesso nome?"
::msgcat::mcset it "Open file" "Aprire file"
::msgcat::mcset it "The file" "Il file"
::msgcat::mcset it "is already opened! Save the current opened file to\
                    another name and reopen it from disk!"\
                   "è già aperto! Salvarlo eventualmente con un altro nome e\
                    riaprirlo dal disco"
::msgcat::mcset it "File" "File"
::msgcat::mcset it "saved" "salvato"
::msgcat::mcset it "cannot be written!" "non è scrivibile!"
::msgcat::mcset it "The contents of " "Il contenuto di"
::msgcat::mcset it "has changed on disk, save it anyway?"\
                   "è cambiato sul disco, salvare comunque?"
::msgcat::mcset it "has been modified outside of Scipad. Do you want to\
                    reload it?"\
                   "è stato cambiato fuori da Scipad. Ricaricarlo dal disco?"
::msgcat::mcset it "File has changed!" "Il file è cambiato"
::msgcat::mcset it "Save as" "Registra come"
::msgcat::mcset it "Scilab files" "Files Scilab"
::msgcat::mcset it "Scicos files" "Files Scicos"
::msgcat::mcset it "XML files" "Files XML"
::msgcat::mcset it "Wait seconds while loading and colorizing file"\
                   "Attendere - apertura e colorazione del file in corso"
::msgcat::mcset it "The contents of this file may have changed, do you wish\
                    to save your changes?"\
                   "Il contenuto di questo file potrebbe essere cambiato,\
                    registrare la nuova versione?"
::msgcat::mcset it "Revert" "Ritornare alla versione di"
::msgcat::mcset it "to saved?" "salvata sul disco?"
::msgcat::mcset it "Revert Confirm?" "Conferma recupero?"
::msgcat::mcset it "Unreadable file" "File illeggibile!"
::msgcat::mcset it "exists but is not readable!" "esiste, ma non è leggibile!"
::msgcat::mcset it "This file is rather large and colorization might take\
                    some time.\n\nColorize anyway?"\
                   "Questo file è relativamente grosso, e la colorazione\
                    potrebbe richiedere tempo. Colorare comunque?"
::msgcat::mcset it "File size warning" "Avvertimento: grosso file"


# findreplace.tcl
::msgcat::mcset it "The string" "La stringa \["
::msgcat::mcset it "could not be found in" "\] non è stata trovata"
::msgcat::mcset it "could not be found in any of the opened files !"\
                   "\] non è stata trovata in nessuno dei files aperti!"
::msgcat::mcset it "replacements done" "sostituzioni effettuate"
::msgcat::mcset it "Find" "Cerca"
::msgcat::mcset it "No match found in the selection for"\
                   "Nessuna occorrenza di \["
::msgcat::mcset it "\nWould you like to look for it in the entire text?"\
                   "\] trovata all'interno della selezione. Continuare la\
                    ricerca nel resto del testo?"
::msgcat::mcset it "You are searching for an empty string!"\
                   "La stringa da cercare è vuota!"
::msgcat::mcset it "Re&place" "&Sostituisci"
::msgcat::mcset it "Find what:" "Cerca:"
::msgcat::mcset it "Replace with:" "Cambia in:"
::msgcat::mcset it "Find &Next" "Trova il segue&nte"
::msgcat::mcset it "Replace &All" "Rimpiazza &tutti"
::msgcat::mcset it "&Upwards" "Verso l'&alto"
::msgcat::mcset it "&Downwards" "Verso il &basso"
::msgcat::mcset it "Match &whole word only" "Solo parola &intera"
::msgcat::mcset it "Match &case" "Rispetta &maiuscole"
::msgcat::mcset it "&Regular expression" "Espressione &regolare (regexp)"
::msgcat::mcset it "In all &opened files" "In tutti i &files aperti"
::msgcat::mcset it "In &selection only" "Solo nella sele&zione"
::msgcat::mcset it "In a director&y" "In una &directory:"
::msgcat::mcset it "Directory r&ecurse search" "Ricerca ri&corsiva"
::msgcat::mcset it "In files/file types:" "Nei files/tipi di files"
::msgcat::mcset it "In directory:" "Nella directory:"
::msgcat::mcset it "Wrapped around" "Continuato oltre la fine"
::msgcat::mcset it "Back at the first file searched!"\
                   "Di nuovo al primo file esaminato!"
::msgcat::mcset it "No more matches" "Nessuna altra corrispondenza"
::msgcat::mcset it "The string to regexp search for cannot be compiled by\
                    Tcl.\nCheck again, maybe certain characters should be\
                    escaped."\
                   "La stringa da cercare come espressione regolare non ha\
                    potuto essere compilata. Controllare. Per esempio,\
                    alcuni caratteri speciali vanno preceduti da \\."


# findinfiles.tcl
::msgcat::mcset it "Directory access issue" "Errore di accesso alla directory"
::msgcat::mcset it "The directory you specified cannot be reached. The\
                    search won't bring any result."\
                   "Scipad non può accedere alla directory specificata. La\
                    ricerca non darà risultato"
::msgcat::mcset it "Some directories or files could not be open and will be\
                    ignored during search.\n You might miss read access to\
                    them.\n\n The following ones will be ignored"\
                   "Certe directories o certi files non possono essere\
                    aperti (probabilmente sono protetti in lettura) e\
                    saranno ignorati durante la ricerca. I seguenti saranno\
                    ignorati:"
::msgcat::mcset it "Ignored files" "Files ignorati"
::msgcat::mcset it "Searching in file:" "Ricerca in:"
::msgcat::mcset it "End of file search:" "Fine della ricerca:"
::msgcat::mcset it "matches found" "corrispondenze trovate"
::msgcat::mcset it "Constructing file list:"\
                   "Costruzione della lista dei files:"
::msgcat::mcset it "&Previous" "&Precedente"
::msgcat::mcset it "&Next" "&Successivo"
::msgcat::mcset it "Scipad search results" "Risultati della ricerca"


# helps.tcl
::msgcat::mcset it "About" "A proposito"
::msgcat::mcset it "aboutme_message"\
                   "\n Originato da tknotepad di Joseph Acosta,\
                    joeja@mindspring.com.\n Modificato dal Consorzio\
                    Scilab.\n Migliorato da Enrico Segre 2003-2006\n e da\
                    François Vogel (in particolare il debugger) 2004-2006.\n\
                    Traduzioni: \n   norvegese, Torbjørn Pettersen 2005; \n  \
                    polacco, Bartosz Kosiorek 2005; \n   svedese, Gunnar\
                    Lindholm 2005; \n   tedesco, Dirk Reusch 2005; \n  \
                    spagnolo, Juan Nicolás Quesada 2006."


# infomessages.tcl
::msgcat::mcset it "Line:" "Linea:"
::msgcat::mcset it "Column:" "Colonna:"
::msgcat::mcset it "Line" "Linea"
::msgcat::mcset it "in" "di"
::msgcat::mcset it "Logical line:" "Linea logica:"
::msgcat::mcset it " \[ReadOnly\]" " \[SolaLettura\]"
::msgcat::mcset it " (modified)" " (modificato)"


# inputtext.tcl
::msgcat::mcset it "No corresponding <" "Nessun <"
::msgcat::mcset it "> found!" "> corrispondente trovato!"
::msgcat::mcset it "Code editing is not allowed during debug!"\
                   "Impossible modificare il codice durante il debugging!"


# menues.tcl
::msgcat::mcset it "&File" "&File"
::msgcat::mcset it "&Edit" "&Modifica"
::msgcat::mcset it "&Search" "&Cerca"
::msgcat::mcset it "E&xecute" "&Esecuzione"
::msgcat::mcset it "&Debug" "&Debug"
::msgcat::mcset it "S&cheme" "&Schema"
::msgcat::mcset it "&Options" "&Opzioni"
::msgcat::mcset it "&Windows" "F&inestre"
::msgcat::mcset it "&Help" "&Aiuto"

# menues.tcl - File menu
::msgcat::mcset it "&New" "&Nuovo"
::msgcat::mcset it "&Open..." "&Apri..."
::msgcat::mcset it "Open &in new" "Apri &in un nuovo pannello"
::msgcat::mcset it "&horizontal tile" "a &fianco"
::msgcat::mcset it "&vertical tile" "di &sotto"
::msgcat::mcset it "Op&en source of..." "Apri sorgente di..."
::msgcat::mcset it "&Save" "&Salva"
::msgcat::mcset it "Save &as..." "Salva &come..."
::msgcat::mcset it "Save a&ll" "Salva &tutti"
::msgcat::mcset it "&Revert..." "&Recupera..."
::msgcat::mcset it "Import &Matlab file..." "Importa file &Matlab..."
::msgcat::mcset it "Create help s&keleton..." "Crea scheletro di aiuto &XML..."
::msgcat::mcset it "Compile as &help page" "Compila come file di ai&uto"
::msgcat::mcset it "Open &function source" "Apri la sorgente della &funzione"
::msgcat::mcset it "Print Se&tup" "Confi&gura stampante"
::msgcat::mcset it "&Print" "Stam&pa"
::msgcat::mcset it "E&xit" "&Esci"

# menues.tcl - Edit menu
::msgcat::mcset it "&Undo" "&Annulla"
::msgcat::mcset it "&Redo" "&Ripristina"
::msgcat::mcset it "Cu&t" "&Taglia"
::msgcat::mcset it "&Copy" "&Copia"
::msgcat::mcset it "&Paste" "&Incolla"
::msgcat::mcset it "&Delete" "&Elimina"
::msgcat::mcset it "Select &All" "Sele&ziona tutto"
::msgcat::mcset it "Insert &Time/Date" "Inserisci &ora/data"
::msgcat::mcset it "Co&mment selection" "Co&mmenta la selezione"
::msgcat::mcset it "U&ncomment selection" "&Scommenta la selezione"
::msgcat::mcset it "&Indent selection" "&Indenta la selezione"
::msgcat::mcset it "Unin&dent selection" "Arretra la se&lezione"

# menues.tcl - Search menu
::msgcat::mcset it "&Find..." "&Trova..."
::msgcat::mcset it "Find &Next" "Trova il segue&nte"
::msgcat::mcset it "&Replace..." "&Sostituisci..."
::msgcat::mcset it "&Goto Line..." "&Vai a linea..."

# menues.tcl - Exec menu
::msgcat::mcset it "&Load into Scilab" "&Carica in Scilab"
::msgcat::mcset it "&Evaluate selection" "&Esegui la selezione"

# menues.tcl - Debug menu
::msgcat::mcset it "&Insert/Remove breakpoint" "&Inserisci/rimuovi un arresto"
::msgcat::mcset it "Remove &all breakpoints" "Rimuovi &tutti gli arresti"
::msgcat::mcset it "&Configure execution..." "&Configura l'esecuzione..."
::msgcat::mcset it "Go to next b&reakpoint" "&Vai fino al prossimo arresto"
::msgcat::mcset it "&Step by step" "&Passo a passo"
::msgcat::mcset it "Step &into" "Entra &dentro"
::msgcat::mcset it "Step o&ver" "Passa sop&ra"
::msgcat::mcset it "Step &out" "Esci &fuori"
::msgcat::mcset it "Run to c&ursor" "Esegui fino al curso&re"
::msgcat::mcset it "G&o on ignoring any breakpoint"\
                   "Co&ntinua ignorando gli arresti"
::msgcat::mcset it "Show &watch" "Finestra &di controllo"
::msgcat::mcset it "&Break" "&Stop"
::msgcat::mcset it "Cance&l debug" "&Annulla il debug"

#    ::msgcat::mcset fr "language scheme" "schéma langage"
# menues.tcl - Scheme menu
::msgcat::mcset it "S&cilab" "&Scilab"
::msgcat::mcset it "&XML" "&XML"
::msgcat::mcset it "&none" "&nessuno"
::msgcat::mcset it "&Colorize" "&Colora"

# menues.tcl - Options menu
::msgcat::mcset it "&Font size" "&Dimensione carattere"
::msgcat::mcset it "&micro" "&micro"
::msgcat::mcset it "&small" "&piccolo"
::msgcat::mcset it "m&edium" "m&edio"
::msgcat::mcset it "&large" "&grande"
::msgcat::mcset it "&Colors" "&Colori"
::msgcat::mcset it "Colori&ze" "C&olorare"
::msgcat::mcset it "&Always" "&Sempre"
::msgcat::mcset it "Ask for &big files" "&Chiedere per files grandi"
::msgcat::mcset it "&Never" "&Mai"
::msgcat::mcset it "Colorize \'&strings\'" "Colora &\'stringhe\'"
::msgcat::mcset it "Show c&ontinued lines" "Colora linee &incomplete"
::msgcat::mcset it "Word &wrap" "&Ritorno a capo visuale"
::msgcat::mcset it "&Double-click behavior" "Doppio clic &seleziona come"
::msgcat::mcset it "&Linux" "&Linux"
::msgcat::mcset it "Windo&ws" "Windo&w$"
::msgcat::mcset it "&Scilab" "&Scilab"
::msgcat::mcset it "&Tabs and indentation" "&Tabulatori e indentazione"
::msgcat::mcset it "Tab inserts &spaces" "&Tab inserisce spazi"
::msgcat::mcset it "&Indentation spaces" "&Spazi di indentazione"
::msgcat::mcset it "Com&pletion" "&Autocompletamento"
::msgcat::mcset it "&Tab" "&Tab"
::msgcat::mcset it "&Control-Tab" "&Control-Tab"
::msgcat::mcset it "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset it "&Shift-Tab" "&Shift-Tab"
::msgcat::mcset it "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
::msgcat::mcset it "S&hift-Control-Tab" "S&hift-Control-Tab"
::msgcat::mcset it "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
::msgcat::mcset it "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
::msgcat::mcset it "File&names" "Nome dei &files"
::msgcat::mcset it "&Full path" "&Percorso completo"
::msgcat::mcset it "Full path if &ambiguous" "Percorso completo se &ambiguo"
::msgcat::mcset it "&Unambiguous pruned path" "Percorso &ridotto univoco"
::msgcat::mcset it "&Recent files" "&Numero files recenti"
::msgcat::mcset it "&Backup files depth" "Numero &versioni backup"
::msgcat::mcset it "&Locale" "&Localizzazione"
# locale names (common)
::msgcat::mcset it "de locale (de)" "Deutsch (de)"
::msgcat::mcset it "fr locale (fr)" "Français (fr)"
::msgcat::mcset it "eng locale (eng)" "English (eng)"
::msgcat::mcset it "es locale (es)" "Español (es)"
::msgcat::mcset it "it locale (it)" "Italiano (it)"
::msgcat::mcset it "no locale (no)" "Norsk (no)"
::msgcat::mcset it "pl locale (pl)" "Polski (pl)"
::msgcat::mcset it "se locale (se)" "Svenska (se)"

# menues.tcl - Windows menu
::msgcat::mcset it "&Maximize" "&Massimizza"
::msgcat::mcset it "&Split" "Dividi in &orizzontale"
::msgcat::mcset it "S&plit (side by side)" "Dividi in &verticale"
::msgcat::mcset it "Tile all &vertically" "&Impila tutte"
::msgcat::mcset it "Tile all &horizontally" "&Affianca tutte"
::msgcat::mcset it "Space sashes &evenly" "Spezzatura &uniforme"
::msgcat::mcset it "Opened Files" "Files aperti"

# menues.tcl - Help menu
::msgcat::mcset it "&Help..." "&Aiuto..."
::msgcat::mcset it "&What's?..." "&Cos'è?..."
::msgcat::mcset it "&About" "A &proposito"
::msgcat::mcset it "Adding &translations..." "&Tradurre Scipad..."
::msgcat::mcset it "Adding Scipad Translations..."\
                   "Creare una nuova traduzione di Scipad:"
::msgcat::mcset it "&Bugs \& wishlist" "&Bachi \& richieste"
::msgcat::mcset it "Scipad Known bugs" "Bachi noti in Scipad"
::msgcat::mcset it "Change&log" "Cronologia &modifiche"
::msgcat::mcset it "Changes in the Scipad codebase"\
                   "Cronologia delle modifiche al codice di Scipad"


# popupmenus.tcl
::msgcat::mcset it "Open the source of" "Apri la sorgente di"
::msgcat::mcset it "AddWatch" "Tieni %s sotto controllo"
::msgcat::mcset it "Jump to" "Salta a"


# print.tcl
::msgcat::mcset it "Print Setup" "Configura stampante"
::msgcat::mcset it "Print Command: " "Comando di stampa:"


# scilabexec.tcl
::msgcat::mcset it "No point in loading an empty file!"\
                   "Inutile caricare un file vuoto!"
::msgcat::mcset it "Silent file save failed"\
                   "Salvataggio silente non riuscito"
::msgcat::mcset it "File could not be saved in a temporary\
                    location.\nOverwrite original file?"\
                   "Non è stato possibile copiare il file in una cartella\
                    temporanea. Sovrascrivere il file originale?"
::msgcat::mcset it "Exec done" "Exec terminato"
::msgcat::mcset it "Matlab files" "Files Matlab"
::msgcat::mcset it "Matlab file to convert" "File Matlab da importare"
::msgcat::mcset it "Scilab is converting, please hold on..."\
                   "Scilab sta traducendo, attendere..."
::msgcat::mcset it "Matlab file import" "Importazione di un file Matlab"
::msgcat::mcset it "Conversion of the file failed, see the Scilab window for\
                    details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"\
                   "La traduzione del file è fallita, vedere la finestra\
                    Scilab per dettagli -- Il baco può essere riportato a\
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi\
                    includendo il file incriminato ed il messagggio di\
                    errore."
::msgcat::mcset it "Please select destination path for the xml source of the\
                    help file"\
                   "Scegliere una cartella di destinazione per la sorgente\
                    XML della pagina di aiuto"
::msgcat::mcset it "Older version of" "Esiste già una versione di"
::msgcat::mcset it "found!" "!"
::msgcat::mcset it "An old version of" "Esiste una versione precedente di"
::msgcat::mcset it "already exists: open the old file instead?"\
                   ": apro il file esistente invece?"
::msgcat::mcset it "Existing file" "File esistente"
::msgcat::mcset it "New skeleton" "Crea nuovo scheletro"
::msgcat::mcset it "ScilabEval command cannot be passed to Scilab!"\
                   "Un comando ScilabEval non ha potuto essere passato a\
                    Scilab!"
::msgcat::mcset it "impossibleScilabEval_message"\
                   "ATTENZIONE : Un comando non ha potuto essere passato a\
                    Scilab.  ed è stato ignorato\n\n  La ragione più\
                    probabile è che la cartella TMPDIR di Scilab non sia\
                    scrivibile. Ne può risultare un comportamento\
                    imprevedibile.\n\n Il comando in questione è:\n\n-->"
::msgcat::mcset it "impossibleScilabEval_message2"\
                   "ATTENZIONE : Un comando non ha potuto essere passato a\
                    Scilab ed è stato ignorato perché troppo lungo. Ne può\
                    risultare un comportamento imprevedibile.\n\n Se si è\
                    nel corso di una sessione di debug, provare a ridurre le\
                    variabili osservate. \n\n Il comando in questione\
                    è:\n\n-->"
::msgcat::mcset it "(end of command skipped)" "(resto del comando omesso)"
::msgcat::mcset it "Scilab working" "Scilab occupato"
::msgcat::mcset it "Scilab is working, please wait for the prompt to execute\
                    this command!"\
                   "Scilab è occupato, attendere il prompt per eseguire\
                    questo comando!"
::msgcat::mcset it "Scilab is working, wait for the prompt to load file"\
                   "Scilab è occupato, attendere il prompt per caricare il\
                    file"
::msgcat::mcset it "Scilab is working, wait for the prompt to execute the\
                    selection."\
                   "Scilab è occupato, attendere il prompt per eseguire la\
                    selezione"
::msgcat::mcset it "Scilab is working, wait for the prompt to convert a\
                    Matlab file."\
                   "Scilab è occupato, attendere il prompt per convertire un\
                    file Matlab"
::msgcat::mcset it "Scilab is working, wait for the prompt to compile the\
                    help file."\
                   "Scilab è occupato, attendere il prompt per compilare la\
                    pagina di aiuto"
::msgcat::mcset it "Scilab is working, wait for the prompt to issue a\
                    debugger command."\
                   "Scilab è occupato, aspettare il prompt per eseguire\
                    comandi di debug."
::msgcat::mcset it "Scilab execution error"\
                   "Errore di Scilab durante l'esecuzione"
::msgcat::mcset it "The shell reported an error while trying to execute "\
                   "Lo shell ha riportato un errore nel cercare di eseguire"
::msgcat::mcset it " at line " " alla linea "
::msgcat::mcset it " of " " di "
::msgcat::mcset it "Execution aborted!" "Esecuzione annullata!"


# scipad.tcl
::msgcat::mcset it "NOT CONNECTED TO SCILAB" "MANCA LA CONNESSIONE A SCILAB"


# textarea.tcl
::msgcat::mcset it "Font size" "Dimensione carattere"


# whichfun.tcl
::msgcat::mcset it "Being at line" "Essendo a linea"
::msgcat::mcset it ", function" ", la funzione"
::msgcat::mcset it "begins at" "comincia a"
::msgcat::mcset it ", and there are" ", e ci sono"
::msgcat::mcset it "continued lines, i.e. we are at line"\
                   "linee continuate, cioè siamo alla linea"
::msgcat::mcset it "of" "di"
::msgcat::mcset it "Function" "Funzione"
::msgcat::mcset it "line" "linea"
::msgcat::mcset it "The cursor is not currently inside a function body."\
                   "Il cursore non si trova dentro il corpo di una funzione"
::msgcat::mcset it "Which function?" "Quale funzione?"


#preference variables description
::msgcat::mcset it "BGCOLOR" "Sfondo"
::msgcat::mcset it "FGCOLOR" "Testo semplice"
::msgcat::mcset it "CURCOLOR" "Cursore"
::msgcat::mcset it "PARCOLOR" "Parentesi tonde ()"
::msgcat::mcset it "BRAKCOLOR" "Parentesi quadre \\\[\\\]"
::msgcat::mcset it "BRACCOLOR" "Parentesi graffe {}"
::msgcat::mcset it "PUNCOLOR" "Punteggiatura"
::msgcat::mcset it "INTFCOLOR" "Funzioni interne"
::msgcat::mcset it "COMMCOLOR" "Comandi Scilab"
::msgcat::mcset it "SCICCOLOR" "Funzioni Scicos"
::msgcat::mcset it "OPCOLOR" "Operatori"
::msgcat::mcset it "QTXTCOLOR" "Stringhe"
::msgcat::mcset it "REMCOLOR" "Commenti"
::msgcat::mcset it "XMLCOLOR" "Tags XML"
::msgcat::mcset it "NUMCOLOR" "Numeri"
::msgcat::mcset it "SELCOLOR" "Sfondo selezione"
::msgcat::mcset it "BREAKPOINTCOLOR" "Punti d'arresto"
::msgcat::mcset it "LFUNCOLOR" "Funzioni di libreria"
::msgcat::mcset it "PDEFCOLOR" "Variabili predefinite"
::msgcat::mcset it "FOUNDTEXTCOLOR" "Testo trovato"
::msgcat::mcset it "CONTLINECOLOR" "Linea incompleta"
::msgcat::mcset it "USERFUNCOLOR" "Funzioni utente"


#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset it "&Directory:" "&Cartella"
::msgcat::mcset it "File &name:" "&Nome del file:"
::msgcat::mcset it "Files of &type:" "&Tipo di file:"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
::msgcat::mcset it "&Open" "A&pri"
::msgcat::mcset it "Open" "Apri"
::msgcat::mcset it "Save As" "Salva come"
::msgcat::mcset it "&Cancel" "&Annulla"
::msgcat::mcset it "&Selection:" "&Selezione:"
::msgcat::mcset it "&Yes" "&Si"
::msgcat::mcset it "&No" "&No"
::msgcat::mcset it "&Red" "&Rosso"
::msgcat::mcset it "&Green" "&Verde"
::msgcat::mcset it "&Blue" "&Blu"
::msgcat::mcset it "Directory \"%1\$s\" does not exist."\
                   "La cartella \"%1\$s\" non esiste."
