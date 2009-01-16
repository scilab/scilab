#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2009 - Francois Vogel
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
# in sync at least with v6.155


# commons
::msgcat::mcset it_it "Cancel" "Annulla"
::msgcat::mcset it_it "Cance&l" "Annu&lla"
::msgcat::mcset it_it "OK" "OK"
::msgcat::mcset it_it "All files" "Tutti i files"
::msgcat::mcset it_it "Add/Change" "Aggiungi/modifica"
::msgcat::mcset it_it "Add/Chan&ge" "Aggiungi/&modifica"
::msgcat::mcset it_it "&Remove" "&Rimuovi"
::msgcat::mcset it_it "Rem&ove" "Rimu&ovi"
::msgcat::mcset it_it "Untitled" "SenzaTitolo"
::msgcat::mcset it_it "Close" "Chiudi"
::msgcat::mcset it_it "&Close" "&Chiudi"
::msgcat::mcset it_it "Error " "Errore "
::msgcat::mcset it_it "The file" "Il file"
::msgcat::mcset it_it "Function" "Funzione"
::msgcat::mcset it_it "Line" "Linea"


# buffernavigation.tcl
::msgcat::mcset it_it "Hide" "Nascondi"
::msgcat::mcset it_it "Close this tile, keep content"\
                      "Chiudi il pannello, lascia il file aperto"
::msgcat::mcset it_it "Close this tile" "Chiudi questo pannello"
::msgcat::mcset it_it "Goto Line?" "Vai alla linea?"
::msgcat::mcset it_it "Go to:" "Vai a:"
::msgcat::mcset it_it "&logical line" "linea &logica"
::msgcat::mcset it_it "&physical line" "linea fi&sica"
::msgcat::mcset it_it "in &function" "della &funzione"
::msgcat::mcset it_it "in &current file" "del file &corrente"
::msgcat::mcset it_it "Outside of function definition"\
                      "fuori dalla definizione della funzione"


# colorize.tcl
::msgcat::mcset it_it "Wait seconds while recolorizing file"\
                      "Attendere, ricolorazione in corso"
::msgcat::mcset it_it "You can't do that while colorization is in progress.\
                       Please try again when finished."\
                      "Non si può fare questo prima che la colorazione sia\
                       completata. Aspettare il termine della colorazione e\
                       riprovare"
::msgcat::mcset it_it "Command forbidden during colorization"\
                      "Comando non consentito mentre la colorazione è in\
                       corso"


# completion.tcl
::msgcat::mcset it_it "No possible completion" "Nessun completamento possibile"
::msgcat::mcset it_it "possible completions" "completamenti possibili"
::msgcat::mcset it_it "Possible completions" "Completamenti possibili"


# db_argsmanagement.tcl
::msgcat::mcset it_it "Add" "Aggiungi"
::msgcat::mcset it_it "This variable can be watched but cannot be edited!"\
                      "Questa variabile può essere osservata, ma non\
                       modificata!"
::msgcat::mcset it_it "A new watch variable will be created."\
                      "Verrà piuttosto proposta una nuova variabile da\
                       osservare."
::msgcat::mcset it_it "Non editable variable" "Variabile non modificabile"
::msgcat::mcset it_it "Variable:" "Variabile:"
::msgcat::mcset it_it "Expression:" "Espressione:"
::msgcat::mcset it_it "Value:" "Valore:"
::msgcat::mcset it_it "&Get current value from Scilab"\
                      "Recupera il valore &attuale in Scilab"
::msgcat::mcset it_it "Non watchable variable" "Variabile non osservabile"
::msgcat::mcset it_it "Sorry, variable" "Spiacente, la variabile"
::msgcat::mcset it_it "is special and cannot be watched."\
                      "è di un tipo che non può essere osservato."


# db_bptsgui.tcl
::msgcat::mcset it_it "Breakpoints properties" "Proprietà dei punti d'arresto"
::msgcat::mcset it_it "Enabled" "Abilita"
::msgcat::mcset it_it "Expression" "Espressione"
::msgcat::mcset it_it "Hit if" "Marca se"
::msgcat::mcset it_it "istrue" "è vero"
::msgcat::mcset it_it "haschanged" "cambia"
::msgcat::mcset it_it "Watched" "Osservato"
::msgcat::mcset it_it "Hit count" "n° accessi"
::msgcat::mcset it_it "Break if" "Arresta se"
::msgcat::mcset it_it "Threshold" "Soglia"
::msgcat::mcset it_it "always" "sempre"
::msgcat::mcset it_it "equalto" "è uguale a"
::msgcat::mcset it_it "multipleof" "è un multiplo di"
::msgcat::mcset it_it "greaterthan" "è più grande di"
::msgcat::mcset it_it "lessthan" "è più piccolo di"
::msgcat::mcset it_it "Reset hit" "azzera accessi"
::msgcat::mcset it_it "Breakpoints vocabulary"\
                      "Vocabolario dei punti d'arresto"
::msgcat::mcset it_it "A breakpoint is identified by its location, i.e. its\
                       position (a line number) in a function."\
                      "Un punto d'arresto è identificato dalla sua\
                       posizione, cioè dal suo numero di linea nel corpo\
                       della funzione."
::msgcat::mcset it_it "When the execution process encounters an enabled\
                       breakpoint, then this breakpoint is said to be\
                       reached."\
                      "Quando l'esecuzione del programma incontra un punto\
                       d'arresto abilitato, si dice che il punto d'arresto\
                       è stato raggiunto."
::msgcat::mcset it_it "When the breakpoint is reached and its associated\
                       conditional expression is true (or changed), then the\
                       breakpoint is said to be hit."\
                      "Quando un punto d'arresto è raggiunto e\
                       l'espressione condizionale associata è vera (o\
                       cambia), allora il punto d'arresto si dice\
                       \"acceduto\"."
::msgcat::mcset it_it "The hit count is the number of times the breakpoint\
                       has been hit."\
                      "Il numero di accessi è il numero di volte che questo\
                       avviene."
::msgcat::mcset it_it "Execution will stop at a breakpoint if the hit count\
                       satisfies the selectable break condition."\
                      "L'esecuzione si fermerà ad un punto d'arresto se il\
                       numero di accessi soddisfa la condizione prescelta."


# db_bptsmanagement.tcl
::msgcat::mcset it_it "No breakpoint here!"\
                      "Non si può mettere un arresto qui!"
::msgcat::mcset it_it "Delete all breakpoints"\
                      "Cancella tutti i punti d'arresto"
::msgcat::mcset it_it "Are you sure you want to delete all the existing\
                       breakpoints?"\
                      "Sicuro di voler cancellare tutti i punti d'arresto_"
::msgcat::mcset it_it "All breakpoints deleted" "Tutti gli arresti eliminati"
::msgcat::mcset it_it "No action taken" "Nessun cambiamento effettuato"
::msgcat::mcset it_it "No breakpoint!" "Nessun punto d'arresto!"


# db_configureexec.tcl
::msgcat::mcset it_it "Configure execution" "Configura l'esecuzione"
::msgcat::mcset it_it "Function name:" "Nome della funzione:"
::msgcat::mcset it_it "&Obtain" "&Ottieni"
::msgcat::mcset it_it "Input arguments:" "Argumenti di ingresso:"
::msgcat::mcset it_it "The selected function has not been found in the\
                       opened files, maybe it was deleted since last\
                       configuration of the debugger.\nUse button \""\
                      "la funzione selezionata non compare in nessuno dei\
                       files aperti, potrebbe essere stata cancellata\
                       dall'ultima volta che il debugger è stato\
                       configurato. Usare il tasto \""
::msgcat::mcset it_it "\" to refresh the list of available functions."\
                      "\" per rinfrescare la lista delle funzioni disponibili"
::msgcat::mcset it_it "Function name or input arguments do not match\
                       definition of the function"\
                      "Il nome della funzione o i suoi argomenti non\
                       corrispondono alla definizione della funzione"
::msgcat::mcset it_it "in the file!\n\nCheck function name and arguments\
                       (names, order) in the configuration dialog.\
                       \nArguments order can be changed using drag and drop\
                       with right mouse button in the arguments listbox."\
                      "nel file!\n\n Verificare il nome della funzione e gli\
                       argomenti (nome, ordine di comparsa) nella casella\
                       del dialogo di configurazione.\n L'ordine degli\
                       argomenti può essere modificato trascinando e\
                       deponendo col mouse."
::msgcat::mcset it_it "Error in selected function name or arguments"\
                      "Errore nella funzione selezionata o nei suoi argomenti"
::msgcat::mcset it_it "At least one input argument does not match definition\
                       of the function"\
                      "Uno o più argomenti non corrispondono alla\
                       definizione della funzione"
::msgcat::mcset it_it "in the file!\n\nCheck function arguments (their\
                       names) in the configuration dialog."\
                      "nel file!\n\nVerificare il nome della funzione e gli\
                       argomenti (nome, ordine di comparsa) nel dialogo di\
                       configurazione."
::msgcat::mcset it_it "Error in selected function arguments"\
                      "Errore negli argomenti della funzione selezionata"
::msgcat::mcset it_it "Main level code found" "Codice a livello principale"
::msgcat::mcset it_it "Scipad ancillary file" "File ancillario di Scipad"
::msgcat::mcset it_it "The current file contains only Scipad ancillaries\
                       that cannot be debugged in Scipad."\
                      "Il file attuale contiene funzioni usate da Scipad,\
                       che non possono essere debuggate all'interno di\
                       Scipad."
::msgcat::mcset it_it "Reserved function names are:\n"\
                      "I nomi di funzione riservati sono:\n"
::msgcat::mcset it_it "See help scipad for further details."\
                      "Consultare l'aiuto di SciPad per maggiori dettagli."
::msgcat::mcset it_it "This file contains main level code.\nDebug as a\
                       single .sce file?"\
                      "Questo file contiene codice a livello principale.\
                       Eseguire il debug trattando tutto il file come un\
                       unica funzione?"
::msgcat::mcset it_it "No function found" "Nessuna funzione trovata"
::msgcat::mcset it_it "This file contains no function and will be treated"\
                      "Questo file non contiene alcuna funzione e sarà\
                       trattato"
::msgcat::mcset it_it "as a single .sce file.\nStart debugging?"\
                      "Come un unico file .sce. Cominciare il debug?"
::msgcat::mcset it_it "You must enable colorization to be able to launch the\
                       debugger!"\
                      "Occorre attivare la colorazione per potere usare il\
                       debugger!"
::msgcat::mcset it_it "The debugger needs colorization"\
                      "Il debugger ha bisogno della colorazione"
::msgcat::mcset it_it "Warning!\n\n  A function missing the closing\
                       \"endfunction\" has been found in the currently\
                       opened files.\n  Such syntax is now obsolete.\n\n \
                       Please terminate function "\
                      "Attenzione!\n E' stata trovata una funzione non\
                       terminata dalla parola chiave \"endfunction\" in uno\
                       dei files correntemente aperti. Questa sintassi è\
                       obsoleta.\n\n Terminare la funzione "
::msgcat::mcset it_it " and try again." " con \"endfunction\" e riprovare."
::msgcat::mcset it_it "Note: This message might also be triggered by a\
                       string\n  quoted with single quotes and containing\
                       the word \"function\",\n  when \"Colorize\
                       \'strings\'\" is unchecked in the Options menu."\
                      "Nota: questo messaggio può anche essere dovuto ad\
                       una stringa contenente la parola \"function\"\
                       delimitata da apostrofi invece che da virgolette, se\
                       l'opzione \"Colora \'stringhe\' non è selezionata\
                       nel menu Opzioni."
::msgcat::mcset it_it "This known bug can be worked around simply by\
                       checking \"Colorize \'&strings\'\"."\
                      "Questo baco, noto, può essere evitato semplicemente\
                       selezionando l\'opzione suddetta."
::msgcat::mcset it_it "Unterminated function definition found"\
                      "Trovata funzione non terminata"
::msgcat::mcset it_it "Warning!\n\n  One or more function is defined more\
                       than once in the currently opened files.\n  The\
                       debugger cannot cope with multiple copies of\
                       functions.\n  The following duplicates were\
                       detected:\n"\
                      "Attenzione! Una o più funzioni sono definite più\
                       volte nei files aperti. Il debugger non è in grado\
                       di scegliere la definizione pertinente e non può\
                       procedere. Sono stati rilevati i seguenti doppioni:"
::msgcat::mcset it_it "\n--> Definition of function "\
                      "\n--> Definizione della funzione "
::msgcat::mcset it_it " found in:\n" " trovata in:\n"
::msgcat::mcset it_it "Duplicate function definitions found"\
                      "Doppie definizioni di funzioni trovate"


# db_debugsession.tcl
::msgcat::mcset it_it "Too many breakpointed functions"\
                      "Troppe funzioni con punti di arresto"
::msgcat::mcset it_it "You have currently" "Ci sono al momento"
::msgcat::mcset it_it "functions in your opened files." "funzioni."
::msgcat::mcset it_it "Scilab supports a maximum of"\
                      "Scilab supporta al massimo"
::msgcat::mcset it_it "possible breakpointed functions (see help setbpt)."\
                      "funzioni con punti d'arresto (vedi help setbpt)."
::msgcat::mcset it_it "Step-by-step hence cannot be performed."\
                      "Il modo passo-passo non è pertanto possible."
::msgcat::mcset it_it "This command will actually run to the next breakpoint."\
                      "Questo comando andrà solo fino all'arresto successivo"
::msgcat::mcset it_it "Break hence cannot be performed."\
                      "Il comando Stop non può essere eseguito."
::msgcat::mcset it_it "Too many breakpoints" "Troppi punti d'arresto"
::msgcat::mcset it_it "Executing this command would require to set"\
                      "Eseguire questo comando richiederebbe di inserire"
::msgcat::mcset it_it "breakpoints in your opened files."\
                      "punti d'arresto nei files aperti."
::msgcat::mcset it_it "possible breakpoints in Scilab (see help setbpt)."\
                      "possibili punti d'arresto (vedere help setbpt)."
::msgcat::mcset it_it "Cursor must be in a function"\
                      "Il cursore deve essere dentro una funzione"
::msgcat::mcset it_it "This command cannot succeed if the function where the\
                       debug just stopped is not opened in Scipad!"\
                      "Questo comando non può essere eseguito se la\
                       funzione in cui il debugger si è fermato non è\
                       aperta in Scipad!"
::msgcat::mcset it_it "Cannot run to return point"\
                      "Impossibile eseguire fino al punto di ritorno"
::msgcat::mcset it_it "Warning!\nThe line where the debugger just stopped\
                       contains a return statement (either endfunction,\
                       return or resume) but this statement is not the first\
                       one on the line."\
                      "Attenzione!\nLa linea su cui il debugger si è\
                       fermato contiene una istruzione di ritorno\
                       (endfunction, return o resume) che non è la prima\
                       della linea."
::msgcat::mcset it_it "As a consequence the debugger cannot predict if the\
                       next debug step will really jump outside of the\
                       current function, which is what you just asked the\
                       debugger to do."\
                      "Il debugger non può prevedere se il prossimo passo\
                       di debug provocherà davvero l'uscita (come\
                       richiesto) o meno."
::msgcat::mcset it_it "To avoid this unresolvable case, it is recommended to\
                       place the return statement alone on its line."\
                      "Per evitare casi non risolubili (per il debugger)\
                       come questo è raccomandato di piazzare l'istruzione\
                       di ritorno isolata sulla sua linea."
::msgcat::mcset it_it "Do you want to see this warning again during this\
                       Scipad session?"\
                      "Vuoi vedere ancora questo messaggio durante questa\
                       sessione di Scipad?"
::msgcat::mcset it_it "Unresolvable case found while running to return point"\
                      "Caso irrisolubile incontrato durante \"esegui fino al\
                       punto di ritorno\""
::msgcat::mcset it_it "No effect - The debugged file is not stuck"\
                      "Senza effetto - il file in debug non è piantato"


# db_init.tcl
::msgcat::mcset it_it "Please wait..." "Attendere..."


# db_states.tcl
::msgcat::mcset it_it "Currently no debug session"\
                      "Nessuna sessione di debug in corso"
::msgcat::mcset it_it "Ready to start debug" "Pronto per il debug"
::msgcat::mcset it_it "Debug in progress" "Debug in corso"
::msgcat::mcset it_it "Scilab is back at the main level now."\
                      "Scilab è tornato al livello principale."


# db_watch.tcl
::msgcat::mcset it_it "Watch" "Controllo esecuzione e variabili"
::msgcat::mcset it_it "Watch variables:" "Variabili osservate:"
::msgcat::mcset it_it "A&uto (local variables)"\
                      "&Auto (tutte le variabili locali)"
::msgcat::mcset it_it "Au&to watch globals too"\
                      "Anche variabili &globali usate dalla funzione"
::msgcat::mcset it_it "Generic expressions:" "Espressioni generiche:"
::msgcat::mcset it_it "Call stack:" "stack di programma:"
::msgcat::mcset it_it "Always on to&p" "Sempre in &primo piano"
::msgcat::mcset it_it "Hide watch &variables" "Nascondi le &variabili"
::msgcat::mcset it_it "Hide e&xpressions" "Nascondi le &espressioni"
::msgcat::mcset it_it "Hide call &stack" "Nascondi lo &stack"
::msgcat::mcset it_it "&Dock" "&Incassa"
::msgcat::mcset it_it "&Add" "Agg&ungi"
::msgcat::mcset it_it "&Filter errors" "&Filtra (non mostrare) gli errori"
::msgcat::mcset it_it "Breakpoints" "Punti d'arresto"
::msgcat::mcset it_it "Breakpointed functions" "Funzioni con arresti"
::msgcat::mcset it_it "Scilab busy indicator"\
                      "indicatore per Scilab pronto/occupato"
::msgcat::mcset it_it "Debugger state indicator"\
                      "indicatore dello stato del debugger"
::msgcat::mcset it_it "Undefined elements are not legal in variable"\
                      "Gli elementi indefiniti sono illegali in questa\
                       variabile"
::msgcat::mcset it_it ".\nThis variable will not be updated in Scilab."\
                      ".\nQuesta variabile non sarà aggiornata in Scilab"
::msgcat::mcset it_it "Illegal undefined element found"\
                      "Trovato un elemento indefinito"
::msgcat::mcset it_it "evaluates to:" "risulta in:"
::msgcat::mcset it_it "produces error" "produce un errore"
::msgcat::mcset it_it "Scipad cannot find source code for the selected\
                       function.\nPlease open the adequate file manually and\
                       try again."\
                      "Scipad non trova il codice sorgente della funzione\
                       selezionata.\nAprire manualmente il file\
                       corrispondente e riprovare."
::msgcat::mcset it_it "Missing source code" "Codice sorgente mancante"


# filecommands.tcl
::msgcat::mcset it_it "New script" "Nuovo testo"
::msgcat::mcset it_it "New view on the same file"\
                      "Nuovo pannello per lo stesso file"
::msgcat::mcset it_it "The contents of" "Il contenuto di"
::msgcat::mcset it_it "may have changed, do you wish to save your changes?"\
                      "potrebbe essere cambiato, registrare le modifiche?"
::msgcat::mcset it_it "Save Confirm?" "Conferma salvataggio?"
::msgcat::mcset it_it "Open source of..." "Apri la sorgente di..."
::msgcat::mcset it_it "Open source of:" "Apri la sorgente di:"
::msgcat::mcset it_it "Weird drag&drop" "Strano trascina e deponi"
::msgcat::mcset it_it "is neither a valid filename nor a valid directory.\
                       Either you're dragging an object of another type, or\
                       you hit a bug of the dnd mechanism."\
                      "non è un nome valido per un file. O è stato\
                       trascinato un oggetto di tipo sbagliato, o il\
                       meccanismo di d&d ha un baco."
::msgcat::mcset it_it "File not found" "File non trovato"
::msgcat::mcset it_it "does not exist anymore. Do you want to create an\
                       empty file with the same name?"\
                      "non esiste più. Creare un nuovo file collo stesso\
                       nome?"
::msgcat::mcset it_it "Open file" "Aprire file"
#::msgcat::mcset fr_fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset it_it "is already opened! Save the current opened file to\
                       another name and reopen it from disk!"\
                      "è già aperto! Salvarlo eventualmente con un altro\
                       nome e riaprirlo dal disco"
::msgcat::mcset it_it "File" "File"
::msgcat::mcset it_it "saved" "salvato"
::msgcat::mcset it_it "cannot be written!" "non è scrivibile!"
::msgcat::mcset it_it "The contents of " "Il contenuto di"
::msgcat::mcset it_it "has changed on disk, save it anyway?"\
                      "è cambiato sul disco, salvare comunque?"
::msgcat::mcset it_it "The contents and/or properties of " "Il contenuto e/o proprietà di"
::msgcat::mcset it_it "has been modified outside of Scipad. Do you want to\
                       reload it?"\
                      "è stato cambiato fuori da Scipad. Ricaricarlo dal\
                       disco?"
::msgcat::mcset it_it "File has changed!" "Il file è cambiato"
::msgcat::mcset it_it "Save as" "Registra come"
::msgcat::mcset it_it "Scilab files" "Files Scilab"
::msgcat::mcset it_it "Scicos files" "Files Scicos"
::msgcat::mcset it_it "XML files" "Files XML"
::msgcat::mcset it_it "Wait seconds while loading and colorizing file"\
                      "Attendere - apertura e colorazione del file in corso"
::msgcat::mcset it_it "The contents of this file may have changed, do you\
                       wish to save your changes?"\
                      "Il contenuto di questo file potrebbe essere cambiato,\
                       registrare la nuova versione?"
::msgcat::mcset it_it "Revert" "Ritornare alla versione di"
::msgcat::mcset it_it "to saved?" "salvata sul disco?"
::msgcat::mcset it_it "Revert Confirm?" "Conferma recupero?"
::msgcat::mcset it_it "Unreadable file" "File illeggibile!"
::msgcat::mcset it_it "exists but is not readable!"\
                      "esiste, ma non è leggibile!"
::msgcat::mcset it_it "This file is rather large and colorization might take\
                       some time.\n\nColorize anyway?"\
                      "Questo file è relativamente grosso, e la colorazione\
                       potrebbe richiedere tempo. Colorare comunque?"
::msgcat::mcset it_it "File size warning" "Avvertimento: grosso file"
::msgcat::mcset it_it "Windows shortcut file" "File scorciatoia Windows"
#::msgcat::mcset fr_fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset it_it "is a Windows shortcut and will not be opened!"\
                      "è una scorciatoia windows e non sarà aperto!"


# findreplace.tcl
::msgcat::mcset it_it "Dialog box already open!"\
                      "Finestra di dialogo già aperta!"
::msgcat::mcset it_it "The string" "La stringa \["
::msgcat::mcset it_it "could not be found in" "\] non è stata trovata in"
::msgcat::mcset it_it "could not be found in any of the opened files !"\
                      "\] non è stata trovata in nessuno dei files aperti!"
::msgcat::mcset it_it "replacements done" "sostituzioni effettuate"
::msgcat::mcset it_it "Find" "Cerca"
::msgcat::mcset it_it "No match found in the selection for"\
                      "Nessuna occorrenza di \["
::msgcat::mcset it_it "\nWould you like to look for it in the entire text?"\
                      "\] trovata all'interno della selezione. Continuare la\
                       ricerca nel resto del testo?"
::msgcat::mcset it_it "You are searching for an empty string!"\
                      "La stringa da cercare è vuota!"
::msgcat::mcset it_it "Re&place" "&Sostituisci"
::msgcat::mcset it_it "Find what:" "Cerca:"
::msgcat::mcset it_it "Replace with:" "Cambia in:"
::msgcat::mcset it_it "Find &Next" "Trova il &seguente"
::msgcat::mcset it_it "Replace &All" "Rimpiazza &tutti"
::msgcat::mcset it_it "Direction" "Direzione"
::msgcat::mcset it_it "&Upwards" "Verso l'&alto"
::msgcat::mcset it_it "&Downwards" "Verso il &basso"
::msgcat::mcset it_it "Text types" "Tipo di testo"
::msgcat::mcset it_it "Find options" "Opzioni di ricerca"
::msgcat::mcset it_it "Match &whole word only" "Solo parola &intera"
::msgcat::mcset it_it "Match &case" "Rispetta &maiuscole"
::msgcat::mcset it_it "&Regular expression" "Espressione &regolare (regexp)"
::msgcat::mcset it_it "In all &opened files" "In tutti i &files aperti"
::msgcat::mcset it_it "In &selection only" "Solo nella sele&zione"
::msgcat::mcset it_it "In typed te&xt" "Nel testo di ti&po"
::msgcat::mcset it_it "In a director&y" "In una &directory:"
::msgcat::mcset it_it "Find in files search options"\
                      "Opzioni di ricerca nelle directories"
::msgcat::mcset it_it "Directory r&ecurse search" "Ricerca ri&corsiva"
::msgcat::mcset it_it "Include &hidden files" "Includere files &nascosti"
::msgcat::mcset it_it "In files/file types:" "Nei files/tipi di files"
::msgcat::mcset it_it "In directory:" "Nella directory:"
::msgcat::mcset it_it "Search for file names" "Cercare tra i nomi dei files"
::msgcat::mcset it_it "You have given an empty search string. Are you sure\
                       you want to search among file names?"\
                      "La stringa da cercare è vuota -- sicuri di volere\
                       cercare tra nomi di files?"
::msgcat::mcset it_it "Wrapped around" "Continuato oltre la fine"
::msgcat::mcset it_it "Back at the first file searched!"\
                      "Di nuovo al primo file esaminato!"
::msgcat::mcset it_it "No more matches" "Nessuna altra corrispondenza"
::msgcat::mcset it_it "The string to regexp search for cannot be compiled by\
                       Tcl.\nCheck again, maybe certain characters should be\
                       escaped."\
                      "La stringa da cercare come espressione regolare non\
                       ha potuto essere compilata. Controllare. Per esempio,\
                       alcuni caratteri speciali vanno preceduti da \\."
::msgcat::mcset it_it "Any single character" "Un carattere qualsiasi"
::msgcat::mcset it_it "Zero or more" "Zero o più"
::msgcat::mcset it_it "One or more" "Uno o più"
::msgcat::mcset it_it "Zero or one" "Zero o uno"
::msgcat::mcset it_it "Any one character in the set"\
                      "Qualsiasi carattere dell'insieme"
::msgcat::mcset it_it "Any one character not in the set"\
                      "Qualsiasi carattere non nell'insieme"
::msgcat::mcset it_it "Or" "Oppure"
::msgcat::mcset it_it "Beginning of line" "Inizio riga"
::msgcat::mcset it_it "End of line" "Fine riga"
::msgcat::mcset it_it "Beginning of word" "Principio di parola"
::msgcat::mcset it_it "End of word" "Fine di parola"
::msgcat::mcset it_it "A letter" "Lettera"
::msgcat::mcset it_it "A decimal digit" "Cifra decimale"
::msgcat::mcset it_it "An alphanumeric (letter or digit)"\
                      "Carattere alfanumerico (lettera o cifra)"
::msgcat::mcset it_it "A space or tab character" "Spazio o tabulatore"
::msgcat::mcset it_it "A rational number" "Numero razionale"
::msgcat::mcset it_it "A floating point number" "Numero a virgola mobile"
::msgcat::mcset it_it "A Scilab comment" "Commento Scilab"
::msgcat::mcset it_it "A Scilab name" "Variabile o parola chiave Scilab"
::msgcat::mcset it_it "A Scilab continued line (with dots)"\
                      "Linea Scilab continuata con .."


# findinfiles.tcl
::msgcat::mcset it_it "Directory access issue"\
                      "Errore di accesso alla directory"
::msgcat::mcset it_it "The directory you specified cannot be reached. The\
                       search won't bring any result."\
                      "Scipad non può accedere alla directory specificata.\
                       La ricerca non darà risultato"
::msgcat::mcset it_it "Some directories or files could not be open and were\
                       ignored during search.\n You might miss read access\
                       to them.\n\n The following ones were ignored"\
                      "Certe directories o certi files non possono essere\
                       aperti (probabilmente sono protetti in lettura) e\
                       sono stati ignorati durante la ricerca. I seguenti\
                       sono stati ignorati:"
::msgcat::mcset it_it "Ignored files" "Files ignorati"
::msgcat::mcset it_it "Searching in file:" "Ricerca in:"
::msgcat::mcset it_it "End of file search:" "Fine della ricerca:"
::msgcat::mcset it_it "matches found" "corrispondenze trovate"
::msgcat::mcset it_it "searched files" "files esaminati"
::msgcat::mcset it_it "Constructing file list:"\
                      "Costruzione della lista dei files:"
::msgcat::mcset it_it "&Previous" "&Precedente"
::msgcat::mcset it_it "&Next" "&Successivo"
::msgcat::mcset it_it "Scipad search results" "Risultati della ricerca"


# fonts.tcl
::msgcat::mcset it_it "Choose font" "Scegliere tipo di carattere"
::msgcat::mcset it_it "Family" "Famiglia"
::msgcat::mcset it_it "Style" "Stile"
::msgcat::mcset it_it "&Bold" "&Grassetto"
::msgcat::mcset it_it "&Italic" "&Corsivo"
::msgcat::mcset it_it "&Underline" "&Sottolineato"
::msgcat::mcset it_it "Size" "Corpo"
::msgcat::mcset it_it "&Defaults" "&Predefinito"
::msgcat::mcset it_it "Use as..." "Utilizzare per..."
::msgcat::mcset it_it "&Text font" "Carattere di &testo"
::msgcat::mcset it_it "&Menu font" "Carattere dei &menu"
::msgcat::mcset it_it "Sample" "Campione"
::msgcat::mcset it_it "Font size" "Dimensione carattere"
::msgcat::mcset it_it "Revert fonts" "Ripristinare i caratteri predefiniti"
::msgcat::mcset it_it "Are you sure you want to revert all fonts to defaults?"\
                      "Sicuri di voler ripristinare i caratteri predefiniti?"


# FormatStringsForWatch.sci
::msgcat::mcset it_it " what you try to watch is not supported by the\
                       debugger"\
                      "Il debugger non supporta quello che si sta cercando\
                       di osservare"
::msgcat::mcset it_it " what you try to watch is of type "\
                      "Stai cercando di osservare un oggetto di tipo "
::msgcat::mcset it_it " - this type is not supported by the debugger"\
                      " - questo tipo non viene supportato dal debugger"
::msgcat::mcset it_it "Type:" "Tipo:"
::msgcat::mcset it_it "(unsupported)" "(non supportato)"
::msgcat::mcset it_it "size:" "dimensione:"
::msgcat::mcset it_it "real" "reale"
::msgcat::mcset it_it "complex" "complesso"
::msgcat::mcset it_it "elements" "elementi"
::msgcat::mcset it_it "graphic handle" "rif. oggetto grafico"
::msgcat::mcset it_it "uncompiled function" "funzione non compilata"
::msgcat::mcset it_it "compiled function" "funzione compilata"
::msgcat::mcset it_it "library" "libreria"
::msgcat::mcset it_it "pointer" "puntatore"
::msgcat::mcset it_it "size implicit index" "indice a dimensione implicita"
::msgcat::mcset it_it "primitive" "primitiva"


# FormatWhereForWatch.sci
::msgcat::mcset it_it "Breakpoint" "Arresto"
::msgcat::mcset it_it "called at line" "alla linea"
::msgcat::mcset it_it "of exec file" "in un file exec"
::msgcat::mcset it_it "of execstr instruction" "in una istruzione execstr"
::msgcat::mcset it_it "called under pause" "chiamata durante \"pause\""
::msgcat::mcset it_it "of macro" "della macro"


# helps.tcl
::msgcat::mcset it_it "About" "A proposito"
::msgcat::mcset it_it "aboutme_message"\
                      "\n Originato da tknotepad di Joseph Acosta,\
                       joeja@mindspring.com.\n Modificato dal Consorzio\
                       Scilab.\n Migliorato da Enrico Segre 2003-2006\n e da\
                       François Vogel (in particolare il debugger)\
                       2004-2009.\n Traduzioni: \n   norvegese, Torbjørn\
                       Pettersen 2005; \n   polacco, Bartosz Kosiorek 2005;\
                       \n   svedese, Gunnar Lindholm 2005; \n   tedesco,\
                       Dirk Reusch 2005; \n   spagnolo, Juan Nicolás\
                       Quesada 2006;\n   cinese,  Yung-Jang Lee 2007;\n  \
                       danese, Kim B. Hansen, 2007."
::msgcat::mcset it_it "Scipad_is_released_under_GPL"\
                      "SciPad è rilasciato sotto la licenza GPL"


# infomessages.tcl
::msgcat::mcset it_it "Line:" "Linea:"
::msgcat::mcset it_it "Column:" "Colonna:"
#::msgcat::mcset fr_fr "Line" "Ligne" ; # now common
::msgcat::mcset it_it "in" "di"
::msgcat::mcset it_it "Logical line:" "Linea logica:"
::msgcat::mcset it_it " \[ReadOnly\]" " \[SolaLettura\]"
::msgcat::mcset it_it " (modified)" " (modificato)"


# inputtext.tcl
::msgcat::mcset it_it "No corresponding <" "Nessun <"
::msgcat::mcset it_it "> found!" "> corrispondente trovato!"
::msgcat::mcset it_it "Code editing is not allowed during debug!"\
                      "Impossible modificare il codice durante il debugging!"


# menues.tcl
::msgcat::mcset it_it "&File" "&File"
::msgcat::mcset it_it "&Edit" "&Modifica"
::msgcat::mcset it_it "&Search" "&Cerca"
::msgcat::mcset it_it "E&xecute" "&Esecuzione"
::msgcat::mcset it_it "&Debug" "&Debug"
::msgcat::mcset it_it "S&cheme" "&Schema"
::msgcat::mcset it_it "&Options" "&Opzioni"
::msgcat::mcset it_it "&Windows" "F&inestre"
::msgcat::mcset it_it "&Help" "&Aiuto"

# menues.tcl - File menu
::msgcat::mcset it_it "&New" "&Nuovo"
::msgcat::mcset it_it "&Open..." "&Apri..."
::msgcat::mcset it_it "Open &in new" "Apri &in un nuovo pannello"
::msgcat::mcset it_it "&horizontal tile" "a &fianco"
::msgcat::mcset it_it "&vertical tile" "di &sotto"
::msgcat::mcset it_it "Op&en source of..." "Apri sorgente di..."
::msgcat::mcset it_it "&Save" "&Salva"
::msgcat::mcset it_it "Save &as..." "Salva &come..."
::msgcat::mcset it_it "Save a&ll" "Salva &tutti"
::msgcat::mcset it_it "&Revert..." "&Recupera..."
::msgcat::mcset it_it "Import &Matlab file..." "Importa file &Matlab..."
::msgcat::mcset it_it "Create help s&keleton..."\
                      "Crea scheletro di aiuto &XML..."
::msgcat::mcset it_it "Compile as &help page" "Compila come file di ai&uto"
::msgcat::mcset it_it "Open &function source"\
                      "Apri la sorgente della &funzione"
::msgcat::mcset it_it "Print Se&tup" "Confi&gura stampante"
::msgcat::mcset it_it "&Print" "Stam&pa"
::msgcat::mcset it_it "&Close file" "Chiudere questo file"
::msgcat::mcset it_it "E&xit" "&Esci"

# menues.tcl - Edit menu
::msgcat::mcset it_it "&Undo" "&Annulla"
::msgcat::mcset it_it "&Redo" "&Ripristina"
::msgcat::mcset it_it "Cu&t" "&Taglia"
::msgcat::mcset it_it "B&lock cut" "Ta&glia blocco"
::msgcat::mcset it_it "&Copy" "&Copia"
::msgcat::mcset it_it "&Paste" "&Incolla"
::msgcat::mcset it_it "&Block paste" "Incolla &blocco"
::msgcat::mcset it_it "&Delete" "&Elimina"
::msgcat::mcset it_it "Select &All" "Sele&ziona tutto"
::msgcat::mcset it_it "Insert &Time/Date" "Inserisci &ora/data"
::msgcat::mcset it_it "Co&mment selection" "Co&mmenta la selezione"
::msgcat::mcset it_it "U&ncomment selection" "&Scommenta la selezione"
::msgcat::mcset it_it "&Indent selection" "&Indenta la selezione"
::msgcat::mcset it_it "Unin&dent selection" "Arretra la se&lezione"

# menues.tcl - Search menu
::msgcat::mcset it_it "&Find..." "&Trova..."
::msgcat::mcset it_it "Find &Next" "Trova il &seguente"
::msgcat::mcset it_it "&Replace..." "&Sostituisci..."
::msgcat::mcset it_it "&Goto Line..." "&Vai a linea..."

# menues.tcl - Exec menu
::msgcat::mcset it_it "&Load into Scilab" "&Carica in Scilab"
::msgcat::mcset it_it "&Evaluate selection" "&Esegui la selezione"

# menues.tcl - Debug menu
::msgcat::mcset it_it "&Insert/Remove breakpoint"\
                      "&Inserisci/rimuovi un arresto"
::msgcat::mcset it_it "&Edit breakpoints" "&Modifica condizioni d'arresto"
::msgcat::mcset it_it "Remove &all breakpoints" "Rimuovi &tutti gli arresti"
::msgcat::mcset it_it "&Configure execution..." "&Configura l'esecuzione..."
::msgcat::mcset it_it "Go to next b&reakpoint" "&Vai fino al prossimo arresto"
::msgcat::mcset it_it "&Step by step" "&Passo a passo"
::msgcat::mcset it_it "Step &into" "Entra &dentro"
::msgcat::mcset it_it "Step o&ver" "Passa s&opra"
::msgcat::mcset it_it "Step &out" "Esci f&uori"
::msgcat::mcset it_it "Run to re&turn point" "Esegui &fino al ritorno"
::msgcat::mcset it_it "Run to c&ursor" "Esegui fino al curso&re"
::msgcat::mcset it_it "G&o on ignoring any breakpoint"\
                      "Co&ntinua ignorando gli arresti"
::msgcat::mcset it_it "Show &watch" "Finestra &di controllo"
::msgcat::mcset it_it "&Break" "&Stop"
::msgcat::mcset it_it "Cance&l debug" "&Annulla il debug"

#    ::msgcat::mcset fr_fr "language scheme" "schéma langage"
# menues.tcl - Scheme menu
::msgcat::mcset it_it "S&cilab" "&Scilab"
::msgcat::mcset it_it "&XML" "&XML"
::msgcat::mcset it_it "&none" "&nessuno"
::msgcat::mcset it_it "&Colorize" "&Colora"

# menues.tcl - Options menu
::msgcat::mcset it_it "&Fonts..." "C&aratteri..."
::msgcat::mcset it_it "&Colors" "&Colori"
::msgcat::mcset it_it "Colori&ze" "C&olorare"
::msgcat::mcset it_it "&Always" "&Sempre"
::msgcat::mcset it_it "Ask for &big files" "&Chiedere per files grandi"
::msgcat::mcset it_it "&Never" "&Mai"
::msgcat::mcset it_it "Colorize \'&strings\'" "Colora &\'stringhe\'"
::msgcat::mcset it_it "Colorize &user functions" "Colora funzioni &utente"
::msgcat::mcset it_it "Show c&ontinued lines" "Colora linee &incomplete"
::msgcat::mcset it_it "Line n&umbers" "N&umeri di linea"
::msgcat::mcset it_it "&Hide" "&Nascosti"
::msgcat::mcset it_it "&Left aligned" "Allineati a &sinistra"
::msgcat::mcset it_it "&Right aligned" "Allineati a &destra"
::msgcat::mcset it_it "Word &wrap" "&Ritorno a capo visuale"
::msgcat::mcset it_it "&Double-click behavior" "Doppio clic &seleziona come"
::msgcat::mcset it_it "&Linux" "&Linux"
::msgcat::mcset it_it "Windo&ws" "Windo&w$"
::msgcat::mcset it_it "&Scilab" "&Scilab"
::msgcat::mcset it_it "&Tabs and indentation" "&Tabulatori e indentazione"
::msgcat::mcset it_it "Tab inserts &spaces" "&Tab inserisce spazi"
::msgcat::mcset it_it "&Indentation spaces" "&Spazi di indentazione"
::msgcat::mcset it_it "&Tab size (characters)" "&Numero spazi tabulazione"
::msgcat::mcset it_it "Use &keyword indentation" "&Indentazione automatica"
::msgcat::mcset it_it "Com&pletion" "Autocom&pletamento"
::msgcat::mcset it_it "&Tab" "&Tab"
::msgcat::mcset it_it "&Control-Tab" "&Control-Tab"
::msgcat::mcset it_it "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset it_it "&Shift-Tab" "&Shift-Tab"
::msgcat::mcset it_it "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
::msgcat::mcset it_it "S&hift-Control-Tab" "S&hift-Control-Tab"
::msgcat::mcset it_it "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
::msgcat::mcset it_it "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
::msgcat::mcset it_it "File&names" "Nome dei &files"
::msgcat::mcset it_it "&Full path" "&Percorso completo"
::msgcat::mcset it_it "Full path if &ambiguous" "Percorso completo se &ambiguo"
::msgcat::mcset it_it "&Unambiguous pruned path" "Percorso &ridotto univoco"
::msgcat::mcset it_it "Windows menu &sorting" "Ordine del &menu Finestre"
::msgcat::mcset it_it "&File opening order" "secondo &momento di apertura"
::msgcat::mcset it_it "&Alphabetically" "ordine &alfabetico"
::msgcat::mcset it_it "&Most recently used" "secondo &uso più recente"
::msgcat::mcset it_it "&Recent files" "&Numero files recenti"
::msgcat::mcset it_it "&Backup files depth" "Numero &versioni backup"
::msgcat::mcset it_it "&Locale" "&Localizzazione"
::msgcat::mcset it_it "Execution &errors" "&Errori d'esecuzione"
::msgcat::mcset it_it "In Scilab &shell only" "Solo nella finestra di &Scilab"
::msgcat::mcset it_it "Copied in a &message box"\
                      "Anche in una finestra di &dialogo"

# menues.tcl - Windows menu
::msgcat::mcset it_it "&Maximize" "&Massimizza"
::msgcat::mcset it_it "&Split" "Dividi in &orizzontale"
::msgcat::mcset it_it "S&plit (side by side)" "Dividi in &verticale"
::msgcat::mcset it_it "Sp&lit file" "&Dividi il file in orizzontale"
::msgcat::mcset it_it "Spl&it file (side by side)"\
                      "Dividi il file (&fianco a fianco)"
::msgcat::mcset it_it "Tile all &vertically" "&Impila tutti"
::msgcat::mcset it_it "Tile all &horizontally" "Affianca &tutti"
::msgcat::mcset it_it "Space sashes &evenly" "Spezzatura &uniforme"
::msgcat::mcset it_it "Opened Files" "Files aperti"

# menues.tcl - Help menu
::msgcat::mcset it_it "&Help..." "&Aiuto..."
::msgcat::mcset it_it "&What's?..." "&Cos'è?..."
::msgcat::mcset it_it "&About" "A &proposito"
::msgcat::mcset it_it "Adding &translations..." "&Tradurre Scipad..."
::msgcat::mcset it_it "Adding Scipad Translations..."\
                      "Creare una nuova traduzione di Scipad:"
::msgcat::mcset it_it "&Bugs \& wishlist" "&Bachi \& richieste"
::msgcat::mcset it_it "Scipad Known bugs" "Bachi noti in Scipad"
::msgcat::mcset it_it "Change&log" "Cronologia &modifiche"
::msgcat::mcset it_it "Changes in the Scipad codebase"\
                      "Cronologia delle modifiche al codice di Scipad"


# popupmenus.tcl
::msgcat::mcset it_it "Open the source of" "Apri la sorgente di"
::msgcat::mcset it_it "AddWatch" "Tieni %s sotto controllo"
::msgcat::mcset it_it "AddGenExp" "Aggiungere %s come espressione generica"
::msgcat::mcset it_it "Jump to" "Salta a"


# print.tcl
::msgcat::mcset it_it "Print Setup" "Configura stampante"
::msgcat::mcset it_it "Print Command: " "Comando di stampa:"


# scilabexec.tcl
::msgcat::mcset it_it "No point in loading an empty file!"\
                      "Inutile caricare un file vuoto!"
::msgcat::mcset it_it "Silent file save failed"\
                      "Salvataggio silente non riuscito"
::msgcat::mcset it_it "File could not be saved in a temporary\
                       location.\nOverwrite original file?"\
                      "Non è stato possibile copiare il file in una\
                       cartella temporanea. Sovrascrivere il file\
                       originale?"
::msgcat::mcset it_it "Exec done" "Exec terminato"
::msgcat::mcset it_it "Matlab files" "Files Matlab"
::msgcat::mcset it_it "Matlab file to convert" "File Matlab da importare"
::msgcat::mcset it_it "Scilab is converting, please hold on..."\
                      "Scilab sta traducendo, attendere..."
::msgcat::mcset it_it "Matlab file import" "Importazione di un file Matlab"
::msgcat::mcset it_it "Conversion of the file failed, see the Scilab window\
                       for details -- Perhaps report the error text and the\
                       offending Matlab file to"\
                      "La traduzione del file è fallita, vedere la finestra\
                       Scilab per dettagli -- Il baco può essere riportato\
                       includendo il file incriminato ed il messagggio di\
                       errore :"
::msgcat::mcset it_it "Please select destination path for the xml source of\
                       the help file"\
                      "Scegliere una cartella di destinazione per la\
                       sorgente XML della pagina di aiuto"
::msgcat::mcset it_it "Older version of" "Esiste già una versione di"
::msgcat::mcset it_it "found!" "!"
::msgcat::mcset it_it "An old version of" "Esiste una versione precedente di"
::msgcat::mcset it_it "already exists: open the old file instead?"\
                      ": apro il file esistente invece?"
::msgcat::mcset it_it "Existing file" "File esistente"
::msgcat::mcset it_it "New skeleton" "Crea nuovo scheletro"
::msgcat::mcset it_it "This feature is no longer available in Scilab 5 due\
                       to bug "\
                      "Questa funzionalità non è più disponibile in Scilab 5,\
                       per via del bug "
::msgcat::mcset it_it "See details at " "Maggiori informazioni in: "
::msgcat::mcset it_it "See also:" "Vedi anche:"
::msgcat::mcset it_it "Note: Scilab 4.x and Scilab-gtk are offering this\
                       feature."\
                      "Nota: Scilab 4.x e Scilab-gtk offrono questa\
                       funzionalità."
::msgcat::mcset it_it "A solution is to use a backported version of Scipad\
                       inside one of these environments, for instance:"\
                      "Una soluzione è utilizzare una versione\
                       \"backport\" di Scipad in una di queste versioni di Scilab,\
                       per esempio: "
::msgcat::mcset it_it "Feature missing from Scilab 5"\
                      "Funzionalità mancante in Scilab 5"
::msgcat::mcset it_it "ScilabEval command cannot be passed to Scilab!"\
                      "Un comando ScilabEval non ha potuto essere passato a\
                       Scilab!"
::msgcat::mcset it_it "impossibleScilabEval_message"\
                      "ATTENZIONE : Un comando non ha potuto essere passato\
                       a Scilab.  ed è stato ignorato\n\n  La ragione più\
                       probabile è che la cartella TMPDIR di Scilab non sia\
                       scrivibile. Ne può risultare un comportamento\
                       imprevedibile.\n\n Il comando in questione\
                       è:\n\n-->"
::msgcat::mcset it_it "impossibleScilabEval_message2"\
                      "ATTENZIONE : Un comando non ha potuto essere passato\
                       a Scilab ed è stato ignorato perché troppo lungo.\
                       Ne può risultare un comportamento imprevedibile.\n\n\
                       Se si è nel corso di una sessione di debug, provare\
                       a ridurre le variabili osservate. \n\n Il comando in\
                       questione è:\n\n-->"
::msgcat::mcset it_it "(end of command skipped)" "(resto del comando omesso)"
::msgcat::mcset it_it "Scilab working" "Scilab occupato"
::msgcat::mcset it_it "Scilab is working, please wait for the prompt to\
                       execute this command!"\
                      "Scilab è occupato, attendere il prompt per eseguire\
                       questo comando!"
::msgcat::mcset it_it "Scilab is working, wait for the prompt to load file"\
                      "Scilab è occupato, attendere il prompt per caricare\
                       il file"
::msgcat::mcset it_it "Scilab is working, wait for the prompt to execute the\
                       selection."\
                      "Scilab è occupato, attendere il prompt per eseguire\
                       la selezione"
::msgcat::mcset it_it "Scilab is working, wait for the prompt to convert a\
                       Matlab file."\
                      "Scilab è occupato, attendere il prompt per\
                       convertire un file Matlab"
::msgcat::mcset it_it "Scilab is working, wait for the prompt to compile the\
                       help file."\
                      "Scilab è occupato, attendere il prompt per compilare\
                       la pagina di aiuto"
::msgcat::mcset it_it "Scilab is working, wait for the prompt to issue a\
                       debugger command."\
                      "Scilab è occupato, aspettare il prompt per eseguire\
                       comandi di debug."
::msgcat::mcset it_it "Scilab execution error"\
                      "Errore di Scilab durante l'esecuzione"
::msgcat::mcset it_it "The shell reported an error while trying to execute "\
                      "Lo shell ha riportato un errore nel cercare di eseguire"
::msgcat::mcset it_it ": error " ": errore "
::msgcat::mcset it_it "at line " "alla linea "
::msgcat::mcset it_it " of " " di "
::msgcat::mcset it_it "Execution aborted!" "Esecuzione annullata!"


# scipad.tcl
::msgcat::mcset it_it "NOT CONNECTED TO SCILAB" "MANCA LA CONNESSIONE A SCILAB"


# whichfun.tcl
::msgcat::mcset it_it "Being at line" "Essendo a linea"
::msgcat::mcset it_it ", function" ", la funzione"
::msgcat::mcset it_it "begins at" "comincia a"
::msgcat::mcset it_it ", and there are" ", e ci sono"
::msgcat::mcset it_it "continued lines, i.e. we are at line"\
                      "linee continuate, cioè siamo alla linea"
::msgcat::mcset it_it "of" "di"
#::msgcat::mcset fr_fr "Function" "Fonction" ; # now global
::msgcat::mcset it_it "line" "linea"
::msgcat::mcset it_it "The cursor is not currently inside a function body."\
                      "Il cursore non si trova dentro il corpo di una funzione"
::msgcat::mcset it_it "Which function?" "Quale funzione?"


#preference variables description
::msgcat::mcset it_it "BGCOLOR" "Sfondo"
::msgcat::mcset it_it "BGLNMARGCOLOR" "Sfondo numeri linea"
::msgcat::mcset it_it "FGCOLOR" "Testo semplice"
::msgcat::mcset it_it "FGLNMARGCOLOR" "Testo numeri linea"
::msgcat::mcset it_it "CURCOLOR" "Cursore"
::msgcat::mcset it_it "PARCOLOR" "Parentesi tonde ()"
::msgcat::mcset it_it "BRAKCOLOR" "Parentesi quadre \\\[\\\]"
::msgcat::mcset it_it "BRACCOLOR" "Parentesi graffe {}"
::msgcat::mcset it_it "PUNCOLOR" "Punteggiatura"
::msgcat::mcset it_it "INTFCOLOR" "Funzioni interne"
::msgcat::mcset it_it "COMMCOLOR" "Comandi Scilab"
::msgcat::mcset it_it "SCICCOLOR" "Funzioni Scicos"
::msgcat::mcset it_it "OPCOLOR" "Operatori"
::msgcat::mcset it_it "QTXTCOLOR" "Stringhe"
::msgcat::mcset it_it "REMCOLOR" "Commenti"
::msgcat::mcset it_it "XMLCOLOR" "Tags XML"
::msgcat::mcset it_it "NUMCOLOR" "Numeri"
::msgcat::mcset it_it "SELCOLOR" "Sfondo selezione"
::msgcat::mcset it_it "BREAKPOINTCOLOR" "Punti d'arresto"
::msgcat::mcset it_it "LFUNCOLOR" "Funzioni di libreria"
::msgcat::mcset it_it "PDEFCOLOR" "Variabili predefinite"
::msgcat::mcset it_it "FOUNDTEXTCOLOR" "Testo trovato"
::msgcat::mcset it_it "CONTLINECOLOR" "Linea incompleta"
::msgcat::mcset it_it "USERFUNCOLOR" "Funzioni utente"


#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset it_it "&Directory:" "&Cartella"
::msgcat::mcset it_it "File &name:" "&Nome del file:"
::msgcat::mcset it_it "Files of &type:" "&Tipo di file:"
#::msgcat::mcset fr_fr "&Save" "&Enregistrer" # already included
::msgcat::mcset it_it "&Open" "A&pri"
::msgcat::mcset it_it "Open" "Apri"
::msgcat::mcset it_it "Save As" "Salva come"
::msgcat::mcset it_it "&Cancel" "&Annulla"
::msgcat::mcset it_it "&Selection:" "&Selezione:"
::msgcat::mcset it_it "&Yes" "&Si"
::msgcat::mcset it_it "&No" "&No"
::msgcat::mcset it_it "&Red" "&Rosso"
::msgcat::mcset it_it "&Green" "&Verde"
::msgcat::mcset it_it "&Blue" "&Blu"
::msgcat::mcset it_it "Directory \"%1\$s\" does not exist."\
                      "La cartella \"%1\$s\" non esiste."
