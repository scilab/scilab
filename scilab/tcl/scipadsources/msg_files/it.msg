# Scipad texts in Italian
# in sync at least with v4.15


# commons
::msgcat::mcset it "Cancel" "Annulla"
::msgcat::mcset it "Cance&l" "Annu&lla"
::msgcat::mcset it "OK" "OK"
::msgcat::mcset it "Scilab working" "Scilab occupato"
::msgcat::mcset it "All files" "Tutti i files"
::msgcat::mcset it "Add/Change" "Aggiungi/modifica"
::msgcat::mcset it "Untitled" "SenzaTitolo"


# buffernavigation.tcl
::msgcat::mcset it "Goto Line?" "Vai alla linea?"
::msgcat::mcset it "Goto Line: " "Vai alla linea: "


# colorize.tcl
::msgcat::mcset it "Wait seconds while recolorizing file" \
       "Attendere, ricolorazione in corso"


# db_argsmanagement.tcl
::msgcat::mcset it "Variable:" "Variabile:"
::msgcat::mcset it "Value:" "Valore:"
::msgcat::mcset it "Get current value from Scilab" \
                    "Recupera il valore attuale in Scilab"

# db_configureexec.tcl
::msgcat::mcset it "Configure execution" "Configura l'esecuzione"
::msgcat::mcset it "Function name:" "Nome della funzione:"
::msgcat::mcset it "Obtain" "Ottieni"
::msgcat::mcset it "Input arguments:" "Argumenti di ingresso:"
::msgcat::mcset it "Remove" "Rimuovi"
::msgcat::mcset it "Check function definition: it must be on a single code line." \
                   "Controllare la definizione della funzione: deve\
                    essere data su un unica linea"
::msgcat::mcset it "Function name or input arguments do not match definition of the function" \
                    "Il nome della funzione o i suoi argomenti non corrispondono alla definizione della funzione"
    ::msgcat::mcset it "in the file!\n\nCheck function\
       name and arguments (names, order) in the configuration dialog.\
       \nArguments order can be changed using drag and drop with\
       right mouse button in the arguments listbox." \
      "nel file!\n\n\
        Verificare il nome della funzione e gli argomenti (nome, ordine\
        di comparsa) nella casella del dialogo di configurazione.\n\
        L'ordine degli argomenti può essere modificato trascinando e deponendo\
        col mouse."
::msgcat::mcset it "Error in selected function name or arguments" \
        "Errore nella funzione selezionata o nei suoi argomenti"
::msgcat::mcset it "Debugger is not yet developed for sce scripts!"\
         "Il debugger non funziona, per ora, su programmi *.sce!"


# db_debugsession.tcl
::msgcat::mcset it "Scilab is working, wait for the prompt to issue a debugger command." \
   "Scilab è occupato, aspettare il prompt per eseguire comandi di debug."
::msgcat::mcset it "Cursor position is out of reach!" \
     "La posizione del cursore è irraggiungibile"
::msgcat::mcset it "Cursor must be in a function" \
     "Il cursore deve essere dentro una funzione"
::msgcat::mcset it "No effect - The debugged file is not stuck" \
     "Senza effetto - il file in debug non è piantato"
::msgcat::mcset it "Scilab execution error" \
    "Errore di Scilab durante l'esecuzione"
::msgcat::mcset it "The shell reported an error while trying to execute " \
   "Lo shell ha riportato un errore nel cercare di eseguire"
    ::msgcat::mcset it " at line " " alla linea "
    ::msgcat::mcset it " of " " di "
::msgcat::mcset it "Execution aborted!" "Esecuzione annullata!"


# db_init.tcl
::msgcat::mcset it "Please wait..." "Attendere..."


# db_states.tcl
::msgcat::mcset it "Currently no debug session" \
      "Nessuna sessione di debug in corso"
::msgcat::mcset it "Ready to start debug" "Pronto per il debug"
::msgcat::mcset it "Debug in progress" "Debug in corso"


# db_watch.tcl
::msgcat::mcset it "Watch" "Controllo esecuzione e variabili"
::msgcat::mcset it "Watch variables:" "Variabili osservate:"
::msgcat::mcset it "Remove" "Rimuovi"
::msgcat::mcset it "Call stack:" "stack di programma:"
::msgcat::mcset it "Close" "Chiudi"
::msgcat::mcset it "Show watch variables" "Mostra le variabili"
::msgcat::mcset it "Hide watch variables" "Nascondi le variabili"
::msgcat::mcset it "Show call stack" "Mostra lo stack"
::msgcat::mcset it "Hide call stack" "Nascondi lo stack"


# filecommands.tcl
::msgcat::mcset it "New Script" "Nuovo testo"
::msgcat::mcset it "The contents of" "Il contenuto di"
   ::msgcat::mcset it "may have changed, do you wish to save your changes?"\
            "potrebbe essere cambiato, registrare le modifiche?"
::msgcat::mcset it "Save Confirm?" "Conferma salvataggio?"
::msgcat::mcset it "Weird drag&drop" "Strano trascina e deponi"
::msgcat::mcset it "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                   "non è un nome valido per un file. O è stato trascinato\
                    un oggetto di tipo sbagliato, o il meccanismo di d&d\
                    ha un baco." 
::msgcat::mcset it "File not found" "File non trovato"
::msgcat::mcset it "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "non esiste più. Creare un nuovo file collo\
                    stesso nome?"
::msgcat::mcset it "Open file" "Aprire file"
::msgcat::mcset it "The file" "Il file"
    ::msgcat::mcset it "is already opened! Save the current opened file\
          to another name and reopen it from disk!" \
 "è già aperto! Salvarlo eventualmente con un altro nome e riaprirlo dal disco"
::msgcat::mcset it "File" "File"
    ::msgcat::mcset it "saved" "salvato"
::msgcat::mcset it "cannot be written!" "non è scrivibile!"
::msgcat::mcset it "The contents of " "Il contenuto di"
    ::msgcat::mcset it "has changed on disk, save it anyway?" \
       "è cambiato sul disco, salvare comunque?"
::msgcat::mcset it "File has changed!" "Il file è cambiato"
::msgcat::mcset it "Save as" "Registra come"
::msgcat::mcset it "Scilab files" "Files Scilab"
::msgcat::mcset it "XML files" "Files XML"
::msgcat::mcset it "Wait seconds while loading and colorizing file" \
          "Attendere - apertura e colorazione del file in corso"
::msgcat::mcset it "The contents of this file may have changed,\
                     do you wish to save your changes?" \
                   "Il contenuto di questo file potrebbe essere cambiato,\
                     registrare la nuova versione?"
::msgcat::mcset it "Revert" "Ritornare alla versione di"
    ::msgcat::mcset it "to saved?" "salvata sul disco?"
::msgcat::mcset it "Revert Confirm?" "Conferma recupero?"


# findreplace.tcl
::msgcat::mcset it "The string" "La stringa"
    ::msgcat::mcset it "could not be found" "non è stata trovata"
::msgcat::mcset it "replacements done" "sostituzioni effettuate"
::msgcat::mcset it "Find" "Cerca"
::msgcat::mcset it "No match found in the selection for" \
         "Nessuna corrispondenza trovata all'interno della selezione."
 ::msgcat::mcset it "\nWould you like to look for it in the entire text?" \
                   "Continuare la ricerca nel resto del testo?"
::msgcat::mcset it "You are searching for an empty string!" \
          "La stringa da cercare è vuota!"
::msgcat::mcset it "Re&place" "Rim&piazza"
::msgcat::mcset it "Find what:" "Cerca:"
::msgcat::mcset it "Replace with:" "Cambia in:"
::msgcat::mcset it "Find &Next" "Trova il segue&nte"
::msgcat::mcset it "Replace &All" "Rimpi&azza tutti"
::msgcat::mcset it "&Upwards" "Verso l'alto"
::msgcat::mcset it "Downward&s" "Verso il ba&sso" 
::msgcat::mcset it "Match &case" "Rispetta maius&cole"
::msgcat::mcset it "&Regular expression" "Espressione &regolare (regexp)"


# helps.tcl
::msgcat::mcset it "About" "A proposito"
::msgcat::mcset it aboutme_message \
                   "\n Originato da tknotepad di Joseph Acosta, joeja@mindspring.com.\n\
                    Modificato dal Consorzio Scilab.\n\
                    Migliorato da Enrico Segre 2003-2005\n\
                    e da François Vogel (in particolare il debugger) 2004-2005.\n\
                    Traduzioni:\
                    \n   svedese, Gunnar Lindholm 2005;\
                    \n   norvegese, Torbjørn Pettersen 2005;\
                    \n   tedesco, Dirk Reusch 2005."


# infomessages.tcl
::msgcat::mcset it "Line:" "Linea:"
    ::msgcat::mcset it "Column:" "Colonna:"
::msgcat::mcset it "Line" "Linea"
    ::msgcat::mcset it "in" "di"
::msgcat::mcset it " \[ReadOnly\]" " \[SolaLettura\]"
::msgcat::mcset it " (modified)" " (modificato)"


# inputtext.tcl
::msgcat::mcset it "No corresponding <" "Nessun <"
    ::msgcat::mcset it "> found!" "> corrispondente trovato!"
::msgcat::mcset it "Code editing is not allowed during debug!" \
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
    ::msgcat::mcset it "&Save" "&Salva"
    ::msgcat::mcset it "Save &as..." "Salva &come..."
    ::msgcat::mcset it "&Revert..." "&Recupera..."
    ::msgcat::mcset it "Import &Matlab file..."  "Importa file &Matlab..."
    ::msgcat::mcset it "Create help s&keleton..." \
           "Crea scheletro di aiuto &XML..."
    ::msgcat::mcset it "Compile as &help page" "Compila come file di ai&uto"
    ::msgcat::mcset it "Open &function source" \
           "Apri la sorgente della &funzione"
    ::msgcat::mcset it "Print Se&tup" "Confi&gura stampante"
    ::msgcat::mcset it "&Print" "S&tampa"
    ::msgcat::mcset it "&Close" "C&hiudi"
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
    ::msgcat::mcset it "Find &Next" "Trova il &prossimo"
    ::msgcat::mcset it "&Replace..." "&Sostituisci..."
    ::msgcat::mcset it "&Goto Line..." "&Vai a linea..."

    # menues.tcl - Exec menu
    ::msgcat::mcset it "&Load into Scilab" "&Carica in Scilab"
    ::msgcat::mcset it "&Evaluate selection" "&Esegui la selezione"

    # menues.tcl - Debug menu
    ::msgcat::mcset it "&Insert/Remove breakpoint" \
           "&Inserisci/rimuovi un arresto"
    ::msgcat::mcset it "Remove &all breakpoints" "Rimuovi &tutti gli arresti"
    ::msgcat::mcset it "&Configure execution..." "&Configura l'esecuzione..."
    ::msgcat::mcset it "Go to next b&reakpoint" "&Vai fino al prossimo arresto"
    ::msgcat::mcset it "&Step by step" "&Passo a passo"
    ::msgcat::mcset it "Step &into" "Entra &dentro"
    ::msgcat::mcset it "Step o&ver" "Passa sop&ra"
    ::msgcat::mcset it "Step &out" "Esci &fuori"
    ::msgcat::mcset it "Run to c&ursor" "Esegui fino al curso&re"
    ::msgcat::mcset it "G&o on ignoring any breakpoint" \
            "Co&ntinua ignorando gli arresti"
    ::msgcat::mcset it "Show &watch" "Finestra &di controllo"
    ::msgcat::mcset it "&Break" "&Stop"
    ::msgcat::mcset it "Cance&l debug" "&Annulla il debug"

    # menues.tcl - Scheme menu
    ::msgcat::mcset it "S&cilab" "&Scilab"
    ::msgcat::mcset it "&XML" "&XML"
    ::msgcat::mcset it "&none" "&nessuno"

    # menues.tcl - Options menu
    ::msgcat::mcset it "&Font size" "&Dimensione carattere"
        ::msgcat::mcset it "&micro" "&micro"
        ::msgcat::mcset it "&small" "&piccolo"
        ::msgcat::mcset it "m&edium" "m&edio"
        ::msgcat::mcset it "&large" "&grande"
    ::msgcat::mcset it "&Colors" "&Colori"
    ::msgcat::mcset it "&Tabs and indentation" "&Tabulatori e indentazione"
        ::msgcat::mcset it "Tab inserts &spaces" "&Tab inserisce spazi"
        ::msgcat::mcset it "&Indentation spaces" "&Spazi di indentazione"
    ::msgcat::mcset it "Colorize \'&strings\'" "Colora \'&stringhe\'"
    ::msgcat::mcset it "Word &Wrap" "&Ritorno a capo visuale"
    ::msgcat::mcset it "File&names" "Nome dei &files"
        ::msgcat::mcset it "&Full path" "&Percorso completo"
        ::msgcat::mcset it "Full path if &ambiguous"\
                           "Percorso completo se &ambiguo"
        ::msgcat::mcset it "&Unambiguous pruned path" \
                           "Percorso &ridotto univoco"
    ::msgcat::mcset it "&Recent files" "&Numero files recenti"

    # menues.tcl - Windows menu
    ::msgcat::mcset it "Opened Files" "Files aperti"

    # menues.tcl - Help menu
    ::msgcat::mcset it "&Help..." "&Aiuto..."
    ::msgcat::mcset it "&What's?..." "&Cos'è?..."
    ::msgcat::mcset it "&About" "A &proposito"


# popupmenus.tcl
::msgcat::mcset it "Open the source of" "Apri la sorgente di"
::msgcat::mcset it "AddWatch" "Tieni %s sotto controllo"


# print.tcl
::msgcat::mcset it "Print Setup" "Configura stampante"
::msgcat::mcset it "Print Command: " "Comando di stampa:"


# scilabexec.tcl
::msgcat::mcset it "No point in loading an empty file!" "Inutile caricare un file vuoto!"
::msgcat::mcset it "Scilab is working, wait for the prompt to load file" \
      "Scilab è occupato, attendere il prompt per caricare il file"
::msgcat::mcset it "Exec done" "Exec terminato"
::msgcat::mcset it "Scilab is working, wait for the prompt to execute the selection." \
    "Scilab è occupato, attendere il prompt per eseguire la selezione"
::msgcat::mcset it "Scilab is working, wait for the prompt to convert a Matlab file." \
    "Scilab è occupato, attendere il prompt per convertire un file Matlab"
::msgcat::mcset it "Matlab files" "Files Matlab"
::msgcat::mcset it "Matlab file to convert" "File Matlab da importare"
::msgcat::mcset it "Scilab is converting, please hold on..." \
    "Scilab sta traducendo, attendere..."
::msgcat::mcset it "Matlab file import" "Importazione di un file Matlab"
::msgcat::mcset it "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug/index.cgi" \
                   "La traduzione del file è fallita, vedere la finestra\
                    Scilab per dettagli -- Il baco può essere riportato a\
                    http://scilabsoft.inria.fr/bugzilla_bug/index.cgi \
                    includendo il file incriminato ed il messagggio di errore."
::msgcat::mcset it "Please select destination path for the xml source of the help file" \
   "Scegliere una cartella di destinazione per la sorgente XML della pagina di aiuto"
::msgcat::mcset it "Older version of" "Esiste già una versione di"
    ::msgcat::mcset it "found!" "!"
::msgcat::mcset it "An old version of" "Esiste una versione precedente di"
    ::msgcat::mcset it "already exists: open the old file instead?" \
          ": apro il file esistente invece?"
::msgcat::mcset it "Existing file" "File esistente"
::msgcat::mcset it "New skeleton" "Crea nuovo scheletro"
::msgcat::mcset it "Scilab is working, wait for the prompt to compile the help file." \
      "Scilab è occupato, attendere il prompt per compilare la pagina di aiuto"

# scipad.tcl
::msgcat::mcset it "NOT CONNECTED TO SCILAB" "MANCA LA CONNESSIONE A SCILAB"
 

# textarea.tcl
::msgcat::mcset it "Font size" "Dimensione carattere"


# whichfun.tcl
::msgcat::mcset it "Being at line" "Essendo a linea"
    ::msgcat::mcset it ", function" ", la funzione"
    ::msgcat::mcset it "begins at" "comincia a"
    ::msgcat::mcset it ", and there are" ", e ci sono"
    ::msgcat::mcset it "continued lines, i.e. we are at line" \
         "linee continuate, cioè siamo alla linea"
    ::msgcat::mcset it "of" "di"
::msgcat::mcset it "Function" "Funzione"
    ::msgcat::mcset it "line" "linea"
::msgcat::mcset it "The cursor is not currently inside a function body." \
     "Il cursore non si trova dentro il corpo di una funzione"
::msgcat::mcset it "Which function?" "Quale funzione?"


#preference variables description
::msgcat::mcset it "BGCOLOR" "Sfondo"
::msgcat::mcset it "FGCOLOR" "Testo semplice"
::msgcat::mcset it "CURCOLOR" "Cursore"
::msgcat::mcset it "PARCOLOR" "Parentesi tonde ()"
::msgcat::mcset it "BRAKCOLOR" "Parentesi quadre \\\[\\\]"
::msgcat::mcset it "BRACCOLOR" "Parentesi graffe {}"
::msgcat::mcset it "PUNCOLOR"  "Punteggiatura"
::msgcat::mcset it "KEYWCOLOR" "Funzioni interne Scilab"
::msgcat::mcset it "OPCOLOR"   "Operatori"
::msgcat::mcset it "QTXTCOLOR"  "Stringhe"
::msgcat::mcset it "REMCOLOR" "Commenti"
::msgcat::mcset it "XMLCOLOR" "Tags XML"
::msgcat::mcset it "NUMCOLOR" "Numeri"
::msgcat::mcset it "SELCOLOR" "Sfondo selezione"
::msgcat::mcset it "BREAKPOINTCOLOR" "Punti d'arresto"
::msgcat::mcset it "LFUNCOLOR" "Funzioni di libreria"
::msgcat::mcset it "PDEFCOLOR" "Variabili predefinite"
::msgcat::mcset it "FOUNDTEXTCOLOR" "Testo trovato"

#overriding tcl-core dialogs (many are actually already found in 
#  TKDIR/msgs/it.msg)
::msgcat::mcset it "&Directory:" "&Cartella"
::msgcat::mcset it "File &name:" "&Nome del file:"
::msgcat::mcset it "Files of &type:" "&Tipo di file:"
#::msgcat::mcset it "&Save" "&Salva" # already included
::msgcat::mcset it "Save As" "Salva come"
::msgcat::mcset it "&Open" "A&pri"
::msgcat::mcset it "Open" "Apri"
::msgcat::mcset it "&Cancel" "&Annulla"
::msgcat::mcset it "&Selection:" "&Selezione:"
::msgcat::mcset it "&Yes" "&Si"
::msgcat::mcset it "&No" "&No"
::msgcat::mcset it "&Blue" "&Blu"
::msgcat::mcset it "&Green" "&Verde"
::msgcat::mcset it "&Red" "&Rosso"
::msgcat::mcset it "Directory \"%1\$s\" does not exist."\
        "La cartella \"%1\$s\" non esiste."
 
