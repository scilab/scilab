# Scipad texts in German
# in sync at least with v5.0
# provided by Dirk Reusch <reusch@tmb.uni-karlsruhe.de>

# commons
::msgcat::mcset de "Cancel" "Abbrechen"
::msgcat::mcset de "Cance&l" "Abbrechen (Ctrl+l)"
::msgcat::mcset de "OK" "OK"
::msgcat::mcset de "Scilab working" "Scilab arbeitet"
::msgcat::mcset de "All files" "Alle Dateien"
::msgcat::mcset de "Add/Change" "Hinzufügen/Ändern"
::msgcat::mcset de "Untitled" "OhneNamen"


# buffernavigation.tcl
::msgcat::mcset de "Goto Line?" "Gehe zu Zeile?"
::msgcat::mcset de "Goto Line: " "Gehe zu Zeile : "


# colorize.tcl
::msgcat::mcset de "Wait seconds while recolorizing file" "Warten Sie einen Moment während Farben aktualisiert werden"


# db_argsmanagement.tcl
::msgcat::mcset de "Variable:" "Variable :"
::msgcat::mcset de "Value:" "Wert :"
::msgcat::mcset de "Get current value from Scilab" "Verwende aktuellen Wert aus Scilab"

# db_configureexec.tcl
::msgcat::mcset de "Configure execution" "Ausführung konfigurieren"
::msgcat::mcset de "Function name:" "Funktionsname :"
::msgcat::mcset de "Obtain" "Erhält"
::msgcat::mcset de "Input arguments:" "Eingabeargumente :"
::msgcat::mcset de "Remove" "Entfernen"
::msgcat::mcset de "Check function definition: it must be on a single code line." \
                   "Überprüfen Sie die Funktionsdefinition, sie muß aus einer Programmzeile bestehen."
::msgcat::mcset de "Function name or input arguments do not match definition of the function" \
                    "Funktionsname und Eingabeargumente stimmen nicht mit der Funktionsdefinition überein"
    ::msgcat::mcset de "in the file!\n\nCheck function\
                        name and arguments (names, order) in the configuration dialog.\
                        \nArguments order can be changed using drag and drop with\
                        right mouse button in the arguments listbox." \
                       "In der Datei!\n\n\
                        Überprüfen Sie den Funktionsnamen und die Argumente\
                        (Namen, Reihenfolge) im Konfigurationsdialog.\
                        Die Reihenfolge der Argumente kann mittels Drag&Drop\
                        (rechte Maustaste) verändert werden."
::msgcat::mcset de "Error in selected function name or arguments" "Fehler in markiertem Funktionsnamen oder markierten Argumenten"
::msgcat::mcset de "Debugger is not yet developed for sce scripts!" "Debug-Programm kann zur Zeit noch nicht für Skripte (*.sce) eingesetzt werden!"


# db_debugsession.tcl
::msgcat::mcset de "Scilab is working, wait for the prompt to issue a debugger command." "Scilab arbeitet! Warten Sie auf die Eingabeaufforderung, um einen Debug-Befehl auszuführen."
::msgcat::mcset de "Cursor position is out of reach!" "Eingabezeiger befindet sich außerhalb der Reichweite!"
::msgcat::mcset de "Cursor must be in a function" "Eingabezeiger muß sich innerhalb einer Funktion befinden"
::msgcat::mcset de "No effect - The debugged file is not stuck" "Keine Wirkung - Die Ausführung der Debug-Datei wurde nicht unterbrochen"
::msgcat::mcset de "Scilab execution error" "Fehler bei der Ausführung in Scilab"
::msgcat::mcset de "The shell reported an error while trying to execute " "Die Shell signalisierte einen Fehler bei der Ausführung von "
    ::msgcat::mcset de " at line " " in Zeile "
    ::msgcat::mcset de " of " " von "
::msgcat::mcset de "Execution aborted!" "Ausführung abgebrochen!"


# db_init.tcl
::msgcat::mcset de "Please wait..." "Bitte warten Sie..."


# db_states.tcl
::msgcat::mcset de "Currently no debug session" "Im Moment läuft keine Debug-Sitzung"
::msgcat::mcset de "Ready to start debug" "Bereit zum Starten einer Debug-Sitzung"
::msgcat::mcset de "Debug in progress" "Debug-Vorgang läuft"


# db_watch.tcl
::msgcat::mcset de "Watch" "Beobachten"
::msgcat::mcset de "Watch variables:" "Beobachtete Variablen :"
::msgcat::mcset de "Remove" "Entfernen"
::msgcat::mcset de "Call stack:" "Ausführungsstapel:"
::msgcat::mcset de "Close" "Schließen"
::msgcat::mcset de "Show watch variables" "Beobachtete Variablen zeigen"
::msgcat::mcset de "Hide watch variables" "Beobachtete Variablen verstecken"
::msgcat::mcset de "Show call stack" "Ausführungsstapel zeigen"
::msgcat::mcset de "Hide call stack" "Ausführungsstapel verstecken"
#::msgcat::mcset de "Undefined elements are not legal in variable"
#::msgcat::mcset de ".\nThis variable will not be updated in Scilab."
#::msgcat::mcset de "Illegal undefined element found"


# filecommands.tcl
::msgcat::mcset de "New Script" "Neues Skript"
::msgcat::mcset de "The contents of" "Der Inhalt von"
    ::msgcat::mcset de "may have changed, do you wish to save your changes?" "wurde verändert. Möchten Sie die Änderungen speichern?"
::msgcat::mcset de "Save Confirm?" "Speichern bestätigen?"
::msgcat::mcset de "Weird drag&drop" "Drag&Drop"
::msgcat::mcset de "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                    "repräsentiert weder einen gültigen Dateinamen, noch\
                     einen gültigen Dateipfad. Entweder handelt es sich um\
                     ein anderen Objekttyp, oder Sie haben einen Fehler im\
                     Drag&Drop-Mechanismus von SciPad gefunden."
             
::msgcat::mcset de "File not found" "Datei wurde nicht gefunden"
::msgcat::mcset de "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "existiert nicht mehr. Möchten Sie eine leere Datei mit dem gleichen\
                    Namen erzeugen ?"
::msgcat::mcset de "Open file" "Datei öffnen"
::msgcat::mcset de "The file" "Die Datei"
    ::msgcat::mcset de "is already opened! Save the current opened file to another name and reopen it from disk!" "wurde bereits geöffnet! Speichern Sie die Datei unter einem anderen Namen und öffnen Sie sie wieder!"
::msgcat::mcset de "File" "Datei"
    ::msgcat::mcset de "saved" "gespeichert"
::msgcat::mcset de "cannot be written!" "ist schreibgeschützt!"
::msgcat::mcset de "The contents of " "Der Inhalt von"
    ::msgcat::mcset de "has changed on disk, save it anyway?" "auf der Festplatte hat sich verändert. Möchten Sie trotzdem abspeichern ?"
::msgcat::mcset de "File has changed!" "Datei wurde verändert!"
::msgcat::mcset de "Save as" "Speichern unter"
::msgcat::mcset de "Scilab files" "Scilab-Dateien"
::msgcat::mcset de "XML files" "XML-Dateien"
::msgcat::mcset de "Wait seconds while loading and colorizing file" "Bitte warten Sie einen Moment während Datei geöffnet wird"
::msgcat::mcset de "The contents of this file may have changed, do you wish to save your changes?" "Diese Datei wurde verändert. Möchten Sie ihre Änderungen speichern?"
::msgcat::mcset de "Revert" "Möchten Sie auf die gespeicherte Version von"
    ::msgcat::mcset de "to saved?" "zurückgreifen?"
::msgcat::mcset de "Revert Confirm?" "Verwerfen bestätigen?"


# findreplace.tcl
::msgcat::mcset de "The string" "Die Zeichenkette"
    ::msgcat::mcset de "could not be found" "konnte nicht gefunden werden"
::msgcat::mcset de "replacements done" "Ersetzungen ausgeführt"
::msgcat::mcset de "Find" "Suchen"
::msgcat::mcset de "No match found in the selection for" "Die Zeichenkette"
::msgcat::mcset de "\nWould you like to look for it in the entire text?" "konnte im markierten Bereich nicht gefunden werden.\nMöchten Sie in der gesamten Datei danach suchen ?"
::msgcat::mcset de "You are searching for an empty string!" "Nach einer leeren Zeichenkette kann nicht gesucht werden!"
::msgcat::mcset de "Re&place" "Ersetzen (Ctrl+p)"
::msgcat::mcset de "Find what:" "Suche nach:"
::msgcat::mcset de "Replace with:" "Ersetze durch:"
::msgcat::mcset de "Find &Next" "Suche fortsetze&n"
::msgcat::mcset de "Replace &All" "&Alle Ersetzen"
::msgcat::mcset de "&Upwards" "R&ueckwärts"
::msgcat::mcset de "Downward&s" "Vorwärt&s" 
::msgcat::mcset de "Match &case" "Groß-/Kleins&chreibung beachten"
::msgcat::mcset de "&Regular expression" "&Regulärer Ausdruck"


# helps.tcl
::msgcat::mcset de "About" "Über SciPad"
::msgcat::mcset de aboutme_message \
                   "\n Ursprüngliche Version von Joseph Acosta, joeja@mindspring.com.\n\
                    Veränderungen durch das Scilab Konsortium.\n\
                    Überarbeitung durch Enrico Segre 2003-2005.\n\
                    Verbesserungen (insbesondere Debugger) von François Vogel 2004-2005.\n\
                    Schwedische Übersetzung von Gunnar Lindholm 2005.\n\
                    Norwegische Übersetzung von Torbjørn Pettersen 2005.\n\
                    Deutsche Übersetzung von Dirk Reusch 2005."


# infomessages.tcl
::msgcat::mcset de "Line:" "Zeile:"
    ::msgcat::mcset de "Column:" "Spalte:"
::msgcat::mcset de "Line" "Zeile"
    ::msgcat::mcset de "in" "in"
::msgcat::mcset de " \[ReadOnly\]" " \[Schreibschutz\]"
::msgcat::mcset de " (modified)" " (geändert)"


# inputtext.tcl
::msgcat::mcset de "No corresponding <" "Keine entsprechende <"
    ::msgcat::mcset de "> found!" "> gefunden!"
::msgcat::mcset de "Code editing is not allowed during debug!" "Während der Debug-Sitzung kann das Programm nicht verändert werden !"


# menues.tcl
::msgcat::mcset de "&File" "&Datei"
::msgcat::mcset de "&Edit" "&Bearbeiten"
::msgcat::mcset de "&Search" "&Suchen"
::msgcat::mcset de "E&xecute" "&Ausführen"
::msgcat::mcset de "&Debug" "D&ebug"
::msgcat::mcset de "S&cheme" "S&chema"
::msgcat::mcset de "&Options" "&Optionen"
::msgcat::mcset de "&Windows" "&Fenster"
::msgcat::mcset de "&Help" "&Hilfe"

    # menues.tcl - File menu
    ::msgcat::mcset de "&New" "&Neu"
    ::msgcat::mcset de "&Open..." "&Öffnen..."
    ::msgcat::mcset de "&Save" "&Speichern"
    ::msgcat::mcset de "Save &as..." "S&peichern unter..."
    ::msgcat::mcset de "&Revert..." "&Verwerfen..."
    ::msgcat::mcset de "Import &Matlab file..."  "&Matlab-Datei importieren..."
    ::msgcat::mcset de "Create help s&keleton..." "&XML-Skelett für Hilfe-Datei..."
    ::msgcat::mcset de "Compile as &help page" "XML-&Hilfe-Datei übersetzen"
    ::msgcat::mcset de "Open &function source" "&Quellcode der Funktion"
    ::msgcat::mcset de "Print Se&tup" "D&rucken einrichten..."
    ::msgcat::mcset de "&Print" "&Drucken"
    ::msgcat::mcset de "&Close" "S&chließen"
    ::msgcat::mcset de "E&xit" "&Beenden"

    # menues.tcl - Edit menu
    ::msgcat::mcset de "&Undo" "&Rückgängig"
    ::msgcat::mcset de "&Redo" "&Wiederholen"
    ::msgcat::mcset de "Cu&t" "&Ausschneiden"
    ::msgcat::mcset de "&Copy" "&Kopieren"
    ::msgcat::mcset de "&Paste" "&Einfügen"
    ::msgcat::mcset de "&Delete" "&Löschen"
    ::msgcat::mcset de "Select &All" "Alles &Markieren"
    ::msgcat::mcset de "Insert &Time/Date" "&Zeit/Datum einfügen"
    ::msgcat::mcset de "Co&mment selection" "Markierter Bereich als &Kommentar"
    ::msgcat::mcset de "U&ncomment selection" "Markierter Bereich &ohne Kommentarzeichen"
    ::msgcat::mcset de "&Indent selection" "Mark&ierten Bereich einrücken"
    ::msgcat::mcset de "Unin&dent selection" "Markier&ten Bereich ausrücken"

    # menues.tcl - Search menu
    ::msgcat::mcset de "&Find..." "&Suchen..."
    ::msgcat::mcset de "Find &Next" "Suche fortsetze&n"
    ::msgcat::mcset de "&Replace..." "E&rsetzen..."
    ::msgcat::mcset de "&Goto Line..." "&Gehe zu..."

    # menues.tcl - Exec menu
    ::msgcat::mcset de "&Load into Scilab" "&In Scilab laden"
    ::msgcat::mcset de "&Evaluate selection" "&Markierten Bereich ausführen"

    # menues.tcl - Debug menu
    ::msgcat::mcset de "&Insert/Remove breakpoint" "&Stoppunkt setzen/löschen"
    ::msgcat::mcset de "Remove &all breakpoints" "&Alle Stoppunkte löschen"
    ::msgcat::mcset de "&Configure execution..." "A&usführung konfigurieren..."
    ::msgcat::mcset de "Go to next b&reakpoint" "&Gehe zu nächstem Stoppunkt"
    ::msgcat::mcset de "&Step by step" "S&chritt für Schritt"
    ::msgcat::mcset de "Step &into" "&Into"
    ::msgcat::mcset de "Step o&ver" "O&ver"
    ::msgcat::mcset de "Step &out" "&Out"
    ::msgcat::mcset de "Run to c&ursor" "Ausführen &bis Eingabezeiger"
    ::msgcat::mcset de "G&o on ignoring any breakpoint" "Ausführung &ohne Stop fortsetzen"
    ::msgcat::mcset de "Show &watch" "B&eobachten"
    ::msgcat::mcset de "&Break" "U&nterbrechen"
    ::msgcat::mcset de "Cance&l debug" "Abb&rechen"

#    ::msgcat::mcset de "language scheme" "Sprachschema"
    # menues.tcl - Scheme menu
    ::msgcat::mcset de "S&cilab" "S&cilab"
    ::msgcat::mcset de "&XML" "&XML"
    ::msgcat::mcset de "&none" "kei&ns"

    # menues.tcl - Options menu
    ::msgcat::mcset de "&Font size" "&Schriftgröße"
        ::msgcat::mcset de "&micro" " &winzig"
        ::msgcat::mcset de "&small" " &klein"
        ::msgcat::mcset de "m&edium" " &normal"
        ::msgcat::mcset de "&large" " &groß"
    ::msgcat::mcset de "&Colors" "&Farben"
    ::msgcat::mcset de "&Tabs and indentation" "&Tabulatoren und Einrücken"
        ::msgcat::mcset de "Tab inserts &spaces" "&Tabulator fügt Leerzeichen ein"
        ::msgcat::mcset de "&Indentation spaces" "&Leerzeichen für das Einrücken"
    ::msgcat::mcset de "Colorize \'&strings\'" "Farbige \'&Zeichenketten\'"
    ::msgcat::mcset de "Word &Wrap" "&Automatischer Zeilenumbruch"
    ::msgcat::mcset de "File&names" "&Dateinamen"
        ::msgcat::mcset de "&Full path" "&Vollständiger Pfad"
        ::msgcat::mcset de "Full path if &ambiguous" "V&ollständiger Pfad (falls mehrdeutig)"
        ::msgcat::mcset de "&Unambiguous pruned path" "&Eindeutig abgekürzter Pfad"
    ::msgcat::mcset de "&Recent files" "Z&uletzt geöffnete Dateien"

    # menues.tcl - Windows menu
    ::msgcat::mcset de "Opened Files" "Offene Dateien"

    # menues.tcl - Help menu
    ::msgcat::mcset de "&Help..." "&Hilfe..."
    ::msgcat::mcset de "&What's?..." "&Was ist?..."
    ::msgcat::mcset de "&About" "Über &SciPad"


# popupmenus.tcl
::msgcat::mcset de "Open the source of" "Quellcode öffnen"
::msgcat::mcset de "AddWatch" "Variable %s beobachten"


# print.tcl
::msgcat::mcset de "Print Setup" "Drucken einrichten"
::msgcat::mcset de "Print Command: " "Befehl zum Drucken: "


# scilabexec.tcl
::msgcat::mcset de "No point in loading an empty file!" "Es macht keinen Sinn eine leere Datei in Scilab zu laden!"
::msgcat::mcset de "Scilab is working, wait for the prompt to load file" "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung, um eine Datei zu laden"
::msgcat::mcset de "Exec done" "Ausführung beendet"
::msgcat::mcset de "Scilab is working, wait for the prompt to execute the selection." "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung, um den markierten Bereich auszuführen."
::msgcat::mcset de "Scilab is working, wait for the prompt to convert a Matlab file." "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung, um eine Matlab-Datei zu importieren."
::msgcat::mcset de "Matlab files" "Matlab-Dateien"
::msgcat::mcset de "Matlab file to convert" "Zu importierende Matlab-Datei"
::msgcat::mcset de "Scilab is converting, please hold on..." "Scilab konvertiert gerade, bitte warten Sie..."
::msgcat::mcset de "Matlab file import" "Import einer Matlab-Datei"
::msgcat::mcset de "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug/index.cgi" \
                   "Der Import der Datei ist fehlgeschlagen, siehe Scilab-Fenster\
                    für weitere Details -- Vielleicht sollten die Fehlermeldungen\
                    und die verantwortliche Matlab-Datei an
                    http://scilabsoft.inria.fr/bugzilla_bug/index.cgi weiterleiten."
::msgcat::mcset de "Please select destination path for the xml source of the help file" "Bitte wählen sie einen Zielpfad für die XML-Hilfe-Datei"
::msgcat::mcset de "Older version of" "Es existiert eine ältere Version von"
    ::msgcat::mcset de "found!" "!"
::msgcat::mcset de "An old version of" "Eine alte Version von"
    ::msgcat::mcset de "already exists: open the old file instead?" "existiert bereits: Wollen Sie sie öffnen?"
::msgcat::mcset de "Existing file" "Datei existiert"
::msgcat::mcset de "New skeleton" "Neues XML-Skelett für Hilfe-Datei"
::msgcat::mcset de "Scilab is working, wait for the prompt to compile the help file." "Scilab arbeitet! Bitte warten Sie auf die Eingabeaufforderung, um die Hilfe-Datei zu übersetzen"


# scipad.tcl
::msgcat::mcset de "NOT CONNECTED TO SCILAB" "KEINE VERBINDUNG ZU SCILAB"


# textarea.tcl
::msgcat::mcset de "Font size" "Schriftgröße"


# whichfun.tcl
::msgcat::mcset de "Being at line" "Wir befinden uns in Zeile"
    ::msgcat::mcset de ", function" ", die Funktion"
    ::msgcat::mcset de "begins at" "beginnt bei"
    ::msgcat::mcset de ", and there are" ", und es gibt"
    ::msgcat::mcset de "continued lines, i.e. we are at line" "Fortsetzungszeilen, d.h. wir befinden uns in Zeile"
    ::msgcat::mcset de "of" "von"
::msgcat::mcset de "Function" "Funktion"
    ::msgcat::mcset de "line" "Zeile"
::msgcat::mcset de "The cursor is not currently inside a function body." "Der Eingabezeiger befindet sich nicht innerhalb einer Funktionsdefinition."
::msgcat::mcset de "Which function?" "Welche Funktion?"


#preference variables description
::msgcat::mcset de "BGCOLOR" "Hintergrund"
::msgcat::mcset de "FGCOLOR" "Normaler Text"
::msgcat::mcset de "CURCOLOR" "Eingabezeiger"
::msgcat::mcset de "PARCOLOR" "Runde Klammern ()"
::msgcat::mcset de "BRAKCOLOR" "Eckige Klammern \\\[\\\]"
::msgcat::mcset de "BRACCOLOR" "Geschweifte Klammern {}"
::msgcat::mcset de "PUNCOLOR"  "Interpunktion"
::msgcat::mcset de "KEYWCOLOR" "Schlüsselworte (Scilab)"
::msgcat::mcset de "OPCOLOR"   "Operatoren"
::msgcat::mcset de "QTXTCOLOR"  "Zeichenketten"
::msgcat::mcset de "REMCOLOR" "Kommentare"
::msgcat::mcset de "XMLCOLOR" "XML-Marken"
::msgcat::mcset de "NUMCOLOR" "Zahlen"
::msgcat::mcset de "SELCOLOR" "Markierte Bereiche"
::msgcat::mcset de "BREAKPOINTCOLOR" "Stoppunkte"
::msgcat::mcset de "LFUNCOLOR" "Bibliotheksfunktionen"
::msgcat::mcset de "PDEFCOLOR" "Vordefinierte Variablen"
::msgcat::mcset de "FOUNDTEXTCOLOR" "Suchergebnisse"

#overriding tcl-core dialogs
::msgcat::mcset de "&Directory:" "&Verzeichnis:"
::msgcat::mcset de "File &name:" "Datei&name:"
::msgcat::mcset de "Files of &type:" "Datei&typen:"
#::msgcat::mcset de "&Save" "&Speichern" # already included
::msgcat::mcset de "&Open" "&Öffnen"
::msgcat::mcset de "Open" "Öffnen"
::msgcat::mcset de "Save As" "Speichern unter"
::msgcat::mcset de "&Cancel" "&Abbrechen"
::msgcat::mcset de "&Selection:" "Au&swahl:"
::msgcat::mcset de "&Yes" "&Ja"
::msgcat::mcset de "&No" "&Nein"
::msgcat::mcset de "&Red" "&Rot"
::msgcat::mcset de "&Green" "&Grün"
::msgcat::mcset de "&Blue" "&Blau"
::msgcat::mcset de "Directory \"%1\$s\" does not exist." "Das Verzeichnis \"%1\$s\" existiert nicht."

