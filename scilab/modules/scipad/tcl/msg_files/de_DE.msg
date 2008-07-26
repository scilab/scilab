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
::msgcat::mcset de_DE "Cancel" "Abbrechen"
::msgcat::mcset de_DE "Cance&l" "A&bbrechen"
::msgcat::mcset de_DE "OK" "OK"
::msgcat::mcset de_DE "All files" "Alle Dateien"
::msgcat::mcset de_DE "Add/Change" "Hinzuf\u00FCgen/\u00C4ndern"
::msgcat::mcset de_DE "Add/Chan&ge" "&Hinzuf\u00FCgen/\u00C4ndern"
::msgcat::mcset de_DE "&Remove" "&Entfernen"
::msgcat::mcset de_DE "Rem&ove" "Ent&fernen"
::msgcat::mcset de_DE "Untitled" "OhneNamen"
::msgcat::mcset de_DE "Close" "Schlie\u00DFen"
::msgcat::mcset de_DE "&Close" "S&chlie\u00DFen"
::msgcat::mcset de_DE "Error " "Fehler "
::msgcat::mcset de_DE "The file" "Die Datei"


# buffernavigation.tcl
::msgcat::mcset de_DE "Hide" "Verstecken"
::msgcat::mcset de_DE "Close this tile, keep content"\
                   "Diese Ansicht schlie\u00DFen (Datei ge\u00F6ffnet lassen)"
::msgcat::mcset de_DE "Close this tile" "Diese Ansicht schlie\u00DFen"
::msgcat::mcset de_DE "Goto Line?" "Gehe zu Zeile?"
::msgcat::mcset de_DE "Go to:" "Gehe zu:"
::msgcat::mcset de_DE "&logical line" "&Programmzeile"
::msgcat::mcset de_DE "&physical line" "&Textzeile"
::msgcat::mcset de_DE "in &function" "der &Funktion"
::msgcat::mcset de_DE "in &current file" "der aktuellen &Datei"
::msgcat::mcset de_DE "Outside of function definition"\
                   "Au\u00DFerhalb der Funktionsdefinition"


# colorize.tcl
::msgcat::mcset de_DE "Wait seconds while recolorizing file"\
                   "Warten Sie einen Moment w\u00E4hrend die farbige\
                    Darstellung aktualisiert wird"
::msgcat::mcset de_DE "You can't do that while colorization is in progress.\
                    Please try again when finished."\
                   "Aktion kann w\u00E4hrend der Aktualsierung der farbigen\
                    Darstellung nicht ausgef\u00FChrt werden.\
                    Versuchen Sie es sp\u00E4ter noch einmal."
::msgcat::mcset de_DE "Command forbidden during colorization"\
                   "Befehl kann w\u00E4hrend der Aktualisierung der farbigen
                    Darstellung nicht ausge\u00FChrt werden."


# completion.tcl
::msgcat::mcset de_DE "No possible completion"\
                   "Keine Vervollst\u00E4ndigung m\u00F6glich"
::msgcat::mcset de_DE "possible completions" "m\u00F6gliche Vervollst\u00E4ndigungen"
::msgcat::mcset de_DE "Possible completions" "M\u00F6gliche Vervollst\u00E4ndigungen"


# db_argsmanagement.tcl
::msgcat::mcset de_DE "Add" "Hinzuf\u00FCgen"
::msgcat::mcset de_DE "This variable can be watched but cannot be edited!"\
                   "Diese Variable kann beobachtet aber nicht ver\u00E4ndert werden!"
::msgcat::mcset de_DE "A new watch variable will be created."\
                   "Eine neue Variable zum Beobachten wird angelegt."
::msgcat::mcset de_DE "Non editable variable" "Nicht ver\u00E4nderbare Variable"
::msgcat::mcset de_DE "Variable:" "Variable:"
::msgcat::mcset de_DE "Expression:" "Ausdruck:"
::msgcat::mcset de_DE "Value:" "Wert:"
::msgcat::mcset de_DE "&Get current value from Scilab"\
                   "&Aktuellen Wert aus Scilab \u00FCbernehmen"
::msgcat::mcset de_DE "Non watchable variable" "Nicht beobachtbare Variable"
::msgcat::mcset de_DE "Sorry, variable" "Entschuldigung, die Variable"
::msgcat::mcset de_DE "is special and cannot be watched."\
                   "ist von einem Typ, der nicht beobachtet werden kann."


# db_bptsmanagement.tcl
::msgcat::mcset de_DE "No breakpoint here!"\
                   "Hier kann kein Stoppunkt gesetzt werden!"


# db_configureexec.tcl
::msgcat::mcset de_DE "Configure execution" "Ausf\u00FChrung konfigurieren"
::msgcat::mcset de_DE "Function name:" "Funktionsname:"
::msgcat::mcset de_DE "&Obtain" "Erh\u00E4l&t"
::msgcat::mcset de_DE "Input arguments:" "Eingabeargumente:"
::msgcat::mcset de_DE "The selected function has not been found in the opened\
                    files, maybe it was deleted since last configuration of\
                    the debugger.\nUse button \""\
                   "Die gew\u00E4hlte Funktion wurde in den ge\u00F6ffneten Dateien\
                    nicht gefunden. Wahrscheinlich wurde Sie nach der letzten\
                    Konfiguration des Debuggers gel\u00F6scht.\
                    Dr\u00FCcken Sie \""
::msgcat::mcset de_DE "\" to refresh the list of available functions."\
                   "\" um die Liste der verf\u00FCgbaren Funktionen zu aktualisieren."
::msgcat::mcset de_DE "Function name or input arguments do not match definition\
                    of the function"\
                   "Funktionsname und Eingabeargumente stimmen nicht mit der\
                    Funktionsdefinition \u00FCberein"
::msgcat::mcset de_DE "in the file!\n\nCheck function name and arguments\
                    (names, order) in the configuration dialog. \nArguments\
                    order can be changed using drag and drop with right\
                    mouse button in the arguments listbox."\
                   "in der Datei!\n\n\u00DCberpr\u00FCfen Sie den Funktionsnamen\
                    und die Argumente (Namen, Reihenfolge) im\
                    Konfigurationsdialog. Die Reihenfolge der Argumente kann\
                    mittels Drag&Drop (rechte Maustaste) ver\u00E4ndert werden."
::msgcat::mcset de_DE "Error in selected function name or arguments"\
                   "Fehler in markiertem Funktionsnamen oder markierten\
                    Argumenten"
::msgcat::mcset de_DE "At least one input argument does not match definition of\
                    the function"\
                   "Mindestens ein Eingabeargument entspricht nicht der\
                    Funktionsdefinition"
::msgcat::mcset de_DE "in the file!\n\nCheck function arguments (their names)\
                    in the configuration dialog."\
                   "in der Datei!\n\n\u00DCberpr\u00FCfen Sie den Funktionsnamen\
                    und die Argumente (Namen, Reihenfolge) im\
                    Konfigurationsdialog."
::msgcat::mcset de_DE "Error in selected function arguments"\
                   "Fehler in den Argumenten der ausgew\u00E4hlten Funktion"
::msgcat::mcset de_DE "Main level code found" "Code f\u00FCr die Hauptebene"
::msgcat::mcset de_DE "Scipad ancillary file" "Scipad-Hilfsdatei"
::msgcat::mcset de_DE "The current file contains only Scipad ancillaries that\
                    cannot be debugged in Scipad."\
                   "Die aktuelle Datei enth\u00E4lt nur Hilfsfunktionen von Scipad,\
                    die nicht mit dem Scipad-Debugger ausgef\u00FChrt werden k\u00F6nnen."
::msgcat::mcset de_DE "This file contains main level code.\nDebug as a single\
                    .sce file?"\
                   "Diese Datei enth\u00E4lt Code f\u00FCr die Hauptebene. Soll der\
                    Debugger sie wie eine .sce Datei behandeln?"
::msgcat::mcset de_DE "No function found" "Keine Funktion gefunden"
::msgcat::mcset de_DE "This file contains no function and will be treated"\
                   "Diese Datei enth\u00E4lt keine Funktion und wird"
::msgcat::mcset de_DE "as a single .sce file.\nStart debugging?"\
                   "wie eine .sce Datei behandelt. Debugger starten?"
::msgcat::mcset de_DE "You must enable colorization to be able to launch the\
                    debugger!"\
                   "Farbige Darstellung mu\u00DF aktiviert sein, um den Debugger\
                    starten zu k\u00F6nnen!"
::msgcat::mcset de_DE "The debugger needs colorization"\
                   "Der Debugger ben\u00F6tigt die farbige Darstellung"
::msgcat::mcset de_DE "Warning!\n\n  A function missing the closing\
                    \"endfunction\" has been found in the currently opened\
                    files.\n  Such syntax is now obsolete.\n\n  Please\
                    terminate function "\
                   "Warnung!\n In den ge\u00F6ffneten Dateien wurde eine Funktion\
                    gefunden, die nicht mit \"endfunction\" endet.\
                    Diese Syntax ist nicht mehr zul\u00E4ssig. Bitte\
                    beenden Sie die Funktion "
::msgcat::mcset de_DE " and try again." " mit \"endfunction\" und versuchen\
                    Sie es erneut."
::msgcat::mcset de_DE "Note: This message might also be triggered by a string\n \
                    quoted with single quotes and containing the word\
                    \"function\",\n  when \"Colorize 'strings'\" is\
                    unchecked in the Options menu."\
                   "Diese Meldung kann auch durch eine Zeichenkette\
                    \"function\" in einfachen Anf\u00FChrungszeichen ausgel\u00F6st\
                    werden, wenn die \"Farbige 'Zeichenketten'\" im Men\u00FC\
                    Optionen nicht aktiviert ist."
::msgcat::mcset de_DE "This known bug can be worked around simply by checking\
                    \"Colorize \'&strings'\"."\
                   "Dieser bekannte Fehler kann durch die Aktvierung von\
                    \"Farbige 'Zeichenketten'\" einfach behoben werden."
::msgcat::mcset de_DE "Unterminated function definition found"\
                   "Funktionsdefinition ohne \"endfunction\" gefunden"
::msgcat::mcset de_DE "Warning!\n\n  One or more function is defined more than\
                    once in the currently opened files.\n  The debugger\
                    cannot cope with multiple copies of functions.\n  The\
                    following duplicates were detected:\n"\
                   "Warnung!\n\n Eine oder mehrere Funktionen sind in den\
                    ge\u00F6ffneten Dateien mehrfach definiert. Der Debugger kann\
                    mit mehrfach definierten Funktionen nicht arbeiten. Dies\
                    betrifft folgende Funktionen:\n"
::msgcat::mcset de_DE "\n--> Definition of function "\
                   "\n--> Definition der Funktion "
::msgcat::mcset de_DE " found in:\n" "gefunden in:\n"
::msgcat::mcset de_DE "Duplicate function definitions found"\
                   "Doppelte Funktionsdefinition gefunden"


# db_debugsession.tcl
::msgcat::mcset de_DE "Too many breakpointed functions"\
                   "Zu viele Funktionen mit Stoppunkten"
::msgcat::mcset de_DE "You have currently" "Sie haben im Moment"
::msgcat::mcset de_DE "functions in your opened files."\
                   "Funktionen in Ihren ge\u00F6ffneten Dateien."
::msgcat::mcset de_DE "Scilab supports a maximum of"\
                   "Scilab unterst\u00FCtzt maximal"
::msgcat::mcset de_DE "possible breakpointed functions (see help setbpt)."\
                   "Funktionen mit Stoppunkten (siehe help setbpt)."
::msgcat::mcset de_DE "Step-by-step hence cannot be performed."\
                   "\"Schritt-f\u00FCr-Schritt\"-Modus kann nicht ausgef\u00FChrt werden."
::msgcat::mcset de_DE "This command will actually run to the next breakpoint."\
                   "Dieser Befehl wird bis zum n\u00E4chsten Stoppunkt ausf\u00FChren."
::msgcat::mcset de_DE "Break hence cannot be performed."\
                   "\"Unterbrechen\" ist deshalb nicht m\u00F6glich."
::msgcat::mcset de_DE "Too many breakpoints" "Zu viele Stoppunkte"
::msgcat::mcset de_DE "Executing this command would require to set"\
                   "Dieser Befehl erfordert das Setzen von"
::msgcat::mcset de_DE "breakpoints in your opened files."\
                   "Stoppunkten in Ihren ge\u00F6ffneten Dateien."
::msgcat::mcset de_DE "possible breakpoints in Scilab (see help setbpt)."\
                   "m\u00F6gliche Stoppunkte (siehe help setbpt)."
::msgcat::mcset de_DE "Cursor must be in a function"\
                   "Eingabezeiger mu\u00DF sich innerhalb einer Funktion befinden"
::msgcat::mcset de_DE "This command cannot succeed if the function where the\
                    debug just stopped is not opened in Scipad!"\
                   "Dieser Befehl kann nicht fortgesetzt werden, wenn die\
                    Funktion, in der der Debugger sich gerade befindet, nicht\
                    in Scipad ge\u00F6ffnet ist!"
::msgcat::mcset de_DE "Cannot run to return point"\
                   "Ausf\u00FChrung bis zum R\u00FCcksprung ist nicht m\u00F6glich"
::msgcat::mcset de_DE "Warning!\nThe line where the debugger just stopped\
                    contains a return statement (either endfunction, return\
                    or resume) but this statement is not the first one on\
                    the line."\
                   "Warnung!\n Der Debugger befindet sich in einer Zeile\
                    mit R\u00FCcksprunganweisung (endfunction, return oder resume),\
                    die nicht die erste Anweisung in dieser Zeile ist."
::msgcat::mcset de_DE "As a consequence the debugger cannot predict if the next\
                    debug step will really jump outside of the current\
                    function, which is what you just asked the debugger to\
                    do."\
                   "Deshalb kann der Debugger nicht vorhersagen, ob, wie Sie\
                    es w\u00FCnschten, im n\u00E4chsten Schritt aus der aktuellen\
                    Funktion herausgesprungen wird."
::msgcat::mcset de_DE "To avoid this unresolvable case, it is recommended to\
                    place the return statement alone on its line."\
                   "Dieser Zweifelsfall kann vermieden werden. Es wird\
                    empfohlen die R\u00FCcksprunganweisung auf einer separaten\
                    Programmzeile zu platzieren."
::msgcat::mcset de_DE "Do you want to see this warning again during this Scipad\
                    session?"\
                   "M\u00F6chten in dieser Scipad-Sitzung diese Warnung erneut\
                    erhalten?"
::msgcat::mcset de_DE "Unresolvable case found while running to return point"\
                   "Zweifelsfall w\u00E4hrend \"Ausf\u00FChren bis R\u00FCcksprung\" gefunden"
::msgcat::mcset de_DE "No effect - The debugged file is not stuck"\
                   "Keine Wirkung - Die Ausf\u00FChrung der Debug-Datei wurde\
                    nicht unterbrochen"


# db_init.tcl
::msgcat::mcset de_DE "Please wait..." "Bitte warten Sie..."


# db_states.tcl
::msgcat::mcset de_DE "Currently no debug session"\
                   "Im Moment l\u00E4uft keine Debug-Sitzung"
::msgcat::mcset de_DE "Ready to start debug"\
                   "Bereit zum Starten einer Debug-Sitzung"
::msgcat::mcset de_DE "Debug in progress" "Debug-Vorgang l\u00E4uft"
::msgcat::mcset de_DE "Scilab is back at the main level now."\
                   "Scilab ist jetzt wieder in der Hauptebene."


# db_watch.tcl
::msgcat::mcset de_DE "Watch" "Beobachten"
::msgcat::mcset de_DE "Watch variables:" "Beobachtete Variablen:"
::msgcat::mcset de_DE "A&uto (local variables)"\
                   "&Automatisch (alle lokalen Variablen)"
::msgcat::mcset de_DE "Au&to watch globals too"\
                   "Automatisch auch &globale Variablen"
::msgcat::mcset de_DE "Generic expressions:" "Allgemeine Ausdr\u00FCcke:"
::msgcat::mcset de_DE "Call stack:" "Ausf\u00FChrungsstapel:"
::msgcat::mcset de_DE "Always on to&p" "Immer im Vordergrund"
::msgcat::mcset de_DE "Hide watch &variables" "Beobachtete &Variablen verstecken"
::msgcat::mcset de_DE "Hide e&xpressions" "A&usdr\u00FCcke verstecken"
::msgcat::mcset de_DE "Hide call &stack" "Ausf\u00FChrungs&stapel verstecken"
::msgcat::mcset de_DE "&Dock" "Einge&bettet"
::msgcat::mcset de_DE "&Add" "Hinzuf\u00FCgen"
::msgcat::mcset de_DE "&Filter errors" "Fehler nicht anzeigen"
::msgcat::mcset de_DE "Breakpoints" "Stoppunkte"
::msgcat::mcset de_DE "Breakpointed functions" "Funktionen mit Stoppunkten"
::msgcat::mcset de_DE "Scilab busy indicator"\
                   "Aktivit\u00E4tsenzeige von Scilab"
::msgcat::mcset de_DE "Debugger state indicator"\
                   "Zustandsanzeige des Debuggers"
::msgcat::mcset de_DE "Undefined elements are not legal in variable"\
                   "Undefinierte Elemente sind in dieser Variable nicht\
                    erlaubt"
::msgcat::mcset de_DE ".\nThis variable will not be updated in Scilab."\
                   ".\n Diese Variable wird in Scilab nicht aktualisiert."
::msgcat::mcset de_DE "Illegal undefined element found"\
                   "Nicht erlaubtes undefiniertes Element gefunden"
::msgcat::mcset de_DE "evaluates to:" "ergibt:"
::msgcat::mcset de_DE "produces error" "erzeugt Fehler"
::msgcat::mcset de_DE "Scipad cannot find source code for the selected\
                    function.\nPlease open the adequate file manually and\
                    try again."\
                   "Scipad kann den Quellcode f\u00FCr die ausgew\u00E4hlte Funktion nicht\
                    finden.\nBitte \u00D6ffnen Sie manuell die entsprechende Datei\
                    und versuchen Sie es noch einmal."
::msgcat::mcset de_DE "Missing source code" "Quellcode fehlt"


# filecommands.tcl
::msgcat::mcset de_DE "New script" "Neues Skript"
::msgcat::mcset de_DE "New view on the same file"\
                   "Neue Ansicht derselben Datei"
::msgcat::mcset de_DE "The contents of" "Der Inhalt von"
::msgcat::mcset de_DE "may have changed, do you wish to save your changes?"\
                   "wurde ver\u00E4ndert. M\u00F6chten Sie die \u00C4nderungen speichern?"
::msgcat::mcset de_DE "Save Confirm?" "Speichern best\u00E4tigen?"
::msgcat::mcset de_DE "Open source of..." "\u00D6ffnen des Quellcodes von..."
::msgcat::mcset de_DE "Open source of:" "\u00D6ffnen des Quellcodes von :"
::msgcat::mcset de_DE "Weird drag&drop" "Drag&Drop"
::msgcat::mcset de_DE "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or you\
                    hit a bug of the dnd mechanism."\
                   "repr\u00E4sentiert weder einen g\u00FCltigen Dateinamen, noch\
                    einen g\u00FCltigen Dateipfad. Es handelt es sich entweder um\
                    ein anderen Objekttyp, oder Sie haben einen Fehler im\
                    Drag&Drop-Mechanismus von Scipad gefunden."
::msgcat::mcset de_DE "File not found" "Datei wurde nicht gefunden"
::msgcat::mcset de_DE "does not exist anymore. Do you want to create an empty\
                    file with the same name?"\
                   "existiert nicht mehr. M\u00F6chten Sie eine leere Datei mit\
                    dem gleichen Namen erzeugen ?"
::msgcat::mcset de_DE "Open file" "Datei \u00F6ffnen"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset de_DE "is already opened! Save the current opened file to\
                    another name and reopen it from disk!"\
                   "wurde bereits ge\u00F6ffnet! Speichern Sie die Datei unter\
                    einem anderen Namen und \u00F6ffnen Sie sie wieder!"
::msgcat::mcset de_DE "File" "Datei"
::msgcat::mcset de_DE "saved" "gespeichert"
::msgcat::mcset de_DE "cannot be written!" "ist schreibgesch\u00FCtzt!"
::msgcat::mcset de_DE "The contents of " "Der Inhalt von"
::msgcat::mcset de_DE "has changed on disk, save it anyway?"\
                   "auf der Festplatte hat sich ver\u00E4ndert. M\u00F6chten Sie\
                    trotzdem abspeichern?"
::msgcat::mcset de_DE "has been modified outside of Scipad. Do you want to\
                    reload it?"\
                   "wurde au\u00DFerhalb von Scipad ver\u00E4ndert.  M\u00F6chten Sie sie\
                    neu laden?"
::msgcat::mcset de_DE "File has changed!" "Datei wurde ver\u00E4ndert!"
::msgcat::mcset de_DE "Save as" "Speichern unter"
::msgcat::mcset de_DE "Scilab files" "Scilab-Dateien"
::msgcat::mcset de_DE "Scicos files" "Scicos-Dateien"
::msgcat::mcset de_DE "XML files" "XML-Dateien"
::msgcat::mcset de_DE "Wait seconds while loading and colorizing file"\
                   "Bitte warten Sie einen Moment w\u00E4hrend Datei ge\u00F6ffnet\
                    wird"
::msgcat::mcset de_DE "The contents of this file may have changed, do you wish\
                    to save your changes?"\
                   "Diese Datei wurde ver\u00E4ndert. M\u00F6chten Sie ihre\
                    \u00C4nderungen speichern?"
::msgcat::mcset de_DE "Revert" "M\u00F6chten Sie auf die gespeicherte Version von"
::msgcat::mcset de_DE "to saved?" "zur\u00FCckgreifen?"
::msgcat::mcset de_DE "Revert Confirm?" "Verwerfen best\u00E4tigen?"
::msgcat::mcset de_DE "Unreadable file" "Unlesbare Datei!"
::msgcat::mcset de_DE "exists but is not readable!"\
                   "existiert, ist aber unlesbar!"
::msgcat::mcset de_DE "This file is rather large and colorization might take\
                    some time.\n\nColorize anyway?"\
                   "Diese Datei ist ziemlich gro\u00DF und die farbige Darstellung\
                    kann etwas Zeit in Anspruch nehmen. Trotzdem farbig\
                    darstellen?"
::msgcat::mcset de_DE "File size warning" "Warnung: Gro\u00DFe Datei"
::msgcat::mcset de_DE "Windows shortcut file" "Windows Shortcut-Datei"
#::msgcat::mcset fr "The file" "Le fichier" ;# defined in the "commons" section
::msgcat::mcset de_DE "is a Windows shortcut and will not be opened!"\
                   "ist eine Windows Shortcut-Datei und wird nicht ge\u00F6ffnet\
                    werden!"


# findreplace.tcl
::msgcat::mcset de_DE "Dialog box already open!"\
                   "Dialogfenster ist bereits ge\u00F6ffnet!"
::msgcat::mcset de_DE "The string" "Die Zeichenkette"
::msgcat::mcset de_DE "could not be found in"\
                   "konnte nicht gefunden werden. Durchsucht wurde"
::msgcat::mcset de_DE "could not be found in any of the opened files !"\
                   "konnte in keiner der ge\u00F6ffneten Dateien gefunden werden!"
::msgcat::mcset de_DE "replacements done" "Ersetzungen ausgef\u00FChrt"
::msgcat::mcset de_DE "Find" "Suchen"
::msgcat::mcset de_DE "No match found in the selection for" "Die Zeichenkette"
::msgcat::mcset de_DE "\nWould you like to look for it in the entire text?"\
                   "konnte im markierten Bereich nicht gefunden\
                    werden.\nM\u00F6chten Sie in der gesamten Datei danach\
                    suchen ?"
::msgcat::mcset de_DE "You are searching for an empty string!"\
                   "Nach einer leeren Zeichenkette kann nicht gesucht werden!"
::msgcat::mcset de_DE "Re&place" "&Ersetzen"
::msgcat::mcset de_DE "Find what:" "Suche nach:"
::msgcat::mcset de_DE "Replace with:" "Ersetze durch:"
::msgcat::mcset de_DE "Find &Next" "Suche &fortsetzen"
::msgcat::mcset de_DE "Replace &All" "&Alle Ersetzen"
::msgcat::mcset de_DE "Direction" "Richtung"
::msgcat::mcset de_DE "&Upwards" "&R\u00FCckw\u00E4rts"
::msgcat::mcset de_DE "&Downwards" "&Vorw\u00E4rts"
::msgcat::mcset de_DE "Text types" "Texttypen"
::msgcat::mcset de_DE "Find options" "Suchoptionen"
::msgcat::mcset de_DE "Match &whole word only" "Ganzes &Wort"
::msgcat::mcset de_DE "Match &case" "&Gro\u00DF-/Kleinschreibung beachten"
::msgcat::mcset de_DE "&Regular expression" "Reg&ul\u00E4rer Ausdruck"
::msgcat::mcset de_DE "In all &opened files" "In allen ge\u00F6ffneten &Dateien"
::msgcat::mcset de_DE "In &selection only" "In &markiertem Bereich"
::msgcat::mcset de_DE "In typed te&xt" "In Texttypen"
::msgcat::mcset de_DE "In a director&y" "In einem &Verzeichnis"
::msgcat::mcset de_DE "Find in files search options"\
                   "Optionen f\u00FCr die Suche in Verzeichnissen"
::msgcat::mcset de_DE "Directory r&ecurse search" "Re&kursive Suche"
::msgcat::mcset de_DE "Include &hidden files" "versteckte Dateien einbeziehen"
::msgcat::mcset de_DE "In files/file types:" "Dateitypen:"
::msgcat::mcset de_DE "In directory:" "Im Verzeichnis:"
::msgcat::mcset de_DE "Search for file names" "Suche nach Dateinamen"
::msgcat::mcset de_DE "You have given an empty search string. Are you sure you\
                    want to search among file names?"\
                   "Sie haben eine leere Zeichenkette angegeben. Sind sie\
                    sicher, da\u00DF Sie nach Dateinamen suchen wollen?"
::msgcat::mcset de_DE "Wrapped around" "Zeilenumbruch"
::msgcat::mcset de_DE "Back at the first file searched!"\
                   "Erste Datei der Suche wurde wieder erreicht !"
::msgcat::mcset de_DE "No more matches" "Keine weiteren Suchergebnisse"
::msgcat::mcset de_DE "The string to regexp search for cannot be compiled by\
                    Tcl.\nCheck again, maybe certain characters should be\
                    escaped."\
                   "Der regul\u00E4re Ausdruck kann in Tcl nicht interpetiert \
                    werden. \u00DCberpr\u00FCfen Sie, ob bestimmeten Zeichen ein \
                    ESC-Zeichen (\\) vorangestellt werden mu\u00DF."
::msgcat::mcset de_DE "Any single character" "Ein beliebiges Zeichen"
::msgcat::mcset de_DE "Zero or more" "Keins oder Mehr"
::msgcat::mcset de_DE "One or more" "Eins oder Mehr"
::msgcat::mcset de_DE "Zero or one" "Keins oder Eins"
::msgcat::mcset de_DE "Any one character in the set"\
                   "Beliebiges Zeichen aus der Menge"
::msgcat::mcset de_DE "Any one character not in the set"\
                   "Beliebiges Zeichen nicht aus der Menge"
::msgcat::mcset de_DE "Or" "Oder"
::msgcat::mcset de_DE "Beginning of line" "Zeilenanfang"
::msgcat::mcset de_DE "End of line" "Zeilenende"
::msgcat::mcset de_DE "Beginning of word" "Wortanfang"
::msgcat::mcset de_DE "End of word" "Wortende"
::msgcat::mcset de_DE "A letter" "Buchstabe"
::msgcat::mcset de_DE "A decimal digit" "Ziffer (dezimal)"
::msgcat::mcset de_DE "An alphanumeric (letter or digit)"\
                   "Alphanumerisches Zeichen (Buchstabe oder Ziffer)"
::msgcat::mcset de_DE "A space or tab character" "Leerzeichen oder Tabulator"
::msgcat::mcset de_DE "A rational number" "Rationale Zahl"
::msgcat::mcset de_DE "A floating point number" "Flie\u00DFkommazahl"
::msgcat::mcset de_DE "A Scilab comment" "Scilab-Kommentar"
::msgcat::mcset de_DE "A Scilab name" "Variable oder Scilab-SchlÃ¼sselwort"
::msgcat::mcset de_DE "A Scilab continued line (with dots)"\
                   "Scilab-Zeile fortgesetzt mit .."


# findinfiles.tcl
::msgcat::mcset de_DE "Directory access issue"\
                   "Fehler beim Zugriff auf Verzeichnis"
::msgcat::mcset de_DE "The directory you specified cannot be reached. The\
                    search won't bring any result."\
                   "Auf das angegebene Verzeichnis kann nicht zugegriffen\
                    werden.  Die Suche wird ergebnislos sein."
::msgcat::mcset de_DE "Some directories or files could not be open and were\
                    ignored during search.\n You might miss read access to\
                    them.\n\n The following ones were ignored"\
                   "Einige Verzeichnisse oder Dateien konnten nicht\
                    ge\u00F6ffnet  werden. Sie sind von Suchfunktion ignoriert\
                    worden.\n  M\u00F6glicherweise besitzen sie keine Leserechte\
                    f\u00FCr diese  Verzeichnisse/Dateien.\n\n Folgende\
                    Verzeichnisse/Dateien sind ignoriert worden"
::msgcat::mcset de_DE "Ignored files" "Ignorierte Dateien"
::msgcat::mcset de_DE "Searching in file:" "Suche in Datei :"
::msgcat::mcset de_DE "End of file search:" "Ende der Suche :"
::msgcat::mcset de_DE "matches found" "\u00DCbereinstimmungen gefunden"
::msgcat::mcset de_DE "searched files" "durchsuchte Dateien"
::msgcat::mcset de_DE "Constructing file list:" "Erzeugung der Dateiliste:"
::msgcat::mcset de_DE "&Previous" "&Zur\u00FCck"
::msgcat::mcset de_DE "&Next" "&Vorw\u00E4rts"
::msgcat::mcset de_DE "Scipad search results" "Ergebnisse der Scipad-Suche"


# fonts.tcl
::msgcat::mcset de_DE "Choose font" "Schrift ausw\u00E4hlen"
::msgcat::mcset de_DE "Family" "Schriftart"
::msgcat::mcset de_DE "Style" "Stil"
::msgcat::mcset de_DE "&Bold" "&Fett"
::msgcat::mcset de_DE "&Italic" "&Kursiv"
::msgcat::mcset de_DE "&Underline" "&Unterstrichen"
::msgcat::mcset de_DE "Size" "Gr\u00F6\u00DFe"
::msgcat::mcset de_DE "&Defaults" "&Voreinstellungen"
::msgcat::mcset de_DE "Use as..." "Verwenden f\u00FCr..."
::msgcat::mcset de_DE "&Text font" "Programmtext"
::msgcat::mcset de_DE "&Menu font" "Men\u00FCs"
::msgcat::mcset de_DE "Sample" "Muster"
::msgcat::mcset de_DE "Font size" "Schriftgr\u00F6\u00DFe"
::msgcat::mcset de_DE "Revert fonts" "Voreingestellte Schriften "
::msgcat::mcset de_DE "Are you sure you want to revert all fonts to defaults?"\
                   "Sind sie sicher, da\u00DF alle Schriften auf die voreingestellten\
                    Werte zur\u00FCckgesetzt werden sollen?"


# FormatStringsForWatch.sci
::msgcat::mcset de_DE " what you try to watch is not supported by the debugger"\
                   " Sie versuchen etwas zu beobachten, das der Debugger\
                    nicht unterst\u00FCtzt"
::msgcat::mcset de_DE " what you try to watch is of type "\
                   " Sie versuchen ein Objekt vom Typ "
::msgcat::mcset de_DE " - this type is not supported by the debugger"\
                   "zu beobachten - dieser Typ wird vom Debugger\
                    nicht unterst\u00FCtzt"
::msgcat::mcset de_DE "Type:" "Typ:"
::msgcat::mcset de_DE "(unsupported)" "(nicht unterst\u00FCtzt)"
::msgcat::mcset de_DE "size:" "Gr\u00F6\u00DFe:"
::msgcat::mcset de_DE "real" "reell"
::msgcat::mcset de_DE "complex" "komplex"
::msgcat::mcset de_DE "elements" "Elemente"
::msgcat::mcset de_DE "graphic handle" "Grafik-Objekt"
::msgcat::mcset de_DE "uncompiled function" "unkompilierte Funktion"
::msgcat::mcset de_DE "compiled function" "kompilierte Funktion"
::msgcat::mcset de_DE "library" "Bibliothek"
::msgcat::mcset de_DE "pointer" "Zeiger"
::msgcat::mcset de_DE "size implicit index" "Index impliziter Gr\u00F6\u00DFe"
::msgcat::mcset de_DE "primitive" "eingebaute Funktion"


# FormatWhereForWatch.sci
::msgcat::mcset de_DE "Breakpoint" "Stoppunkt"
::msgcat::mcset de_DE "called at line" "in Zeile"
::msgcat::mcset de_DE "of exec file" "der Datei"
::msgcat::mcset de_DE "of execstr instruction" "der Anweisung"
::msgcat::mcset de_DE "called under pause" "w\u00E4hrend \"pause\" aufgerufen"
::msgcat::mcset de_DE "of macro" "des Makros"


# helps.tcl
::msgcat::mcset de_DE "About" "\u00DCber Scipad"
::msgcat::mcset de_DE "aboutme_message"\
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
::msgcat::mcset de_DE "Line:" "Zeile:"
::msgcat::mcset de_DE "Column:" "Spalte:"
::msgcat::mcset de_DE "Line" "Zeile"
::msgcat::mcset de_DE "in" "in"
::msgcat::mcset de_DE "Logical line:" "Programmzeile:"
::msgcat::mcset de_DE " \[ReadOnly\]" " \[Schreibschutz\]"
::msgcat::mcset de_DE " (modified)" " (ge\u00E4ndert)"


# inputtext.tcl
::msgcat::mcset de_DE "No corresponding <" "Keine entsprechende <"
::msgcat::mcset de_DE "> found!" "> gefunden!"
::msgcat::mcset de_DE "Code editing is not allowed during debug!"\
                   "W\u00E4hrend der Debug-Sitzung kann das Programm nicht\
                    ver\u00E4ndert werden !"


# menues.tcl
::msgcat::mcset de_DE "&File" "&Datei"
::msgcat::mcset de_DE "&Edit" "&Bearbeiten"
::msgcat::mcset de_DE "&Search" "&Suchen"
::msgcat::mcset de_DE "E&xecute" "&Ausf\u00FChren"
::msgcat::mcset de_DE "&Debug" "D&ebug"
::msgcat::mcset de_DE "S&cheme" "S&chema"
::msgcat::mcset de_DE "&Options" "&Optionen"
::msgcat::mcset de_DE "&Windows" "&Fenster"
::msgcat::mcset de_DE "&Help" "&Hilfe"

# menues.tcl - File menu
::msgcat::mcset de_DE "&New" "&Neu"
::msgcat::mcset de_DE "&Open..." "&\u00D6ffnen..."
::msgcat::mcset de_DE "Open &in new" "\u00D6&ffnen in neuem Fenster"
::msgcat::mcset de_DE "&horizontal tile" "&rechts daneben"
::msgcat::mcset de_DE "&vertical tile" "&darunter"
::msgcat::mcset de_DE "Op&en source of..." "\u00D6ffn&en des Quellcodes von..."
::msgcat::mcset de_DE "&Save" "&Speichern"
::msgcat::mcset de_DE "Save &as..." "S&peichern unter..."
::msgcat::mcset de_DE "Save a&ll" "A&lles speichern"
::msgcat::mcset de_DE "&Revert..." "&Verwerfen..."
::msgcat::mcset de_DE "Import &Matlab file..." "&Matlab-Datei importieren..."
::msgcat::mcset de_DE "Create help s&keleton..."\
                   "&XML-Skelett f\u00FCr Hilfe-Datei..."
::msgcat::mcset de_DE "Compile as &help page" "XML-&Hilfe-Datei \u00FCbersetzen"
::msgcat::mcset de_DE "Open &function source" "&Quellcode der Funktion"
::msgcat::mcset de_DE "Print Se&tup" "D&rucken einrichten..."
::msgcat::mcset de_DE "&Print" "&Drucken"
::msgcat::mcset de_DE "&Close file" "S&chlie\u00DFen"
::msgcat::mcset de_DE "E&xit" "&Beenden"

# menues.tcl - Edit menu
::msgcat::mcset de_DE "&Undo" "&R\u00FCckg\u00E4ngig"
::msgcat::mcset de_DE "&Redo" "&Wiederholen"
::msgcat::mcset de_DE "Cu&t" "&Ausschneiden"
::msgcat::mcset de_DE "B&lock cut" "&Block ausschneiden"
::msgcat::mcset de_DE "&Copy" "&Kopieren"
::msgcat::mcset de_DE "&Paste" "&Einf\u00FCgen"
::msgcat::mcset de_DE "&Block paste" "Bl&ock einf\u00FCgen"
::msgcat::mcset de_DE "&Delete" "&L\u00F6schen"
::msgcat::mcset de_DE "Select &All" "Alles &Markieren"
::msgcat::mcset de_DE "Insert &Time/Date" "&Zeit/Datum einf\u00FCgen"
::msgcat::mcset de_DE "Co&mment selection" "Markierter Bereich als &Kommentar"
::msgcat::mcset de_DE "U&ncomment selection"\
                   "Markierter Bereich &ohne Kommentarzeichen"
::msgcat::mcset de_DE "&Indent selection" "Mark&ierten Bereich einr\u00FCcken"
::msgcat::mcset de_DE "Unin&dent selection" "Markier&ten Bereich ausr\u00FCcken"

# menues.tcl - Search menu
::msgcat::mcset de_DE "&Find..." "&Suchen..."
::msgcat::mcset de_DE "Find &Next" "Suche &fortsetzen"
::msgcat::mcset de_DE "&Replace..." "&Ersetzen..."
::msgcat::mcset de_DE "&Goto Line..." "&Gehe zu..."

# menues.tcl - Exec menu
::msgcat::mcset de_DE "&Load into Scilab" "&In Scilab laden"
::msgcat::mcset de_DE "&Evaluate selection" "&Markierten Bereich ausf\u00FChren"

# menues.tcl - Debug menu
::msgcat::mcset de_DE "&Insert/Remove breakpoint" "&Stoppunkt setzen/l\u00F6schen"
::msgcat::mcset de_DE "Remove &all breakpoints" "&Alle Stoppunkte l\u00F6schen"
::msgcat::mcset de_DE "&Configure execution..." "A&usf\u00FChrung konfigurieren..."
::msgcat::mcset de_DE "Go to next b&reakpoint"\
                   "Bis zum &n\u00E4chstem Stoppunkt"
::msgcat::mcset de_DE "&Step by step" "S&chritt f\u00FCr Schritt"
::msgcat::mcset de_DE "Step &into" "&Hinein"
::msgcat::mcset de_DE "Step o&ver" "&\u00DCber"
::msgcat::mcset de_DE "Step &out" "H&eraus"
::msgcat::mcset de_DE "Run to re&turn point" "Aus&f\u00FChren bis R\u00FCcksprung"
::msgcat::mcset de_DE "Run to c&ursor" "Ausf\u00FChren &bis Eingabezeiger"
::msgcat::mcset de_DE "G&o on ignoring any breakpoint"\
                   "Ausf\u00FChrung &ohne Stop fortsetzen"
::msgcat::mcset de_DE "Show &watch" "B&eobachten"
::msgcat::mcset de_DE "&Break" "U&nterbrechen"
::msgcat::mcset de_DE "Cance&l debug" "Abb&rechen"

#    ::msgcat::mcset fr "language scheme" "schÃ©ma langage"
# menues.tcl - Scheme menu
::msgcat::mcset de_DE "S&cilab" "S&cilab"
::msgcat::mcset de_DE "&XML" "&XML"
::msgcat::mcset de_DE "&none" "Kei&ns"
::msgcat::mcset de_DE "&Colorize" "&Farbig"

# menues.tcl - Options menu
::msgcat::mcset de_DE "&Fonts..." "&Schriften..."
::msgcat::mcset de_DE "&Colors" "&Farben"
::msgcat::mcset de_DE "Colori&ze" "Farbi&ge Darstellung"
::msgcat::mcset de_DE "&Always" "&Immer"
::msgcat::mcset de_DE "Ask for &big files" "Bei gro\u00DFen &Dateien fragen"
::msgcat::mcset de_DE "&Never" "&Nie"
::msgcat::mcset de_DE "Colorize \'&strings\'" "Farbige \'&Zeichenketten\'"
::msgcat::mcset de_DE "Line n&umbers" "&Zeilennummern"
::msgcat::mcset de_DE "&Hide" "&Verstecken"
::msgcat::mcset de_DE "&Left aligned" "&Linksb\u00FCndig"
::msgcat::mcset de_DE "&Right aligned" "&Rechtsb\u00FCndig"
::msgcat::mcset de_DE "Show c&ontinued lines" "F&ortgesetzte Zeilen anzeigen"
::msgcat::mcset de_DE "Word &wrap" "&Automatischer Zeilenumbruch"
::msgcat::mcset de_DE "&Double-click behavior" "Doppel&klick-Verhalten"
::msgcat::mcset de_DE "&Linux" "&Linux"
::msgcat::mcset de_DE "Windo&ws" "Windo&ws"
::msgcat::mcset de_DE "&Scilab" "&Scilab"
::msgcat::mcset de_DE "&Tabs and indentation" "&Tabulatoren und Einr\u00FCcken"
::msgcat::mcset de_DE "Tab inserts &spaces" "&Tabulator f\u00FCgt Leerzeichen ein"
::msgcat::mcset de_DE "&Indentation spaces" "&Leerzeichen f\u00FCr das Einr\u00FCcken"
::msgcat::mcset de_DE "&Tab size (characters)" "Ta&bulatorbreite"
::msgcat::mcset de_DE "Use &keyword indentation" "&Automatisch einr\u00FCcken"
::msgcat::mcset de_DE "Com&pletion" "&Vervollst\u00E4ndigung"
::msgcat::mcset de_DE "&Tab" "&Tab"
::msgcat::mcset de_DE "&Control-Tab" "&Strg-Tab"
::msgcat::mcset de_DE "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset de_DE "&Shift-Tab" "S&hift-Tab"
::msgcat::mcset de_DE "C&ontrol-Alt-Tab" "St&rg-Alt-Tab"
::msgcat::mcset de_DE "S&hift-Control-Tab" "Sh&ift-Strg-Tab"
::msgcat::mcset de_DE "Sh&ift-Alt-Tab" "Shi&ft-Alt-Tab"
::msgcat::mcset de_DE "Shi&ft-Control-Alt-Tab" "Shift-Str&g-Alt-Tab"
::msgcat::mcset de_DE "File&names" "&Dateinamen"
::msgcat::mcset de_DE "&Full path" "&Vollst\u00E4ndiger Pfad"
::msgcat::mcset de_DE "Full path if &ambiguous"\
                   "V&ollst\u00E4ndiger Pfad (falls mehrdeutig)"
::msgcat::mcset de_DE "&Unambiguous pruned path" "&Eindeutig abgek\u00FCrzter Pfad"
::msgcat::mcset de_DE "Windows menu &sorting" "So&rtierung im Fenstermen\u00FC"
::msgcat::mcset de_DE "&File opening order" "Zuletzt ge&\u00F6ffnet"
::msgcat::mcset de_DE "&Alphabetically" "&Alphabetisch"
::msgcat::mcset de_DE "&Most recently used" "Zuletzt &benutzt"
::msgcat::mcset de_DE "&Recent files" "Z&uletzt ge\u00F6ffnete Dateien"
::msgcat::mcset de_DE "&Backup files depth" "A&nzahl der Sicherungskopien"
::msgcat::mcset de_DE "&Locale" "S&prachumgebung"
::msgcat::mcset de_DE "Execution &errors" "Fehler&meldungen"
::msgcat::mcset de_DE "In Scilab &shell only"\
                   "Nur im &Scilab-Fenster"
::msgcat::mcset de_DE "Copied in a &message box"\
                   "Zus\u00E4tzlich in einem &Dialog-Fenster"

# menues.tcl - Windows menu
::msgcat::mcset de_DE "&Maximize" "&Maximieren"
::msgcat::mcset de_DE "&Split" "&Teilen"
::msgcat::mcset de_DE "S&plit (side by side)" "Teilen (&senkrecht)"
::msgcat::mcset de_DE "Sp&lit file" "&Datei teilen"
::msgcat::mcset de_DE "Spl&it file (side by side)"\
                   "D&atei teilen (senkrecht)"
::msgcat::mcset de_DE "Tile all &vertically" "&\u00DCbereinander"
::msgcat::mcset de_DE "Tile all &horizontally" "&Nebeneinander"
::msgcat::mcset de_DE "Space sashes &evenly" "&Gleichm\u00E4\u00DFige Aufteilung"
::msgcat::mcset de_DE "Opened Files" "Offene Dateien"

# menues.tcl - Help menu
::msgcat::mcset de_DE "&Help..." "&Hilfe..."
::msgcat::mcset de_DE "&What's?..." "&Was ist?..."
::msgcat::mcset de_DE "&About" "\u00DCber &Scipad"
::msgcat::mcset de_DE "Adding &translations..." "&\u00DCbersetzungen hinzuf\u00FCgen..."
::msgcat::mcset de_DE "Adding Scipad Translations..."\
                   "Scipad \u00DCbersetzungen hinzuf\u00FCgen..."
::msgcat::mcset de_DE "&Bugs \& wishlist" "&Fehler \& W\u00FCnsche"
::msgcat::mcset de_DE "Scipad Known bugs" "Bekannte Fehler in Scipad"
::msgcat::mcset de_DE "Change&log" "&Liste der \u00C4nderungen"
::msgcat::mcset de_DE "Changes in the Scipad codebase"\
                   "\u00C4nderungen im Scipad Programm"


# popupmenus.tcl
::msgcat::mcset de_DE "Open the source of" "Quellcode \u00F6ffnen"
::msgcat::mcset de_DE "AddWatch" "Variable %s beobachten"
::msgcat::mcset de_DE "AddGenExp" "%s als allgemeinen Ausdruck hinzuf\u00FCgen"
::msgcat::mcset de_DE "Jump to" "Sprung nach"


# print.tcl
::msgcat::mcset de_DE "Print Setup" "Drucken einrichten"
::msgcat::mcset de_DE "Print Command: " "Befehl zum Drucken: "


# scilabexec.tcl
::msgcat::mcset de_DE "No point in loading an empty file!"\
                   "Es macht keinen Sinn eine leere Datei in Scilab zu laden!"
::msgcat::mcset de_DE "Silent file save failed"\
                   "Dateispeicherung im Hintergrund fehlgeschlagen"
::msgcat::mcset de_DE "File could not be saved in a temporary\
                    location.\nOverwrite original file?"\
                   "Datei k\u00F6nnte nicht tempor\u00E4r gespeichert werden.\
                    Soll die Original-Datei \u00FCberschrieben werden?"
::msgcat::mcset de_DE "Exec done" "Ausf\u00FChrung beendet"
::msgcat::mcset de_DE "Matlab files" "Matlab-Dateien"
::msgcat::mcset de_DE "Matlab file to convert" "Zu importierende Matlab-Datei"
::msgcat::mcset de_DE "Scilab is converting, please hold on..."\
                   "Scilab konvertiert gerade, bitte warten Sie..."
::msgcat::mcset de_DE "Matlab file import" "Import einer Matlab-Datei"
::msgcat::mcset de_DE "Conversion of the file failed, see the Scilab window for\
                    details -- Perhaps report the error text and the\
                    offending Matlab file to"\
                   "Der Import der Datei ist fehlgeschlagen, siehe\
                    Scilab-Fenster f\u00FCr weitere Details -- Vielleicht\
                    sollten Sie die Fehlermeldungen und die verantwortliche\
                    Matlab-Datei weiterleiten:"
::msgcat::mcset de_DE "Please select destination path for the xml source of the\
                    help file"\
                   "Bitte w\u00E4hlen sie einen Zielpfad f\u00FCr die XML-Hilfe-Datei"
::msgcat::mcset de_DE "Older version of" "Es existiert eine \u00E4ltere Version von"
::msgcat::mcset de_DE "found!" "!"
::msgcat::mcset de_DE "An old version of" "Eine alte Version von"
::msgcat::mcset de_DE "already exists: open the old file instead?"\
                   "existiert bereits: Wollen Sie sie \u00F6ffnen?"
::msgcat::mcset de_DE "Existing file" "Datei existiert"
::msgcat::mcset de_DE "New skeleton" "Neues XML-Skelett f\u00FCr Hilfe-Datei"
::msgcat::mcset de_DE "ScilabEval command cannot be passed to Scilab!"\
                   "Ein \"ScilabEval\"-Befehl kann nicht an Scilab\
                    \u00FCbergeben werden!"
::msgcat::mcset de_DE "impossibleScilabEval_message"\
                   "WARNUNG : Ein Befehl konnte nicht an Scilab \u00FCbertragen\
                    werden und wurde ignoriert.\n\n Die\
                    wahrscheinlichste Ursache hierf\u00FCr ist, da\u00DF Sie  keine\
                    Schreibrechte f\u00FCr das Scilab-Verzeichnis TMPDIR\
                    besitzen. Unvorhersehbare Ergebnisse k\u00F6nnen die Folge\
                    sein. Bei dem Befehl handelte  es sich um:\n\n-->"
::msgcat::mcset de_DE "impossibleScilabEval_message2"\
                   "WARNUNG : Ein Befehl konnte, weil er zu lang ist,\
                    nicht an Scilab \u00FCbertragen werden und wurde ignoriert.\n\n\
                    Unvorhersehbare Ergebnisse k\u00F6nnen die Folge sein.\
                    Wenn Sie gerade den Debugger verwenden, dann versuchen Sie\
                    die Anzahl der zu beobachtenden Variablen zu verringern.\n\n\
                    Es handelt sich bei dem Befehl um:\n\n-->"
::msgcat::mcset de_DE "(end of command skipped)" "(Restlichen Befehl \u00FCbersprungen)"
::msgcat::mcset de_DE "Scilab working" "Scilab arbeitet"
::msgcat::mcset de_DE "Scilab is working, please wait for the prompt to execute\
                    this command!"\
                   "Scilab arbeitet. Bitte warten Sie auf die\
                    Eingabeaufforderung, um den Befehl auszuf\u00FChren!"
::msgcat::mcset de_DE "Scilab is working, wait for the prompt to load file"\
                   "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung,\
                    um eine Datei zu laden"
::msgcat::mcset de_DE "Scilab is working, wait for the prompt to execute the\
                    selection."\
                   "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung,\
                    um den markierten Bereich auszuf\u00FChren."
::msgcat::mcset de_DE "Scilab is working, wait for the prompt to convert a\
                    Matlab file."\
                   "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung,\
                    um eine Matlab-Datei zu importieren."
::msgcat::mcset de_DE "Scilab is working, wait for the prompt to compile the\
                    help file."\
                   "Scilab arbeitet! Bitte warten Sie auf die\
                    Eingabeaufforderung, um die Hilfe-Datei zu \u00FCbersetzen"
::msgcat::mcset de_DE "Scilab is working, wait for the prompt to issue a\
                    debugger command."\
                   "Scilab arbeitet! Warten Sie auf die Eingabeaufforderung,\
                    um einen Debug-Befehl auszuf\u00FChren."
::msgcat::mcset de_DE "Scilab execution error"\
                   "Fehler bei der Ausf\u00FChrung in Scilab"
::msgcat::mcset de_DE "The shell reported an error while trying to execute "\
                   "Die Shell signalisierte einen Fehler bei der Ausf\u00FChrung\
                    von "
::msgcat::mcset de_DE ": error " ": Fehler "
::msgcat::mcset de_DE "at line " "in Zeile "
::msgcat::mcset de_DE " of " " von "
::msgcat::mcset de_DE "Execution aborted!" "Ausf\u00FChrung abgebrochen!"


# scipad.tcl
::msgcat::mcset de_DE "NOT CONNECTED TO SCILAB" "KEINE VERBINDUNG ZU SCILAB"


# whichfun.tcl
::msgcat::mcset de_DE "Being at line" "Wir befinden uns in Zeile"
::msgcat::mcset de_DE ", function" ", die Funktion"
::msgcat::mcset de_DE "begins at" "beginnt bei"
::msgcat::mcset de_DE ", and there are" ", und es gibt"
::msgcat::mcset de_DE "continued lines, i.e. we are at line"\
                   "Fortsetzungszeilen, d.h. wir befinden uns in Zeile"
::msgcat::mcset de_DE "of" "von"
::msgcat::mcset de_DE "Function" "Funktion"
::msgcat::mcset de_DE "line" "Zeile"
::msgcat::mcset de_DE "The cursor is not currently inside a function body."\
                   "Der Eingabezeiger befindet sich nicht innerhalb einer\
                    Funktionsdefinition."
::msgcat::mcset de_DE "Which function?" "Welche Funktion?"


#preference variables description
::msgcat::mcset de_DE "BGCOLOR" "Hintergrund"
::msgcat::mcset de_DE "BGLNMARGCOLOR" "Zeilennummern (Hintergrund)"
::msgcat::mcset de_DE "FGCOLOR" "Normaler Text"
::msgcat::mcset de_DE "FGLNMARGCOLOR" "Zeilennummern"
::msgcat::mcset de_DE "CURCOLOR" "Eingabezeiger"
::msgcat::mcset de_DE "PARCOLOR" "Runde Klammern ()"
::msgcat::mcset de_DE "BRAKCOLOR" "Eckige Klammern \\\[\\\]"
::msgcat::mcset de_DE "BRACCOLOR" "Geschweifte Klammern {}"
::msgcat::mcset de_DE "PUNCOLOR" "Interpunktion"
::msgcat::mcset de_DE "INTFCOLOR" "Eingebaute Funktionen"
::msgcat::mcset de_DE "COMMCOLOR" "Befehle"
::msgcat::mcset de_DE "SCICCOLOR" "Scicos-Funktionen"
::msgcat::mcset de_DE "OPCOLOR" "Operatoren"
::msgcat::mcset de_DE "QTXTCOLOR" "Zeichenketten"
::msgcat::mcset de_DE "REMCOLOR" "Kommentare"
::msgcat::mcset de_DE "XMLCOLOR" "XML-Marken"
::msgcat::mcset de_DE "NUMCOLOR" "Zahlen"
::msgcat::mcset de_DE "SELCOLOR" "Markierte Bereiche"
::msgcat::mcset de_DE "BREAKPOINTCOLOR" "Stoppunkte"
::msgcat::mcset de_DE "LFUNCOLOR" "Bibliotheksfunktionen"
::msgcat::mcset de_DE "PDEFCOLOR" "Vordefinierte Variablen"
::msgcat::mcset de_DE "FOUNDTEXTCOLOR" "Suchergebnisse"
::msgcat::mcset de_DE "CONTLINECOLOR" "Fortgesetzte Zeile"
::msgcat::mcset de_DE "USERFUNCOLOR" "Benutzerdefinierte Funktionen"


#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset de_DE "&Directory:" "&Verzeichnis:"
::msgcat::mcset de_DE "File &name:" "Datei&name:"
::msgcat::mcset de_DE "Files of &type:" "Datei&typen:"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
::msgcat::mcset de_DE "&Open" "&\u00D6ffnen"
::msgcat::mcset de_DE "Open" "\u00D6ffnen"
::msgcat::mcset de_DE "Save As" "Speichern unter"
::msgcat::mcset de_DE "&Cancel" "&Abbrechen"
::msgcat::mcset de_DE "&Selection:" "Au&swahl:"
::msgcat::mcset de_DE "&Yes" "&Ja"
::msgcat::mcset de_DE "&No" "&Nein"
::msgcat::mcset de_DE "&Red" "&Rot"
::msgcat::mcset de_DE "&Green" "&Gr\u00FCn"
::msgcat::mcset de_DE "&Blue" "&Blau"
::msgcat::mcset de_DE "Directory \"%1\$s\" does not exist."\
                   "Das Verzeichnis \"%1\$s\" existiert nicht."

