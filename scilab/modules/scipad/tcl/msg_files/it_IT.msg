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

# Scipad texts in Italian by Enrico Segre
# in sync at least with v6.149


# commons
::msgcat::mcset it_IT "Cancel" "Annulla"
::msgcat::mcset it_IT "Cance&l" "Annu&lla"
::msgcat::mcset it_IT "OK" "OK"
::msgcat::mcset it_IT "All files" "Tutti i files"
::msgcat::mcset it_IT "Add/Change" "Aggiungi/modifica"
::msgcat::mcset it_IT "Add/Chan&ge" "Aggiungi/&modifica"
::msgcat::mcset it_IT "&Remove" "&Rimuovi"
::msgcat::mcset it_IT "Rem&ove" "Rimu&ovi"
::msgcat::mcset it_IT "Untitled" "SenzaTitolo"
::msgcat::mcset it_IT "Close" "Chiudi"
::msgcat::mcset it_IT "&Close" "&Chiudi"
::msgcat::mcset it_IT "Error " "Errore "
::msgcat::mcset it_IT "The file" "Il file"
::msgcat::mcset it_IT "Function" "Funzione"
::msgcat::mcset it_IT "Line" "Linea"


# buffernavigation.tcl
::msgcat::mcset it_IT "Hide" "Nascondi"
::msgcat::mcset it_IT "Close this tile, keep content"\
                   "Chiudi il pannello, lascia il file aperto"
::msgcat::mcset it_IT "Close this tile" "Chiudi questo pannello"
::msgcat::mcset it_IT "Goto Line?" "Vai alla linea?"
::msgcat::mcset it_IT "Go to:" "Vai a:"
::msgcat::mcset it_IT "&logical line" "linea &logica"
::msgcat::mcset it_IT "&physical line" "linea fi&sica"
::msgcat::mcset it_IT "in &function" "della &funzione"
::msgcat::mcset it_IT "in &current file" "del file &corrente"
::msgcat::mcset it_IT "Outside of function definition"\
                   "fuori dalla definizione della funzione"


# colorize.tcl
::msgcat::mcset it_IT "Wait seconds while recolorizing file"\
                   "Attendere, ricolorazione in corso"
::msgcat::mcset it_IT "You can't do that while colorization is in progress.\
                    Please try again when finished."\
                   "Non si può fare questo prima che la colorazione sia\
                    completata. Aspettare il termine della colorazione e\
                    riprovare"
::msgcat::mcset it_IT "Command forbidden during colorization"\
                   "Comando non consentito mentre la colorazione è in corso"


# completion.tcl
::msgcat::mcset it_IT "No possible completion" "Nessun completamento possibile"
::msgcat::mcset it_IT "possible completions" "completamenti possibili"
::msgcat::mcset it_IT "Possible completions" "Completamenti possibili"


# db_argsmanagement.tcl
::msgcat::mcset it_IT "Add" "Aggiungi"
::msgcat::mcset it_IT "This variable can be watched but cannot be edited!"\
                   "Questa variabile può essere osservata, ma non modificata!"
::msgcat::mcset it_IT "A new watch variable will be created."\
                   "Verrà piuttosto proposta una nuova variabile da\
                    osservare."
::msgcat::mcset it_IT "Non editable variable" "Variabile non modificabile"
::msgcat::mcset it_IT "Variable:" "Variabile:"
::msgcat::mcset it_IT "Expression:" "Espressione:"
::msgcat::mcset it_IT "Value:" "Valore:"
::msgcat::mcset it_IT "&Get current value from Scilab"\
                   "Recupera il valore &attuale in Scilab"
::msgcat::mcset it_IT "Non watchable variable" "Variabile non osservabile"
::msgcat::mcset it_IT "Sorry, variable" "Spiacente, la variabile"
::msgcat::mcset it_IT "is special and cannot be watched."\
                   "è di un tipo che non può essere osservato."


# db_bptsgui.tcl
::msgcat::mcset it_IT "Breakpoints properties"\
                   "Proprietà dei punti d'arresto"
::msgcat::mcset it_IT "Enabled" "Abilita"
::msgcat::mcset it_IT "Expression" "Espressione"
::msgcat::mcset it_IT "Hit if" "Marca se"
::msgcat::mcset it_IT "istrue" "è vero"
::msgcat::mcset it_IT "haschanged" "cambia"
::msgcat::mcset it_IT "Watched" "Osservato"
::msgcat::mcset it_IT "Hit count" "n° accessi"
::msgcat::mcset it_IT "Break if" "Arresta se"
::msgcat::mcset it_IT "Threshold" "Soglia"
::msgcat::mcset it_IT "always" "sempre"
::msgcat::mcset it_IT "equalto" "è uguale a"
::msgcat::mcset it_IT "multipleof" "è un multiplo di"
::msgcat::mcset it_IT "greaterthan" "è più grande di"
::msgcat::mcset it_IT "lessthan" "è più piccolo di"
::msgcat::mcset it_IT "Reset hit" "azzera accessi"
::msgcat::mcset it_IT "Breakpoints vocabulary"\
                   "Vocabolario dei punti d'arresto"
::msgcat::mcset it_IT "A breakpoint is identified by its location, i.e. its\
                    position (a line number) in a function."\
                   "Un punto d'arresto è identificato dalla sua posizione,\
                    cioè dal suo numero di linea nel corpo della funzione."
::msgcat::mcset it_IT "When the execution process encounters an enabled\
                    breakpoint, then this breakpoint is said to be\
                    reached."\
                   "Quando l'esecuzione del programma incontra un punto d'arresto\
                    abilitato, si dice che il punto d'arresto è stato raggiunto."
::msgcat::mcset it_IT "When the breakpoint is reached and its associated\
                    conditional expression is true (or changed), then the\
                    breakpoint is said to be hit."\
                   "Quando un punto d'arresto è raggiunto e l'espressione\
                    condizionale associata è vera (o cambia),\
                    allora il punto d'arresto si dice \"acceduto\"."
::msgcat::mcset it_IT "The hit count is the number of times the breakpoint has\
                    been hit."\
                   "Il numero di accessi è il numero di volte che\
                    questo avviene."
::msgcat::mcset it_IT "Execution will stop at a breakpoint if the hit count\
                    satisfies the selectable break condition."\
                   "L'esecuzione si fermerà ad un punto d'arresto\
                    se il numero di accessi soddisfa la condizione prescelta."


# db_bptsmanagement.tcl
::msgcat::mcset it_IT "No breakpoint here!" "Non si può mettere un arresto qui!"
::msgcat::mcset it_IT "Delete all breakpoints"\
                   "Cancella tutti i punti d'arresto"
::msgcat::mcset it_IT "Are you sure you want to delete all the existing\
                    breakpoints?"\
                   "Sicuro di voler cancellare tutti i punti d'arresto_"
::msgcat::mcset it_IT "All breakpoints deleted" "Tutti gli arresti eliminati"
::msgcat::mcset it_IT "No action taken" "Nessun cambiamento effettuato"
::msgcat::mcset it_IT "No breakpoint!" "Nessun punto d'arresto!"


# db_configureexec.tcl
::msgcat::mcset it_IT "Configure execution" "Configura l'esecuzione"
::msgcat::mcset it_IT "Function name:" "Nome della funzione:"
::msgcat::mcset it_IT "&Obtain" "&Ottieni"
::msgcat::mcset it_IT "Input arguments:" "Argumenti di ingresso:"
::msgcat::mcset it_IT "The selected function has not been found in the opened\
                    files, maybe it was deleted since last configuration of\
                    the debugger.\nUse button \""\
                   "la funzione selezionata non compare in nessuno dei files\
                    aperti, potrebbe essere stata cancellata dall'ultima\
                    volta che il debugger è stato configurato. Usare il\
                    tasto \""
::msgcat::mcset it_IT "\" to refresh the list of available functions."\
                   "\" per rinfrescare la lista delle funzioni disponibili"
::msgcat::mcset it_IT "Function name or input arguments do not match definition\
                    of the function"\
                   "Il nome della funzione o i suoi argomenti non\
                    corrispondono alla definizione della funzione"
::msgcat::mcset it_IT "in the file!\n\nCheck function name and arguments\
                    (names, order) in the configuration dialog. \nArguments\
                    order can be changed using drag and drop with right\
                    mouse button in the arguments listbox."\
                   "nel file!\n\n Verificare il nome della funzione e gli\
                    argomenti (nome, ordine di comparsa) nella casella del\
                    dialogo di configurazione.\n L'ordine degli argomenti\
                    può essere modificato trascinando e deponendo col\
                    mouse."
::msgcat::mcset it_IT "Error in selected function name or arguments"\
                   "Errore nella funzione selezionata o nei suoi argomenti"
::msgcat::mcset it_IT "At least one input argument does not match definition of\
                    the function"\
                   "Uno o più argomenti non corrispondono alla definizione\
                    della funzione"
::msgcat::mcset it_IT "in the file!\n\nCheck function arguments (their names)\
                    in the configuration dialog."\
                   "nel file!\n\nVerificare il nome della funzione e gli\
                    argomenti (nome, ordine di comparsa) nel dialogo di\
                    configurazione."
::msgcat::mcset it_IT "Error in selected function arguments"\
                   "Errore negli argomenti della funzione selezionata"
::msgcat::mcset it_IT "Main level code found" "Codice a livello principale"
::msgcat::mcset it_IT "Scipad ancillary file" "File ancillario di Scipad"
::msgcat::mcset it_IT "The current file contains only Scipad ancillaries that\
                    cannot be debugged in Scipad."\
                   "Il file attuale contiene funzioni usate da Scipad, che\
                    non possono essere debuggate all'interno di Scipad."
::msgcat::mcset it_IT "Reserved function names are:\n"\
                   "I nomi di funzione riservati sono:\n"
::msgcat::mcset it_IT "See help scipad for further details."\
                   "Consultare l'aiuto di SciPad per maggiori dettagli."
::msgcat::mcset it_IT "This file contains main level code.\nDebug as a single\
                    .sce file?"\
                   "Questo file contiene codice a livello principale.\
                    Eseguire il debug trattando tutto il file come un unica\
                    funzione?"
::msgcat::mcset it_IT "No function found" "Nessuna funzione trovata"
::msgcat::mcset it_IT "This file contains no function and will be treated"\
                   "Questo file non contiene alcuna funzione e sarà trattato"
::msgcat::mcset it_IT "as a single .sce file.\nStart debugging?"\
                   "Come un unico file .sce. Cominciare il debug?"
::msgcat::mcset it_IT "You must enable colorization to be able to launch the\
                    debugger!"\
                   "Occorre attivare la colorazione per potere usare il\
                    debugger!"
::msgcat::mcset it_IT "The debugger needs colorization"\
                   "Il debugger ha bisogno della colorazione"
::msgcat::mcset it_IT "Warning!\n\n  A function missing the closing\
                    \"endfunction\" has been found in the currently opened\
                    files.\n  Such syntax is now obsolete.\n\n  Please\
                    terminate function "\
                   "Attenzione!\n E' stata trovata una funzione non\
                    terminata dalla parola chiave \"endfunction\" in uno dei\
                    files correntemente aperti. Questa sintassi è\
                    obsoleta.\n\n Terminare la funzione "
::msgcat::mcset it_IT " and try again." " con \"endfunction\" e riprovare."
::msgcat::mcset it_IT "Note: This message might also be triggered by a string\n \
                    quoted with single quotes and containing the word\
                    \"function\",\n  when \"Colorize \'strings\'\" is\
                    unchecked in the Options menu."\
                   "Nota: questo messaggio può anche essere dovuto ad una\
                    stringa contenente la parola \"function\" delimitata da\
                    apostrofi invece che da virgolette, se l'opzione\
                    \"Colora \'stringhe\' non è selezionata nel menu\
                    Opzioni."
::msgcat::mcset it_IT "This known bug can be worked around simply by checking\
                    \"Colorize \'&strings\'\"."\
                   "Questo baco, noto, può essere evitato semplicemente\
                    selezionando l\'opzione suddetta."
::msgcat::mcset it_IT "Unterminated function definition found"\
                   "Trovata funzione non terminata"
::msgcat::mcset it_IT "Warning!\n\n  One or more function is defined more than\
                    once in the currently opened files.\n  The debugger\
                    cannot cope with multiple copies of functions.\n  The\
                    following duplicates were detected:\n"\
                   "Attenzione! Una o più funzioni sono definite più volte\
                    nei files aperti. Il debugger non è in grado di\
                    scegliere la definizione pertinente e non può\
                    procedere. Sono stati rilevati i seguenti doppioni:"
::msgcat::mcset it_IT "\n--> Definition of function "\
                   "\n--> Definizione della funzione "
::msgcat::mcset it_IT " found in:\n" " trovata in:\n"
::msgcat::mcset it_IT "Duplicate function definitions found"\
                   "Doppie definizioni di funzioni trovate"


# db_debugsession.tcl
::msgcat::mcset it_IT "Too many breakpointed functions"\
                   "Troppe funzioni con punti di arresto"
::msgcat::mcset it_IT "You have currently" "Ci sono al momento"
::msgcat::mcset it_IT "functions in your opened files." "funzioni."
::msgcat::mcset it_IT "Scilab supports a maximum of" "Scilab supporta al massimo"
::msgcat::mcset it_IT "possible breakpointed functions (see help setbpt)."\
                   "funzioni con punti d'arresto (vedi help setbpt)."
::msgcat::mcset it_IT "Step-by-step hence cannot be performed."\
                   "Il modo passo-passo non è pertanto possible."
::msgcat::mcset it_IT "This command will actually run to the next breakpoint."\
                   "Questo comando andrà solo fino all'arresto successivo"
::msgcat::mcset it_IT "Break hence cannot be performed."\
                   "Il comando Stop non può essere eseguito."
::msgcat::mcset it_IT "Too many breakpoints" "Troppi punti d'arresto"
::msgcat::mcset it_IT "Executing this command would require to set"\
                   "Eseguire questo comando richiederebbe di inserire"
::msgcat::mcset it_IT "breakpoints in your opened files."\
                   "punti d'arresto nei files aperti."
::msgcat::mcset it_IT "possible breakpoints in Scilab (see help setbpt)."\
                   "possibili punti d'arresto (vedere help setbpt)."
::msgcat::mcset it_IT "Cursor must be in a function"\
                   "Il cursore deve essere dentro una funzione"
::msgcat::mcset it_IT "This command cannot succeed if the function where the\
                    debug just stopped is not opened in Scipad!"\
                   "Questo comando non può essere eseguito se la funzione\
                    in cui il debugger si è fermato non è aperta in\
                    Scipad!"
::msgcat::mcset it_IT "Cannot run to return point"\
                   "Impossibile eseguire fino al punto di ritorno"
::msgcat::mcset it_IT "Warning!\nThe line where the debugger just stopped\
                    contains a return statement (either endfunction, return\
                    or resume) but this statement is not the first one on\
                    the line."\
                   "Attenzione!\nLa linea su cui il debugger si è fermato\
                    contiene una istruzione di ritorno (endfunction, return\
                    o resume) che non è la prima della linea."
::msgcat::mcset it_IT "As a consequence the debugger cannot predict if the next\
                    debug step will really jump outside of the current\
                    function, which is what you just asked the debugger to\
                    do."\
                   "Il debugger non può prevedere se il prossimo passo di\
                    debug provocherà davvero l'uscita (come richiesto) o\
                    meno."
::msgcat::mcset it_IT "To avoid this unresolvable case, it is recommended to\
                    place the return statement alone on its line."\
                   "Per evitare casi non risolubili (per il debugger) come\
                    questo è raccomandato di piazzare l'istruzione di\
                    ritorno isolata sulla sua linea."
::msgcat::mcset it_IT "Do you want to see this warning again during this Scipad\
                    session?"\
                   "Vuoi vedere ancora questo messaggio durante questa\
                    sessione di Scipad?"
::msgcat::mcset it_IT "Unresolvable case found while running to return point"\
                   "Caso irrisolubile incontrato durante \"esegui fino al\
                    punto di ritorno\""
::msgcat::mcset it_IT "No effect - The debugged file is not stuck"\
                   "Senza effetto - il file in debug non è piantato"


# db_init.tcl
::msgcat::mcset it_IT "Please wait..." "Attendere..."


# db_states.tcl
::msgcat::mcset it_IT "Currently no debug session"\
                   "Nessuna sessione di debug in corso"
::msgcat::mcset it_IT "Ready to start debug" "Pronto per il debug"
::msgcat::mcset it_IT "Debug in progress" "Debug in corso"
::msgcat::mcset it_IT "Scilab is back at the main level now."\
                   "Scilab è tornato al livello principale."


# db_watch.tcl
::msgcat::mcset it_IT "Watch" "Controllo esecuzione e variabili"
::msgcat::mcset it_IT "Watch variables:" "Variabili osservate:"
::msgcat::mcset it_IT "A&uto (local variables)"\
                   "&Auto (tutte le variabili locali)"
::msgcat::mcset it_IT "Au&to watch globals too"\
                   "Anche variabili &globali usate dalla funzione"
::msgcat::mcset it_IT "Generic expressions:" "Espressioni generiche:"
::msgcat::mcset it_IT "Call stack:" "stack di programma:"
::msgcat::mcset it_IT "Always on to&p" "Sempre in &primo piano"
::msgcat::mcset it_IT "Hide watch &variables" "Nascondi le &variabili"
::msgcat::mcset it_IT "Hide e&xpressions" "Nascondi le &espressioni"
::msgcat::mcset it_IT "Hide call &stack" "Nascondi lo &stack"
::msgcat::mcset it_IT "&Dock" "&Incassa"
::msgcat::mcset it_IT "&Add" "Agg&ungi"
::msgcat::mcset it_IT "&Filter errors" "&Filtra (non mostrare) gli errori"
::msgcat::mcset it_IT "Breakpoints" "Punti d'arresto"
::msgcat::mcset it_IT "Breakpointed functions" "Funzioni con arresti"
::msgcat::mcset it_IT "Scilab busy indicator"\
                   "indicatore per Scilab pronto/occupato"
::msgcat::mcset it_IT "Debugger state indicator"\
                   "indicatore dello stato del debugger"
::msgcat::mcset it_IT "Undefined elements are not legal in variable"\
                   "Gli elementi indefiniti sono illegali in questa variabile"
::msgcat::mcset it_IT ".\nThis variable will not be updated in Scilab."\
                   ".\nQuesta variabile non sarà aggiornata in Scilab"
::msgcat::mcset it_IT "Illegal undefined element found"\
                   "Trovato un elemento indefinito"
::msgcat::mcset it_IT "evaluates to:" "risulta in:"
::msgcat::mcset it_IT "produces error" "produce un errore"
::msgcat::mcset it_IT "Scipad cannot find source code for the selected\
                    function.\nPlease open the adequate file manually and\
                    try again."\
                   "Scipad non trova il codice sorgente della funzione\
                    selezionata.\nAprire manualmente il file corrispondente\
                    e riprovare."
::msgcat::mcset it_IT "Missing source code" "Codice sorgente mancante"


# filecommands.tcl
::msgcat::mcset it_IT "New script" "Nuovo testo"
::msgcat::mcset it_IT "New view on the same file"\
                   "Nuovo pannello per lo stesso file"
::msgcat::mcset it_IT "The contents of" "Il contenuto di"
::msgcat::mcset it_IT "may have changed, do you wish to save your changes?"\
                   "potrebbe essere cambiato, registrare le modifiche?"
::msgcat::mcset it_IT "Save Confirm?" "Conferma salvataggio?"
::msgcat::mcset it_IT "Open source of..." "Apri la sorgente di..."
::msgcat::mcset it_IT "Open source of:" "Apri la sorgente di:"
::msgcat::mcset it_IT "Weird drag&drop" "Strano trascina e deponi"
::msgcat::mcset it_IT "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or you\
                    hit a bug of the dnd mechanism."\
                   "non è un nome valido per un file. O è stato trascinato\
                    un oggetto di tipo sbagliato, o il meccanismo di d&d ha\
                    un baco."
::msgcat::mcset it_IT "File not found" "File non trovato"
::msgcat::mcset it_IT "does not exist anymore. Do you want to create an empty\
                    file with the same name?"\
                   "non esiste più. Creare un nuovo file collo stesso nome?"
::msgcat::mcset it_IT "Open file" "Aprire file"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset it_IT "is already opened! Save the current opened file to\
                    another name and reopen it from disk!"\
                   "è già aperto! Salvarlo eventualmente con un altro nome\
                    e riaprirlo dal disco"
::msgcat::mcset it_IT "File" "File"
::msgcat::mcset it_IT "saved" "salvato"
::msgcat::mcset it_IT "cannot be written!" "non è scrivibile!"
::msgcat::mcset it_IT "The contents of " "Il contenuto di"
::msgcat::mcset it_IT "has changed on disk, save it anyway?"\
                   "è cambiato sul disco, salvare comunque?"
::msgcat::mcset it_IT "has been modified outside of Scipad. Do you want to\
                    reload it?"\
                   "è stato cambiato fuori da Scipad. Ricaricarlo dal disco?"
::msgcat::mcset it_IT "File has changed!" "Il file è cambiato"
::msgcat::mcset it_IT "Save as" "Registra come"
::msgcat::mcset it_IT "Scilab files" "Files Scilab"
::msgcat::mcset it_IT "Scicos files" "Files Scicos"
::msgcat::mcset it_IT "XML files" "Files XML"
::msgcat::mcset it_IT "Wait seconds while loading and colorizing file"\
                   "Attendere - apertura e colorazione del file in corso"
::msgcat::mcset it_IT "The contents of this file may have changed, do you wish\
                    to save your changes?"\
                   "Il contenuto di questo file potrebbe essere cambiato,\
                    registrare la nuova versione?"
::msgcat::mcset it_IT "Revert" "Ritornare alla versione di"
::msgcat::mcset it_IT "to saved?" "salvata sul disco?"
::msgcat::mcset it_IT "Revert Confirm?" "Conferma recupero?"
::msgcat::mcset it_IT "Unreadable file" "File illeggibile!"
::msgcat::mcset it_IT "exists but is not readable!" "esiste, ma non è leggibile!"
::msgcat::mcset it_IT "This file is rather large and colorization might take\
                    some time.\n\nColorize anyway?"\
                   "Questo file è relativamente grosso, e la colorazione\
                    potrebbe richiedere tempo. Colorare comunque?"
::msgcat::mcset it_IT "File size warning" "Avvertimento: grosso file"
::msgcat::mcset it_IT "Windows shortcut file" "File scorciatoia Windows"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset it_IT "is a Windows shortcut and will not be opened!"\
                   "è una scorciatoia windows e non sarà aperto!"


# findreplace.tcl
::msgcat::mcset it_IT "Dialog box already open!"\
                   "Finestra di dialogo già aperta!"
::msgcat::mcset it_IT "The string" "La stringa \["
::msgcat::mcset it_IT "could not be found in" "\] non è stata trovata in"
::msgcat::mcset it_IT "could not be found in any of the opened files !"\
                   "\] non è stata trovata in nessuno dei files aperti!"
::msgcat::mcset it_IT "replacements done" "sostituzioni effettuate"
::msgcat::mcset it_IT "Find" "Cerca"
::msgcat::mcset it_IT "No match found in the selection for"\
                   "Nessuna occorrenza di \["
::msgcat::mcset it_IT "\nWould you like to look for it in the entire text?"\
                   "\] trovata all'interno della selezione. Continuare la\
                    ricerca nel resto del testo?"
::msgcat::mcset it_IT "You are searching for an empty string!"\
                   "La stringa da cercare è vuota!"
::msgcat::mcset it_IT "Re&place" "&Sostituisci"
::msgcat::mcset it_IT "Find what:" "Cerca:"
::msgcat::mcset it_IT "Replace with:" "Cambia in:"
::msgcat::mcset it_IT "Find &Next" "Trova il &seguente"
::msgcat::mcset it_IT "Replace &All" "Rimpiazza &tutti"
::msgcat::mcset it_IT "Direction" "Direzione"
::msgcat::mcset it_IT "&Upwards" "Verso l'&alto"
::msgcat::mcset it_IT "&Downwards" "Verso il &basso"
::msgcat::mcset it_IT "Text types" "Tipo di testo"
::msgcat::mcset it_IT "Find options" "Opzioni di ricerca"
::msgcat::mcset it_IT "Match &whole word only" "Solo parola &intera"
::msgcat::mcset it_IT "Match &case" "Rispetta &maiuscole"
::msgcat::mcset it_IT "&Regular expression" "Espressione &regolare (regexp)"
::msgcat::mcset it_IT "In all &opened files" "In tutti i &files aperti"
::msgcat::mcset it_IT "In &selection only" "Solo nella sele&zione"
::msgcat::mcset it_IT "In typed te&xt" "Nel testo di ti&po"
::msgcat::mcset it_IT "In a director&y" "In una &directory:"
::msgcat::mcset it_IT "Find in files search options"\
                   "Opzioni di ricerca nelle directories"
::msgcat::mcset it_IT "Directory r&ecurse search" "Ricerca ri&corsiva"
::msgcat::mcset it_IT "Include &hidden files" "Includere files &nascosti"
::msgcat::mcset it_IT "In files/file types:" "Nei files/tipi di files"
::msgcat::mcset it_IT "In directory:" "Nella directory:"
::msgcat::mcset it_IT "Search for file names" "Cercare tra i nomi dei files"
::msgcat::mcset it_IT "You have given an empty search string. Are you sure you\
                    want to search among file names?"\
                   "La stringa da cercare è vuota -- sicuri di volere\
                    cercare tra nomi di files?"
::msgcat::mcset it_IT "Wrapped around" "Continuato oltre la fine"
::msgcat::mcset it_IT "Back at the first file searched!"\
                   "Di nuovo al primo file esaminato!"
::msgcat::mcset it_IT "No more matches" "Nessuna altra corrispondenza"
::msgcat::mcset it_IT "The string to regexp search for cannot be compiled by\
                    Tcl.\nCheck again, maybe certain characters should be\
                    escaped."\
                   "La stringa da cercare come espressione regolare non ha\
                    potuto essere compilata. Controllare. Per esempio,\
                    alcuni caratteri speciali vanno preceduti da \\."
::msgcat::mcset it_IT "Any single character" "Un carattere qualsiasi"
::msgcat::mcset it_IT "Zero or more" "Zero o più"
::msgcat::mcset it_IT "One or more" "Uno o più"
::msgcat::mcset it_IT "Zero or one" "Zero o uno"
::msgcat::mcset it_IT "Any one character in the set"\
                   "Qualsiasi carattere dell'insieme"
::msgcat::mcset it_IT "Any one character not in the set"\
                   "Qualsiasi carattere non nell'insieme"
::msgcat::mcset it_IT "Or" "Oppure"
::msgcat::mcset it_IT "Beginning of line" "Inizio riga"
::msgcat::mcset it_IT "End of line" "Fine riga"
::msgcat::mcset it_IT "Beginning of word" "Principio di parola"
::msgcat::mcset it_IT "End of word" "Fine di parola"
::msgcat::mcset it_IT "A letter" "Lettera"
::msgcat::mcset it_IT "A decimal digit" "Cifra decimale"
::msgcat::mcset it_IT "An alphanumeric (letter or digit)"\
                   "Carattere alfanumerico (lettera o cifra)"
::msgcat::mcset it_IT "A space or tab character" "Spazio o tabulatore"
::msgcat::mcset it_IT "A rational number" "Numero razionale"
::msgcat::mcset it_IT "A floating point number" "Numero a virgola mobile"
::msgcat::mcset it_IT "A Scilab comment" "Commento Scilab"
::msgcat::mcset it_IT "A Scilab name" "Variabile o parola chiave Scilab"
::msgcat::mcset it_IT "A Scilab continued line (with dots)"\
                   "Linea Scilab continuata con .."


# findinfiles.tcl
::msgcat::mcset it_IT "Directory access issue" "Errore di accesso alla directory"
::msgcat::mcset it_IT "The directory you specified cannot be reached. The\
                    search won't bring any result."\
                   "Scipad non può accedere alla directory specificata. La\
                    ricerca non darà risultato"
::msgcat::mcset it_IT "Some directories or files could not be open and were\
                    ignored during search.\n You might miss read access to\
                    them.\n\n The following ones were ignored"\
                   "Certe directories o certi files non possono essere\
                    aperti (probabilmente sono protetti in lettura) e sono\
                    stati ignorati durante la ricerca. I seguenti sono stati\
                    ignorati:"
::msgcat::mcset it_IT "Ignored files" "Files ignorati"
::msgcat::mcset it_IT "Searching in file:" "Ricerca in:"
::msgcat::mcset it_IT "End of file search:" "Fine della ricerca:"
::msgcat::mcset it_IT "matches found" "corrispondenze trovate"
::msgcat::mcset it_IT "searched files" "files esaminati"
::msgcat::mcset it_IT "Constructing file list:"\
                   "Costruzione della lista dei files:"
::msgcat::mcset it_IT "&Previous" "&Precedente"
::msgcat::mcset it_IT "&Next" "&Successivo"
::msgcat::mcset it_IT "Scipad search results" "Risultati della ricerca"


# fonts.tcl
::msgcat::mcset it_IT "Choose font" "Scegliere tipo di carattere"
::msgcat::mcset it_IT "Family" "Famiglia"
::msgcat::mcset it_IT "Style" "Stile"
::msgcat::mcset it_IT "&Bold" "&Grassetto"
::msgcat::mcset it_IT "&Italic" "&Corsivo"
::msgcat::mcset it_IT "&Underline" "&Sottolineato"
::msgcat::mcset it_IT "Size" "Corpo"
::msgcat::mcset it_IT "&Defaults" "&Predefinito"
::msgcat::mcset it_IT "Use as..." "Utilizzare per..."
::msgcat::mcset it_IT "&Text font" "Carattere di &testo"
::msgcat::mcset it_IT "&Menu font" "Carattere dei &menu"
::msgcat::mcset it_IT "Sample" "Campione"
::msgcat::mcset it_IT "Font size" "Dimensione carattere"
::msgcat::mcset it_IT "Revert fonts" "Ripristinare i caratteri predefiniti"
::msgcat::mcset it_IT "Are you sure you want to revert all fonts to defaults?"\
                   "Sicuri di voler ripristinare i caratteri predefiniti?"


# FormatStringsForWatch.sci
::msgcat::mcset it_IT " what you try to watch is not supported by the debugger"\
                   "Il debugger non supporta quello che si sta cercando di\
                    osservare"
::msgcat::mcset it_IT " what you try to watch is of type "\
                   "Stai cercando di osservare un oggetto di tipo "
::msgcat::mcset it_IT " - this type is not supported by the debugger"\
                   " - questo tipo non viene supportato dal debugger"
::msgcat::mcset it_IT "Type:" "Tipo:"
::msgcat::mcset it_IT "(unsupported)" "(non supportato)"
::msgcat::mcset it_IT "size:" "dimensione:"
::msgcat::mcset it_IT "real" "reale"
::msgcat::mcset it_IT "complex" "complesso"
::msgcat::mcset it_IT "elements" "elementi"
::msgcat::mcset it_IT "graphic handle" "rif. oggetto grafico"
::msgcat::mcset it_IT "uncompiled function" "funzione non compilata"
::msgcat::mcset it_IT "compiled function" "funzione compilata"
::msgcat::mcset it_IT "library" "libreria"
::msgcat::mcset it_IT "pointer" "puntatore"
::msgcat::mcset it_IT "size implicit index" "indice a dimensione implicita"
::msgcat::mcset it_IT "primitive" "primitiva"


# FormatWhereForWatch.sci
::msgcat::mcset it_IT "Breakpoint" "Arresto"
::msgcat::mcset it_IT "called at line" "alla linea"
::msgcat::mcset it_IT "of exec file" "in un file exec"
::msgcat::mcset it_IT "of execstr instruction" "in una istruzione execstr"
::msgcat::mcset it_IT "called under pause" "chiamata durante \"pause\""
::msgcat::mcset it_IT "of macro" "della macro"


# helps.tcl
::msgcat::mcset it_IT "About" "A proposito"
::msgcat::mcset it_IT "aboutme_message"\
                   "\n Originato da tknotepad di Joseph Acosta,\
                    joeja@mindspring.com.\n Modificato dal Consorzio\
                    Scilab.\n Migliorato da Enrico Segre 2003-2006\n e da\
                    François Vogel (in particolare il debugger)\
                    2004-2008.\n Traduzioni: \n   norvegese, Torbjørn\
                    Pettersen 2005; \n   polacco, Bartosz Kosiorek 2005; \n  \
                    svedese, Gunnar Lindholm 2005; \n   tedesco, Dirk Reusch\
                    2005; \n   spagnolo, Juan Nicolás Quesada 2006;\n  \
                    cinese,  Yung-Jang Lee 2007;\n   danese, Kim B. Hansen,\
                    2007."
::msgcat::mcset it_IT "Scipad_is_released_under_GPL"\
                   "SciPad è rilasciato sotto la licenza GPL"


# infomessages.tcl
::msgcat::mcset it_IT "Line:" "Linea:"
::msgcat::mcset it_IT "Column:" "Colonna:"
#::msgcat::mcset fr "Line" "Ligne" ; # now common
::msgcat::mcset it_IT "in" "di"
::msgcat::mcset it_IT "Logical line:" "Linea logica:"
::msgcat::mcset it_IT " \[ReadOnly\]" " \[SolaLettura\]"
::msgcat::mcset it_IT " (modified)" " (modificato)"


# inputtext.tcl
::msgcat::mcset it_IT "No corresponding <" "Nessun <"
::msgcat::mcset it_IT "> found!" "> corrispondente trovato!"
::msgcat::mcset it_IT "Code editing is not allowed during debug!"\
                   "Impossible modificare il codice durante il debugging!"


# menues.tcl
::msgcat::mcset it_IT "&File" "&File"
::msgcat::mcset it_IT "&Edit" "&Modifica"
::msgcat::mcset it_IT "&Search" "&Cerca"
::msgcat::mcset it_IT "E&xecute" "&Esecuzione"
::msgcat::mcset it_IT "&Debug" "&Debug"
::msgcat::mcset it_IT "S&cheme" "&Schema"
::msgcat::mcset it_IT "&Options" "&Opzioni"
::msgcat::mcset it_IT "&Windows" "F&inestre"
::msgcat::mcset it_IT "&Help" "&Aiuto"

# menues.tcl - File menu
::msgcat::mcset it_IT "&New" "&Nuovo"
::msgcat::mcset it_IT "&Open..." "&Apri..."
::msgcat::mcset it_IT "Open &in new" "Apri &in un nuovo pannello"
::msgcat::mcset it_IT "&horizontal tile" "a &fianco"
::msgcat::mcset it_IT "&vertical tile" "di &sotto"
::msgcat::mcset it_IT "Op&en source of..." "Apri sorgente di..."
::msgcat::mcset it_IT "&Save" "&Salva"
::msgcat::mcset it_IT "Save &as..." "Salva &come..."
::msgcat::mcset it_IT "Save a&ll" "Salva &tutti"
::msgcat::mcset it_IT "&Revert..." "&Recupera..."
::msgcat::mcset it_IT "Import &Matlab file..." "Importa file &Matlab..."
::msgcat::mcset it_IT "Create help s&keleton..." "Crea scheletro di aiuto &XML..."
::msgcat::mcset it_IT "Compile as &help page" "Compila come file di ai&uto"
::msgcat::mcset it_IT "Open &function source" "Apri la sorgente della &funzione"
::msgcat::mcset it_IT "Print Se&tup" "Confi&gura stampante"
::msgcat::mcset it_IT "&Print" "Stam&pa"
::msgcat::mcset it_IT "&Close file" "Chiudere questo file"
::msgcat::mcset it_IT "E&xit" "&Esci"

# menues.tcl - Edit menu
::msgcat::mcset it_IT "&Undo" "&Annulla"
::msgcat::mcset it_IT "&Redo" "&Ripristina"
::msgcat::mcset it_IT "Cu&t" "&Taglia"
::msgcat::mcset it_IT "B&lock cut" "Ta&glia blocco"
::msgcat::mcset it_IT "&Copy" "&Copia"
::msgcat::mcset it_IT "&Paste" "&Incolla"
::msgcat::mcset it_IT "&Block paste" "Incolla &blocco"
::msgcat::mcset it_IT "&Delete" "&Elimina"
::msgcat::mcset it_IT "Select &All" "Sele&ziona tutto"
::msgcat::mcset it_IT "Insert &Time/Date" "Inserisci &ora/data"
::msgcat::mcset it_IT "Co&mment selection" "Co&mmenta la selezione"
::msgcat::mcset it_IT "U&ncomment selection" "&Scommenta la selezione"
::msgcat::mcset it_IT "&Indent selection" "&Indenta la selezione"
::msgcat::mcset it_IT "Unin&dent selection" "Arretra la se&lezione"

# menues.tcl - Search menu
::msgcat::mcset it_IT "&Find..." "&Trova..."
::msgcat::mcset it_IT "Find &Next" "Trova il &seguente"
::msgcat::mcset it_IT "&Replace..." "&Sostituisci..."
::msgcat::mcset it_IT "&Goto Line..." "&Vai a linea..."

# menues.tcl - Exec menu
::msgcat::mcset it_IT "&Load into Scilab" "&Carica in Scilab"
::msgcat::mcset it_IT "&Evaluate selection" "&Esegui la selezione"

# menues.tcl - Debug menu
::msgcat::mcset it_IT "&Insert/Remove breakpoint" "&Inserisci/rimuovi un arresto"
::msgcat::mcset it_IT "&Edit breakpoints" "&Modifica condizioni d'arresto"
::msgcat::mcset it_IT "Remove &all breakpoints" "Rimuovi &tutti gli arresti"
::msgcat::mcset it_IT "&Configure execution..." "&Configura l'esecuzione..."
::msgcat::mcset it_IT "Go to next b&reakpoint" "&Vai fino al prossimo arresto"
::msgcat::mcset it_IT "&Step by step" "&Passo a passo"
::msgcat::mcset it_IT "Step &into" "Entra &dentro"
::msgcat::mcset it_IT "Step o&ver" "Passa s&opra"
::msgcat::mcset it_IT "Step &out" "Esci f&uori"
::msgcat::mcset it_IT "Run to re&turn point" "Esegui &fino al ritorno"
::msgcat::mcset it_IT "Run to c&ursor" "Esegui fino al curso&re"
::msgcat::mcset it_IT "G&o on ignoring any breakpoint"\
                   "Co&ntinua ignorando gli arresti"
::msgcat::mcset it_IT "Show &watch" "Finestra &di controllo"
::msgcat::mcset it_IT "&Break" "&Stop"
::msgcat::mcset it_IT "Cance&l debug" "&Annulla il debug"

#    ::msgcat::mcset fr "language scheme" "schéma langage"
# menues.tcl - Scheme menu
::msgcat::mcset it_IT "S&cilab" "&Scilab"
::msgcat::mcset it_IT "&XML" "&XML"
::msgcat::mcset it_IT "&none" "&nessuno"
::msgcat::mcset it_IT "&Colorize" "&Colora"

# menues.tcl - Options menu
::msgcat::mcset it_IT "&Fonts..." "C&aratteri..."
::msgcat::mcset it_IT "&Colors" "&Colori"
::msgcat::mcset it_IT "Colori&ze" "C&olorare"
::msgcat::mcset it_IT "&Always" "&Sempre"
::msgcat::mcset it_IT "Ask for &big files" "&Chiedere per files grandi"
::msgcat::mcset it_IT "&Never" "&Mai"
::msgcat::mcset it_IT "Colorize \'&strings\'" "Colora &\'stringhe\'"
::msgcat::mcset it_IT "Colorize &user functions"\
                   "Colora funzioni &utente"
::msgcat::mcset it_IT "Show c&ontinued lines" "Colora linee &incomplete"
::msgcat::mcset it_IT "Line n&umbers" "N&umeri di linea"
::msgcat::mcset it_IT "&Hide" "&Nascosti"
::msgcat::mcset it_IT "&Left aligned" "Allineati a &sinistra"
::msgcat::mcset it_IT "&Right aligned" "Allineati a &destra"
::msgcat::mcset it_IT "Word &wrap" "&Ritorno a capo visuale"
::msgcat::mcset it_IT "&Double-click behavior" "Doppio clic &seleziona come"
::msgcat::mcset it_IT "&Linux" "&Linux"
::msgcat::mcset it_IT "Windo&ws" "Windo&w$"
::msgcat::mcset it_IT "&Scilab" "&Scilab"
::msgcat::mcset it_IT "&Tabs and indentation" "&Tabulatori e indentazione"
::msgcat::mcset it_IT "Tab inserts &spaces" "&Tab inserisce spazi"
::msgcat::mcset it_IT "&Indentation spaces" "&Spazi di indentazione"
::msgcat::mcset it_IT "&Tab size (characters)" "&Numero spazi tabulazione"
::msgcat::mcset it_IT "Use &keyword indentation" "&Indentazione automatica"
::msgcat::mcset it_IT "Com&pletion" "Autocom&pletamento"
::msgcat::mcset it_IT "&Tab" "&Tab"
::msgcat::mcset it_IT "&Control-Tab" "&Control-Tab"
::msgcat::mcset it_IT "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset it_IT "&Shift-Tab" "&Shift-Tab"
::msgcat::mcset it_IT "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
::msgcat::mcset it_IT "S&hift-Control-Tab" "S&hift-Control-Tab"
::msgcat::mcset it_IT "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
::msgcat::mcset it_IT "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
::msgcat::mcset it_IT "File&names" "Nome dei &files"
::msgcat::mcset it_IT "&Full path" "&Percorso completo"
::msgcat::mcset it_IT "Full path if &ambiguous" "Percorso completo se &ambiguo"
::msgcat::mcset it_IT "&Unambiguous pruned path" "Percorso &ridotto univoco"
::msgcat::mcset it_IT "Windows menu &sorting" "Ordine del &menu Finestre"
::msgcat::mcset it_IT "&File opening order" "secondo &momento di apertura"
::msgcat::mcset it_IT "&Alphabetically" "ordine &alfabetico"
::msgcat::mcset it_IT "&Most recently used" "secondo &uso più recente"
::msgcat::mcset it_IT "&Recent files" "&Numero files recenti"
::msgcat::mcset it_IT "&Backup files depth" "Numero &versioni backup"
::msgcat::mcset it_IT "&Locale" "&Localizzazione"
::msgcat::mcset it_IT "Execution &errors" "&Errori d'esecuzione"
::msgcat::mcset it_IT "In Scilab &shell only" "Solo nella finestra di &Scilab"
::msgcat::mcset it_IT "Copied in a &message box"\
                   "Anche in una finestra di &dialogo"

# menues.tcl - Windows menu
::msgcat::mcset it_IT "&Maximize" "&Massimizza"
::msgcat::mcset it_IT "&Split" "Dividi in &orizzontale"
::msgcat::mcset it_IT "S&plit (side by side)" "Dividi in &verticale"
::msgcat::mcset it_IT "Sp&lit file" "&Dividi il file in orizzontale"
::msgcat::mcset it_IT "Spl&it file (side by side)"\
                   "Dividi il file (&fianco a fianco)"
::msgcat::mcset it_IT "Tile all &vertically" "&Impila tutti"
::msgcat::mcset it_IT "Tile all &horizontally" "Affianca &tutti"
::msgcat::mcset it_IT "Space sashes &evenly" "Spezzatura &uniforme"
::msgcat::mcset it_IT "Opened Files" "Files aperti"

# menues.tcl - Help menu
::msgcat::mcset it_IT "&Help..." "&Aiuto..."
::msgcat::mcset it_IT "&What's?..." "&Cos'è?..."
::msgcat::mcset it_IT "&About" "A &proposito"
::msgcat::mcset it_IT "Adding &translations..." "&Tradurre Scipad..."
::msgcat::mcset it_IT "Adding Scipad Translations..."\
                   "Creare una nuova traduzione di Scipad:"
::msgcat::mcset it_IT "&Bugs \& wishlist" "&Bachi \& richieste"
::msgcat::mcset it_IT "Scipad Known bugs" "Bachi noti in Scipad"
::msgcat::mcset it_IT "Change&log" "Cronologia &modifiche"
::msgcat::mcset it_IT "Changes in the Scipad codebase"\
                   "Cronologia delle modifiche al codice di Scipad"


# popupmenus.tcl
::msgcat::mcset it_IT "Open the source of" "Apri la sorgente di"
::msgcat::mcset it_IT "AddWatch" "Tieni %s sotto controllo"
::msgcat::mcset it_IT "AddGenExp" "Aggiungere %s come espressione generica"
::msgcat::mcset it_IT "Jump to" "Salta a"


# print.tcl
::msgcat::mcset it_IT "Print Setup" "Configura stampante"
::msgcat::mcset it_IT "Print Command: " "Comando di stampa:"


# scilabexec.tcl
::msgcat::mcset it_IT "No point in loading an empty file!"\
                   "Inutile caricare un file vuoto!"
::msgcat::mcset it_IT "Silent file save failed" "Salvataggio silente non riuscito"
::msgcat::mcset it_IT "File could not be saved in a temporary\
                    location.\nOverwrite original file?"\
                   "Non è stato possibile copiare il file in una cartella\
                    temporanea. Sovrascrivere il file originale?"
::msgcat::mcset it_IT "Exec done" "Exec terminato"
::msgcat::mcset it_IT "Matlab files" "Files Matlab"
::msgcat::mcset it_IT "Matlab file to convert" "File Matlab da importare"
::msgcat::mcset it_IT "Scilab is converting, please hold on..."\
                   "Scilab sta traducendo, attendere..."
::msgcat::mcset it_IT "Matlab file import" "Importazione di un file Matlab"
::msgcat::mcset it_IT "Conversion of the file failed, see the Scilab window for\
                    details -- Perhaps report the error text and the\
                    offending Matlab file to"\
                   "La traduzione del file è fallita, vedere la finestra\
                    Scilab per dettagli -- Il baco può essere riportato\
                    includendo il file incriminato ed il messagggio di\
                    errore :"
::msgcat::mcset it_IT "Please select destination path for the xml source of the\
                    help file"\
                   "Scegliere una cartella di destinazione per la sorgente\
                    XML della pagina di aiuto"
::msgcat::mcset it_IT "Older version of" "Esiste già una versione di"
::msgcat::mcset it_IT "found!" "!"
::msgcat::mcset it_IT "An old version of" "Esiste una versione precedente di"
::msgcat::mcset it_IT "already exists: open the old file instead?"\
                   ": apro il file esistente invece?"
::msgcat::mcset it_IT "Existing file" "File esistente"
::msgcat::mcset it_IT "New skeleton" "Crea nuovo scheletro"
::msgcat::mcset it_IT "ScilabEval command cannot be passed to Scilab!"\
                   "Un comando ScilabEval non ha potuto essere passato a\
                    Scilab!"
::msgcat::mcset it_IT "impossibleScilabEval_message"\
                   "ATTENZIONE : Un comando non ha potuto essere passato a\
                    Scilab.  ed è stato ignorato\n\n  La ragione più\
                    probabile è che la cartella TMPDIR di Scilab non sia\
                    scrivibile. Ne può risultare un comportamento\
                    imprevedibile.\n\n Il comando in questione è:\n\n-->"
::msgcat::mcset it_IT "impossibleScilabEval_message2"\
                   "ATTENZIONE : Un comando non ha potuto essere passato a\
                    Scilab ed è stato ignorato perché troppo lungo. Ne\
                    può risultare un comportamento imprevedibile.\n\n Se si\
                    è nel corso di una sessione di debug, provare a\
                    ridurre le variabili osservate. \n\n Il comando in\
                    questione è:\n\n-->"
::msgcat::mcset it_IT "(end of command skipped)" "(resto del comando omesso)"
::msgcat::mcset it_IT "Scilab working" "Scilab occupato"
::msgcat::mcset it_IT "Scilab is working, please wait for the prompt to execute\
                    this command!"\
                   "Scilab è occupato, attendere il prompt per eseguire\
                    questo comando!"
::msgcat::mcset it_IT "Scilab is working, wait for the prompt to load file"\
                   "Scilab è occupato, attendere il prompt per caricare il\
                    file"
::msgcat::mcset it_IT "Scilab is working, wait for the prompt to execute the\
                    selection."\
                   "Scilab è occupato, attendere il prompt per eseguire la\
                    selezione"
::msgcat::mcset it_IT "Scilab is working, wait for the prompt to convert a\
                    Matlab file."\
                   "Scilab è occupato, attendere il prompt per convertire\
                    un file Matlab"
::msgcat::mcset it_IT "Scilab is working, wait for the prompt to compile the\
                    help file."\
                   "Scilab è occupato, attendere il prompt per compilare la\
                    pagina di aiuto"
::msgcat::mcset it_IT "Scilab is working, wait for the prompt to issue a\
                    debugger command."\
                   "Scilab è occupato, aspettare il prompt per eseguire\
                    comandi di debug."
::msgcat::mcset it_IT "Scilab execution error"\
                   "Errore di Scilab durante l'esecuzione"
::msgcat::mcset it_IT "The shell reported an error while trying to execute "\
                   "Lo shell ha riportato un errore nel cercare di eseguire"
::msgcat::mcset it_IT ": error " ": errore "
::msgcat::mcset it_IT "at line " "alla linea "
::msgcat::mcset it_IT " of " " di "
::msgcat::mcset it_IT "Execution aborted!" "Esecuzione annullata!"


# scipad.tcl
::msgcat::mcset it_IT "NOT CONNECTED TO SCILAB" "MANCA LA CONNESSIONE A SCILAB"


# whichfun.tcl
::msgcat::mcset it_IT "Being at line" "Essendo a linea"
::msgcat::mcset it_IT ", function" ", la funzione"
::msgcat::mcset it_IT "begins at" "comincia a"
::msgcat::mcset it_IT ", and there are" ", e ci sono"
::msgcat::mcset it_IT "continued lines, i.e. we are at line"\
                   "linee continuate, cioè siamo alla linea"
::msgcat::mcset it_IT "of" "di"
#::msgcat::mcset fr "Function" "Fonction" ; # now global
::msgcat::mcset it_IT "line" "linea"
::msgcat::mcset it_IT "The cursor is not currently inside a function body."\
                   "Il cursore non si trova dentro il corpo di una funzione"
::msgcat::mcset it_IT "Which function?" "Quale funzione?"


#preference variables description
::msgcat::mcset it_IT "BGCOLOR" "Sfondo"
::msgcat::mcset it_IT "BGLNMARGCOLOR" "Sfondo numeri linea"
::msgcat::mcset it_IT "FGCOLOR" "Testo semplice"
::msgcat::mcset it_IT "FGLNMARGCOLOR" "Testo numeri linea"
::msgcat::mcset it_IT "CURCOLOR" "Cursore"
::msgcat::mcset it_IT "PARCOLOR" "Parentesi tonde ()"
::msgcat::mcset it_IT "BRAKCOLOR" "Parentesi quadre \\\[\\\]"
::msgcat::mcset it_IT "BRACCOLOR" "Parentesi graffe {}"
::msgcat::mcset it_IT "PUNCOLOR" "Punteggiatura"
::msgcat::mcset it_IT "INTFCOLOR" "Funzioni interne"
::msgcat::mcset it_IT "COMMCOLOR" "Comandi Scilab"
::msgcat::mcset it_IT "SCICCOLOR" "Funzioni Scicos"
::msgcat::mcset it_IT "OPCOLOR" "Operatori"
::msgcat::mcset it_IT "QTXTCOLOR" "Stringhe"
::msgcat::mcset it_IT "REMCOLOR" "Commenti"
::msgcat::mcset it_IT "XMLCOLOR" "Tags XML"
::msgcat::mcset it_IT "NUMCOLOR" "Numeri"
::msgcat::mcset it_IT "SELCOLOR" "Sfondo selezione"
::msgcat::mcset it_IT "BREAKPOINTCOLOR" "Punti d'arresto"
::msgcat::mcset it_IT "LFUNCOLOR" "Funzioni di libreria"
::msgcat::mcset it_IT "PDEFCOLOR" "Variabili predefinite"
::msgcat::mcset it_IT "FOUNDTEXTCOLOR" "Testo trovato"
::msgcat::mcset it_IT "CONTLINECOLOR" "Linea incompleta"
::msgcat::mcset it_IT "USERFUNCOLOR" "Funzioni utente"


#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset it_IT "&Directory:" "&Cartella"
::msgcat::mcset it_IT "File &name:" "&Nome del file:"
::msgcat::mcset it_IT "Files of &type:" "&Tipo di file:"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
::msgcat::mcset it_IT "&Open" "A&pri"
::msgcat::mcset it_IT "Open" "Apri"
::msgcat::mcset it_IT "Save As" "Salva come"
::msgcat::mcset it_IT "&Cancel" "&Annulla"
::msgcat::mcset it_IT "&Selection:" "&Selezione:"
::msgcat::mcset it_IT "&Yes" "&Si"
::msgcat::mcset it_IT "&No" "&No"
::msgcat::mcset it_IT "&Red" "&Rosso"
::msgcat::mcset it_IT "&Green" "&Verde"
::msgcat::mcset it_IT "&Blue" "&Blu"
::msgcat::mcset it_IT "Directory \"%1\$s\" does not exist."\
                   "La cartella \"%1\$s\" non esiste."
