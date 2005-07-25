# Scipad texts in Polish
# in sync at least with v5.2
# Created by Bartosz Kosiorek <gang65@poczta.onet.pl>

# commons
::msgcat::mcset pl "Cancel" "Anuluj"
::msgcat::mcset pl "Cance&l" "Anu&luj"
::msgcat::mcset pl "OK" "OK"
::msgcat::mcset pl "Scilab working" "Scilab pracuje"
::msgcat::mcset pl "All files" "Wszystkie pliki"
::msgcat::mcset pl "Add/Change" "Dodaj/Zmie\u0144"
::msgcat::mcset pl "Untitled" "Bez nazwy"


# buffernavigation.tcl
::msgcat::mcset pl "Goto Line?" "I\u015bæ do linii?"
::msgcat::mcset pl "Goto Line: " "Id\u017a do linii: "


# colorize.tcl
::msgcat::mcset pl "Wait seconds while recolorizing file" "Prosz\u0119 czekaæ, dop\u00f3ki nie przekoloruje pliku"


# db_argsmanagement.tcl
::msgcat::mcset pl "Variable:" "Zmienna :"
::msgcat::mcset pl "Value:" "Warto\u015bæ :"
::msgcat::mcset pl "Get current value from Scilab" "Pobiera bierz\u0105c\u0105 warto\u015bæ z programu Scilab"


# db_configureexec.tcl
::msgcat::mcset pl "Configure execution" "Konfiguracja wykonania"
::msgcat::mcset pl "Function name:" "Nazwa funkcji :"
::msgcat::mcset pl "Obtain" "Otrzymano"
::msgcat::mcset pl "Input arguments:" "Parametry wej\u015bciowe :"
::msgcat::mcset pl "Remove" "Usu\u0144"
::msgcat::mcset pl "Check function definition: it must be on a single code line." \
                   "Spawdzanie definicji funkcji : funkcja musi byæ w pojedy\u0144czej linii kodu."
::msgcat::mcset pl "Function name or input arguments do not match definition of the function" \
                    "Nazwa funkcji lub jej argumenty, nie pasuj\u0105 do definicji funkcji"
    ::msgcat::mcset pl "in the file!\n\nCheck function\
                        name and arguments (names, order) in the configuration dialog.\
                        \nArguments order can be changed using drag and drop with\
                        right mouse button in the arguments listbox." \
                       "w pliku!\n\n\
                        Sprawdz nazw\u0119 i argumenty funkcji w oknie konfiguracji.\
                        \nKolejno\u015bæ argument\u00f3w mo\u017ce zostaæ zmieniona przy pomocy metody \
                        przeci\u0105gnij i upu\u015bæ (prawym klawiszem myszy na li\u015bcie argument\u00f3w)"
::msgcat::mcset pl "Error in selected function name or arguments" "B\u0142\u0105d w nazwie zaznaczonej funkcji, lub jej parametrach"
::msgcat::mcset pl "Debugger is not yet developed for sce scripts!" "Debugger nie jest jeszcze rozwijany dla skrypt\u00f3w sce!"


# db_debugsession.tcl
::msgcat::mcset pl "Scilab is working, wait for the prompt to issue a debugger command." "Scilab pracuje, poczekaj na znak zach\u0119ty, aby wydaæ polecenie odpluskwiaczowi."
::msgcat::mcset pl "Cursor position is out of reach!" "Pozycja kursora jest poza zasi\u0119giem!"
::msgcat::mcset pl "Cursor must be in a function" "Kursor musi znajdowaæ si\u0119 w funkcji"
::msgcat::mcset pl "No effect - The debugged file is not stuck" "Brak efektu - odpluskwiany plik nie ma nieprawid\u0142owo\u015bci"
::msgcat::mcset pl "Scilab execution error" "B\u0142\u0105d wykonywania Scilab"
::msgcat::mcset pl "The shell reported an error while trying to execute " "Pow\u0142oka wys\u0142a\u0142a b\u0142\u0105d, podczas pr\u00f3by wykonywania "
    ::msgcat::mcset pl " at line " " w linii "
    ::msgcat::mcset pl " of " " na "
::msgcat::mcset pl "Execution aborted!" "Wykonywanie przerwane!"


# db_init.tcl
::msgcat::mcset pl "Please wait..." "Prosz\u0119 czekaæ..."


# db_states.tcl
::msgcat::mcset pl "Currently no debug session" "Teraz nie jest uruchomione odpluskwianie"
::msgcat::mcset pl "Ready to start debug" "Gotowy do uruchamiania odpluskwiania"
::msgcat::mcset pl "Debug in progress" "Trwa odpluskwianie"


# db_watch.tcl
::msgcat::mcset pl "Watch" "Obserwowane"
::msgcat::mcset pl "Watch variables:" "Zmienne obserwowane :"
::msgcat::mcset pl "Remove" "Usu\u0144"
::msgcat::mcset pl "Call stack:" "Stos wywo\u0142a\u0144:"
::msgcat::mcset pl "Close" "Zamknij"
::msgcat::mcset pl "Show watch variables" "Wy\u015bwietl zmienne obserwowane"
::msgcat::mcset pl "Hide watch variables" "Ukryj zmienne obserwowane"
::msgcat::mcset pl "Show call stack" "Wy\u015bwietl stos wywo\u0142a\u0144"
::msgcat::mcset pl "Hide call stack" "Ukryj stos wywo\u0142a\u0144"
::msgcat::mcset fr "Undefined elements are not legal in variable" "Les éléments indéfinis sont illégaux pour la variable"
::msgcat::mcset fr ".\nThis variable will not be updated in Scilab." ".\nCette variable ne sera pas mise à jour dans Scilab"
::msgcat::mcset fr "Illegal undefined element found" "Elément indéfini illégal identifié"


# filecommands.tcl
::msgcat::mcset pl "New Script" "Nowy skrypt"
::msgcat::mcset pl "The contents of" "Zawarto\u015bæ"
    ::msgcat::mcset pl "may have changed, do you wish to save your changes?" "mog\u0142a ulec zmianie. Czy chcesz zapisaæ zmiany?"
::msgcat::mcset pl "Save Confirm?" "Potwierdzenie zapisu"
::msgcat::mcset fr "Weird drag&drop" "Glisser&déposer inattendu"
::msgcat::mcset fr "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                    "n'est ni un nom de fichier valide ni un nom de répertoire\
                     valide. Soit vous avez tenté de glisser un objet d'un \
                     type différent, soit c'est un bug du mécanisme de \
                     glisser&déposer."
::msgcat::mcset pl "File not found" "Nie znaleziono pliku"
::msgcat::mcset pl "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "Plik ju\u017c nie istnieje. Czy chcesz utworzyæ \
                    pusty plik o tej samej nazwie? ?"
::msgcat::mcset pl "Open file" "Otw\u00f3rz plik"
::msgcat::mcset pl "The file" "Plik"
    ::msgcat::mcset pl "is already opened! Save the current opened file to another name and reopen it from disk!" "jest ju\u017c otwarty! Zapisz bie\u017c\u0105cy plik pod inna nazw\u0105 i otw\u00f3rz ponownie z dysku!"
::msgcat::mcset pl "File" "Plik"
    ::msgcat::mcset pl "saved" "zapisano"
::msgcat::mcset pl "cannot be written!" "nie mo\u017ce byæ zapisany!"
::msgcat::mcset pl "The contents of " "Zawarto\u015bæ pliku"
    ::msgcat::mcset pl "has changed on disk, save it anyway?" "uleg\u0142a zmianie na dysku. Czy mimo to zapisaæ?"
::msgcat::mcset pl "File has changed!" "Plik zosta\u0142 zmieniony"
::msgcat::mcset pl "Save as" "Zapisz jako"
::msgcat::mcset pl "Scilab files" "Pliki Scilab"
::msgcat::mcset pl "XML files" "Pliki XML"
::msgcat::mcset pl "Wait seconds while loading and colorizing file" "Prosz\u0119 czekaæ. Trwa wczytywanie i dodawanie koloru do pliku"
::msgcat::mcset pl "The contents of this file may have changed, do you wish to save your changes?" "Zawarto\u015bæ tego pliku mog\u0142a ulec zmianie. Czy chcesz zapisaæ zmiany?"
::msgcat::mcset pl "Revert" "Przywr\u00f3ciæ do wersji zapisanej w pliku"
    ::msgcat::mcset pl "to saved?" "?"
::msgcat::mcset pl "Revert Confirm?" "Potwierdzenie przywr\u00f3cenia"


# findreplace.tcl
::msgcat::mcset pl "The string" "Ci\u0105g znak\u00f3w"
    ::msgcat::mcset pl "could not be found" "nie zosta\u0142 znaleziony"
::msgcat::mcset pl "replacements done" "Zamienianie zako\u0144czone"
::msgcat::mcset pl "Find" "Znajd\u017a"
::msgcat::mcset pl "No match found in the selection for" "Nie znaleziono w zaznaczeniu"
    ::msgcat::mcset pl "\nWould you like to look for it in the entire text?" "\nCzy chcesz przeszukaæ ca\u0142y tekst ?"
::msgcat::mcset pl "You are searching for an empty string!" "Szukasz pustego \u0142a\u0144cucha!"
::msgcat::mcset pl "Re&place" "Za&mie\u0144"
::msgcat::mcset pl "Find what:" "Znajd\u017a :"
::msgcat::mcset pl "Replace with:" "Zamie\u0144 na :"
::msgcat::mcset pl "Find &Next" "Z&najd\u017a nast\u0119pne"
::msgcat::mcset pl "Replace &All" "Z&amie\u0144 wszystko"
::msgcat::mcset pl "&Upwards" "&Do przodu"
::msgcat::mcset pl "Downward&s" "D&o ty\u0142u" 
::msgcat::mcset pl "Match &case" "Uwzgl\u0119dnij wielko\u015bæ &liter"
::msgcat::mcset pl "&Regular expression" "&Uwzgl\u0119dnij tylko ca\u0142e wyrazy"


# helps.tcl
::msgcat::mcset pl "About" "O autorach"
::msgcat::mcset pl aboutme_message \
                    "\n Program zapocz\u0105tkowany przez Joseph Acosta, joeja@mindspring.com.\n\
                    Zmodyfikowany przez Scilab Consortium.\n\
                    Skorygowany przez Enrico Segre 2003-2005.\n\
                    R\u00f3\u017cnorodne ulepszenia, narz\u0119dzia debuguj\u0105ce,\
                    zosta\u0142y stworzone przez Francois Vogel 2004-2005.\n\
                    Autorem szwedzkiej lokalizacji jest Lindholm 2005.\n\
                    Norweskej lokalizacji jest Torbjørn Pettersen 2005.\n\
                    Niemieckiej lokalizacji jest Dirk Reusch 2005.\n\
			  Polskiej lokalizacji jest Bartosz Kosiorek 2005."

# infomessages.tcl
::msgcat::mcset pl "Line:" "Linia:"
    ::msgcat::mcset pl "Column:" "Kolumna:"
::msgcat::mcset pl "Line" "Linia"
    ::msgcat::mcset pl "in" "do"
::msgcat::mcset pl " \[ReadOnly\]" " \[Tylko do odczytu\]"
::msgcat::mcset pl " (modified)" " (zmodyfikowany)"


# inputtext.tcl
::msgcat::mcset fr "No corresponding <" "Aucun <"
    ::msgcat::mcset fr "> found!" "> correspondant trouvé !"
::msgcat::mcset fr "Code editing is not allowed during debug!" "Impossible de modifier le code durant le débug !"


# menues.tcl
::msgcat::mcset pl "&File" "&Plik"
::msgcat::mcset pl "&Edit" "&Edycja"
::msgcat::mcset pl "&Search" "&Szukaj"
::msgcat::mcset pl "E&xecute" "&Wykonaj"
::msgcat::mcset pl "&Debug" "&Odpluskwianie"
::msgcat::mcset pl "S&cheme" "&Schemat"
::msgcat::mcset pl "&Options" "&Opcje"
::msgcat::mcset pl "&Windows" "&Okna"
::msgcat::mcset pl "&Help" "&Pomoc"

    # menues.tcl - File menu
    ::msgcat::mcset pl "&New" "&Nowy"
    ::msgcat::mcset pl "&Open..." "&Otw\u00f3rz..."
    ::msgcat::mcset pl "&Save" "&Zapisz"
    ::msgcat::mcset pl "Save &as..." "Z&apisz jako..."
    ::msgcat::mcset pl "&Revert..." "&Przywr\u00f3æ..."
    ::msgcat::mcset pl "Import &Matlab file..."  "Importuj plik &Matlab-a..."
    ::msgcat::mcset pl "Create help s&keleton..." "Utw\u00f3rz sz&kielet pomocy..."
    ::msgcat::mcset pl "Compile as &help page" "Kompiluj jako &strona pomocy"
    ::msgcat::mcset pl "Open &function source" "Otw\u00f3rz \u017ar\u00f3d\u0142o &funkcji"
    ::msgcat::mcset pl "Print Se&tup" "U&stawienia drukowania"
    ::msgcat::mcset pl "&Print" "&Drukuj"
    ::msgcat::mcset pl "&Close" "&Zamknij"
    ::msgcat::mcset pl "E&xit" "&Wyjd\u017a"

    # menues.tcl - Edit menu
    ::msgcat::mcset pl "&Undo" "&Cofnij"
    ::msgcat::mcset pl "&Redo" "&Powt\u00f3rz"
    ::msgcat::mcset pl "Cu&t" "&Wytnij"
    ::msgcat::mcset pl "&Copy" "&Kopiuj"
    ::msgcat::mcset pl "&Paste" "W&klej"
    ::msgcat::mcset pl "&Delete" "&Usu\u0144"
    ::msgcat::mcset pl "Select &All" "Z&aznacz wszystko"
    ::msgcat::mcset pl "Insert &Time/Date" "Wstaw &czas/dat\u0119"
    ::msgcat::mcset pl "Co&mment selection" "Dodaj ko&mentarz do zaznaczenia"
    ::msgcat::mcset pl "U&ncomment selection" "Usu\u0144 kome&ntarz z zaznaczenia"
    ::msgcat::mcset pl "&Indent selection" "Powi\u0119ksz &wci\u0119cie zaznaczenia"
    ::msgcat::mcset pl "Unin&dent selection" "&Zmniejsz wci\u0119cie zaznaczenia"

    # menues.tcl - Search menu
    ::msgcat::mcset pl "&Find..." "&Znajd\u017a..."
    ::msgcat::mcset pl "Find &Next" "Z&najd\u017a nast\u0119pne"
    ::msgcat::mcset pl "&Replace..." "Z&amie\u0144..."
    ::msgcat::mcset pl "&Goto Line..." "&Id\u017a do linii..."

    # menues.tcl - Exec menu
    ::msgcat::mcset pl "&Load into Scilab" "&Za\u0142aduj do Scilab-a"
    ::msgcat::mcset pl "&Evaluate selection" "&Uruchom zaznaczenie"

    # menues.tcl - Debug menu
    ::msgcat::mcset pl "&Insert/Remove breakpoint" "&Dodaj/usu\u0144 pu\u0142apk\u0119"
    ::msgcat::mcset pl "Remove &all breakpoints" "&Usu\u0144 wszystkie pu\u0142apki"
    ::msgcat::mcset pl "&Configure execution..." "&Konfiguruj wykonanie..."
    ::msgcat::mcset pl "Go to next b&reakpoint" "Id\u017a do nast\u0119pnej &pu\u0142apki"
    ::msgcat::mcset pl "&Step by step" "&Krok po kroku"
    ::msgcat::mcset fr "Step &into" "Pas à pas &principal"
    ::msgcat::mcset fr "Step o&ver" "Pas à pas &détaillé"
    ::msgcat::mcset fr "Step &out" "Pas à pas &sortant"
    ::msgcat::mcset pl "Run to c&ursor" "Uruchom do k&ursora"
    ::msgcat::mcset fr "G&o on ignoring any breakpoint" "&Continuer sans aucun point d'arr\u0119t"
    ::msgcat::mcset pl "Show &watch" "Poka\u017c zmienne &obserwowane"
    ::msgcat::mcset pl "&Break" "&Przerwij"
    ::msgcat::mcset pl "Cance&l debug" "&Anuluj odpluskwianie"

#    ::msgcat::mcset pl "language scheme" "motyw j\u0119zykowy"
    # menues.tcl - Scheme menu
    ::msgcat::mcset pl "S&cilab" "S&cilab"
    ::msgcat::mcset pl "&XML" "&XML"
    ::msgcat::mcset pl "&none" "&brak"

    # menues.tcl - Options menu
    ::msgcat::mcset pl "&Font size" "&Rozmiar czcionki"
        ::msgcat::mcset pl "&micro" "m&ikro"
        ::msgcat::mcset pl "&small" "&ma\u0142a"
        ::msgcat::mcset pl "m&edium" "\u015b&rednia"
        ::msgcat::mcset pl "&large" "&du\u017ca"
    ::msgcat::mcset pl "&Colors" "&Kolory"
    ::msgcat::mcset pl "&Tabs and indentation" "&Tabulator i wci\u0119cia"
        ::msgcat::mcset pl "Tab inserts &spaces" "Klawisz Tab wstawia &spacje"
        ::msgcat::mcset pl "&Indentation spaces" "&Ilo\u015bæ spacji w wci\u0119ciu"
    ::msgcat::mcset pl "Colorize \'&strings\'" "K&oloruj \'\u0142añcuchy\'"
    ::msgcat::mcset pl "Word &Wrap" "&Zawijanie wierszy"
    ::msgcat::mcset pl "File&names" "&Nazwy plik\u00f3w"
        ::msgcat::mcset pl "&Full path" "&Pe\u0142na \u015bcie\u017cka"
        ::msgcat::mcset fr "Full path if &ambiguous" "Complets si &ambigus"
        ::msgcat::mcset fr "&Unambiguous pruned path" "&Raccourcis non ambigus"
    ::msgcat::mcset pl "&Recent files" "&Ostatnie pliki"

    # menues.tcl - Windows menu
    ::msgcat::mcset pl "Opened Files" "Otwarte pliki"

    # menues.tcl - Help menu
    ::msgcat::mcset pl "&Help..." "&Pomoc..."
    ::msgcat::mcset pl "&What's?..." "&Co to jest?..."
    ::msgcat::mcset pl "&About" "&O programie"


# popupmenus.tcl
::msgcat::mcset pl "Open the source of" "Otw\u00f3rz \u017ar\u00f3d\u0142o dla"
::msgcat::mcset pl "AddWatch" "Dodaj %s jako obserwowan\u0105 zmienn\u0105"


# print.tcl
::msgcat::mcset pl "Print Setup" "Ustawienia wydruku"
::msgcat::mcset pl "Print Command: " "Komenda wydruku : "


# scilabexec.tcl
::msgcat::mcset pl "No point in loading an empty file!" "Pr\u00f3bujesz wczytaæ pusty plik!"
::msgcat::mcset pl "Scilab is working, wait for the prompt to load file" "Scilab pracuje, poczekaj na znak zach\u0119ty, aby wczytaæ plik"
::msgcat::mcset pl "Exec done" "Wykonanie zako\u0144czone"
::msgcat::mcset pl "Scilab is working, wait for the prompt to execute the selection." "Scilab pracuje, poczekaj na znak zach\u0119ty, aby wykonaæ zaznaczenie."
::msgcat::mcset pl "Scilab is working, wait for the prompt to convert a Matlab file." "Scilab pracuje, poczekaj na znak zach\u0119ty, aby przekonwerterowaæ plik Matlab."
::msgcat::mcset pl "Matlab files" "Pliki Matlab-a"
::msgcat::mcset pl "Matlab file to convert" "Plik Matlab-a do konwersji"
::msgcat::mcset pl "Scilab is converting, please hold on..." "Scilab teraz konwertuje, prosz\u0119 czekaæ..."
::msgcat::mcset pl "Matlab file import" "Importowanie pliku programu Matlab"
::msgcat::mcset fr "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug/index.cgi" \
                   "La conversion du ficher a échoué, voyez dans la fen\u0119tre\
                    Scilab pour plus d'informations -- Veuillez communiquer le\
                    message d'erreur et le fichier Matlab incriminé à\
                    http://scilabsoft.inria.fr/bugzilla_bug/index.cgi"
::msgcat::mcset fr "Please select destination path for the xml source of the help file" "Veuillez choisir le chemin de destination pour le source xml du fichier d'aide"
::msgcat::mcset pl "Older version of" "Znaleziono starsz\u0105 wersj\u0119"
    ::msgcat::mcset pl "found!" "!"
::msgcat::mcset pl "An old version of" "Starsza wersja pliku"
    ::msgcat::mcset pl "already exists: open the old file instead?" "ju\u017c istnieje: otworzyæ stary plik ?"
::msgcat::mcset pl "Existing file" "Istniej\u0105cy plik"
::msgcat::mcset pl "New skeleton" "Nowy szkielet"
::msgcat::mcset pl "Scilab is working, wait for the prompt to compile the help file." "Scilab pracuje, poczekaj na znak zach\u0119ty, aby skompilowaæ plik pomocy."
::msgcat::mcset fr "ScilabEval command cannot be passed to Scilab!" "Une commande ScilabEval ne peut pas \u0119tre passée à Scilab!"
::msgcat::mcset fr impossibleScilabEval_message \
                   "ATTENTION : Une commande n'a pas pu \u0119tre passée à Scilab et a été ignorée.\n\n \
                   La raison probable est que vous n'avez pas les droits d'écriture dans le répertoire TMPDIR de Scilab. \
                   Un comportement imprévisible peut se produire.\n\n \
                   La commande en cause est:\n\n-->"


# scipad.tcl
::msgcat::mcset pl "NOT CONNECTED TO SCILAB" "NIE PO\u0142\u0105CZONO ZE SCILAB-em"


# textarea.tcl
::msgcat::mcset pl "Font size" "Rozmiar czcionki"


# whichfun.tcl
::msgcat::mcset fr "Being at line" "Etant à la ligne"
    ::msgcat::mcset fr ", function" ", la fonction"
    ::msgcat::mcset fr "begins at" "débute en"
    ::msgcat::mcset fr ", and there are" ", et il y a"
    ::msgcat::mcset fr "continued lines, i.e. we are at line" "lignes multiples, i.e. nous sommes à la ligne"
    ::msgcat::mcset fr "of" "de"
::msgcat::mcset fr "Function" "Fonction"
    ::msgcat::mcset fr "line" "ligne"
::msgcat::mcset fr "The cursor is not currently inside a function body." "Le curseur ne se trouve pas à l'intérieur d'une fonction."
::msgcat::mcset fr "Which function?" "Quelle fonction ?"


#preference variables description
::msgcat::mcset pl "BGCOLOR" "T\u0142a"
::msgcat::mcset pl "FGCOLOR" "Tekstu"
::msgcat::mcset pl "CURCOLOR" "Kursora"
::msgcat::mcset pl "PARCOLOR" "Nawiasu okr\u0105g\u0142ego ()"
::msgcat::mcset pl "BRAKCOLOR" "Nawiasu kwadratowego \\\[\\\]"
::msgcat::mcset pl "BRACCOLOR" "Klamry {}"
::msgcat::mcset pl "PUNCOLOR"  "Interpunkcji"
::msgcat::mcset pl "KEYWCOLOR" "Wewn\u0119trzne Scilab-a"
::msgcat::mcset pl "OPCOLOR"   "Operator\u00f3w"
::msgcat::mcset pl "QTXTCOLOR"  "\u0142a\u0144cucha znak\u00f3w"
::msgcat::mcset pl "REMCOLOR" "Komentarzy"
::msgcat::mcset pl "XMLCOLOR" "Tag\u00f3w XML"
::msgcat::mcset pl "NUMCOLOR" "Liczb"
::msgcat::mcset pl "SELCOLOR" "Zaznaczenia"
::msgcat::mcset pl "BREAKPOINTCOLOR" "Pu\u0142apki"
::msgcat::mcset pl "LFUNCOLOR" "Funkcji bibliotecznych"
::msgcat::mcset pl "PDEFCOLOR" "Predefiniowanych zmiennych"
::msgcat::mcset pl "FOUNDTEXTCOLOR" "Znalezionego tekstu"


#overriding tcl-core dialogs (many are actually already found in 
#  TKDIR/msgs/fr.msg)
::msgcat::mcset pl "&Directory:" "&Katalog :"
::msgcat::mcset pl "File &name:" "&Nazwa pliku :"
::msgcat::mcset pl "Files of &type:" "Pliki &typu :"
#::msgcat::mcset pl "&Save" "&Zapisz" # already included
::msgcat::mcset pl "&Open" "&Otw\u00f3rz"
::msgcat::mcset pl "Open" "Otw\u00f3rz"
::msgcat::mcset pl "Save As" "Zapisz jako"
::msgcat::mcset pl "&Cancel" "&Anuluj"
::msgcat::mcset pl "&Selection:" "&Zaznaczenie :"
::msgcat::mcset pl "&Yes" "&Tak"
::msgcat::mcset pl "&No" "&Nie"
::msgcat::mcset pl "&Red" "&Czerwony"
::msgcat::mcset pl "&Green" "&Zielony"
::msgcat::mcset pl "&Blue" "&Niebieski"
::msgcat::mcset pl "Directory \"%1\$s\" does not exist."\
        "Katalog \"%1\$s\" nie istnieje."
