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

# Scipad texts in Catalan by Oriol Gonzalez (oriolsbd@yahoo.es)
# in sync at least with v6.155


# commons
::msgcat::mcset ca_es "Cancel" "Cancel·la"
::msgcat::mcset ca_es "Cance&l" "Cance&l·la"
::msgcat::mcset ca_es "OK" "D'acord"
::msgcat::mcset ca_es "All files" "Tots els fitxers"
::msgcat::mcset ca_es "Add/Change" "Afegeix/Modifica"
::msgcat::mcset ca_es "Add/Chan&ge" "Afegeix/&Modifica"
::msgcat::mcset ca_es "&Remove" "&Elimina"
::msgcat::mcset ca_es "Rem&ove" "El&imina"
::msgcat::mcset ca_es "Untitled" "Sense-Títol"
::msgcat::mcset ca_es "Close" "Tanca"
::msgcat::mcset ca_es "&Close" "&Tanca"
::msgcat::mcset ca_es "Error " "Error "
::msgcat::mcset ca_es "The file" "El fitxer"
::msgcat::mcset ca_es "Function" "Funció"
::msgcat::mcset ca_es "Line" "Línia"


# buffernavigation.tcl
::msgcat::mcset ca_es "Hide" "Amaga"
::msgcat::mcset ca_es "Close this tile, keep content"\
                      "Tanca el panell, mantenint el contingut"
::msgcat::mcset ca_es "Close this tile" "Tanca aquest panell"
::msgcat::mcset ca_es "Goto Line?" "Anar a la línia?"
::msgcat::mcset ca_es "Go to:" "Vés a:"
::msgcat::mcset ca_es "&logical line" "línia &lògica"
::msgcat::mcset ca_es "&physical line" "línia fí&sica"
::msgcat::mcset ca_es "in &function" "a la &funció"
::msgcat::mcset ca_es "in &current file" "del fitxer a&ctual"
::msgcat::mcset ca_es "Outside of function definition"\
                      "fora de la definició de la funció"


# colorize.tcl
::msgcat::mcset ca_es "Wait seconds while recolorizing file"\
                      "Espereu, s'està acolorint el fitxer"
::msgcat::mcset ca_es "You can't do that while colorization is in progress.\
                       Please try again when finished."\
                      "No podeu fer això mentre s'està acolorint el fitxer.\
                       Proveu-ho de nou quan s'acabi."
::msgcat::mcset ca_es "Command forbidden during colorization"\
                      "Aquesta comanda és prohibida durant l'acoloriment"


# completion.tcl
::msgcat::mcset ca_es "No possible completion" "No s'ha pogut completar"
::msgcat::mcset ca_es "possible completions" "s'ha pogut completar"
::msgcat::mcset ca_es "Possible completions" "S'ha pogut completar"


# db_argsmanagement.tcl
::msgcat::mcset ca_es "Add" "Afegeix"
::msgcat::mcset ca_es "This variable can be watched but cannot be edited!"\
                      "Es pot fer seguiment d'aquesta variabile, però no\
                       modificar-la."
::msgcat::mcset ca_es "A new watch variable will be created."\
                      "Es crearà un nou seguiment de variable."
::msgcat::mcset ca_es "Non editable variable" "Variable no modificable"
::msgcat::mcset ca_es "Variable:" "Variable:"
::msgcat::mcset ca_es "Expression:" "Expressió:"
::msgcat::mcset ca_es "Value:" "Valor:"
::msgcat::mcset ca_es "&Get current value from Scilab"\
                      "Recupera el valor &actual de l'Scilab"
::msgcat::mcset ca_es "Non watchable variable" "No es pot fer seguiment de la\
                       variable"
::msgcat::mcset ca_es "Sorry, variable" "La variable"
::msgcat::mcset ca_es "is special and cannot be watched."\
                      "és especial i se'n pot fer seguiment."


# db_bptsgui.tcl
::msgcat::mcset ca_es "Breakpoints properties" "Propietats del punt d'interrupció"
::msgcat::mcset ca_es "Enabled" "Activat"
::msgcat::mcset ca_es "Expression" "Expressió"
::msgcat::mcset ca_es "Hit if" "Accedida si"
::msgcat::mcset ca_es "istrue" "és cert"
::msgcat::mcset ca_es "haschanged" "ha canviat"
::msgcat::mcset ca_es "Watched" "Seguit"
::msgcat::mcset ca_es "Hit count" "nombre d'accessos"
::msgcat::mcset ca_es "Break if" "Atura si"
::msgcat::mcset ca_es "Threshold" "Llindar"
::msgcat::mcset ca_es "always" "sempre"
::msgcat::mcset ca_es "equalto" "és igual a"
::msgcat::mcset ca_es "multipleof" "és multiple de"
::msgcat::mcset ca_es "greaterthan" "és major que"
::msgcat::mcset ca_es "lessthan" "és menor que"
::msgcat::mcset ca_es "Reset hit" "reinicia els accessos"
::msgcat::mcset ca_es "Breakpoints vocabulary"\
                      "Vocabulari de punts d'interrupció"
::msgcat::mcset ca_es "A breakpoint is identified by its location, i.e. its\
                       position (a line number) in a function."\
                      "Els punts d'aturada s'identifiquen per la seva ubicació,\
                       o sigui, pel seu número de línia a la funció."
::msgcat::mcset ca_es "When the execution process encounters an enabled\
                       breakpoint, then this breakpoint is said to be\
                       reached."\
                      "Quan un procés en execució es troba un punt\
                       d'interrupció activat, es diu que s'ha arribat a aquest\
                       punt d'interrupció."
::msgcat::mcset ca_es "When the breakpoint is reached and its associated\
                       conditional expression is true (or changed), then the\
                       breakpoint is said to be hit."\
                      "Quan s'arriba a un punt d'interrupció, i la seva\
                       expressió condicional és certa (o ha canviat), es diu\
                       que s'ha accedit al punt d'interrupció."

::msgcat::mcset ca_es "The hit count is the number of times the breakpoint\
                       has been hit."\
                      "El nombre d'accessos és el nombre de cops que s'ha\
                       accedit a un punt d'interrupció."
::msgcat::mcset ca_es "Execution will stop at a breakpoint if the hit count\
                       satisfies the selectable break condition."\
                      "S'aturarà l'execució en un punt d'interrupció si el\
                       nombre d'accessos satisfà la condició escollida."


# db_bptsmanagement.tcl
::msgcat::mcset ca_es "No breakpoint here!"\
                      "No es pot posar un punt d'interrupció aquí."
::msgcat::mcset ca_es "Delete all breakpoints"\
                      "Elimina tots els punts d'interrupció"
::msgcat::mcset ca_es "Are you sure you want to delete all the existing\
                       breakpoints?"\
                      "Esteu segur que voleu eliminar tots els punts\
                       d'interrupció?"
::msgcat::mcset ca_es "All breakpoints deleted"\
                      "S'ha eliminat tots els punts d'interrupció"
::msgcat::mcset ca_es "No action taken" "No s'ha realitzat cap acció"
::msgcat::mcset ca_es "No breakpoint!" "No hi ha cap punt d'interrupció."


# db_configureexec.tcl
::msgcat::mcset ca_es "Configure execution" "Configura l'execució"
::msgcat::mcset ca_es "Function name:" "Nom de la funció:"
::msgcat::mcset ca_es "&Obtain" "&Obtenir"
::msgcat::mcset ca_es "Input arguments:" "Arguments d'entrada:"
::msgcat::mcset ca_es "The selected function has not been found in the\
                       opened files, maybe it was deleted since last\
                       configuration of the debugger.\nUse button \""\
                      "No s'ha trobat la funció seleccionada als fitxers\
                       oberts, potser s'ha eliminat des de la darrera\
                       configuració del depurador.\nUtilitzeu el botó\""

::msgcat::mcset ca_es "\" to refresh the list of available functions."\
                      "\" per refrescar la llista de funcions disponibles."
::msgcat::mcset ca_es "Function name or input arguments do not match\
                       definition of the function"\
                      "El nom de la funció o els arguments d'entrada no es\
                       corresponen amb la definició de la funció"
::msgcat::mcset ca_es "in the file!\n\nCheck function name and arguments\
                       (names, order) in the configuration dialog.\
                       \nArguments order can be changed using drag and drop\
                       with right mouse button in the arguments listbox."\
                      "en el fitxer\n\nComproveu el nom de la funció i els\
                       arguments (nom, ordre) al diàleg de configuració.\n\
                       L'ordre dels arguments es pot canviar arrossegant amb\
                       el botó dret del ratolí."
::msgcat::mcset ca_es "Error in selected function name or arguments"\
                      "Error en el nom de la funció o els arguments\
                       seleccionats"
::msgcat::mcset ca_es "At least one input argument does not match definition\
                       of the function"\
                      "Com a mínim un argument d'entrada no es correspon amb\
                       la definició de la funció"
::msgcat::mcset ca_es "in the file!\n\nCheck function arguments (their\
                       names) in the configuration dialog."\
                      "al fitxer.\n\nComproveu els noms dels arguments de la\
                       funció en el diàleg de configuració."
::msgcat::mcset ca_es "Error in selected function arguments"\
                      "Error en els arguments de la funció seleccionats"
::msgcat::mcset ca_es "Main level code found" "Codi de nivell principal trobat"
::msgcat::mcset ca_es "Scipad ancillary file" "Fitxer auxiliar de l'Scipad"
::msgcat::mcset ca_es "The current file contains only Scipad ancillaries\
                       that cannot be debugged in Scipad."\
                      "El fitxer actual només conté funcions auxiliars de\
                       Scipad que no es poden depurar."
::msgcat::mcset ca_es "Reserved function names are:\n"\
                      "Els noms de funció reservats són:\n"
::msgcat::mcset ca_es "See help scipad for further details."\
                      "Consulteu l'ajuda de l'Scipad per a més detalls."
::msgcat::mcset ca_es "This file contains main level code.\nDebug as a\
                       single .sce file?"\
                      "Aquest fitxer conté codi de nivell principal.\nVoleu\
                       depurar-lo com un sol fitxer .sce?"
::msgcat::mcset ca_es "No function found" "No s'ha trobat cap funció"
::msgcat::mcset ca_es "This file contains no function and will be treated"\
                      "Aquest fitxer no conté cap funció i es tractarà"
::msgcat::mcset ca_es "as a single .sce file.\nStart debugging?"\
                      "com un sol fitxer .sce.\nVoleu començar a depurar-lo?"
::msgcat::mcset ca_es "You must enable colorization to be able to launch the\
                       debugger!"\
                      "Heu d'activar l'acoloriment per poder depurar."
::msgcat::mcset ca_es "The debugger needs colorization"\
                      "El depurador necessita l'acoloriment"
::msgcat::mcset ca_es "Warning!\n\n  A function missing the closing\
                       \"endfunction\" has been found in the currently\
                       opened files.\n  Such syntax is now obsolete.\n\n \
                       Please terminate function "\
                      "Avís!\n S'ha trobat una funció que no té el\
                       \"endfunction\" final en els fitxers oberts.\n Aquesta\
                        sintaxi és obsoleta.\n\nFinalitzeu la funció "
::msgcat::mcset ca_es " and try again." " i torneu-ho a provar."
::msgcat::mcset ca_es "Note: This message might also be triggered by a\
                       string\n  quoted with single quotes and containing\
                       the word \"function\",\n  when \"Colorize\
                       \'strings\'\" is unchecked in the Options menu."\
                      "Nota: Aquest missatge també pot haver estat provocat per\
                       una\ncadena entre cometes simples, i que conté la paraula\
                       \"function\",\nquan està desmarcada l'opció de menú\
                       \"Acoloreix \'&cadenes\'\"."
::msgcat::mcset ca_es "This known bug can be worked around simply by\
                       checking \"Colorize \'&strings\'\"."\
                      "Aquest error es pot saltar marcant l'opció \"Acoloreix\
                       \'&cadenes\'\"."
::msgcat::mcset ca_es "Unterminated function definition found"\
                      "S'ha trobat una funció sense finalització"
::msgcat::mcset ca_es "Warning!\n\n  One or more function is defined more\
                       than once in the currently opened files.\n  The\
                       debugger cannot cope with multiple copies of\
                       functions.\n  The following duplicates were\
                       detected:\n"\
                      "Avís!\n\n Hi ha una o més funcions definides diversos\
                       cops als fitxers oberts.\nEl depurador no pot treballar\
                       amb múltiples còpies de les funcions.\nS'ha detectat\
                       els següents duplicats:\n"
::msgcat::mcset ca_es "\n--> Definition of function "\
                      "\n--> Definició de la funció "
::msgcat::mcset ca_es " found in:\n" " trobada a:\n"
::msgcat::mcset ca_es "Duplicate function definitions found"\
                      "S'ha trobat definicions de funcions duplicades"


# db_debugsession.tcl
::msgcat::mcset ca_es "Too many breakpointed functions"\
                      "Hi ha massa funcions amb punts d'interrupció"
::msgcat::mcset ca_es "You have currently" "Actualment, teniu"
::msgcat::mcset ca_es "functions in your opened files." "funcions."
::msgcat::mcset ca_es "Scilab supports a maximum of"\
                      "Scilab suporta com a màxim"
::msgcat::mcset ca_es "possible breakpointed functions (see help setbpt)."\
                      "funcions amb punts d'interrupció (veieu help setbpt)."
::msgcat::mcset ca_es "Step-by-step hence cannot be performed."\
                      "Il modo passo-passo non è pertanto possible."
::msgcat::mcset ca_es "This command will actually run to the next breakpoint."\
                      "Aquesta comanda anirà fins al següent punt d'interrupció."
::msgcat::mcset ca_es "Break hence cannot be performed."\
                      "No es pot realitzar la interrupció."
::msgcat::mcset ca_es "Too many breakpoints" "Hi ha massa punts d'interrupció"
::msgcat::mcset ca_es "Executing this command would require to set"\
                      "L'execució d'aquesta comanda implicaria definir"
::msgcat::mcset ca_es "breakpoints in your opened files."\
                      "punts d'interrupció als vostres fitxers oberts."
::msgcat::mcset ca_es "possibles de punts d'interrupció a l'Scilab\
                       (veieu help setbpt)."
::msgcat::mcset ca_es "Cursor must be in a function"\
                      "El cursor ha de ser en una funció"
::msgcat::mcset ca_es "This command cannot succeed if the function where the\
                       debug just stopped is not opened in Scipad!"\
                      "No es pot executar aquesta comanda si la funció on\
                       s'ha aturat la depuració no està obert a l'Scipad."
::msgcat::mcset ca_es "Cannot run to return point"\
                      "No es pot executar fins al punt de retorn"
::msgcat::mcset ca_es "Warning!\nThe line where the debugger just stopped\
                       contains a return statement (either endfunction,\
                       return or resume) but this statement is not the first\
                       one on the line."\
                      "Avís!\nLa línia on s'ha aturat el depurador conté una\
                       sentència de retorn (endfunction, return o resume) però\
                       aquesta no és la primera sentència de la línia."
::msgcat::mcset ca_es "As a consequence the debugger cannot predict if the\
                       next debug step will really jump outside of the\
                       current function, which is what you just asked the\
                       debugger to do."\
                      "El depurador no pot preveure si el següent pas de\
                       depuració saltarà fora de la funció actual, que és el que\
                       heu demanat al depurador que faci."
::msgcat::mcset ca_es "To avoid this unresolvable case, it is recommended to\
                       place the return statement alone on its line."\
                      "Per evitar aquest cas sense solució, es recomanda situar\
                       la sentència de retorn sol en una línia."
::msgcat::mcset ca_es "Do you want to see this warning again during this\
                       Scipad session?"\
                      "Voleu tornar a veure aquest missatge durant aquesta\
                       sessió de Scipad?"
::msgcat::mcset ca_es "Unresolvable case found while running to return point"\
                      "S'ha trobat un cas sense solució en el punt de retorn"
::msgcat::mcset ca_es "No effect - The debugged file is not stuck"\
                      "Sense efecte - El fitxer depurat no està agafat"

# db_init.tcl
::msgcat::mcset ca_es "Please wait..." "Espereu..."


# db_states.tcl
::msgcat::mcset ca_es "Currently no debug session"\
                      "No hi ha cap sessió actual de depuració"
::msgcat::mcset ca_es "Ready to start debug" "Es pot començar a depurar"
::msgcat::mcset ca_es "Debug in progress" "S'està depurant"
::msgcat::mcset ca_es "Scilab is back at the main level now."\
                      "L'Scilab ha tornat al nivell principal."


# db_watch.tcl
::msgcat::mcset ca_es "Watch" "Seguiment"
::msgcat::mcset ca_es "Watch variables:" "Variables en seguiment:"
::msgcat::mcset ca_es "A&uto (local variables)"\
                      "A&uto (totes les variables locals)"
::msgcat::mcset ca_es "Au&to watch globals too"\
                      "Seguiment &automàtic de les variables globals"
::msgcat::mcset ca_es "Generic expressions:" "Expressions genèriques:"
::msgcat::mcset ca_es "Call stack:" "Crides a la pila:"
::msgcat::mcset ca_es "Always on to&p" "Sempre a so&bre"
::msgcat::mcset ca_es "Hide watch &variables" "Amaga el seguiment de &variables"
::msgcat::mcset ca_es "Hide e&xpressions" "Amaga les &expressions"
::msgcat::mcset ca_es "Hide call &stack" "Amaga les crides a la &pila"
::msgcat::mcset ca_es "&Dock" "A&cobla"
::msgcat::mcset ca_es "&Add" "Afe&geix"
::msgcat::mcset ca_es "&Filter errors" "&Filtra els errors"
::msgcat::mcset ca_es "Breakpoints" "Punts d'interrupció"
::msgcat::mcset ca_es "Breakpointed functions" "Funcions amb interrupcions"
::msgcat::mcset ca_es "Scilab busy indicator"\
                      "indicador d'ocupació de l'Scilab"
::msgcat::mcset ca_es "Debugger state indicator"\
                      "indicador de l'estat del depurador"
::msgcat::mcset ca_es "Undefined elements are not legal in variable"\
                      "Els elements indefinits són invàlids en aquesta variable"
::msgcat::mcset ca_es ".\nThis variable will not be updated in Scilab."\
                      ".\nAquesta variable no s'actualitzarà a l'Scilab"
::msgcat::mcset ca_es "Illegal undefined element found"\
                      "S'ha trobat un element indefinit"
::msgcat::mcset ca_es "evaluates to:" "resulta en:"
::msgcat::mcset ca_es "produces error" "produeix un error"
::msgcat::mcset ca_es "Scipad cannot find source code for the selected\
                       function.\nPlease open the adequate file manually and\
                       try again."\
                      "L'Scipad no ha trobat el codi de la funció seleccionada.\
                       \nObriu el fitxer adequat manualment i torneu-ho a\
                       provar."
::msgcat::mcset ca_es "Missing source code" "No s'ha trobat el codi font"


# filecommands.tcl
::msgcat::mcset ca_es "New script" "Nou script"
::msgcat::mcset ca_es "New view on the same file"\
                      "Nou panell al mateix fitxer"
::msgcat::mcset ca_es "The contents of" "El contingut de"
::msgcat::mcset ca_es "may have changed, do you wish to save your changes?"\
                      "pot haver canviat, voleu desar els canvis?"
::msgcat::mcset ca_es "Save Confirm?" "Confirmar els canvis?"
::msgcat::mcset ca_es "Open source of..." "Obre la font de..."
::msgcat::mcset ca_es "Open source of:" "Obre la font de:"
::msgcat::mcset ca_es "Weird drag&drop" "Arrosse&gat estrany"
::msgcat::mcset ca_es "is neither a valid filename nor a valid directory.\
                       Either you're dragging an object of another type, or\
                       you hit a bug of the dnd mechanism."\
                      "no és un directori ni un nom vàlid de fitxer. O bé\
                       esteu arrossegant un objecte d'un altre tipus, o bé heu
                       trobat un error al mecanisme d'arrossegar."
::msgcat::mcset ca_es "File not found" "No s'ha trobat el fitxer"
::msgcat::mcset ca_es "does not exist anymore. Do you want to create an\
                       empty file with the same name?"\
                      "ja no existeix. Voleu crear un fitxer buit amb el\
                       mateix nom?"
::msgcat::mcset ca_es "Open file" "Obre el fitxer"
#::msgcat::mcset ca_es "The file" "El fitxer" ;# defined in the "commons" section
::msgcat::mcset ca_es "is already opened! Save the current opened file to\
                       another name and reopen it from disk!"\
                      "ja és obert. Deseu el fitxer actual amb un altre nom i\
                       torneu-lo a obrir des del disc."
::msgcat::mcset ca_es "File" "Fitxer"
::msgcat::mcset ca_es "saved" "desat"
::msgcat::mcset ca_es "cannot be written!" "no es pot escriure."
::msgcat::mcset ca_es "The contents of " "El contingut de"
::msgcat::mcset ca_es "has changed on disk, save it anyway?"\
                      "ha canviat al disc, el voleu desar igualment?"
::msgcat::mcset ca_es "The contents and/or properties of "\
                      "El contingut i/o les propietats de "
::msgcat::mcset ca_es "has been modified outside of Scipad. Do you want to\
                       reload it?"\
                      "s'ha modifcat des de fora de l'Scipad. Voleu tornar-lo a\
                       carregar?"
::msgcat::mcset ca_es "File has changed!" "Ha canviat el fitxer"
::msgcat::mcset ca_es "Save as" "Anomena i desa"
::msgcat::mcset ca_es "Scilab files" "Fitxers de l'Scilab"
::msgcat::mcset ca_es "Scicos files" "Fitxers de l'Scicos"
::msgcat::mcset ca_es "XML files" "Fitxers XML"
::msgcat::mcset ca_es "Wait seconds while loading and colorizing file"\
                      "Espereu mentre es carrega i s'acoloreix el fitxer"
::msgcat::mcset ca_es "The contents of this file may have changed, do you\
                       wish to save your changes?"\
                      "El contingut d'aquest fitxer pot haver canviat, voleu\
                       desar-lo?"
::msgcat::mcset ca_es "Revert" "Tornar el"
::msgcat::mcset ca_es "to saved?" "a la versió desada?"
::msgcat::mcset ca_es "Revert Confirm?" "Confirmeu la tornada?"
::msgcat::mcset ca_es "Unreadable file" "El fitxer no es pot llegir!"
::msgcat::mcset ca_es "exists but is not readable!"\
                      "existeix, però no es pot llegir!"
::msgcat::mcset ca_es "This file is rather large and colorization might take\
                       some time.\n\nColorize anyway?"\
                      "Aquest fitxer és força gran, i l'acoloriment pot durar\
                       una estona.\nVoleu realitzar-ho igualment?"
::msgcat::mcset ca_es "File size warning" "Avís per la mida del fitxer"
::msgcat::mcset ca_es "Windows shortcut file" "Drecera de Windows"
#::msgcat::mcset ca_es "The file" "El fitxer" ;# defined in the "commons" section
::msgcat::mcset ca_es "is a Windows shortcut and will not be opened!"\
                      "és una drecera de Windows i no es pot obrir."


# findreplace.tcl
::msgcat::mcset ca_es "Dialog box already open!"\
                      "Aquesta finestra ja està oberta."
::msgcat::mcset ca_es "The string" "La cadena '"
::msgcat::mcset ca_es "could not be found in" "' no s'ha trobat a"
::msgcat::mcset ca_es "could not be found in any of the opened files !"\
                      "' no s'ha trobat a cap dels fitxers oberts."
::msgcat::mcset ca_es "replacements done" "reemplaçaments fets"
::msgcat::mcset ca_es "Find" "Cerca"
::msgcat::mcset ca_es "No match found in the selection for"\
                      "No s'ha trobat cap coincidència de"
::msgcat::mcset ca_es "\nWould you like to look for it in the entire text?"\
                      "\nVoleu cercar a la resta del text?"
::msgcat::mcset ca_es "You are searching for an empty string!"\
                      "La cadena de cerca és buida"
::msgcat::mcset ca_es "Re&place" "&Reemplaça"
::msgcat::mcset ca_es "Find what:" "Cerca:"
::msgcat::mcset ca_es "Replace with:" "Reemplaça per:"
::msgcat::mcset ca_es "Find &Next" "T&orna a cercar"
::msgcat::mcset ca_es "Replace &All" "Reemplaça-ho &tot"
::msgcat::mcset ca_es "Direction" "Direcció"
::msgcat::mcset ca_es "&Upwards" "Cap &amunt"
::msgcat::mcset ca_es "&Downwards" "Cap a&vall"
::msgcat::mcset ca_es "Text types" "Tipus de text"
::msgcat::mcset ca_es "Find options" "Opcions de cerca"
::msgcat::mcset ca_es "Match &whole word only" "Només la paraula sen&cera"
::msgcat::mcset ca_es "Match &case" "Respecta &majúscules"
::msgcat::mcset ca_es "&Regular expression" "Expressió &regular"
::msgcat::mcset ca_es "In all &opened files" "A tots els &fitxers oberts"
::msgcat::mcset ca_es "In &selection only" "Només a la se&lecció"
::msgcat::mcset ca_es "In typed te&xt" "Als fitxers d'un ti&pus"
::msgcat::mcset ca_es "In a director&y" "En un &directori:"
::msgcat::mcset ca_es "Find in files search options"\
                      "Opcions de cerca en fitxers"
::msgcat::mcset ca_es "Directory r&ecurse search" "Cerca r&ecursiva"
::msgcat::mcset ca_es "Include &hidden files" "Inclou els fitxers &amagats"
::msgcat::mcset ca_es "In files/file types:" "Als fitxers/tipus de fitxer"
::msgcat::mcset ca_es "In directory:" "En el directori:"
::msgcat::mcset ca_es "Search for file names" "Cerca els noms de fitxer"
::msgcat::mcset ca_es "You have given an empty search string. Are you sure\
                       you want to search among file names?"\
                      "Heu entrat una cadena buida. Esteu segur que voleu cercar\
                       entre els noms de fitxer?"
::msgcat::mcset ca_es "Wrapped around" "Continuarà des del començament"
::msgcat::mcset ca_es "Back at the first file searched!"\
                      "Es tornarà a cercar des del primer fitxer."
::msgcat::mcset ca_es "No more matches" "No hi ha més coincidències"
::msgcat::mcset ca_es "The string to regexp search for cannot be compiled by\
                       Tcl.\nCheck again, maybe certain characters should be\
                       escaped."\
                      "La cadena amb expressions regulars a cercar no es pot\
                       compilar.\nComproveu si hi ha algun caràcter que hauria\
                       d'anar precedit per \'\\\'."
::msgcat::mcset ca_es "Any single character" "Qualsevol caràcter"
::msgcat::mcset ca_es "Zero or more" "Zero o més"
::msgcat::mcset ca_es "One or more" "U o més"
::msgcat::mcset ca_es "Zero or one" "Zero o u"
::msgcat::mcset ca_es "Any one character in the set"\
		      "Qualsevol caràcter dels indicats"
::msgcat::mcset ca_es "Any one character not in the set"\
                      "Qualsevol caràcter excepte els indicats"
::msgcat::mcset ca_es "Or" "O"
::msgcat::mcset ca_es "Beginning of line" "Inici de la línia"
::msgcat::mcset ca_es "End of line" "Final de la línia"
::msgcat::mcset ca_es "Beginning of word" "Inici de la paraula"
::msgcat::mcset ca_es "End of word" "Final de la paraula"
::msgcat::mcset ca_es "A letter" "Lletra"
::msgcat::mcset ca_es "A decimal digit" "Dígit"
::msgcat::mcset ca_es "An alphanumeric (letter or digit)"\
                      "Caràcter alfanumèric (lletra o dígit)"
::msgcat::mcset ca_es "A space or tab character" "Espai o tabulació"
::msgcat::mcset ca_es "A rational number" "Nombre racional"
::msgcat::mcset ca_es "A floating point number" "Nombre en coma flotant"
::msgcat::mcset ca_es "A Scilab comment" "Comentari de l'Scilab"
::msgcat::mcset ca_es "A Scilab name" "Nom de l'Scilab"
::msgcat::mcset ca_es "A Scilab continued line (with dots)"\
                      "Línia de l'Scilab continuada amb punts"


# findinfiles.tcl
::msgcat::mcset ca_es "Directory access issue"\
                      "Error d'accés al directori"
::msgcat::mcset ca_es "The directory you specified cannot be reached. The\
                       search won't bring any result."\
                      "No es pot accedir al directori especificat. La cerca\
                       no donarà cap resultat."
::msgcat::mcset ca_es "Some directories or files could not be open and were\
                       ignored during search.\n You might miss read access\
                       to them.\n\n The following ones were ignored"\
                      "No s'ha pogut obrir algun directori o fitxer, i s'ignoraran\
                       en la cerca.\nPot ser que no hi tingueu permisos de lectura.\
                       \n\nS'ha ignorat els següents:"
::msgcat::mcset ca_es "Ignored files" "Fitxers ignorats"
::msgcat::mcset ca_es "Searching in file:" "Cercant a:"
::msgcat::mcset ca_es "End of file search:" "Final de la cerca:"
::msgcat::mcset ca_es "matches found" "coincidències trobades"
::msgcat::mcset ca_es "searched files" "fitxers cercats"
::msgcat::mcset ca_es "Constructing file list:"\
                      "Construint la llista de fitxers:"
::msgcat::mcset ca_es "&Previous" "&Anterior"
::msgcat::mcset ca_es "&Next" "&Següent"
::msgcat::mcset ca_es "Scipad search results" "Resultats de la cerca"


# fonts.tcl
::msgcat::mcset ca_es "Choose font" "Escolliu la lletra"
::msgcat::mcset ca_es "Family" "Família"
::msgcat::mcset ca_es "Style" "Estil"
::msgcat::mcset ca_es "&Bold" "&Negreta"
::msgcat::mcset ca_es "&Italic" "&Cursiva"
::msgcat::mcset ca_es "&Underline" "&Subratllat"
::msgcat::mcset ca_es "Size" "Mida"
::msgcat::mcset ca_es "&Defaults" "&Predeterminada"
::msgcat::mcset ca_es "Use as..." "Utilitzeu a..."
::msgcat::mcset ca_es "&Text font" "Caràcters del &text"
::msgcat::mcset ca_es "&Menu font" "Caràcters del &menú"
::msgcat::mcset ca_es "Sample" "Mostra"
::msgcat::mcset ca_es "Font size" "Mida de la lletra"
::msgcat::mcset ca_es "Revert fonts" "Lletra predeterminada"
::msgcat::mcset ca_es "Are you sure you want to revert all fonts to defaults?"\
                      "Esteu segur que voleu tornar a la lletra predeterminada?"


# FormatStringsForWatch.sci
::msgcat::mcset ca_es " what you try to watch is not supported by the\
                       debugger"\
                      "El depurador no suporta el seguiment que voleu fer"
::msgcat::mcset ca_es " what you try to watch is of type "\
                      "El que voleu seguir és del tipus  "
::msgcat::mcset ca_es " - this type is not supported by the debugger"\
                      " - aquest tipus no està suportat pel depurador"
::msgcat::mcset ca_es "Type:" "Tipus:"
::msgcat::mcset ca_es "(unsupported)" "(no suportat)"
::msgcat::mcset ca_es "size:" "mida:"
::msgcat::mcset ca_es "real" "real"
::msgcat::mcset ca_es "complex" "complex"
::msgcat::mcset ca_es "elements" "elements"
::msgcat::mcset ca_es "graphic handle" "tractament gràfic"
::msgcat::mcset ca_es "uncompiled function" "funció no compilada"
::msgcat::mcset ca_es "compiled function" "funció compilada"
::msgcat::mcset ca_es "library" "llibreria"
::msgcat::mcset ca_es "pointer" "punter"
::msgcat::mcset ca_es "size implicit index" "índex de dimensió implícita"
::msgcat::mcset ca_es "primitive" "primitiva"


# FormatWhereForWatch.sci
::msgcat::mcset ca_es "Breakpoint" "Punt d'interrupció"
::msgcat::mcset ca_es "called at line" "a la línia"
::msgcat::mcset ca_es "of exec file" "del fitxer exec"
::msgcat::mcset ca_es "of execstr instruction" "de la instrucció execstr"
::msgcat::mcset ca_es "called under pause" "cridada en una pausa"
::msgcat::mcset ca_es "of macro" "de la macro"


# helps.tcl
::msgcat::mcset ca_es "About" "Quant a"
::msgcat::mcset ca_es "aboutme_message"\
                      "\n Creat a partir del tknotepad de Joseph Acosta,\
                       joeja@mindspring.com.\n Modificat pel Consorci\
                       Scilab.\n Revisat per Enrico Segre 2003-2006\n i per\
                       François Vogel (sobretot el depurador)\
                       2004-2009.\n\
                       Traducció al suec per Gunnar Lindholm 2005.\n\
                       Traducció al norueg per Torbjørn Pettersen 2005.\n\
                       Traducció a l'alemany per Dirk Reusch 2005.\n\
                       Traducció al polac per Bartosz Kosiorek 2005.\n\
                       Traducció a l'espanyol per Juan Nicolás Quesada 2006.\n\
                       Traducció al xinès per Yung-Jang Lee 2007.\n\
                       Traducció al danès per Kim B. Hansen 2007.\n\
                       Traducció al català per Oriol González 2009."

::msgcat::mcset ca_es "Scipad_is_released_under_GPL"\
                      "L'SciPad s'ha llençat sota la llicència GPL"


# infomessages.tcl
::msgcat::mcset ca_es "Line:" "Línia:"
::msgcat::mcset ca_es "Column:" "Columna:"
#::msgcat::mcset ca_es "Line" "Línia" ; # now common
::msgcat::mcset ca_es "in" "de"
::msgcat::mcset ca_es "Logical line:" "Línia lògica:"
::msgcat::mcset ca_es " \[ReadOnly\]" " \[NomésLectura\]"
::msgcat::mcset ca_es " (modified)" " (modificat)"


# inputtext.tcl
::msgcat::mcset ca_es "No corresponding <" "No s'ha trobat la correspondència a <"
::msgcat::mcset ca_es "> found!" ">."
::msgcat::mcset ca_es "Code editing is not allowed during debug!"\
                      "No es pot editar el codi durant la depuració."


# menues.tcl
::msgcat::mcset ca_es "&File" "&Fitxer"
::msgcat::mcset ca_es "&Edit" "&Edita"
::msgcat::mcset ca_es "&Search" "&Cerca"
::msgcat::mcset ca_es "E&xecute" "E&xecuta"
::msgcat::mcset ca_es "&Debug" "&Depura"
::msgcat::mcset ca_es "S&cheme" "E&squema"
::msgcat::mcset ca_es "&Options" "&Opcions"
::msgcat::mcset ca_es "&Windows" "F&inestra"
::msgcat::mcset ca_es "&Help" "&Ajuda"

# menues.tcl - File menu
::msgcat::mcset ca_es "&New" "&Nou"
::msgcat::mcset ca_es "&Open..." "&Obre..."
::msgcat::mcset ca_es "Open &in new" "Obre &en un nou panell"
::msgcat::mcset ca_es "&horizontal tile" "&horitzontal"
::msgcat::mcset ca_es "&vertical tile" "&vertical"
::msgcat::mcset ca_es "Op&en source of..." "O&bre la font de..."
::msgcat::mcset ca_es "&Save" "&Desa"
::msgcat::mcset ca_es "Save &as..." "&Anomena i desa..."
::msgcat::mcset ca_es "Save a&ll" "Desa-ho &tot"
::msgcat::mcset ca_es "&Revert..." "&Recupera..."
::msgcat::mcset ca_es "Import &Matlab file..." "Importa un fitxer de &Matlab..."
::msgcat::mcset ca_es "Create help s&keleton..."\
                      "Crea un esquelet d'a&juda..."
::msgcat::mcset ca_es "Compile as &help page" "Co&mpila com un fitxer d'ajuda"
::msgcat::mcset ca_es "Open &function source"\
                      "Obre la &font de la funció"
::msgcat::mcset ca_es "Print Se&tup" "Confi&gura la impressió"
::msgcat::mcset ca_es "&Print" "&Imprimeix"
::msgcat::mcset ca_es "&Close file" "&Tanca el fitxer"
::msgcat::mcset ca_es "E&xit" "&Surt"

# menues.tcl - Edit menu
::msgcat::mcset ca_es "&Undo" "&Desfés"
::msgcat::mcset ca_es "&Redo" "&Refés"
::msgcat::mcset ca_es "Cu&t" "&Talla"
::msgcat::mcset ca_es "B&lock cut" "Ta&lla el bloc"
::msgcat::mcset ca_es "&Copy" "&Copia"
::msgcat::mcset ca_es "&Paste" "&Enganxa"
::msgcat::mcset ca_es "&Block paste" "Enganxa el &bloc"
::msgcat::mcset ca_es "&Delete" "&Elimina"
::msgcat::mcset ca_es "Select &All" "Selecciona-ho &tot"
::msgcat::mcset ca_es "Insert &Time/Date" "Insereix l'&hora/data"
::msgcat::mcset ca_es "Co&mment selection" "Co&menta la selecció"
::msgcat::mcset ca_es "U&ncomment selection" "&Descomenta la selecció"
::msgcat::mcset ca_es "&Indent selection" "&Sagna la selecció"
::msgcat::mcset ca_es "Unin&dent selection" "Desfés el sagnat de la se&lecció"

# menues.tcl - Search menu
::msgcat::mcset ca_es "&Find..." "&Cerca..."
::msgcat::mcset ca_es "Find &Next" "&Següent"
::msgcat::mcset ca_es "&Replace..." "&Reemplaça..."
::msgcat::mcset ca_es "&Goto Line..." "&Vés a la línia..."

# menues.tcl - Exec menu
::msgcat::mcset ca_es "&Load into Scilab" "&Carrega a l'Scilab"
::msgcat::mcset ca_es "&Evaluate selection" "&Avalua la selecció"

# menues.tcl - Debug menu
::msgcat::mcset ca_es "&Insert/Remove breakpoint"\
                      "&Afegeix/Elimina un punt d'interrupció"
::msgcat::mcset ca_es "&Edit breakpoints" "&Modifica els punts d'interrupció"
::msgcat::mcset ca_es "Remove &all breakpoints" "Elimina &tots els punts\
                       d'interrupció"
::msgcat::mcset ca_es "&Configure execution..." "&Configura l'execució..."
::msgcat::mcset ca_es "Go to next b&reakpoint" "&Vés al proper punt d'interrupció"
::msgcat::mcset ca_es "&Step by step" "&Pas a pas"
::msgcat::mcset ca_es "Step &into" "Entra &dins"
::msgcat::mcset ca_es "Step o&ver" "Sa&lta"
::msgcat::mcset ca_es "Step &out" "&Surt"
::msgcat::mcset ca_es "Run to re&turn point" "Vés fins al punt de &retorn"
::msgcat::mcset ca_es "Run to c&ursor" "Vés fins al &cursor"
::msgcat::mcset ca_es "G&o on ignoring any breakpoint"\
                      "Co&ntinua ignorant tots els punts d'interrupció"
::msgcat::mcset ca_es "Show &watch" "Finestra de se&guiment"
::msgcat::mcset ca_es "&Break" "&Atura"
::msgcat::mcset ca_es "Cance&l debug" "Cance&l·la la depuració"

#    ::msgcat::mcset ca_es "language scheme" "esquema d'idioma"
# menues.tcl - Scheme menu
::msgcat::mcset ca_es "S&cilab" "&Scilab"
::msgcat::mcset ca_es "&XML" "&XML"
::msgcat::mcset ca_es "&none" "&cap"
::msgcat::mcset ca_es "&Colorize" "&Acoloreix"

# menues.tcl - Options menu
::msgcat::mcset ca_es "&Fonts..." "&Lletres..."
::msgcat::mcset ca_es "&Colors" "&Colors"
::msgcat::mcset ca_es "Colori&ze" "&Acoloreix"
::msgcat::mcset ca_es "&Always" "&Sempre"
::msgcat::mcset ca_es "Ask for &big files" "&Pregunta per fitxers grans"
::msgcat::mcset ca_es "&Never" "&Mai"
::msgcat::mcset ca_es "Colorize \'&strings\'" "Acoloreix \'ca&denes\'"
::msgcat::mcset ca_es "Colorize &user functions" "Acoloreix les funcions d'&usuari"
::msgcat::mcset ca_es "Show c&ontinued lines" "Mostra les línies &incompletes"
::msgcat::mcset ca_es "Line n&umbers" "Números de línia"
::msgcat::mcset ca_es "&Hide" "A&maga"
::msgcat::mcset ca_es "&Left aligned" "Alinea a l'es&querra"
::msgcat::mcset ca_es "&Right aligned" "Alinea a la dre&ta"
::msgcat::mcset ca_es "Word &wrap" "A&justa a la paraula"
::msgcat::mcset ca_es "&Double-click behavior" "C&omportament del doble clic"
::msgcat::mcset ca_es "&Linux" "&Linux"
::msgcat::mcset ca_es "Windo&ws" "&Windows"
::msgcat::mcset ca_es "&Scilab" "&Scilab"
::msgcat::mcset ca_es "&Tabs and indentation" "&Tabulacions i sagnats"
::msgcat::mcset ca_es "Tab inserts &spaces" "Tab afegeix e&spais"
::msgcat::mcset ca_es "&Indentation spaces" "&Espais de sagnat"
::msgcat::mcset ca_es "&Tab size (characters)" "&Nombre d'espais a la tabulació"
::msgcat::mcset ca_es "Use &keyword indentation" "&Sagnat automàtic"
::msgcat::mcset ca_es "Com&pletion" "Autoco&mpletació"
::msgcat::mcset ca_es "&Tab" "&Tab"
::msgcat::mcset ca_es "&Control-Tab" "&Ctrl+Tab"
::msgcat::mcset ca_es "&Alt-Tab" "&Alt+Tab"
::msgcat::mcset ca_es "&Shift-Tab" "&Maj+Tab"
::msgcat::mcset ca_es "C&ontrol-Alt-Tab" "C&trl+Alt+Tab"
::msgcat::mcset ca_es "S&hift-Control-Tab" "M&aj+Ctrl+Tab"
::msgcat::mcset ca_es "Sh&ift-Alt-Tab" "Ma&j+Alt+Tab"
::msgcat::mcset ca_es "Shi&ft-Control-Alt-Tab" "Maj+Ctr&l+Alt+Tab"
::msgcat::mcset ca_es "File&names" "&Noms dels fitxers"
::msgcat::mcset ca_es "&Full path" "&Camí complet"
::msgcat::mcset ca_es "Full path if &ambiguous" "Camí complet si és &ambigu"
::msgcat::mcset ca_es "&Unambiguous pruned path" "Camí complet sense a&mbigüitat"
::msgcat::mcset ca_es "Windows menu &sorting" "Ordre del &menú finestra"
::msgcat::mcset ca_es "&File opening order" "&Ordre d'obertura de fitxers"
::msgcat::mcset ca_es "&Alphabetically" "Ordre &alfabètic"
::msgcat::mcset ca_es "&Most recently used" "El &més recent"
::msgcat::mcset ca_es "&Recent files" "Fitxers &recents"
::msgcat::mcset ca_es "&Backup files depth" "Número de còpies de seguretat"
::msgcat::mcset ca_es "&Locale" "&Localització"
::msgcat::mcset ca_es "Execution &errors" "&Errors d'execució"
::msgcat::mcset ca_es "In Scilab &shell only" "Només a la finestra de l'&Scilab"
::msgcat::mcset ca_es "Copied in a &message box"\
                      "Copiat en una finestra de &missatges"
::msgcat::mcset ca_es "Enc&oding" "Codi&ficació"
::msgcat::mcset ca_es "More enc&odings" "Més codificacions"
::msgcat::mcset ca_es "&Recent encodings" "Codificacions &recents"
::msgcat::mcset ca_es "&Auto-detect encoding when loading XML files" "Detecció\
                       &automàtica en fitxers XML"
::msgcat::mcset ca_es "E&xit options" "Opcions de &sortida"
::msgcat::mcset ca_es "Show closure &X" "&Mostra el botó de sortida"
::msgcat::mcset ca_es "Exit on &last file close" "&Surt en tancar l'últim fitxer"

# menues.tcl - Windows menu
::msgcat::mcset ca_es "&Maximize" "&Maximitza"
::msgcat::mcset ca_es "&Split" "Divideix &horitzontalment"
::msgcat::mcset ca_es "S&plit (side by side)" "Divideix &verticalment"
::msgcat::mcset ca_es "Sp&lit file" "&Divideix el fitxer horitzontalment"
::msgcat::mcset ca_es "Spl&it file (side by side)"\
                      "Divideix el &fitxer verticalment"
::msgcat::mcset ca_es "Tile all &vertically" "Di&sposa verticalment"
::msgcat::mcset ca_es "Tile all &horizontally" "Dis&posa horitzontalment"
::msgcat::mcset ca_es "Space sashes &evenly" "&Espaiat uniforme"
::msgcat::mcset ca_es "Opened Files" "Fitxers oberts"

# menues.tcl - Help menu
::msgcat::mcset ca_es "&Help..." "&Ajuda..."
::msgcat::mcset ca_es "&What's?..." "&Què és?..."
::msgcat::mcset ca_es "&About" "&Quant a..."
::msgcat::mcset ca_es "Adding &translations..." "&Traducció de l'Scipad..."
::msgcat::mcset ca_es "Adding Scipad Translations..."\
                      "Crear una nova traducció de l'Scipad:"
::msgcat::mcset ca_es "&Bugs \& wishlist" "&Errors \& destitjos"
::msgcat::mcset ca_es "Scipad Known bugs" "Errors coneguts de l'Scipad"
::msgcat::mcset ca_es "Change&log" "Re&gistre de canvis"
::msgcat::mcset ca_es "Changes in the Scipad codebase"\
                      "Canvis al codi base de l'Scipad"


# popupmenus.tcl
::msgcat::mcset ca_es "Open the source of" "Obre la font de"
::msgcat::mcset ca_es "AddWatch" "Afegeix un seguiment a %s"
::msgcat::mcset ca_es "AddGenExp" "Afegeix %s com a expressió genèrica"
::msgcat::mcset ca_es "Jump to" "Salta a"


# print.tcl
::msgcat::mcset ca_es "Print Setup" "Configura la impressió"
::msgcat::mcset ca_es "Print Command: " "Comanda d'impressió:"


# scilabexec.tcl
::msgcat::mcset ca_es "No point in loading an empty file!"\
                      "No té sentit carregar un fitxer buit."
::msgcat::mcset ca_es "Silent file save failed"\
                      "Ha fallat el desat automàtic del fitxer"
::msgcat::mcset ca_es "File could not be saved in a temporary\
                       location.\nOverwrite original file?"\
                      "No s'ha pogut desar el fitxer en una ubicació temporal.\n\
                       Voleu sobreescriure el fitxer original?"
::msgcat::mcset ca_es "Exec done" "Execució feta"
::msgcat::mcset ca_es "Matlab files" "Fitxers de Matlab"
::msgcat::mcset ca_es "Matlab file to convert" "Fitxer de Matlab a convertir"
::msgcat::mcset ca_es "Scilab is converting, please hold on..."\
                      "L'Scilab està convertint el fitxer, espereu..."
::msgcat::mcset ca_es "Matlab file import" "Importació d'un fitxer de Matlab"
::msgcat::mcset ca_es "Conversion of the file failed, see the Scilab window\
                       for details -- Perhaps report the error text and the\
                       offending Matlab file to"\
                      "La traducció del fitxer ha fallat, veieu la finestra de\
                       l'Scilab per a més detalls -- Envieu l'informe de l'error\
                       i el fitxer de Matlab a:"
::msgcat::mcset ca_es "Please select destination path for the xml source of\
                       the help file"\
                      "Escolliu el camí de destí per a la font xml del fitxer\
                       d'ajuda"
::msgcat::mcset ca_es "Older version of" "S'ha trobat una versió antiga de"
::msgcat::mcset ca_es "found!" "!"
::msgcat::mcset ca_es "An old version of" "Ja existeix una versió antiga de "
::msgcat::mcset ca_es "already exists: open the old file instead?"\
                      ": voleu obrir el fitxer antic?"
::msgcat::mcset ca_es "Existing file" "Ja existeix el fitxer"
::msgcat::mcset ca_es "New skeleton" "Crea un esquelet nou"
::msgcat::mcset ca_es "This feature is no longer available in Scilab 5 due\
                       to bug "\
                      "Aquesta funcionalitat ja no és disponible a l'Scilab 5\
                       degut a l'error "
::msgcat::mcset ca_es "See details at " "Més detalls a: "
::msgcat::mcset ca_es "See also:" "Veieu també:"
::msgcat::mcset ca_es "Note: Scilab 4.x and Scilab-gtk are offering this\
                       feature."\
                      "Nota: L'Scilab 4.x i l'Scilab-gtk ofereixen aquesta\
                       funcionalitat."
::msgcat::mcset ca_es "A solution is to use a backported version of Scipad\
                       inside one of these environments, for instance:"\
                      "Una solució és utilitzar una versió \"backport\"\
                       d'Scipad en una d'aquestes versions d'Scilab,\
                       per exemple: "
::msgcat::mcset ca_es "Feature missing from Scilab 5"\
                      "Funcionalitat no disponible a l'Scilab 5"
::msgcat::mcset ca_es "ScilabEval command cannot be passed to Scilab!"\
                      "No s'ha pogut passar la comanda ScilabEval a l'Scilab."
::msgcat::mcset ca_es "impossibleScilabEval_message"\
                      "ATENCIÓ : No s'ha pogut passar una comanda a l'Scilab.\
                       S'ha ignorata Scilab. S'ha ignorat\n\n La raó més\
                       probable és que no es pot escriure a la carpeta TMPDIR\
                       de Scilab. Això pot tenir en un comporatament\
                       impredictible.\n\n La comanda en qüestió és:\n\n-->"
::msgcat::mcset ca_es "impossibleScilabEval_message2"\
                      "ATENCIÓ : No s'ha pogut passar una comanda a l'Scilab\
                       i s'ha ignorat perquè era massa llarga. Això pot\
                       tenir un comportament impredictible.\n\nSi esteu en\
                       una sessió de depuració, disminuiu el nombre de\
                       variables en seguiment.\n\nLa comanda en qüestió és\
                       :\n\n-->"
::msgcat::mcset ca_es "(end of command skipped)" "(resta de la comanda obviada)"
::msgcat::mcset ca_es "Scilab working" "Scilab ocupat"
::msgcat::mcset ca_es "Scilab is working, please wait for the prompt to\
                       execute this command!"\
                      "L'Scilab està ocupat, espereu que aparegui el\
                       \"prompt\" per executar la comanda."
::msgcat::mcset ca_es "Scilab is working, wait for the prompt to load file"\
                      "L'Scilab està ocupat, espereu que aparegui el\
                       \"prompt\" per carregar el fitxer."
::msgcat::mcset ca_es "Scilab is working, wait for the prompt to execute the\
                       selection."\
                      "L'Scilab està ocupat, espereu que aparegui el\
                       \"prompt\" per executar la selecció."
::msgcat::mcset ca_es "Scilab is working, wait for the prompt to convert a\
                       Matlab file."\
                      "L'Scilab està ocupat, espereu que aparegui el\
                       \"prompt\" per convertir el fitxer de Matlab."
::msgcat::mcset ca_es "Scilab is working, wait for the prompt to compile the\
                       help file."\
                      "L'Scilab està ocupat, espereu que aparegui el\
                       \"prompt\" per compilar la pàgina d'ajuda."
::msgcat::mcset ca_es "Scilab is working, wait for the prompt to issue a\
                       debugger command."\
                      "L'Scilab està ocupat, espereu que aparegui el\
                       \"prompt\" per continuar la depuració."
::msgcat::mcset ca_es "Scilab execution error"\
                      "Error de Scilab durant l'execució"
::msgcat::mcset ca_es "The shell reported an error while trying to execute "\
                      "La shell ha informat d'un error quan intentava executar "
::msgcat::mcset ca_es ": error " ": error "
::msgcat::mcset ca_es "at line " "a la linea "
::msgcat::mcset ca_es " of " " de "
::msgcat::mcset ca_es "Execution aborted!" "S'ha interromput l'execució."


# scipad.tcl
::msgcat::mcset ca_es "NOT CONNECTED TO SCILAB" "NO HI HA CONNEXIÓ AMB L'SCILAB"


# whichfun.tcl
::msgcat::mcset ca_es "Being at line" "Estant a la línia"
::msgcat::mcset ca_es ", function" ", la funció"
::msgcat::mcset ca_es "begins at" "comença a"
::msgcat::mcset ca_es ", and there are" ", i hi ha"
::msgcat::mcset ca_es "continued lines, i.e. we are at line"\
                      "línies múltiples, i.e. som a la línia"
::msgcat::mcset ca_es "of" "de"
#::msgcat::mcset ca_es "Function" "Funció" ; # now global
::msgcat::mcset ca_es "line" "línia"
::msgcat::mcset ca_es "The cursor is not currently inside a function body."\
                      "El cursor no és al cos de cap funció"
::msgcat::mcset ca_es "Which function?" "Quina funció?"


#preference variables description
::msgcat::mcset ca_es "BGCOLOR" "Fons"
::msgcat::mcset ca_es "BGLNMARGCOLOR" "Fons dels números de línia"
::msgcat::mcset ca_es "FGCOLOR" "Text"
::msgcat::mcset ca_es "FGLNMARGCOLOR" "Text dels números de línia"
::msgcat::mcset ca_es "CURCOLOR" "Cursor"
::msgcat::mcset ca_es "PARCOLOR" "Parèntesis ()"
::msgcat::mcset ca_es "BRAKCOLOR" "Claudàtors \\\[\\\]"
::msgcat::mcset ca_es "BRACCOLOR" "Claus {}"
::msgcat::mcset ca_es "PUNCOLOR" "Puntuació"
::msgcat::mcset ca_es "INTFCOLOR" "Funcions internes"
::msgcat::mcset ca_es "COMMCOLOR" "Comandes de Scilab"
::msgcat::mcset ca_es "SCICCOLOR" "Funcions de Scicos"
::msgcat::mcset ca_es "OPCOLOR" "Operadors"
::msgcat::mcset ca_es "QTXTCOLOR" "Cadenes de text"
::msgcat::mcset ca_es "REMCOLOR" "Comentaris"
::msgcat::mcset ca_es "XMLCOLOR" "Etiquetes XML"
::msgcat::mcset ca_es "NUMCOLOR" "Números"
::msgcat::mcset ca_es "SELCOLOR" "Selecció de fons"
::msgcat::mcset ca_es "BREAKPOINTCOLOR" "Punts d'interrupció"
::msgcat::mcset ca_es "LFUNCOLOR" "Funcions de llibreria"
::msgcat::mcset ca_es "PDEFCOLOR" "Variables predefinides"
::msgcat::mcset ca_es "FOUNDTEXTCOLOR" "Text trobat"
::msgcat::mcset ca_es "CONTLINECOLOR" "Línies incompletes"
::msgcat::mcset ca_es "USERFUNCOLOR" "Funcions d'usuari"


#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset ca_es "&Directory:" "&Directori"
::msgcat::mcset ca_es "File &name:" "&Nom del fitxer:"
::msgcat::mcset ca_es "Files of &type:" "&Tipus de fitxer:"
#::msgcat::mcset ca_es "&Save" "&Desa" # already included
::msgcat::mcset ca_es "&Open" "&Obre"
::msgcat::mcset ca_es "Open" "Obre"
::msgcat::mcset ca_es "Save As" "Anomena i desa..."
::msgcat::mcset ca_es "&Cancel" "&Cancel·la"
::msgcat::mcset ca_es "&Selection:" "&Selecció:"
::msgcat::mcset ca_es "&Yes" "&Sí"
::msgcat::mcset ca_es "&No" "&No"
::msgcat::mcset ca_es "&Red" "&Vermell"
::msgcat::mcset ca_es "&Green" "V&erd"
::msgcat::mcset ca_es "&Blue" "&Blau"
::msgcat::mcset ca_es "Directory \"%1\$s\" does not exist."\
                      "El directori \"%1\$s\" no existeix."
