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

# Scipad texts in German
# in sync at least with v6.128BP1
# provided by Dirk Reusch <reusch@tmb.uni-karlsruhe.de>

# commons
::msgcat::mcset de_de  "Cancel" "Abbrechen"
::msgcat::mcset de_de "Cance&l" "A&bbrechen"
::msgcat::mcset de_de "OK" "OK"
::msgcat::mcset de_de "All files" "Alle Dateien"
::msgcat::mcset de_de "Add/Change" "Hinzuf\u00FCgen/\u00C4ndern"
::msgcat::mcset de_de "Add/Chan&ge" "&Hinzuf\u00FCgen/\u00C4ndern"
::msgcat::mcset de_de "&Remove" "&Entfernen"
::msgcat::mcset de_de "Rem&ove" "Ent&fernen"
::msgcat::mcset de_de "Untitled" "OhneNamen"
::msgcat::mcset de_de "Close" "Schlie\u00DFen"
::msgcat::mcset de_de "&Close" "S&chlie\u00DFen"
::msgcat::mcset de_de "Error " "Fehler "
::msgcat::mcset de_de "The file" "Die Datei"


# buffernavigation.tcl
::msgcat::mcset de_de "Hide" "Verstecken"
::msgcat::mcset de_de "Close this tile, keep content"\
                   "Diese Ansicht schlie\u00DFen (Datei ge\u00F6ffnet lassen)"
::msgcat::mcset de_de "Close this tile" "Diese Ansicht schlie\u00DFen"
::msgcat::mcset de_de "Goto Line?" "Gehe zu Zeile?"
::msgcat::mcset de_de "Go to:" "Gehe zu:"
::msgcat::mcset de_de "&logical line" "&Programmzeile"
::msgcat::mcset de_de "&physical line" "&Textzeile"
::msgcat::mcset de_de "in &function" "der &Funktion"
::msgcat::mcset de_de "in &current file" "der aktuellen &Datei"
::msgcat::mcset de_de "Outside of function definition"\
                   "Au\u00DFerhalb der Funktionsdefinition"


# colorize.tcl
::msgcat::mcset de_de "Wait seconds while recolorizing file"\
                   "Warten Sie einen Moment w\u00E4hrend die farbige\
                    Darstellung aktualisiert wird"
::msgcat::mcset de_de "You can't do that while colorization is in progress.\
                    Please try again when finished."\
                   "Aktion kann w\u00E4hrend der Aktualsierung der farbigen\
                    Darstellung nicht ausgef\u00FChrt werden.\
                    Versuchen Sie es sp\u00E4ter noch einmal."
::msgcat::mcset de_de "Command forbidden during colorization"\
                   "Befehl kann w\u00E4hrend der Aktualisierung der farbigen
                    Darstellung nicht ausge\u00FChrt werden."


# completion.tcl
::msgcat::mcset de_de "No possible completion"\
                   "Keine Vervollst\u00E4ndigung m\u00F6glich"
::msgcat::mcset de_de "possible completions" "m\u00F6gliche Vervollst\u00E4ndigungen"
::msgcat::mcset de_de "Possible completions" "M\u00F6gliche Vervollst\u00E4ndigungen"


# db_argsmanagement.tcl
::msgcat::mcset de_de "Add" "Hinzuf\u00FCgen"
::msgcat::mcset de_de "This variable can be watched but cannot be edited!"\
                   "Diese Variable kann beobachtet aber nicht ver\u00E4ndert werden!"
::msgcat::mcset de_de "A new watch variable will be created."\
                   "Eine neue Variable zum Beobachten wird angelegt."
::msgcat::mcset de_de "Non editable variable" "Nicht ver\u00E4nderbare Variable"
::msgcat::mcset de_de "Variable:" "Variable:"
::msgcat::mcset de_de "Expression:" "Ausdruck:"
::msgcat::mcset de_de "Value:" "Wert:"
::msgcat::mcset de_de "&Get current value from Scilab"\
                   "&Aktuellen Wert aus Scilab \u00FCbernehmen"
::msgcat::mcset de_de "Non watchable variable" "Nicht beobachtbare Variable"
::msgcat::mcset de_de "Sorry, variable" "Entschuldigung, die Variable"
::msgcat::mcset de_de "is special and cannot be watched."\
                   "ist von einem Typ, der nicht beobachtet werden kann."


# db_bptsmanagement.tcl
::msgcat::mcset de_de "No breakpoint here!"\
                   "Hier kann kein Stoppunkt gesetzt werden!"


# db_configureexec.tcl
::msgcat::mcset de_de "Configure execution" "Ausf\u00FChrung konfigurieren"
::msgcat::mcset de_de "Function name:" "Funktionsname:"
::msgcat::mcset de_de "&Obtain" "Erh\u00E4l&t"
::msgcat::mcset de_de "Input arguments:" "Eingabeargumente:"
::msgcat::mcset de_de "The selected function has not been found in the opened\
                    files, maybe it was deleted since last configuration of\
                    the debugger.\nUse button \""\
                   "Die gew\u00E4hlte Funktion wurde in den ge\u00F6ffneten Dateien\
                    nicht gefunden. Wahrscheinlich wurde Sie nach der letzten\
                    Konfiguration des Debuggers gel\u00F6scht.\
                    Dr\u00FCcken Sie \""
::msgcat::mcset de_de "\" to refresh the list of available functions."\
                   "\" um die Liste der verf\u00FCgbaren Funktionen zu aktualisieren."
::msgcat::mcset de_de "Function name or input arguments do not match definition\
                    of the function"\
                   "Funktionsname und Eingabeargumente stimmen nicht mit der\
                    Funktionsdefinition \u00FCberein"
::msgcat::mcset de_de "in the file!\n\nCheck function name and arguments\
                    (names, order) in the configuration dialog. \nArguments\
                    order can be changed using drag and drop with right\
                    mouse button in the arguments listbox."\
                   "in der Datei!\n\n\u00DCberpr\u00FCfen Sie den Funktionsnamen\
                    und die Argumente (Namen, Reihenfolge) im\
                    Konfigurationsdialog. Die Reihenfolge der Argumente kann\
                    mittels Drag&Drop (rechte Maustaste) ver\u00E4ndert werden."
::msgcat::mcset de_de "Error in selected function name or arguments"\
                   "Fehler in markiertem Funktionsnamen oder markierten\
                    Argumenten"
::msgcat::mcset de_de "At least one input argument does not match definition of\
                    the function"\
                   "Mindestens ein Eingabeargument entspricht nicht der\
                    Funktionsdefinition"
::msgcat::mcset de_de "in the file!\n\nCheck function arguments (their names)\
                    in the configuration dialog."\
                   "in der Datei!\n\n\u00DCberpr\u00FCfen Sie den Funktionsnamen\
                    und die Argumente (Namen, Reihenfolge) im\
                    Konfigurationsdialog."
::msgcat::mcset de_de "Error in selected function arguments"\
                   "Fehler in den Argumenten der ausgew\u00E4hlten Funktion"
::msgcat::mcset de_de "Main level code found" "Code f\u00FCr die Hauptebene"
::msgcat::mcset de_de "Scipad ancillary file" "Scipad-Hilfsdatei"
::msgcat::mcset de_de "The current file contains only Scipad ancillaries that\
                    cannot be debugged in Scipad."\
                   "Die aktuelle Datei enth\u00E4lt nur Hilfsfunktionen von Scipad,\
                    die nicht mit dem Scipad-Debugger ausgef\u00FChrt werden k\u00F6nnen."
::msgcat::mcset de_de "This file contains main level code.\nDebug as a single\
                    .sce file?"\
                   "Diese Datei enth\u00E4lt Code f\u00FCr die Hauptebene. Soll der\
                    Debugger sie wie eine .sce Datei behandeln?"
::msgcat::mcset de_de "No function found" "Keine Funktion gefunden"
::msgcat::mcset de_de "This file contains no function and will be treated"\
                   "Diese Datei enth\u00E4lt keine Funktion und wird"
::msgcat::mcset de_de "as a single .sce file.\nStart debugging?"\
                   "wie eine .sce Datei behandelt. Debugger starten?"
::msgcat::mcset de_de "You must enable colorization to be able to launch the\
                    debugger!"\
                   "Farbige Darstellung mu\u00DF aktiviert sein, um den Debugger\
                    starten zu k\u00F6nnen!"
::msgcat::mcset de_de "The debugger needs colorization"\
                   "Der Debugger ben\u00F6tigt die farbige Darstellung"
::msgcat::mcset de_de "Warning!\n\n  A function missing the closing\
                    \"endfunction\" has been found in the currently opened\
                    files.\n  Such syntax is now obsolete.\n\n  Please\
                    terminate function "\
                   "Warnung!\n In den ge\u00F6ffneten Dateien wurde eine Funktion\
                    gefunden, die nicht mit \"endfunction\" endet.\
                    Diese Syntax ist nicht mehr zul\u00E4ssig. Bitte\
                    beenden Sie die Funktion "
::msgcat::mcset de_de " and try again." " mit \"endfunction\" und versuchen\
                    Sie es erneut."
::msgcat::mcset de_de "Note: This message might also be triggered by a string\n \
                    quoted with single quotes and containing the word\
                    \"function\",\n  when \"Colorize 'strings'\" is\
                    unchecked in the Options menu."\
                   "Diese Meldung kann auch durch eine Zeichenkette\
                    \"function\" in einfachen Anf\u00FChrungszeichen ausgel\u00F6st\
                    werden, wenn die \"Farbige 'Zeichenketten'\" im Men\u00FC\
                    Optionen nicht aktiviert ist."
::msgcat::mcset de_de "This known bug can be worked around simply by checking\
                    \"Colorize \'&strings'\"."\
                   "Dieser bekannte Fehler kann durch die Aktvierung von\
                    \"Farbige 'Zeichenketten'\" einfach behoben werden."
::msgcat::mcset de_de "Unterminated function definition found"\
                   "Funktionsdefinition ohne \"endfunction\" gefunden"
::msgcat::mcset de_de "Warning!\n\n  One or more function is defined more than\
                    once in the currently opened files.\n  The debugger\
                    cannot cope with multiple copies of functions.\n  The\
                    following duplicates were detected:\n"\
                   "Warnung!\n\n Eine oder mehrere Funktionen sind in den\
                    ge\u00F6ffneten Dateien mehrfach definiert. Der Debugger kann\
                    mit mehrfach definierten Funktionen nicht arbeiten. Dies\
                    betrifft folgende Funktionen:\n"
::msgcat::mcset de_de "\n--> Definition of function "\
                   "\n--> Definition der Funktion "
::msgcat::mcset de_de " found in:\n" "gefunden in:\n"
::msgcat::mcset de_de "Duplicate function definitions found"\
                   "Doppelte Funktionsdefinition gefunden"


# db_debugsession.tcl
::msgcat::mcset de_de "Too many breakpointed functions"\
                   "Zu viele Funktionen mit Stoppunkten"
::msgcat::mcset de_de "You have currently" "Sie haben im Moment"
::msgcat::mcset de_de "functions in your opened files."\
                   "Funktionen in Ihren ge\u00F6ffneten Dateien."
::msgcat::mcset de_de "Scilab supports a maximum of"\
                   "Scilab unterst\u00FCtzt maximal"
::msgcat::mcset de_de "possible breakpointed functions (see help setbpt)."\
                   "Funktionen mit Stoppunkten (siehe help setbpt)."
::msgcat::mcset de_de "Step-by-step hence cannot be performed."\
                   "\"Schritt-f\u00FCr-Schritt\"-Modus kann nicht ausgef\u00FChrt werden."
::msgcat::mcset de_de "This command will actually run to the next breakpoint."\
                   "Dieser Befehl wird bis zum n\u00E4chsten Stoppunkt ausf\u00FChren."
::msgcat::mcset de_de "Break hence cannot be performed."\
                   "\"Unterbrechen\" ist deshalb nicht m\u00F6glich."
::msgcat::mcset de_de "Too many breakpoints" "Zu viele Stoppunkte"
::msgcat::mcset de_de "Executing this command would require to set"\
                   "Dieser Befehl erfordert das Setzen von"
::msgcat::mcset de_de "breakpoints in your opened files."\
                   "Stoppunkten in Ihren ge\u00F6ffneten Dateien."
::msgcat::mcset de_de "possible breakpoints in Scilab (see help setbpt)."\
                   "m\u00F6gliche Stoppunkte (siehe help setbpt)."
::msgcat::mcset de_de "Cursor must be in a function"\
                   "Eingabezeiger mu\u00DF sich innerhalb einer Funktion befinden"
::msgcat::mcset de_de "This command cannot succeed if the function where the\
                    debug just stopped is not opened in Scipad!"\
                   "Dieser Befehl kann nicht fortgesetzt werden, wenn die\
                    Funktion, in der der Debugger sich gerade befindet, nicht\
                    in Scipad ge\u00F6ffnet ist!"
::msgcat::mcset de_de "Cannot run to return point"\
                   "Ausf\u00FChrung bis zum R\u00FCcksprung ist nicht m\u00F6glich"
::msgcat::mcset de_de "Warning!\nThe line where the debugger just stopped\
                    contains a return statement (either endfunction, return\
                    or resume) but this statement is not the first one on\
                    the line."\
                   "Warnung!\n Der Debugger befindet sich in einer Zeile\
                    mit R\u00FCcksprunganweisung (endfunction, return oder resume),\
                    die nicht die erste Anweisung in dieser Zeile ist."
::msgcat::mcset de_de "As a consequence the debugger cannot predict if the next\
                    debug step will really jump outside of the current\
                    function, which is what you just asked the debugger to\
                    do."\
                   "Deshalb kann der Debugger nicht vorhersagen, ob, wie Sie\
                    es w\u00FCnschten, im n\u00E4chsten Schritt aus der aktuellen\
                    Funktion herausgesprungen wird."
::msgcat::mcset de_de "To avoid this unresolvable case, it is recommended to\
                    place the return statement alone on its line."\
                   "Dieser Zweifelsfall kann vermieden werden. Es wird\
                    empfohlen die R\u00FCcksprunganweisung auf einer separaten\
                    Programmzeile zu platzieren."
::msgcat::mcset de_de "Do you want to see this warning again during this Scipad\
                    session?"\
                   "M\u00F6chten in dieser Scipad-Sitzung diese Warnung erneut\
                    erhalten?"
::msgcat::mcset de_de "Unresolvable case found while running to return point"\
                   "Zweifelsfall w\u00E4hrend \"Ausf\u00FChren bis R\u00FCcksprung\" gefunden"
::msgcat::mcset de_de "No effect - The debugged file is not stuck"\
                   "Keine Wirkung - Die Ausf\u00FChrung der Debug-Datei wurde\
                    nicht unterbrochen"


# db_init.tcl
::msgcat::mcset de_de "Please wait..." "Bitte warten Sie..."


# db_states.tcl
::msgcat::mcset de_de "Currently no debug session"\
                   "Im Moment l\u00E4uft keine Debug-Sitzung"
::msgcat::mcset de_de "Ready to start debug"\
                   "Bereit zum Starten einer Debug-Sitzung"
::msgcat::mcset de_de "Debug in progress" "Debug-Vorgang l\u00E4uft"
::msgcat::mcset de_de "Scilab is back at the main level now."\
                   "Scilab ist jetzt wieder in der Hauptebene."


# db_watch.tcl
::msgcat::mcset de_de "Watch" "Beobachten"
::msgcat::mcset de_de "Watch variables:" "Beobachtete Variablen:"
::msgcat::mcset de_de "A&uto (local variables)"\
                   "&Automatisch (alle lokalen Variablen)"
::msgcat::mcset de_de "Au&to watch globals too"\
                   "Automatisch auch &globale Variablen"
::msgcat::mcset de_de "Generic expressions:" "Allgemeine Ausdr\u00FCcke:"
::msgcat::mcset de_de "Call stack:" "Ausf\u00FChrungsstapel:"
::msgcat::mcset de_de "Always on to&p" "Immer im Vordergrund"
::msgcat::mcset de_de "Hide watch &variables" "Beobachtete &Variablen verstecken"
::msgcat::mcset de_de "Hide e&xpressions" "A&usdr\u00FCcke verstecken"
::msgcat::mcset de_de "Hide call &stack" "Ausf\u00FChrungs&stapel verstecken"
::msgcat::mcset de_de "&Dock" "Einge&bettet"
::msgcat::mcset de_de "&Add" "Hinzuf\u00FCgen"
::msgcat::mcset de_de "&Filter errors" "Fehler nicht anzeigen"
::msgcat::mcset de_de "Breakpoints" "Stoppunkte"
::msgcat::mcset de_de "Breakpointed functions" "Funktionen mit Stoppunkten"
::msgcat::mcset de_de "Scilab busy indicator"\
                   "Aktivit\u00E4tsenzeige von Scilab"
::msgcat::mcset de_de "Debugger state indicator"\
                   "Zustandsanzeige des Debuggers"
::msgcat::mcset de_de "Undefined elements are not legal in variable"\
                   "Undefinierte Elemente sind in dieser Variable nicht\
                    erlaubt"
::msgcat::mcset de_de ".\nThis variable will not be updated in Scilab."\
                   ".\n Diese Variable wird in Scilab nicht aktualisiert."
::msgcat::mcset de_de "Illegal undefined element found"\
                   "Nicht erlaubtes undefiniertes Element gefunden"
::msgcat::mcset de_de "evaluates to:" "ergibt:"
::msgcat::mcset de_de "produces error" "erzeugt Fehler"
::msgcat::mcset de_de "Scipad cannot find source code for the selected\
                    function.\nPlease open the adequate file manually and\
                    try again."\
                   "Scipad kann den Quellcode f\u00FCr die ausgew\u00E4hlte Funktion nicht\
                    finden.\nBitte \u00D6ffnen Sie manuell die entsprechende Datei\
                    und versuchen Sie es noch einmal."
::msgcat::mcset de_de "Missing source code" "Quellcode fehlt"


# filecommands.tcl
::msgcat::mcset de_de "New script" "Neues Skript"
::msgcat::mcset de_de "New view on the same file"\
                   "Neue Ansicht derselben Datei"
::msgcat::mcset de_de "The contents of" "Der Inhalt von"
::msgcat::mcset de_de "may have changed, do you wish to save your changes?"\
                   "wurde ver\u00E4ndert. M\u00F6chten Sie die \u00C4nderungen speichern?"
::msgcat::mcset de_de "Save Confirm?" "Speichern best\u00E4tigen?"
::msgcat::mcset de_de "Open source of..." "\u00D6ffnen des Quellcodes von..."
::msgcat::mcset de_de "Open source of:" "\u00D6ffnen des Quellcodes von :"
::msgcat::mcset de_de "Weird drag&drop" "Drag&Drop"
::msgcat::mcset de_de "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or you\
                    hit a bug of the dnd mechanism."\
                   "repr\u00E4sentiert weder einen g\u00FCltigen Dateinamen, noch\
                    einen g\u00FCltigen Dateipfad. Es handelt es sich entweder um\
                    ein anderen Objekttyp, oder Sie haben einen Fehler im\
                    Drag&Drop-Mechanismus von Scipad gefunden."
::msgcat::mcset de_de "File not found" "Datei wurde nicht gefunden"
::msgcat::mcset de_de "does not exist anymore. Do you want to create an empty\
                    file with the same name?"\
                   "existiert nicht mehr. M\u00F6chten Sie eine leere Datei mit\
                    dem gleichen Namen erzeugen ?"
::msgcat::mcset de_de "Open file" "Datei \u00F6ffnen"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset de_de "is already opened! Save the current opened file to\
                    another name and reopen it from disk!"\
                   "wurde bereits ge\u00F6ffnet! Speichern Sie die Datei unter\
                    einem anderen Namen und \u00F6ffnen Sie sie wieder!"
::msgcat::mcset de_de "File" "Datei"
::msgcat::mcset de_de "saved" "gespeichert"
::msgcat::mcset de_de "cannot be written!" "ist schreibgesch\u00FCtzt!"
::msgcat::mcset de_de "The contents of " "Der Inhalt von"
::msgcat::mcset de_de "has changed on disk, save it anyway?"\
                   "auf der Festplatte hat sich ver\u00E4ndert. M\u00F6chten Sie\
                    trotzdem abspeichern?"
::msgcat::mcset de_de "has been modified outside of Scipad. Do you want to\
                    reload it?"\
                   "wurde au\u00DFerhalb von Scipad ver\u00E4ndert.  M\u00F6chten Sie sie\
                    neu laden?"
::msgcat::mcset de_de "File has changed!" "Datei wurde ver\u00E4ndert!"
::msgcat::mcset de_de "Save as" "Speichern unter"
::msgcat::mcset de_de "Scilab files" "Scilab-Dateien"
::msgcat::mcset de_de "Scicos files" "Scicos-Dateien"
::msgcat::mcset de_de "XML files" "XML-Dateien"
::msgcat::mcset de_de "Wait seconds while loading and colorizing file"\
                   "Bitte warten Sie einen Moment w\u00E4hrend Datei ge\u00F6ffnet\
                    wird"
::msgcat::mcset de_de "The contents of this file may have changed, do you wish\
                    to save your changes?"\
                   "Diese Datei wurde ver\u00E4ndert. M\u00F6chten Sie ihre\
                    \u00C4nderungen speichern?"
::msgcat::mcset de_de "Revert" "M\u00F6chten Sie auf die gespeicherte Version von"
::msgcat::mcset de_de "to saved?" "zur\u00FCckgreifen?"
::msgcat::mcset de_de "Revert Confirm?" "Verwerfen best\u00E4tigen?"
::msgcat::mcset de_de "Unreadable file" "Unlesbare Datei!"
::msgcat::mcset de_de "exists but is not readable!"\
                   "existiert, ist aber unlesbar!"
::msgcat::mcset de_de "This file is rather large and colorization might take\
                    some time.\n\nColorize anyway?"\
                   "Diese Datei ist ziemlich gro\u00DF und die farbige Darstellung\
                    kann etwas Zeit in Anspruch nehmen. Trotzdem farbig\
                    darstellen?"
::msgcat::mcset de_de "File size warning" "Warnung: Gro\u00DFe Datei"
::msgcat::mcset de_de "Windows shortcut file" "Windows Shortcut-Datei"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset de_de "is a Windows shortcut and will not be opened!"\
                   "ist eine Windows Shortcut-Datei und wird nicht ge\u00F6ffnet\
                    werden!"


# findreplace.tcl
::msgcat::mcset de_de "Dialog box already open!"\
                   "Dialogfenster ist bereits ge\u00F6ffnet!"
::msgcat::mcset de_de "The string" "Die Zeichenkette"
::msgcat::mcset de_de "could not be found in"\
                   "konnte nicht gefunden werden. Durchsucht wurde"
::msgcat::mcset de_de "could not be found in any of the opened files !"\
                   "konnte in keiner der ge\u00F6ffneten Dateien gefunden werden!"
::msgcat::mcset de_de "replacements done" "Ersetzungen ausgef\u00FChrt"
::msgcat::mcset de_de "Find" "Suchen"
::msgcat::mcset de_de "No match found in the selection for" "Die Zeichenkette"
::msgcat::mcset de_de "\nWould you like to look for it in the entire text?"\
                   "konnte im markierten Bereich nicht gefunden\
                    werden.\nM\u00F6chten Sie in der gesamten Datei danach\
                    suchen ?"
::msgcat::mcset de_de "You are searching for an empty string!"\
                   "Nach einer leeren Zeichenkette kann nicht gesucht werden!"
::msgcat::mcset de_de "Re&place" "&Ersetzen"
::msgcat::mcset de_de "Find what:" "Suche nach:"
::msgcat::mcset de_de "Replace with:" "Ersetze durch:"
::msgcat::mcset de_de "Find &Next" "Suche &fortsetzen"
::msgcat::mcset de_de "Replace &All" "&Alle Ersetzen"
::msgcat::mcset de_de "Direction" "Richtung"
::msgcat::mcset de_de "&Upwards" "&R\u00FCckw\u00E4rts"
::msgcat::mcset de_de "&Downwards" "&Vorw\u00E4rts"
::msgcat::mcset de_de "Text types" "Texttypen"
::msgcat::mcset de_de "Find options" "Suchoptionen"
::msgcat::mcset de_de "Match &whole word only" "Ganzes &Wort"
::msgcat::mcset de_de "Match &case" "&Gro\u00DF-/Kleinschreibung beachten"
::msgcat::mcset de_de "&Regular expression" "Reg&ul\u00E4rer Ausdruck"
::msgcat::mcset de_de "In all &opened files" "In allen ge\u00F6ffneten &Dateien"
::msgcat::mcset de_de "In &selection only" "In &markiertem Bereich"
::msgcat::mcset de_de "In typed te&xt" "In Texttypen"
::msgcat::mcset de_de "In a director&y" "In einem &Verzeichnis"
::msgcat::mcset de_de "Find in files search options"\
                   "Optionen f\u00FCr die Suche in Verzeichnissen"
::msgcat::mcset de_de "Directory r&ecurse search" "Re&kursive Suche"
::msgcat::mcset de_de "Include &hidden files" "versteckte Dateien einbeziehen"
::msgcat::mcset de_de "In files/file types:" "Dateitypen:"
::msgcat::mcset de_de "In directory:" "Im Verzeichnis:"
::msgcat::mcset de_de "Search for file names" "Suche nach Dateinamen"
::msgcat::mcset de_de "You have given an empty search string. Are you sure you\
                    want to search among file names?"\
                   "Sie haben eine leere Zeichenkette angegeben. Sind sie\
                    sicher, da\u00DF Sie nach Dateinamen suchen wollen?"
::msgcat::mcset de_de "Wrapped around" "Zeilenumbruch"
::msgcat::mcset de_de "Back at the first file searched!"\
                   "Erste Datei der Suche wurde wieder erreicht !"
::msgcat::mcset de_de "No more matches" "Keine weiteren Suchergebnisse"
::msgcat::mcset de_de "The string to regexp search for cannot be compiled by\
                    Tcl.\nCheck again, maybe certain characters should be\
                    escaped."\
                   "Der regul\u00E4re Ausdruck kann in Tcl nicht interpetiert \
                    werden. \u00DCberpr\u00FCfen Sie, ob bestimmeten Zeichen ein \
                    ESC-Zeichen (\\) vorangestellt werden mu\u00DF."
::msgcat::mcset de_de "Any single character" "Ein beliebiges Zeichen"
::msgcat::mcset de_de "Zero or more" "Keins oder Mehr"
::msgcat::mcset de_de "One or more" "Eins oder Mehr"
::msgcat::mcset de_de "Zero or one" "Keins oder Eins"
::msgcat::mcset de_de "Any one character in the set"\
                   "Beliebiges Zeichen aus der Menge"
::msgcat::mcset de_de "Any one character not in the set"\
                   "Beliebiges Zeichen nicht aus der Menge"
::msgcat::mcset de_de "Or" "Oder"
::msgcat::mcset de_de "Beginning of line" "Zeilenanfang"
::msgcat::mcset de_de "End of line" "Zeilenende"
::msgcat::mcset de_de "Beginning of word" "Wortanfang"
::msgcat::mcset de_de "End of word" "Wortende"
::msgcat::mcset de_de "A letter" "Buchstabe"
::msgcat::mcset de_de "A decimal digit" "Ziffer (dezimal)"
::msgcat::mcset de_de "An alphanumeric (letter or digit)"\
                   "Alphanumerisches Zeichen (Buchstabe oder Ziffer)"
::msgcat::mcset de_de "A space or tab character" "Leerzeichen oder Tabulator"
::msgcat::mcset de_de "A rational number" "Rationale Zahl"
::msgcat::mcset de_de "A floating point number" "Flie\u00DFkommazahl"
::msgcat::mcset de_de "A Scilab comment" "Scilab-Kommentar"
::msgcat::mcset de_de "A Scilab name" "Variable oder Scilab-SchlÃ¼sselwort"
::msgcat::mcset de_de "A Scilab continued line (with dots)"\
                   "Scilab-Zeile fortgesetzt mit .."


# findinfiles.tcl
::msgcat::mcset de_de "Directory access issue"\
                   "Fehler beim Zugriff auf Verzeichnis"
::msgcat::mcset de_de "The directory you specified cannot be reached. The\
                    search won't bring any result."\
                   "Auf das angegebene Verzeichnis kann nicht zugegriffen\
                    werden.  Die Suche wird ergebnislos sein."
::msgcat::mcset de_de "Some directories or files could not be open and were\
                    ignored during search.\n You might miss read access to\
                    them.\n\n The following ones were ignored"\
                   "Einige Verzeichnisse oder Dateien konnten nicht\
                    ge\u00F6ffnet  werden. Sie sind von Suchfunktion ignoriert\
                    worden.\n  M\u00F6glicherweise besitzen sie keine Leserechte\
                    f\u00FCr diese  Verzeichnisse/Dateien.\n\n Folgende\
                    Verzeichnisse/Dateien sind ignoriert worden"
::msgcat::mcset de_de "Ignored files" "Ignorierte Dateien"
::msgcat::mcset de_de "Searching in file:" "Suche in Datei :"
::msgcat::mcset de_de "End of file search:" "Ende der Suche :"
::msgcat::mcset de_de "matches found" "\u00DCbereinstimmungen gefunden"
::msgcat::mcset de_de "searched files" "durchsuchte Dateien"
::msgcat::mcset de_de "Constructing file list:" "Erzeugung der Dateiliste:"
::msgcat::mcset de_de "&Previous" "&Zur\u00FCck"
::msgcat::mcset de_de "&Next" "&Vorw\u00E4rts"
::msgcat::mcset de_de "Scipad search results" "Ergebnisse der Scipad-Suche"


# fonts.tcl
::msgcat::mcset de_de "Choose font" "Schrift ausw\u00E4hlen"
::msgcat::mcset de_de "Family" "Schriftart"
::msgcat::mcset de_de "Style" "Stil"
::msgcat::mcset de_de "&Bold" "&Fett"
::msgcat::mcset de_de "&Italic" "&Kursiv"
::msgcat::mcset de_de "&Underline" "&Unterstrichen"
::msgcat::mcset de_de "Size" "Gr\u00F6\u00DFe"
::msgcat::mcset de_de "&Defaults" "&Voreinstellungen"
::msgcat::mcset de_de "Use as..." "Verwenden f\u00FCr..."
::msgcat::mcset de_de "&Text font" "Programmtext"
::msgcat::mcset de_de "&Menu font" "Men\u00FCs"
::msgcat::mcset de_de "Sample" "Muster"
::msgcat::mcset de_de "Font size" "Schriftgr\u00F6\u00DFe"
::msgcat::mcset de_de "Revert fonts" "Voreingestellte Schriften "
::msgcat::mcset de_de "Are you sure you want to revert all fonts to defaults?"\
                   "Sind sie sicher, da\u00DF alle Schriften auf die voreingestellten\
                    Werte zur\u00FCckgesetzt werden sollen?"


# FormatStringsForWatch.sci
::msgcat::mcset de_de " what you try to watch is not supported by the debugger"\
                   " Sie versuchen etwas zu beobachten, das der Debugger\
                    nicht unterst\u00FCtzt"
::msgcat::mcset de_de " what you try to watch is of type "\
                   " Sie versuchen ein Objekt vom Typ "
::msgcat::mcset de_de " - this type is not supported by the debugger"\
                   "zu beobachten - dieser Typ wird vom Debugger\
                    nicht unterst\u00FCtzt"
::msgcat::mcset de_de "Type:" "Typ:"
::msgcat::mcset de_de "(unsupported)" "(nicht unterst\u00FCtzt)"
::msgcat::mcset de_de "size:" "Gr\u00F6\u00DFe:"
::msgcat::mcset de_de "real" "reell"
::msgcat::mcset de_de "complex" "komplex"
::msgcat::mcset de_de "elements" "Elemente"
::msgcat::mcset de_de "graphic handle" "Grafik-Objekt"
::msgcat::mcset de_de "uncompiled function" "unkompilierte Funktion"
::msgcat::mcset de_de "compiled function" "kompilierte Funktion"
::msgcat::mcset de_de "library" "Bibliothek"
::msgcat::mcset de_de "pointer" "Zeiger"
::msgcat::mcset de_de "size implicit index" "Index impliziter Gr\u00F6\u00DFe"
::msgcat::mcset de_de "primitive" "eingebaute Funktion"


# FormatWhereForWatch.sci
::msgcat::mcset de_de "Breakpoint" "Stoppunkt"
::msgcat::mcset de_de "called at line" "in Zeile"
::msgcat::mcset de_de "of exec file" "der Datei"
::msgcat::mcset de_de "of execstr instruction" "der Anweisung"
::msgcat::mcset de_de "called under pause" "w\u00E4hrend \"pause\" aufgerufen"
::msgcat::mcset de_de "of macro" "des Makros"


# helps.tcl
::msgcat::mcset de_de "About" "\u00DCber Scipad"
::msgcat::mcset de_de "aboutme_message"\
                   "\n Urspr\u00FCngliche Version von Joseph Acosta,\
                    joeja@mindspring.com.\n Ver\u00E4nderungen durch das Scilab\
                    Konsortium.\n \u00DCberarbeitung durch Enrico Segre\
                    2003-2006.\n Verbesserungen (insbesondere Debugger) von\
                    Fran\u00E7ois Vogel 2004-2008.\n Schwedische \u00DCbersetzung\
                    von Gunnar Lindholm 2005.\n Norwegische \u00DCbersetzung von\
                    Torbj\u00F8rn Pettersen 2005.\n Deutsche \u00DCbersetzung von\
                    Dirk Reusch 2005-2007.\n Polnische \u00DCbersetzung von\
                    Bartosz Kosiorek 2005.\n Chinesische \u00DCbersetzung von\
                    Yung-Jang  Lee 2007. D\u00E4nische \u00DCbersetzung von Kim B.\
                    Hansen 2007."


# infomessages.tcl
::msgcat::mcset de_de "Line:" "Zeile:"
::msgcat::mcset de_de "Column:" "Spalte:"
::msgcat::mcset de_de "Line" "Zeile"
::msgcat::mcset de_de "in" "in"
::msgcat::mcset de_de "Logical line:" "Programmzeile:"
::msgcat::mcset de_de " \[ReadOnly\]" " \[Schreibschutz\]"
::msgcat::mcset de_de " (modified)" " (ge\u00E4ndert)"


# inputtext.tcl
::msgcat::mcset de_de "No corresponding <" "Keine entsprechende <"
::msgcat::mcset de_de "> found!" "> gefunden!"
::msgcat::mcset de_de "Code editing is not allowed during debug!"\
                   "W\u00E4hrend der Debug-Sitzung kann das Programm nicht\
                    ver\u00E4ndert werden !"


# menues.tcl
::msgcat::mcset de_de "&File" "&Datei"
::msgcat::mcset de_de "&Edit" "&Bearbeiten"
::msgcat::mcset de_de "&Search" "&Suchen"
::msgcat::mcset de_de "E&xecute" "&Ausf\u00FChren"
::msgcat::mcset de_de "&Debug" "D&ebug"
::msgcat::mcset de_de "S&cheme" "S&chema"
::msgcat::mcset de_de "&Options" "&Optionen"
::msgcat::mcset de_de "&Windows" "&Fenster"
::msgcat::mcset de_de "&Help" "&Hilfe"

# menues.tcl - File menu
::msgcat::mcset de_de "&New" "&Neu"
::msgcat::mcset de_de "&Open..." "&\u00D6ffnen..."
::msgcat::mcset de_de "Open &in new" "\u00D6&ffnen in neuem Fenster"
::msgcat::mcset de_de "&horizontal tile" "&rechts daneben"
::msgcat::mcset de_de "&vertical tile" "&darunter"
::msgcat::mcset de_de "Op&en source of..." "\u00D6ffn&en des Quellcodes von..."
::msgcat::mcset de_de "&Save" "&Speichern"
::msgcat::mcset de_de "Save &as..." "S&peichern unter..."
::msgcat::mcset de_de "Save a&ll" "A&lles speichern"
::msgcat::mcset de_de "&Revert..." "&Verwerfen..."
::msgcat::mcset de_de "Import &Matlab file..." "&Matlab-Datei importieren..."
::msgcat::mcset de_de "Create help s&keleton..."\
                   "&XML-Skelett f\u00FCr Hilfe-Datei..."
::msgcat::mcset de_de "Compile as &help page" "XML-&Hilfe-Datei \u00FCbersetzen"
::msgcat::mcset de_de "Open &function source" "&Quellcode der Funktion"
::msgcat::mcset de_de "Print Se&tup" "D&rucken einrichten..."
::msgcat::mcset de_de "&Print" "&Drucken"
::msgcat::mcset de_de "&Close file" "S&chlie\u00DFen"
::msgcat::mcset de_de "E&xit" "&Beenden"

# menues.tcl - Edit menu
::msgcat::mcset de_de "&Undo" "&R\u00FCckg\u00E4ngig"
::msgcat::mcset de_de "&Redo" "&Wiederholen"
::msgcat::mcset de_de "Cu&t" "&Ausschneiden"
::msgcat::mcset de_de "B&lock cut" "&Block ausschneiden"
::msgcat::mcset de_de "&Copy" "&Kopieren"
::msgcat::mcset de_de "&Paste" "&Einf\u00FCgen"
::msgcat::mcset de_de "&Block paste" "Bl&ock einf\u00FCgen"
::msgcat::mcset de_de "&Delete" "&L\u00F6schen"
::msgcat::mcset de_de "Select &All" "Alles &Markieren"
::msgcat::mcset de_de "Insert &Time/Date" "&Zeit/Datum einf\u00FCgen"
::msgcat::mcset de_de "Co&mment selection" "Markierter Bereich als &Kommentar"
::msgcat::mcset de_de "U&ncomment selection"\
                   "Markierter Bereich &ohne Kommentarzeichen"
::msgcat::mcset de_de "&Indent selection" "Mark&ierten Bereich einr\u00FCcken"
::msgcat::mcset de_de "Unin&dent selection" "Markier&ten Bereich ausr\u00FCcken"

# menues.tcl - Search menu
::msgcat::mcset de_de "&Find..." "&Suchen..."
::msgcat::mcset de_de "Find &Next" "Suche &fortsetzen"
::msgcat::mcset de_de "&Replace..." "&Ersetzen..."
::msgcat::mcset de_de "&Goto Line..." "&Gehe zu..."

# menues.tcl - Exec menu
::msgcat::mcset de_de "&Load into Scilab" "&In Scilab laden"
::msgcat::mcset de_de "&Evaluate selection" "&Markierten Bereich ausf\u00FChren"

# menues.tcl - Debug menu
::msgcat::mcset de_de "&Insert/Remove breakpoint" "&Stoppunkt setzen/l\u00F6schen"
::msgcat::mcset de_de "Remove &all breakpoints" "&Alle Stoppunkte l\u00F6schen"
::msgcat::mcset de_de "&Configure execution..." "A&usf\u00FChrung konfigurieren..."
::msgcat::mcset de_de "Go to next b&reakpoint"\
                   "Bis zum &n\u00E4chstem Stoppunkt"
::msgcat::mcset de_de "&Step by step" "S&chritt f\u00FCr Schritt"
::msgcat::mcset de_de "Step &into" "&Hinein"
::msgcat::mcset de_de "Step o&ver" "&\u00DCber"
::msgcat::mcset de_de "Step &out" "H&eraus"
::msgcat::mcset de_de "Run to re&turn point" "Aus&f\u00FChren bis R\u00FCcksprung"
::msgcat::mcset de_de "Run to c&ursor" "Ausf\u00FChren &bis Eingabezeiger"
::msgcat::mcset de_de "G&o on ignoring any breakpoint"\
                   "Ausf\u00FChrung &ohne Stop fortsetzen"
::msgcat::mcset de_de "Show &watch" "B&eobachten"
::msgcat::mcset de_de "&Break" "U&nterbrechen"
::msgcat::mcset de_de "Cance&l debug" "Abb&rechen"

#    ::msgcat::mcset fr "language scheme" "schÃ©ma langage"
# menues.tcl - Scheme menu
::msgcat::mcset de_de "S&cilab" "S&cilab"
::msgcat::mcset de_de "&XML" "&XML"
::msgcat::mcset de_de "&none" "Kei&ns"
::msgcat::mcset de_de "&Colorize" "&Farbig"

# menues.tcl - Options menu
::msgcat::mcset de_de "&Fonts..." "&Schriften..."
::msgcat::mcset de_de "&Colors" "&Farben"
::msgcat::mcset de_de "Colori&ze" "Farbi&ge Darstellung"
::msgcat::mcset de_de "&Always" "&Immer"
::msgcat::mcset de_de "Ask for &big files" "Bei gro\u00DFen &Dateien fragen"
::msgcat::mcset de_de "&Never" "&Nie"
::msgcat::mcset de_de "Colorize \'&strings\'" "Farbige \'&Zeichenketten\'"
::msgcat::mcset de_de "Line n&umbers" "&Zeilennummern"
::msgcat::mcset de_de "&Hide" "&Verstecken"
::msgcat::mcset de_de "&Left aligned" "&Linksb\u00FCndig"
::msgcat::mcset de_de "&Right aligned" "&Rechtsb\u00FCndig"
::msgcat::mcset de_de "Show c&ontinued lines" "F&ortgesetzte Zeilen anzeigen"
::msgcat::mcset de_de "Word &wrap" "&Automatischer Zeilenumbruch"
::msgcat::mcset de_de "&Double-click behavior" "Doppel&klick-Verhalten"
::msgcat::mcset de_de "&Linux" "&Linux"
::msgcat::mcset de_de "Windo&ws" "Windo&ws"
::msgcat::mcset de_de "&Scilab" "&Scilab"
::msgcat::mcset de_de "&Tabs and indentation" "&Tabulatoren und Einr\u00FCcken"
::msgcat::mcset de_de "Tab inserts &spaces" "&Tabulator f\u00FCgt Leerzeichen ein"
::msgcat::mcset de_de "&Indentation spaces" "&Leerzeichen f\u00FCr das Einr\u00FCcken"
::msgcat::mcset de_de "&Tab size (characters)" "Ta&bulatorbreite"
::msgcat::mcset de_de "Use &keyword indentation" "&Automatisch einr\u00FCcken"
::msgcat::mcset de_de "Com&pletion" "&Vervollst\u00E4ndigung"
::msgcat::mcset de_de "&Tab" "&Tab"
::msgcat::mcset de_de "&Control-Tab" "&Strg-Tab"
::msgcat::mcset de_de "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset de_de "&Shift-Tab" "S&hift-Tab"
::msgcat::mcset de_de "C&ontrol-Alt-Tab" "St&rg-Alt-Tab"
::msgcat::mcset de_de "S&hift-Control-Tab" "Sh&ift-Strg-Tab"
::msgcat::mcset de_de "Sh&ift-Alt-Tab" "Shi&ft-Alt-Tab"
::msgcat::mcset de_de "Shi&ft-Control-Alt-Tab" "Shift-Str&g-Alt-Tab"
::msgcat::mcset de_de "File&names" "&Dateinamen"
::msgcat::mcset de_de "&Full path" "&Vollst\u00E4ndiger Pfad"
::msgcat::mcset de_de "Full path if &ambiguous"\
                   "V&ollst\u00E4ndiger Pfad (falls mehrdeutig)"
::msgcat::mcset de_de "&Unambiguous pruned path" "&Eindeutig abgek\u00FCrzter Pfad"
::msgcat::mcset de_de "Windows menu &sorting" "So&rtierung im Fenstermen\u00FC"
::msgcat::mcset de_de "&File opening order" "Zuletzt ge&\u00F6ffnet"
::msgcat::mcset de_de "&Alphabetically" "&Alphabetisch"
::msgcat::mcset de_de "&Most recently used" "Zuletzt &benutzt"
::msgcat::mcset de_de "&Recent files" "Z&uletzt ge\u00F6ffnete Dateien"
::msgcat::mcset de_de "&Backup files depth" "A&nzahl der Sicherungskopien"
::msgcat::mcset de_de "&Locale" "S&prachumgebung"
::msgcat::mcset de_de "Execution &errors" "Fehler&meldungen"
::msgcat::mcset de_de "In Scilab &shell only"\
                   "Nur im &Scilab-Fenster"
::msgcat::mcset de_de "Copied in a &message box"\
                   "Zus\u00E4tzlich in einem &Dialog-Fenster"

# menues.tcl - Windows menu
::msgcat::mcset de_de "&Maximize" "&Maximieren"
::msgcat::mcset de_de "&Split" "&Teilen"
::msgcat::mcset de_de "S&plit (side by side)" "Teilen (&senkrecht)"
::msgcat::mcset de_de "Sp&lit file" "&Datei teilen"
::msgcat::mcset de_de "Spl&it file (side by side)"\
                   "D&atei teilen (senkrecht)"
::msgcat::mcset de_de "Tile all &vertically" "&\u00DCbereinander"
::msgcat::mcset de_de "Tile all &horizontally" "&Nebeneinander"
::msgcat::mcset de_de "Space sashes &evenly" "&Gleichm\u00E4\u00DFige Aufteilung"
::msgcat::mcset de_de "Opened Files" "Offene Dateien"

# menues.tcl - Help menu
::msgcat::mcset de_de "&Help..." "&Hilfe..."
::msgcat::mcset de_de "&What's?..." "&Was ist?..."
::msgcat::mcset de_de "&About" "\u00DCber &Scipad"
::msgcat::mcset de_de "Adding &translations..." "&\u00DCbersetzungen hinzuf\u00FCgen..."
::msgcat::mcset de_de "Adding Scipad Translations..."\
                   "Scipad \u00DCbersetzungen hinzuf\u00FCgen..."
::msgcat::mcset de_de "&Bugs \& wishlist" "&Fehler \& W\u00FCnsche"
::msgcat::mcset de_de "Scipad Known bugs" "Bekannte Fehler in Scipad"
::msgcat::mcset de_de "Change&log" "&Liste der \u00C4nderungen"
::msgcat::mcset de_de "Changes in the Scipad codebase"\
                   "\u00C4nderungen im Scipad Programm"


# popupmenus.tcl
::msgcat::mcset de_de "Open the source of" "Quellcode \u00F6ffnen"
::msgcat::mcset de_de "AddWatch" "Variable %s beobachten"
::msgcat::mcset de_de "AddGenExp" "%s als allgemeinen Ausdruck hinzuf\u00FCgen"
::msgcat::mcset de_de "Jump to" "Sprung nach"


# print.tcl
::msgcat::mcset de_de "Print Setup" "Drucken einrichten"
::msgcat::mcset de_de "Print Command: " "Befehl zum Drucken: "


# scilabexec.tcl
::msgcat::mcset de_de "No point in loading an empty file!"\
                   "Es macht keinen Sinn eine leere Datei in Scilab zu laden!"
::msgcat::mcset de_de "Silent file save failed"\
                   "Dateispeicherung im Hintergrund fehlgeschlagen"
::msgcat::mcset de_de "File could not be saved in a temporary\
                    location.\nOverwrite original file?"\
                   "Datei k\u00F6nnte nicht tempor\u00E4r gespeichert werden.\
                    Soll die Original-Datei \u00FCberschrieben werden?"
::msgcat::mcset de_de "Exec done" "Ausf\u00FChrung beendet"
::msgcat::mcset de_de "Matlab files" "Matlab-Dateien"
::msgcat::mcset de_de "Matlab file to convert" "Zu importierende Matlab-Datei"
::msgcat::mcset de_de "Scilab is converting, please hold on..."\
                   "Scilab konvertiert gerade, bitte warten Sie..."
::msgcat::mcset de_de "Matlab file import" "Import einer Matlab-Datei"
::msgcat::mcset de_de "Conversion of the file failed, see the Scilab window for\
                    details -- Perhaps report the error text and the\
                    offending Matlab file to"\
                   "Der Import der Datei ist fehlgeschlagen, siehe\
                    Scilab-Fenster f\u00FCr weitere Details -- Vielleicht\
                    sollten Sie die Fehlermeldungen und die verantwortliche\
                    Matlab-Datei weiterleiten:"
::msgcat::mcset de_de "Please select destination path for the xml source of the\
                    help file"\
                   "Bitte w\u00E4hlen sie einen Zielpfad f\u00FCr die XML-Hilfe-Datei"
::msgcat::mcset de_de "Older version of" "Es existiert eine \u00E4ltere Version von"
::msgcat::mcset de_de "found!" "!"
::msgcat::mcset de_de "An old version of" "Eine alte Version von"
::msgcat::mcset de_de "already exists: open the old file instead?"\
                   "existiert bereits: Wollen Sie sie \u00F6ffnen?"
::msgcat::mcset de_de "Existing file" "Datei existiert"
::msgcat::mcset de_de "New skeleton" "Neues XML-Skelett f\u00FCr Hilfe-Datei"
::msgcat::mcset de_de "ScilabEval command cannot be passed to Scilab!"\
                   "Ein \"ScilabEval\"-Befehl kann nicht an Scilab\
                    \u00FCbergeben werden!"
::msgcat::mcset de_de "impossibleScilabEval_message"\
                   "WARNUNG : Ein Befehl konnte nicht an Scilab \u00FCbertragen\
                    werden und wurde ignoriert.\n\n Die\
                    wahrscheinlichste Ursache hierf\u00FCr ist, da\u00DF Sie  keine\
                    Schreibrechte f\u00FCr das Scilab-Verzeichnis TMPDIR\
                    besitzen. Unvorhersehbare Ergebnisse k\u00F6nnen die Folge\
                    sein. Bei dem Befehl handelte  es sich um:\n\n-->"
::msgcat::mcset de_de "impossibleScilabEval_message2"\
                   "WARNUNG : Ein Befehl konnte, weil er zu lang ist,\
                    nicht an Scilab \u00FCbertragen werden und wurde ignoriert.\n\n\
                    Unvorhersehbare Ergebnisse k\u00F6nnen die Folge sein.\
                    Wenn Sie gerade den Debugger verwenden, dann versuchen Sie\
                    die Anzahl der zu beobachtenden Variablen zu verringern.\n\n\
                    Es handelt sich bei dem Befehl um:\n\n-->"
::msgcat::mcset de_de "(end of command skipped)" "(Restlichen Befehl \u00FCbersprungen)"
::msgcat::mcset de_de "Scilab working" "Scilab arbeitet"
::msgcat::mcset de_de "Scilab is working, please wait for the prompt to execute\
                    this command!"\
                   "Scilab arbeitet. Bitte warten Sie auf die\
                    Eingabeaufforderung, um den Befehl auszuf\u00FChren!"
::msgcat::mcset de_de "Scilab is working, wait for the prompt to load file"\
                   "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung,\
                    um eine Datei zu laden"
::msgcat::mcset de_de "Scilab is working, wait for the prompt to execute the\
                    selection."\
                   "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung,\
                    um den markierten Bereich auszuf\u00FChren."
::msgcat::mcset de_de "Scilab is working, wait for the prompt to convert a\
                    Matlab file."\
                   "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung,\
                    um eine Matlab-Datei zu importieren."
::msgcat::mcset de_de "Scilab is working, wait for the prompt to compile the\
                    help file."\
                   "Scilab arbeitet! Bitte warten Sie auf die\
                    Eingabeaufforderung, um die Hilfe-Datei zu \u00FCbersetzen"
::msgcat::mcset de_de "Scilab is working, wait for the prompt to issue a\
                    debugger command."\
                   "Scilab arbeitet! Warten Sie auf die Eingabeaufforderung,\
                    um einen Debug-Befehl auszuf\u00FChren."
::msgcat::mcset de_de "Scilab execution error"\
                   "Fehler bei der Ausf\u00FChrung in Scilab"
::msgcat::mcset de_de "The shell reported an error while trying to execute "\
                   "Die Shell signalisierte einen Fehler bei der Ausf\u00FChrung\
                    von "
::msgcat::mcset de_de ": error " ": Fehler "
::msgcat::mcset de_de "at line " "in Zeile "
::msgcat::mcset de_de " of " " von "
::msgcat::mcset de_de "Execution aborted!" "Ausf\u00FChrung abgebrochen!"


# scipad.tcl
::msgcat::mcset de_de "NOT CONNECTED TO SCILAB" "KEINE VERBINDUNG ZU SCILAB"


# whichfun.tcl
::msgcat::mcset de_de "Being at line" "Wir befinden uns in Zeile"
::msgcat::mcset de_de ", function" ", die Funktion"
::msgcat::mcset de_de "begins at" "beginnt bei"
::msgcat::mcset de_de ", and there are" ", und es gibt"
::msgcat::mcset de_de "continued lines, i.e. we are at line"\
                   "Fortsetzungszeilen, d.h. wir befinden uns in Zeile"
::msgcat::mcset de_de "of" "von"
::msgcat::mcset de_de "Function" "Funktion"
::msgcat::mcset de_de "line" "Zeile"
::msgcat::mcset de_de "The cursor is not currently inside a function body."\
                   "Der Eingabezeiger befindet sich nicht innerhalb einer\
                    Funktionsdefinition."
::msgcat::mcset de_de "Which function?" "Welche Funktion?"


#preference variables description
::msgcat::mcset de_de "BGCOLOR" "Hintergrund"
::msgcat::mcset de_de "BGLNMARGCOLOR" "Zeilennummern (Hintergrund)"
::msgcat::mcset de_de "FGCOLOR" "Normaler Text"
::msgcat::mcset de_de "FGLNMARGCOLOR" "Zeilennummern"
::msgcat::mcset de_de "CURCOLOR" "Eingabezeiger"
::msgcat::mcset de_de "PARCOLOR" "Runde Klammern ()"
::msgcat::mcset de_de "BRAKCOLOR" "Eckige Klammern \\\[\\\]"
::msgcat::mcset de_de "BRACCOLOR" "Geschweifte Klammern {}"
::msgcat::mcset de_de "PUNCOLOR" "Interpunktion"
::msgcat::mcset de_de "INTFCOLOR" "Eingebaute Funktionen"
::msgcat::mcset de_de "COMMCOLOR" "Befehle"
::msgcat::mcset de_de "SCICCOLOR" "Scicos-Funktionen"
::msgcat::mcset de_de "OPCOLOR" "Operatoren"
::msgcat::mcset de_de "QTXTCOLOR" "Zeichenketten"
::msgcat::mcset de_de "REMCOLOR" "Kommentare"
::msgcat::mcset de_de "XMLCOLOR" "XML-Marken"
::msgcat::mcset de_de "NUMCOLOR" "Zahlen"
::msgcat::mcset de_de "SELCOLOR" "Markierte Bereiche"
::msgcat::mcset de_de "BREAKPOINTCOLOR" "Stoppunkte"
::msgcat::mcset de_de "LFUNCOLOR" "Bibliotheksfunktionen"
::msgcat::mcset de_de "PDEFCOLOR" "Vordefinierte Variablen"
::msgcat::mcset de_de "FOUNDTEXTCOLOR" "Suchergebnisse"
::msgcat::mcset de_de "CONTLINECOLOR" "Fortgesetzte Zeile"
::msgcat::mcset de_de "USERFUNCOLOR" "Benutzerdefinierte Funktionen"


#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset de_de "&Directory:" "&Verzeichnis:"
::msgcat::mcset de_de "File &name:" "Datei&name:"
::msgcat::mcset de_de "Files of &type:" "Datei&typen:"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
::msgcat::mcset de_de "&Open" "&\u00D6ffnen"
::msgcat::mcset de_de "Open" "\u00D6ffnen"
::msgcat::mcset de_de "Save As" "Speichern unter"
::msgcat::mcset de_de "&Cancel" "&Abbrechen"
::msgcat::mcset de_de "&Selection:" "Au&swahl:"
::msgcat::mcset de_de "&Yes" "&Ja"
::msgcat::mcset de_de "&No" "&Nein"
::msgcat::mcset de_de "&Red" "&Rot"
::msgcat::mcset de_de "&Green" "&Gr\u00FCn"
::msgcat::mcset de_de "&Blue" "&Blau"
::msgcat::mcset de_de "Directory \"%1\$s\" does not exist."\
                   "Das Verzeichnis \"%1\$s\" existiert nicht."

