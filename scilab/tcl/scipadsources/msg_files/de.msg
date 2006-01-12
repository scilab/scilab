# Scipad texts in German
# in sync at least with v5.73
# provided by Dirk Reusch <reusch@tmb.uni-karlsruhe.de>

# commons
::msgcat::mcset de "Cancel" "Abbrechen"
::msgcat::mcset de "Cance&l" "A&bbrechen"
::msgcat::mcset de "OK" "OK"
::msgcat::mcset de "Scilab working" "Scilab arbeitet"
::msgcat::mcset de "All files" "Alle Dateien"
::msgcat::mcset de "Add/Change" "Hinzufügen/Ändern"
::msgcat::mcset de "Untitled" "OhneNamen"
::msgcat::mcset de "Close" "Schließen"
::msgcat::mcset de "&Close" "S&chließen"


# buffernavigation.tcl
::msgcat::mcset de "Goto Line?" "Gehe zu Zeile?"
::msgcat::mcset de "Go to:" "Gehe zu : "
::msgcat::mcset de "&logical line" "&Programmzeile"
::msgcat::mcset de "&physical line" "&Textzeile"
::msgcat::mcset de "in &function" "der &Funktion"
::msgcat::mcset de "in &current file" "der aktuellen &Datei"
::msgcat::mcset de "Outside of function definition"\
                   "Außerhalb der Funktionsdefinition"


# colorize.tcl
::msgcat::mcset de "Wait seconds while recolorizing file"\
                   "Warten Sie einen Moment während Farben aktualisiert werden"
::msgcat::mcset de "You can't do that while colorization is in progress.\
                    Please try again when finished."\
                   "Aktion kann während der Aktualsierung der Farben nicht \
                   ausgeführt werden. Versuchen Sie es später noch einmal."
::msgcat::mcset de "Command forbidden during colorization"\
                   "Befehl kann während der Aktualisierung der Farben nicht \
                   ausgeührt werden."


# completion.tcl
::msgcat::mcset de "No possible completion" "Keine Vervollständigung möglich"
::msgcat::mcset de "possible completions" "mögliche Vervollständigungen"
::msgcat::mcset de "Possible completions" "Mögliche Vervollständigungen"


# db_argsmanagement.tcl
::msgcat::mcset de "Variable:" "Variable :"
::msgcat::mcset de "Value:" "Wert :"
::msgcat::mcset de "Get current value from Scilab"\
                   "Verwende aktuellen Wert aus Scilab"


# db_configureexec.tcl
::msgcat::mcset de "Configure execution" "Ausführung konfigurieren"
::msgcat::mcset de "Function name:" "Funktionsname :"
::msgcat::mcset de "Obtain" "Erhält"
::msgcat::mcset de "Input arguments:" "Eingabeargumente :"
::msgcat::mcset de "Remove" "Entfernen"
::msgcat::mcset de "Function name or input arguments do not match definition\
                    of the function"\
                   "Funktionsname und Eingabeargumente stimmen nicht mit der\
                    Funktionsdefinition überein"
::msgcat::mcset de "in the file!\n\nCheck function name and arguments\
                    (names, order) in the configuration dialog. \nArguments\
                    order can be changed using drag and drop with right\
                    mouse button in the arguments listbox."\
                   "In der Datei!\n\n Überprüfen Sie den Funktionsnamen und\
                    die Argumente (Namen, Reihenfolge) im\
                    Konfigurationsdialog. Die Reihenfolge der Argumente kann\
                    mittels Drag&Drop (rechte Maustaste) verändert werden."
::msgcat::mcset de "Error in selected function name or arguments"\
                   "Fehler in markiertem Funktionsnamen oder markierten\
                    Argumenten"
::msgcat::mcset de "Debugger is not yet developed for sce scripts!"\
                   "Debug-Programm kann zur Zeit noch nicht für Skripte\
                    (*.sce) eingesetzt werden!"


# db_debugsession.tcl
::msgcat::mcset de "Scilab is working, wait for the prompt to issue a\
                    debugger command."\
                   "Scilab arbeitet! Warten Sie auf die Eingabeaufforderung,\
                    um einen Debug-Befehl auszuführen."
::msgcat::mcset de "Cursor position is out of reach!"\
                   "Eingabezeiger befindet sich außerhalb der Reichweite!"
::msgcat::mcset de "Cursor must be in a function"\
                   "Eingabezeiger muß sich innerhalb einer Funktion befinden"
::msgcat::mcset de "No effect - The debugged file is not stuck"\
                   "Keine Wirkung - Die Ausführung der Debug-Datei wurde\
                    nicht unterbrochen"
::msgcat::mcset de "Scilab execution error"\
                   "Fehler bei der Ausführung in Scilab"
::msgcat::mcset de "The shell reported an error while trying to execute "\
                   "Die Shell signalisierte einen Fehler bei der Ausführung\
                    von "
::msgcat::mcset de " at line " " in Zeile "
::msgcat::mcset de " of " " von "
::msgcat::mcset de "Execution aborted!" "Ausführung abgebrochen!"


# db_init.tcl
::msgcat::mcset de "Please wait..." "Bitte warten Sie..."


# db_states.tcl
::msgcat::mcset de "Currently no debug session"\
                   "Im Moment läuft keine Debug-Sitzung"
::msgcat::mcset de "Ready to start debug"\
                   "Bereit zum Starten einer Debug-Sitzung"
::msgcat::mcset de "Debug in progress" "Debug-Vorgang läuft"


# db_watch.tcl
::msgcat::mcset de "Watch" "Beobachten"
::msgcat::mcset de "Watch variables:" "Beobachtete Variablen :"
::msgcat::mcset de "Remove" "Entfernen"
::msgcat::mcset de "Call stack:" "Ausführungsstapel:"
::msgcat::mcset de "Show watch variables" "Beobachtete Variablen zeigen"
::msgcat::mcset de "Hide watch variables" "Beobachtete Variablen verstecken"
::msgcat::mcset de "Show call stack" "Ausführungsstapel zeigen"
::msgcat::mcset de "Hide call stack" "Ausführungsstapel verstecken"
::msgcat::mcset de "Undefined elements are not legal in variable"\
                   "Undefinierte Elemente sind als Variable nicht erlaubt"
::msgcat::mcset de ".\nThis variable will not be updated in Scilab."\
                   ".\n Diese Variable wird in Scilab nicht aktualisiert."
::msgcat::mcset de "Illegal undefined element found"\
                   "Nicht erlaubtes undefiniertes Element gefunden"


# filecommands.tcl
::msgcat::mcset de "New Script" "Neues Skript"
::msgcat::mcset de "The contents of" "Der Inhalt von"
::msgcat::mcset de "may have changed, do you wish to save your changes?"\
                   "wurde verändert. Möchten Sie die Änderungen speichern?"
::msgcat::mcset de "Save Confirm?" "Speichern bestätigen?"
::msgcat::mcset de "Open source of..." "Öffnen des Quellcodes von..."
::msgcat::mcset de "Open source of:" "Öffnen des Quellcodes von :"
::msgcat::mcset de "Weird drag&drop" "Drag&Drop"
::msgcat::mcset de "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or you\
                    hit a bug of the dnd mechanism."\
                   "repräsentiert weder einen gültigen Dateinamen, noch\
                    einen gültigen Dateipfad. Entweder handelt es sich um\
                    ein anderen Objekttyp, oder Sie haben einen Fehler im\
                    Drag&Drop-Mechanismus von SciPad gefunden."
::msgcat::mcset de "File not found" "Datei wurde nicht gefunden"
::msgcat::mcset de "does not exist anymore. Do you want to create an empty\
                    file with the same name?"\
                   "existiert nicht mehr. Möchten Sie eine leere Datei mit\
                    dem gleichen Namen erzeugen ?"
::msgcat::mcset de "Open file" "Datei öffnen"
::msgcat::mcset de "The file" "Die Datei"
::msgcat::mcset de "is already opened! Save the current opened file to\
                    another name and reopen it from disk!"\
                   "wurde bereits geöffnet! Speichern Sie die Datei unter\
                    einem anderen Namen und öffnen Sie sie wieder!"
::msgcat::mcset de "File" "Datei"
::msgcat::mcset de "saved" "gespeichert"
::msgcat::mcset de "cannot be written!" "ist schreibgeschützt!"
::msgcat::mcset de "The contents of " "Der Inhalt von"
::msgcat::mcset de "has changed on disk, save it anyway?"\
                   "auf der Festplatte hat sich verändert. Möchten Sie\
                    trotzdem abspeichern ?"
::msgcat::mcset de "has been modified outside of Scipad. Do you want to\
                    reload it?"\
                   "wurde außerhalb von Scipad verändert. \
                   Möchten sie es neu laden?"
::msgcat::mcset de "File has changed!" "Datei wurde verändert!"
::msgcat::mcset de "Save as" "Speichern unter"
::msgcat::mcset de "Scilab files" "Scilab-Dateien"
::msgcat::mcset de "Scicos files" "Scicos-Dateien"
::msgcat::mcset de "XML files" "XML-Dateien"
::msgcat::mcset de "Wait seconds while loading and colorizing file"\
                   "Bitte warten Sie einen Moment während Datei geöffnet wird"
::msgcat::mcset de "The contents of this file may have changed, do you wish\
                    to save your changes?"\
                   "Diese Datei wurde verändert. Möchten Sie ihre Änderungen\
                    speichern?"
::msgcat::mcset de "Revert" "Möchten Sie auf die gespeicherte Version von"
::msgcat::mcset de "to saved?" "zurückgreifen?"
::msgcat::mcset de "Revert Confirm?" "Verwerfen bestätigen?"
::msgcat::mcset de "Unreadable file" "Unlesbare Datei!"
::msgcat::mcset de "exists but is not readable!"\
                   "existiert, ist aber unlesbar!"


# findreplace.tcl
::msgcat::mcset de "The string" "Die Zeichenkette"
::msgcat::mcset de "could not be found in" "konnte nicht gefunden werden in"
::msgcat::mcset de "could not be found in any of the opened files !"\
                   "konnte in keiner der geöffneten Dateien gefunden werden !"
::msgcat::mcset de "replacements done" "Ersetzungen ausgeführt"
::msgcat::mcset de "Find" "Suchen"
::msgcat::mcset de "No match found in the selection for" "Die Zeichenkette"
::msgcat::mcset de "\nWould you like to look for it in the entire text?"\
                   "konnte im markierten Bereich nicht gefunden\
                    werden.\nMöchten Sie in der gesamten Datei danach suchen\
                    ?"
::msgcat::mcset de "You are searching for an empty string!"\
                   "Nach einer leeren Zeichenkette kann nicht gesucht werden!"
::msgcat::mcset de "Re&place" "&Ersetzen"
::msgcat::mcset de "Find what:" "Suche nach:"
::msgcat::mcset de "Replace with:" "Ersetze durch:"
::msgcat::mcset de "Find &Next" "Suche &fortsetzen"
::msgcat::mcset de "Replace &All" "&Alle Ersetzen"
::msgcat::mcset de "&Upwards" "&Rückwärts"
::msgcat::mcset de "&Downwards" "&Vorwärts"
::msgcat::mcset de "Match &whole word only" "Ganzes &Wort"
::msgcat::mcset de "Match &case" "&Groß-/Kleinschreibung beachten"
::msgcat::mcset de "&Regular expression" "Reg&ulärer Ausdruck"
::msgcat::mcset de "In all &opened files"\
                   "In allen geöffneten &Dateien"
::msgcat::mcset de "In &selection only" "In &markiertem Bereich"
::msgcat::mcset de "In a director&y" "In einem &Verzeichnis"
::msgcat::mcset de "Directory r&ecurse search" "Re&kursive Suche"
::msgcat::mcset de "In files/file types:" "Dateitypen :"
::msgcat::mcset de "In directory:" "Im Verzeichnis :"
::msgcat::mcset de "Wrapped around" "Wrapped around"
::msgcat::mcset de "Back at the first file searched!"\
                   "Erste Datei der Suche wurde wieder erreicht !"
::msgcat::mcset de "No more matches" "Keine weiteren Suchergebnisse"
::msgcat::mcset de "The string to regexp search for cannot be compiled by\
                    Tcl.\nCheck again, maybe certain characters should be\
                    escaped."\
                   "Der reguläre Ausdruck kann in Tcl nicht interpetiert \
                   werden. Überprüfen Sie, ob bestimmeten Zeichen ein \
                   ESC-Zeichen (\\) vorangestellt werden muß."


# findinfiles.tcl
::msgcat::mcset de "Directory access issue"\
                   "Fehler beim Zugriff auf Verzeichnis"
::msgcat::mcset de "The directory you specified cannot be reached. The\
                    search won't bring any result."\
                    "Auf das angegebene Verzeichnis kann nicht zugegriffen werden. \
                    Die Suche wird ergebnislos sein."
::msgcat::mcset de "Some directories or files could not be open and will be\
                    ignored during search.\n You might miss read access to\
                    them.\n\n The following ones will be ignored"\
                    "Einige Verzeichnisse oder Dateien konnten nicht geöffnet \
                    werden. Sie werden von Suchfunktion ignoriert werden.\n \
                    Möglicherweise besitzen sie keine Leserechte für diese \
                    Verzeichnisse/Dateien.\n\n Ignoriert werden folgende \
                    Verzeichnisse/Dateien"
::msgcat::mcset de "Ignored files" "Ignorierte Dateien"
::msgcat::mcset de "Searching in file:" "Suche in Datei :"
::msgcat::mcset de "End of file search:" "Ende der Suche :"
::msgcat::mcset de "matches found" "Übereinstimmungen gefunden"
::msgcat::mcset de "Constructing file list:"\
                   "Erzeugung der Dateiliste:"
::msgcat::mcset de "&Previous" "&Zurück"
::msgcat::mcset de "&Next" "&Vorwärts"
::msgcat::mcset de "Scipad search results" "Ergebnisse der Scipad-Suche"


# helps.tcl
::msgcat::mcset de "About" "Über SciPad"
::msgcat::mcset de "aboutme_message"\
                   "\n Ursprüngliche Version von Joseph Acosta,\
                    joeja@mindspring.com.\n Veränderungen durch das Scilab\
                    Konsortium.\n Überarbeitung durch Enrico Segre\
                    2003-2005.\n Verbesserungen (insbesondere Debugger) von\
                    François Vogel 2004-2005.\n Schwedische Übersetzung von\
                    Gunnar Lindholm 2005.\n Norwegische Übersetzung von\
                    Torbjørn Pettersen 2005.\n Deutsche Übersetzung von Dirk\
                    Reusch 2005, 2006.\n Polnische Übersetzung von Bartosz\
                    Kosiorek 2005."


# infomessages.tcl
::msgcat::mcset de "Line:" "Zeile:"
::msgcat::mcset de "Column:" "Spalte:"
::msgcat::mcset de "Line" "Zeile"
::msgcat::mcset de "in" "in"
::msgcat::mcset de "Logical line:" "Programmzeile:"
::msgcat::mcset de " \[ReadOnly\]" " \[Schreibschutz\]"
::msgcat::mcset de " (modified)" " (geändert)"


# inputtext.tcl
::msgcat::mcset de "No corresponding <" "Keine entsprechende <"
::msgcat::mcset de "> found!" "> gefunden!"
::msgcat::mcset de "Code editing is not allowed during debug!"\
                   "Während der Debug-Sitzung kann das Programm nicht\
                    verändert werden !"


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
::msgcat::mcset de "Open &in new" "Ö&ffnen in neuem Fenster"
::msgcat::mcset de "&horizontal tile" "&rechts daneben"
::msgcat::mcset de "&vertical tile" "&darunter"
::msgcat::mcset de "Op&en source of..." "Öffn&en des Quellcodes von..."
::msgcat::mcset de "&Save" "&Speichern"
::msgcat::mcset de "Save &as..." "S&peichern unter..."
::msgcat::mcset de "&Revert..." "&Verwerfen..."
::msgcat::mcset de "Import &Matlab file..." "&Matlab-Datei importieren..."
::msgcat::mcset de "Create help s&keleton..." "&XML-Skelett für Hilfe-Datei..."
::msgcat::mcset de "Compile as &help page" "XML-&Hilfe-Datei übersetzen"
::msgcat::mcset de "Open &function source" "&Quellcode der Funktion"
::msgcat::mcset de "Print Se&tup" "D&rucken einrichten..."
::msgcat::mcset de "&Print" "&Drucken"
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
::msgcat::mcset de "U&ncomment selection"\
                   "Markierter Bereich &ohne Kommentarzeichen"
::msgcat::mcset de "&Indent selection" "Mark&ierten Bereich einrücken"
::msgcat::mcset de "Unin&dent selection" "Markier&ten Bereich ausrücken"

# menues.tcl - Search menu
::msgcat::mcset de "&Find..." "&Suchen..."
::msgcat::mcset de "Find &Next" "Suche &fortsetzen"
::msgcat::mcset de "&Replace..." "&Ersetzen..."
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
::msgcat::mcset de "G&o on ignoring any breakpoint"\
                   "Ausführung &ohne Stop fortsetzen"
::msgcat::mcset de "Show &watch" "B&eobachten"
::msgcat::mcset de "&Break" "U&nterbrechen"
::msgcat::mcset de "Cance&l debug" "Abb&rechen"

#    ::msgcat::mcset fr "language scheme" "schéma langage"
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
::msgcat::mcset de "Colorize \'&strings\'" "Farbige \'&Zeichenketten\'"
::msgcat::mcset de "Show c&ontinued lines"\
                   "F&ortgesetzte Zeilen anzeigen"
::msgcat::mcset de "Word &Wrap" "&Automatischer Zeilenumbruch"
::msgcat::mcset de "&Tabs and indentation" "&Tabulatoren und Einrücken"
::msgcat::mcset de "Tab inserts &spaces" "&Tabulator fügt Leerzeichen ein"
::msgcat::mcset de "&Indentation spaces" "&Leerzeichen für das Einrücken"
::msgcat::mcset de "Com&pletion" "&Vervollständigung"
::msgcat::mcset de "&Tab" "&Tab"
::msgcat::mcset de "&Control-Tab" "&Strg-Tab"
::msgcat::mcset de "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset de "&Shift-Tab" "S&hift-Tab"
::msgcat::mcset de "C&ontrol-Alt-Tab" "St&rg-Alt-Tab"
::msgcat::mcset de "S&hift-Control-Tab" "Sh&ift-Strg-Tab"
::msgcat::mcset de "Sh&ift-Alt-Tab" "Shi&ft-Alt-Tab"
::msgcat::mcset de "Shi&ft-Control-Alt-Tab" "Shift-Str&g-Alt-Tab"
::msgcat::mcset de "File&names" "&Dateinamen"
::msgcat::mcset de "&Full path" "&Vollständiger Pfad"
::msgcat::mcset de "Full path if &ambiguous"\
                   "V&ollständiger Pfad (falls mehrdeutig)"
::msgcat::mcset de "&Unambiguous pruned path" "&Eindeutig abgekürzter Pfad"
::msgcat::mcset de "&Recent files" "Z&uletzt geöffnete Dateien"
::msgcat::mcset de "&Backup files depth" "A&nzahl der Sicherungskopien"
::msgcat::mcset de "&Locale" "S&prachumgebung"
# locale names (common)
::msgcat::mcset de "de locale (de)" "Deutsch (de)"
::msgcat::mcset de "fr locale (fr)" "Français (fr)"
::msgcat::mcset de "eng locale (eng)" "English (eng)"
::msgcat::mcset de "es locale (es)" "Español (es)"
::msgcat::mcset de "it locale (it)" "Italiano (it)"
::msgcat::mcset de "no locale (no)" "Norsk (no)"
::msgcat::mcset de "pl locale (pl)" "Polski (pl)"
::msgcat::mcset de "se locale (se)" "Svenska (se)"

# menues.tcl - Windows menu
::msgcat::mcset de "&Maximize" "&Maximieren"
::msgcat::mcset de "&Split" "&Teilen"
::msgcat::mcset de "S&plit (side by side)" "Teilen (&senkrecht)"
::msgcat::mcset de "Tile all &vertically" "&Übereinander"
::msgcat::mcset de "Tile all &horizontally" "&Nebeneinander"
::msgcat::mcset de "Space sashes &evenly" "&Gleichmäßige Aufteilung"
::msgcat::mcset de "Opened Files" "Offene Dateien"

# menues.tcl - Help menu
::msgcat::mcset de "&Help..." "&Hilfe..."
::msgcat::mcset de "&What's?..." "&Was ist?..."
::msgcat::mcset de "&About" "Über &SciPad"
::msgcat::mcset de "Adding &translations..." "&Übersetzungen hinzufügen..."
::msgcat::mcset de "Adding Scipad Translations..."\
                   "Scipad Übersetzungen hinzufügen..."
::msgcat::mcset de "&Bugs \& wishlist" "&Fehler \& Wünsche"
::msgcat::mcset de "Scipad Known bugs" "Bekannte Fehler in Scipad"
::msgcat::mcset de "Change&log" "&Liste der Änderungen"
::msgcat::mcset de "Changes in the Scipad codebase"\
                   "Änderungen im Scipad Programm"


# popupmenus.tcl
::msgcat::mcset de "Open the source of" "Quellcode öffnen"
::msgcat::mcset de "AddWatch" "Variable %s beobachten"


# print.tcl
::msgcat::mcset de "Print Setup" "Drucken einrichten"
::msgcat::mcset de "Print Command: " "Befehl zum Drucken: "


# scilabexec.tcl
::msgcat::mcset de "No point in loading an empty file!"\
                   "Es macht keinen Sinn eine leere Datei in Scilab zu laden!"
::msgcat::mcset de "Scilab is working, wait for the prompt to load file"\
                   "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung,\
                    um eine Datei zu laden"
::msgcat::mcset de "Exec done" "Ausführung beendet"
::msgcat::mcset de "Scilab is working, wait for the prompt to execute the\
                    selection."\
                   "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung,\
                    um den markierten Bereich auszuführen."
::msgcat::mcset de "Scilab is working, wait for the prompt to convert a\
                    Matlab file."\
                   "Scilab arbeitet. Warten Sie auf die Eingabeaufforderung,\
                    um eine Matlab-Datei zu importieren."
::msgcat::mcset de "Matlab files" "Matlab-Dateien"
::msgcat::mcset de "Matlab file to convert" "Zu importierende Matlab-Datei"
::msgcat::mcset de "Scilab is converting, please hold on..."\
                   "Scilab konvertiert gerade, bitte warten Sie..."
::msgcat::mcset de "Matlab file import" "Import einer Matlab-Datei"
::msgcat::mcset de "Conversion of the file failed, see the Scilab window for\
                    details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"\
                   "Der Import der Datei ist fehlgeschlagen, siehe\
                    Scilab-Fenster für weitere Details -- Vielleicht sollten\
                    die Fehlermeldungen und die verantwortliche Matlab-Datei\
                    an  http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi\
                    weiterleiten."
::msgcat::mcset de "Please select destination path for the xml source of the\
                    help file"\
                   "Bitte wählen sie einen Zielpfad für die XML-Hilfe-Datei"
::msgcat::mcset de "Older version of" "Es existiert eine ältere Version von"
::msgcat::mcset de "found!" "!"
::msgcat::mcset de "An old version of" "Eine alte Version von"
::msgcat::mcset de "already exists: open the old file instead?"\
                   "existiert bereits: Wollen Sie sie öffnen?"
::msgcat::mcset de "Existing file" "Datei existiert"
::msgcat::mcset de "New skeleton" "Neues XML-Skelett für Hilfe-Datei"
::msgcat::mcset de "Scilab is working, wait for the prompt to compile the\
                    help file."\
                   "Scilab arbeitet! Bitte warten Sie auf die\
                    Eingabeaufforderung, um die Hilfe-Datei zu übersetzen"
::msgcat::mcset de "ScilabEval command cannot be passed to Scilab!"\
                   "Ein \"ScilabEval\"-Befehl kann nicht an Scilab übergeben\
                    werden!"
::msgcat::mcset de "impossibleScilabEval_message"\
                   "WARNUNG : Ein Befehl konnte nicht an Scilab übertragen\
                    werden und wurde  deshalb ignoriert.\n\n Die\
                    wahrscheinlichste Ursache hierfür ist, daß Sie  keine\
                    Schreibrechte für das Scilab-Verzeichnis TMPDIR\
                    besitzen.  Unvorhersehbare Ergebnisse können die Folge\
                    sein. Bei dem Befehl handelte  es sich um:\n\n-->"


# scipad.tcl
::msgcat::mcset de "NOT CONNECTED TO SCILAB" "KEINE VERBINDUNG ZU SCILAB"


# textarea.tcl
::msgcat::mcset de "Font size" "Schriftgröße"


# whichfun.tcl
::msgcat::mcset de "Being at line" "Wir befinden uns in Zeile"
::msgcat::mcset de ", function" ", die Funktion"
::msgcat::mcset de "begins at" "beginnt bei"
::msgcat::mcset de ", and there are" ", und es gibt"
::msgcat::mcset de "continued lines, i.e. we are at line"\
                   "Fortsetzungszeilen, d.h. wir befinden uns in Zeile"
::msgcat::mcset de "of" "von"
::msgcat::mcset de "Function" "Funktion"
::msgcat::mcset de "line" "Zeile"
::msgcat::mcset de "The cursor is not currently inside a function body."\
                   "Der Eingabezeiger befindet sich nicht innerhalb einer\
                    Funktionsdefinition."
::msgcat::mcset de "Which function?" "Welche Funktion?"


#preference variables description
::msgcat::mcset de "BGCOLOR" "Hintergrund"
::msgcat::mcset de "FGCOLOR" "Normaler Text"
::msgcat::mcset de "CURCOLOR" "Eingabezeiger"
::msgcat::mcset de "PARCOLOR" "Runde Klammern ()"
::msgcat::mcset de "BRAKCOLOR" "Eckige Klammern \\\[\\\]"
::msgcat::mcset de "BRACCOLOR" "Geschweifte Klammern {}"
::msgcat::mcset de "PUNCOLOR" "Interpunktion"
::msgcat::mcset de "INTFCOLOR" "Eingebaute Funktionen"
::msgcat::mcset de "COMMCOLOR" "Befehle"
::msgcat::mcset de "SCICCOLOR" "Scicos-Funktionen"
::msgcat::mcset de "OPCOLOR" "Operatoren"
::msgcat::mcset de "QTXTCOLOR" "Zeichenketten"
::msgcat::mcset de "REMCOLOR" "Kommentare"
::msgcat::mcset de "XMLCOLOR" "XML-Marken"
::msgcat::mcset de "NUMCOLOR" "Zahlen"
::msgcat::mcset de "SELCOLOR" "Markierte Bereiche"
::msgcat::mcset de "BREAKPOINTCOLOR" "Stoppunkte"
::msgcat::mcset de "LFUNCOLOR" "Bibliotheksfunktionen"
::msgcat::mcset de "PDEFCOLOR" "Vordefinierte Variablen"
::msgcat::mcset de "FOUNDTEXTCOLOR" "Suchergebnisse"
::msgcat::mcset de "CONTLINECOLOR" "Fortgesetzte Zeile"


#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset de "&Directory:" "&Verzeichnis:"
::msgcat::mcset de "File &name:" "Datei&name:"
::msgcat::mcset de "Files of &type:" "Datei&typen:"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
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
::msgcat::mcset de "Directory \"%1\$s\" does not exist."\
                   "Das Verzeichnis \"%1\$s\" existiert nicht."
