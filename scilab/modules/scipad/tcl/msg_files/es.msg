# Scipad texts in Spanish
# in sync at least with v5.73


# commons
::msgcat::mcset es "Cancel" "Cancelar"
::msgcat::mcset es "Cance&l" "Cancel&ar"
::msgcat::mcset es "OK" "Aceptar"
::msgcat::mcset es "Scilab working" "Scilab trabajando"
::msgcat::mcset es "All files" "Todos los archivos"
::msgcat::mcset es "Add/Change" "Añadir/modificar"
::msgcat::mcset es "Add/Chan&ge" "Añadir/&modificar"
::msgcat::mcset es "&Remove" "&Remover"
::msgcat::mcset es "Untitled" "Sin Título"
::msgcat::mcset es "Close" "Cerrar"
::msgcat::mcset es "&Close" "&Cerrar"


# buffernavigation.tcl
::msgcat::mcset es "Goto Line?" "Ir a la línea?"
::msgcat::mcset es "Go to:" "Ir a:"
::msgcat::mcset es "&logical line" "linea &lógica"
::msgcat::mcset es "&physical line" "linea fí&sica"
::msgcat::mcset es "in &function" "en la &función"
::msgcat::mcset es "in &current file" "en el archivo &actual"
::msgcat::mcset es "Outside of function definition"\
                   "Fuera de la definición de la función"


# colorize.tcl
::msgcat::mcset es "Wait seconds while recolorizing file"\
                   "Espere unos segundos mientras se le da nuevamente color al archivo"
::msgcat::mcset es "You can't do that while colorization is in progress.\
                    Please try again when finished."\
                   "Usted no puede hacer eso mientras se da color al archivo.\
                    Por favor intente de nuevo cuando haya terminado el proceso"
::msgcat::mcset es "Command forbidden during colorization"\
                   "Comando prohibido durante la colorización del archivo"


# completion.tcl
::msgcat::mcset es "No possible completion" "No es posible completar"
::msgcat::mcset es "possible completions" "completaciones posibles"
::msgcat::mcset es "Possible completions" "Completaciones posibles"


# db_argsmanagement.tcl
::msgcat::mcset es "Variable:" "Variable:"
::msgcat::mcset es "Value:" "Valor:"
::msgcat::mcset es "&Get current value from Scilab"\
                   "Obtener el &valor actual de Scilab"

# db_configureexec.tcl
::msgcat::mcset es "Configure execution" "Configurar la Ejecución"
::msgcat::mcset es "Function name:" "Nombre de la función:"
::msgcat::mcset es "&Obtain" "&Obtener"
::msgcat::mcset es "Input arguments:" "Argumentos de entrada:"
::msgcat::mcset es "Function name or input arguments do not match definition\
                    of the function"\
                   "El nombre de la función o los argumentos de entrada\
                    no corresponden con la definición de la función"
::msgcat::mcset es "in the file!\n\nCheck function name and arguments\
                    (names, order) in the configuration dialog. \nArguments\
                    order can be changed using drag and drop with right\
                    mouse button in the arguments listbox."\
                   "En el archivo!\n\n Revisar el nombre de la función y sus\
                    argumentos (nombre, orden) en el diálogo de configuración.\
                    \n El orden de los argumentos \
                    puede ser cambiado arrastrando con el botón derecho del ratón."
::msgcat::mcset es "Error in selected function name or arguments"\
                   "Error en la función o argumentos seleccionados"
::msgcat::mcset es "Debugger is not yet developed for sce scripts!"\
                   "El depurador para los guiones .sce todavía no está disponible "


# db_debugsession.tcl
::msgcat::mcset es "Scilab is working, wait for the prompt to issue a\
                    debugger command."\
                   "Scilab está trabajando, espere por el aviso para \
                    usar el comando de depuración."
::msgcat::mcset es "Cursor position is out of reach!"\
                   "La posición del cursor esta fuera de alcance!"
::msgcat::mcset es "Cursor must be in a function"\
                   "El cursor debe estar en una función"
::msgcat::mcset es "No effect - The debugged file is not stuck"\
                   "No hay efecto - El archivado depurado no esta atascado"
::msgcat::mcset es "Scilab execution error"\
                   "Error de Scilab durante la Ejecución"
::msgcat::mcset es "The shell reported an error while trying to execute "\
                   "La shell ha reportado un error tratando de ejecutar"
::msgcat::mcset es "at line " "a la linea "
::msgcat::mcset es " of " " de "
::msgcat::mcset es "Execution aborted!" "Ejecución abortada!"


# db_init.tcl
::msgcat::mcset es "Please wait..." "Por favor espere..."


# db_states.tcl
::msgcat::mcset es "Currently no debug session"\
                   "No hay ninguna sesión de depuración en curso"
::msgcat::mcset es "Ready to start debug" "Listo para empezar la depuración"
::msgcat::mcset es "Debug in progress" "Depuración en curso"


# db_watch.tcl
::msgcat::mcset es "Watch" "Cuidado"
::msgcat::mcset es "Watch variables:" "Cuidado con las variables:"
::msgcat::mcset es "Call stack:" "Llamar la pila del programa:"
::msgcat::mcset es "Hide watch &variables" "Esconder las &variables"
::msgcat::mcset es "Hide call &stack" "Esconder la &pila"
::msgcat::mcset es "Undefined elements are not legal in variable"\
                   "Elementos no definidos no son validos en variables"
::msgcat::mcset es ".\nThis variable will not be updated in Scilab."\
                   ".\nEsta variable no sera actualizada en Scilab"
::msgcat::mcset es "Illegal undefined element found"\
                   "Elemento ilegal indefinido encontrado"


# filecommands.tcl
::msgcat::mcset es "New script" "Nuevo guión"
::msgcat::mcset es "The contents of" "Los contenidos de"
::msgcat::mcset es "may have changed, do you wish to save your changes?"\
                   "puede haber cambiado, desea guardar los cambios?"
::msgcat::mcset es "Save Confirm?" "Confirma Guardar?"
::msgcat::mcset es "Open source of..." "Abrir la fuente de..."
::msgcat::mcset es "Open source of:" "Abrir la fuente de:"
::msgcat::mcset es "Weird drag&drop" "Arras&tre extraño"
::msgcat::mcset es "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or you\
                    hit a bug of the dnd mechanism."\
                   "No es ni  nombre de archivo valido ni nombre de\
		   directorio valido.\
                    Puede ser que usred esté arrastrando un objeto de otro tipo, o usted\
                    encontro un error el en mecanismo dnd."
::msgcat::mcset es "File not found" "Archivo no encontrado"
::msgcat::mcset es "does not exist anymore. Do you want to create an empty\
                    file with the same name?"\
                   "no existe ya. Crear un archivo vacio con el mismo nombre?"
::msgcat::mcset es "Open file" "Abrir archivo"
::msgcat::mcset es "The file" "El Archivo"
::msgcat::mcset es "is already opened! Save the current opened file to\
                    another name and reopen it from disk!" \
		   "Ya esta abierto! Guarde el archivo abierto actualmente\
		    con otro nombre y reabralo desde el disco!"
::msgcat::mcset es "File" "Archivo"
::msgcat::mcset es "saved" "guardado"
::msgcat::mcset es "cannot be written!" "no puede ser escrito"
::msgcat::mcset es "The contents of " "Los contenidos de"
::msgcat::mcset es "has changed on disk, save it anyway?"\
                   "Ha cambiado en el disco, Guardar de todas maneras?"
::msgcat::mcset es "has been modified outside of Scipad. Do you want to\
                    reload it?"\
                   "han sido cambiados fuera de Scipad. Desea recargarlo?"
::msgcat::mcset es "File has changed!" "El archivo ha cambiado"
::msgcat::mcset es "Save as" "Guardar como"
::msgcat::mcset es "Scilab files" "Archivos de Scilab"
::msgcat::mcset es "Scicos files" "Archivos de Scicos"
::msgcat::mcset es "XML files" "Archivos XML"
::msgcat::mcset es "Wait seconds while loading and colorizing file"\
                   "Espere unos segundos mientras se carga y da color al archivo"
::msgcat::mcset es "The contents of this file may have changed, do you wish\
                    to save your changes?"\
                   "Los contenidos de este archivo pueden haber cambiado,\
                    desea guardar los cambios?"
::msgcat::mcset es "Revert" "Regresar a la versión de"
::msgcat::mcset es "to saved?" "al guardado?"
::msgcat::mcset es "Revert Confirm?" "Confirma recuperación?"
::msgcat::mcset es "Unreadable file" "Archivo ilegible!"
::msgcat::mcset es "exists but is not readable!" "Existe pero no es legible!"



# findreplace.tcl
::msgcat::mcset es "The string" "La secuencia \["
::msgcat::mcset es "could not be found in" "\] no se pudo encotrar en"
::msgcat::mcset es "could not be found in any of the opened files !"\
                   "\] no pudo ser encontrada en ninguno de los archivos abiertos!"
::msgcat::mcset es "replacements done" "sustituciones efectuadas"
::msgcat::mcset es "Find" "Buscar"
::msgcat::mcset es "No match found in the selection for"\
                   "No se encontró nada en la selección\["
::msgcat::mcset es "\nWould you like to look for it in the entire text?"\
                   "\] Quisiera buscarlo en el texto completo\?"
::msgcat::mcset es "You are searching for an empty string!"\
                   "Está buscando una secuencia vacía!"
::msgcat::mcset es "Re&place" "&Sustituir"
::msgcat::mcset es "Find what:" "Encontrar:"
::msgcat::mcset es "Replace with:" "Reemplazar con:"
::msgcat::mcset es "Find &Next" "Buscar el siguie&nte"
::msgcat::mcset es "Replace &All" "Reemplazar &todo"
::msgcat::mcset es "&Upwards" "&Arriba"
::msgcat::mcset es "&Downwards" "A&bajo"
::msgcat::mcset es "Match &whole word only" "Buscar solo la palabra &entera"
::msgcat::mcset es "Match &case" "Respetar &mayúsculas"
::msgcat::mcset es "&Regular expression" "Expresión &regular (regexp)"
::msgcat::mcset es "In all &opened files" "En todos los &archivos abiertos"
::msgcat::mcset es "In &selection only" "Sólo en la selecc&ión"
::msgcat::mcset es "In a director&y" "En un &directorio:"
::msgcat::mcset es "Directory r&ecurse search" "Buscar re&cursivamente"
::msgcat::mcset es "In files/file types:" "En archivos/Tipos de archivos"
::msgcat::mcset es "In directory:" "En directorio:"
::msgcat::mcset es "Wrapped around" "Continuado más allá"
#la anterior seguramente esta mal...
::msgcat::mcset es "Back at the first file searched!"\
                   "Volver al primer archivo buscado!"
::msgcat::mcset es "No more matches" "No hay más correspondencias"
::msgcat::mcset es "The string to regexp search for cannot be compiled by\
                    Tcl.\nCheck again, maybe certain characters should be\
                    escaped."\
                   "La cadena de la busqueda regexp no puede ser compilada\
                    por Tcl. Por favor revísela. Quizá,\
                    deba quitar algún caracter especial \\."


# findinfiles.tcl
::msgcat::mcset es "Directory access issue" "Problema al accesar el directorio"
::msgcat::mcset es "The directory you specified cannot be reached. The\
                    search won't bring any result."\
                   "El directorio que especificó está fuera de alcance. La\
                    búsqueda no arrojó ningún resultado"
::msgcat::mcset es "Some directories or files could not be open and were\
                    ignored during search.\n You might miss read access to\
                    them.\n\n The following ones were ignored"\
                   "Algunos directorios o archivos no pudieron ser abiertos\
                    y se ignoraron durante la búsqueda.\n Quiza no pueda leerlos\
                    Los siguientes se ignoraron:"
::msgcat::mcset es "Ignored files" "Archivos ignorados"
::msgcat::mcset es "Searching in file:" "Buscar en el archivo:"
::msgcat::mcset es "End of file search:" "Fin de la búsqueda de archivos:"
::msgcat::mcset es "matches found" "correspondecias encontradas"
::msgcat::mcset es "Constructing file list:"\
                   "Construyendo la lista de archivos:"
::msgcat::mcset es "&Previous" "&Anterior"
::msgcat::mcset es "&Next" "&Siguiente"
::msgcat::mcset es "Scipad search results" "Resultados de la busqueda de Scipad"


# helps.tcl
::msgcat::mcset es "About" "Acerca de"
::msgcat::mcset es "aboutme_message"\
                   "\n Originado de tknotepad de Joseph Acosta,\
                    joeja@mindspring.com.\n Modificado por el consorcio\
                    Scilab.\n Mejoras por Enrico Segre 2003-2006\n y por\
                    François Vogel (en particular el depurador) 2004-2007.\n\
                    Traducción: \n   noruego, Torbjrn Pettersen 2005; \n  \
                    polaco, Bartosz Kosiorek 2005; \n   sueco, Gunnar\
                    Lindholm 2005; \n   alemán, Dirk Reusch 2005; \n   español, Nicolás Quesada 2006."


# infomessages.tcl
::msgcat::mcset es "Line:" "Línea:"
::msgcat::mcset es "Column:" "Columna:"
::msgcat::mcset es "Line" "Línea"
::msgcat::mcset es "in" "en"
::msgcat::mcset es "Logical line:" "Línea lógica:"
::msgcat::mcset es " \[ReadOnly\]" " \[SoloLectura\]"
::msgcat::mcset es " (modified)" " (modificado)"


# inputtext.tcl
::msgcat::mcset es "No corresponding <" "No corresponde <"
#no estoy seguro de la anterior
::msgcat::mcset es "> found!" "> encontrado!"
::msgcat::mcset es "Code editing is not allowed during debug!"\
                   "La edición de código no está permitida durante la depuración!"


# menues.tcl
::msgcat::mcset es "&File" "&Archivo"
::msgcat::mcset es "&Edit" "&Editar"
::msgcat::mcset es "&Search" "&Buscar"
::msgcat::mcset es "E&xecute" "E&jecutar"
::msgcat::mcset es "&Debug" "&Depurar"
::msgcat::mcset es "S&cheme" "E&squema"
::msgcat::mcset es "&Options" "&Opciones"
::msgcat::mcset es "&Windows" "&Ventanas"
::msgcat::mcset es "&Help" "A&yuda"

# menues.tcl - File menu
::msgcat::mcset es "&New" "&Nuevo"
::msgcat::mcset es "&Open..." "&Abrir..."
::msgcat::mcset es "Open &in new" "Abr&ir en una nueva ventana"
::msgcat::mcset es "&horizontal tile" "Cuadro &horizontal"
::msgcat::mcset es "&vertical tile" "Cuadro &vertical"
::msgcat::mcset es "Op&en source of..." "A&brir el código de..."
::msgcat::mcset es "&Save" "&Guardar"
::msgcat::mcset es "Save &as..." "Guardar &como..."
::msgcat::mcset es "&Revert..." "&Recuperar..."
::msgcat::mcset es "Import &Matlab file..." "Importar un archivo de &Matlab..."
::msgcat::mcset es "Create help s&keleton..." "Crear un archivo de ayuda &XML..."
::msgcat::mcset es "Compile as &help page" "Compilar como página de ay&uda"
::msgcat::mcset es "Open &function source" "Abrir el codigo fuente de la &función"
::msgcat::mcset es "Print Se&tup" "Configurar impresión"
::msgcat::mcset es "&Print" "Im&primir"
::msgcat::mcset es "E&xit" "&Salir"

# menues.tcl - Edit menu
::msgcat::mcset es "&Undo" "&Deshacer"
::msgcat::mcset es "&Redo" "&Rehacer"
::msgcat::mcset es "Cu&t" "&Cortar"
::msgcat::mcset es "&Copy" "C&opiar"
::msgcat::mcset es "&Paste" "&Pegar"
::msgcat::mcset es "&Delete" "&Eliminar"
::msgcat::mcset es "Select &All" "Se&leccionar todo"
::msgcat::mcset es "Insert &Time/Date" "Insertar dia/&hora"
::msgcat::mcset es "Co&mment selection" "Co&mentar selección"
::msgcat::mcset es "U&ncomment selection" "Descome&ntar la selección"
#no se si ese verbo exista, igual con desidentar
::msgcat::mcset es "&Indent selection" "&Identar la selección"
::msgcat::mcset es "Unin&dent selection" "Desiden&tar la selección"

# menues.tcl - Search menu
::msgcat::mcset es "&Find..." "&Buscar..."
::msgcat::mcset es "Find &Next" "Buscar siguie&nte"
::msgcat::mcset es "&Replace..." "&Sustituir..."
::msgcat::mcset es "&Goto Line..." "&Ir a la línea..."

# menues.tcl - Exec menu
::msgcat::mcset es "&Load into Scilab" "&Cargar en Scilab"
::msgcat::mcset es "&Evaluate selection" "&Evaluar la selección"

# menues.tcl - Debug menu
::msgcat::mcset es "&Insert/Remove breakpoint" "&Insertar/remover un limite"
# notese que breakpoint=limite
::msgcat::mcset es "Remove &all breakpoints" "Remover &todos los limites"
::msgcat::mcset es "&Configure execution..." "&Configurar la ejecución..."
::msgcat::mcset es "Go to next b&reakpoint" "Ir al si&guinete límite"
::msgcat::mcset es "&Step by step" "Pa&so a paso"
::msgcat::mcset es "Step &into" "Pasar  a&dentro" 
#ni 
::msgcat::mcset es "Step o&ver" "Quedarse a&qui" 
#####???
::msgcat::mcset es "Step &out" "Quedarse a&fuera"
::msgcat::mcset es "Run to c&ursor" "Correr con el curso&r"
::msgcat::mcset es "G&o on ignoring any breakpoint"\
                   "Co&ntinuar ignorando cualquier límite"
::msgcat::mcset es "Show &watch" "Mostrar &observación"
::msgcat::mcset es "&Break" "&Parar"
::msgcat::mcset es "Cance&l debug" "C&ancelar depuración"

#    ::msgcat::mcset fr "language scheme" "Esquema de Lenguage"
# menues.tcl - Scheme menu
::msgcat::mcset es "S&cilab" "&Scilab"
::msgcat::mcset es "&XML" "&XML"
::msgcat::mcset es "&none" "&ninguno"

# menues.tcl - Options menu
::msgcat::mcset es "&Font size" "&Tamaño de la fuente"
::msgcat::mcset es "&micro" "&micro"
::msgcat::mcset es "&small" "&pequeña"
::msgcat::mcset es "m&edium" "m&ediano"
::msgcat::mcset es "&large" "&grande"
::msgcat::mcset es "&Colors" "&Color"
::msgcat::mcset es "Colorize \'&strings\'" "Dar color \'ca&dena\'"
::msgcat::mcset es "Show c&ontinued lines" "Mostrar líneas continuadas"
::msgcat::mcset es "Word &wrap" "&Regresar al campo visual"
# de esta tampoco estoy muy seguro y estoy basandome en la traducción del italiano
::msgcat::mcset es "&Tabs and indentation" "&Tabulación e identación"
::msgcat::mcset es "Tab inserts &spaces" "T&ab inserta espacios"
::msgcat::mcset es "&Indentation spaces" "E&spacios de identacion"
::msgcat::mcset es "Com&pletion" "A&utocompletación"
::msgcat::mcset es "&Tab" "Ta&b"
::msgcat::mcset es "&Control-Tab" "C&ontrol-Tab"
::msgcat::mcset es "&Alt-Tab" "A&lt-Tab"
::msgcat::mcset es "&Shift-Tab" "S&hift-Tab"
::msgcat::mcset es "C&ontrol-Alt-Tab" "Co&ntrol-Alt-Tab"
::msgcat::mcset es "S&hift-Control-Tab" "Sh&ift-Control-Tab"
::msgcat::mcset es "Sh&ift-Alt-Tab" "Shi&ft-Alt-Tab"
::msgcat::mcset es "Shi&ft-Control-Alt-Tab" "Shift-Control-Alt-Tab"
::msgcat::mcset es "File&names" "Nombre de los arc&hivos"
::msgcat::mcset es "&Full path" "Dirección com&pleta"
::msgcat::mcset es "Full path if &ambiguous" "Toda la dirección si hay &ambiguedad"
::msgcat::mcset es "&Unambiguous pruned path" "Dirección completa sin lugar a ambiguedad"
::msgcat::mcset es "&Recent files" "Archivos recien&tes"
::msgcat::mcset es "&Backup files depth" "&Versión de las copias de seguridad"
::msgcat::mcset es "&Locale" "&Localización"

# menues.tcl - Windows menu
::msgcat::mcset es "&Maximize" "&Maximizar"
::msgcat::mcset es "&Split" "Dividir &horizontalmente"
::msgcat::mcset es "S&plit (side by side)" "Dividir &verticalmente"
::msgcat::mcset es "Tile all &vertically" "Todo en cuadros vert&icales"
::msgcat::mcset es "Tile all &horizontally" "Todo en cu&adros horizontales"
::msgcat::mcset es "Space sashes &evenly" "Espaciamiento &uniforme"
::msgcat::mcset es "Opened Files" "Archivos abiertos"

# menues.tcl - Help menu
::msgcat::mcset es "&Help..." "&Ayuda..."
::msgcat::mcset es "&What's?..." "&Que es esto..."
::msgcat::mcset es "&About" "Acerca &de"
::msgcat::mcset es "Adding &translations..." "Añadiendo &Traducciones..."
::msgcat::mcset es "Adding Scipad Translations..."\
                   "Añadiendo traducciones de Scipad:"
::msgcat::mcset es "&Bugs \& wishlist" "&Errores \& Lista de Deseos"
::msgcat::mcset es "Scipad Known bugs" "Errores conocidos"
::msgcat::mcset es "Change&log" "Cronologia de &modificación"
::msgcat::mcset es "Changes in the Scipad codebase"\
                   "Cambios en el código base de Scipad"


# popupmenus.tcl
::msgcat::mcset es "Open the source of" "Abrir la fuente de"
::msgcat::mcset es "AddWatch" "Añadir observación"
#La anterior probablemente este mala


# print.tcl
::msgcat::mcset es "Print Setup" "Configurar la impresión"
::msgcat::mcset es "Print Command: " "Comando de impresión:"


# scilabexec.tcl
::msgcat::mcset es "No point in loading an empty file!"\
                   "Es inútil cargar un archivo vacío!"
::msgcat::mcset es "Scilab is working, wait for the prompt to load file"\
                   "Scilab está trabajando, espere por el aviso para cargar el\
                    archivo"
::msgcat::mcset es "Exec done" "Ejecución terminada"
::msgcat::mcset es "Scilab is working, wait for the prompt to execute the\
                    selection."\
                   "Scilab esta trabajando, por favor espere el aviso para\
		    ejecutar la selección"
::msgcat::mcset es "Scilab is working, wait for the prompt to convert a\
                    Matlab file."\
                   "Scilab esta trabajando, por favor espere el aviso para\
                    convertir un archivo Matlab"
::msgcat::mcset es "Matlab files" "Archivos Matlab"
::msgcat::mcset es "Matlab file to convert" "Archivo Matlab a convertir"
::msgcat::mcset es "Scilab is converting, please hold on..."\
                   "Scilab esta convirtiendo, por favor espere..."
::msgcat::mcset es "Matlab file import" "Importación de un archivo Matlab"
::msgcat::mcset es "Conversion of the file failed, see the Scilab window for\
                    details -- Perhaps report the error text and the\
                    offending Matlab file to \
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"\
		   "La conversión del archivo falló vea la ventana de\
                    Scilab para ver mas detalles -- Si desea reporte el error a \
                    http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi\
                    incluyendo el archivo matlab."


::msgcat::mcset es "Please select destination path for the xml source of the\
                    help file"\
                   "Por favor seleccione la ubicación de la fuente del
		    archivo xml de ayuda"
::msgcat::mcset es "Older version of" "Versión anteriorde "
::msgcat::mcset es "found!" "encontrado!"
::msgcat::mcset es "An old version of" "Una versión anterior de"
::msgcat::mcset es "already exists: open the old file instead?"\
                   "Ya existe: desea abrir el archivo anterior?"
::msgcat::mcset es "Existing file" "Archivo existente"
::msgcat::mcset es "New skeleton" "Crear nuevo esqueleto"
::msgcat::mcset es "Scilab is working, wait for the prompt to compile the\
                    help file."\
                   "Scilab esta trabajando, espere por el aviso para compilar\
                    el archivo de ayuda"
::msgcat::mcset es "ScilabEval command cannot be passed to Scilab!"\
                   "Un comando ScilabEval no puede ser pasado  a\
                    Scilab!"
::msgcat::mcset es "impossibleScilabEval_message"\
                   "ATENCIÓN : Un comando no ha podido ser pasado a\
                    Scilab."


# scipad.tcl
::msgcat::mcset es "NOT CONNECTED TO SCILAB" "NO CONECTADO A SCILAB"


# textarea.tcl
::msgcat::mcset es "Font size" "Tamaño de la fuente"


# whichfun.tcl
::msgcat::mcset es "Being at line" "Estando en la línea"
::msgcat::mcset es ", function" ", la función"
::msgcat::mcset es "begins at" "comienza en"
::msgcat::mcset es ", and there are" ", y allá estan"
#como raro
::msgcat::mcset es "continued lines, i.e. we are at line"\
                   "líneas continuadas, por ejemplo estamos en la línea"
#no cuadra
::msgcat::mcset es "of" "de"
::msgcat::mcset es "Function" "Función"
::msgcat::mcset es "line" "línea"
::msgcat::mcset es "The cursor is not currently inside a function body."\
                   "El cursor no esta dentro de una función"
::msgcat::mcset es "Which function?" "Cúal función?"


#preference variables description
::msgcat::mcset es "BGCOLOR" "Fondo"
::msgcat::mcset es "FGCOLOR" "Texto simple"
::msgcat::mcset es "CURCOLOR" "Cursor"
::msgcat::mcset es "PARCOLOR" "Paréntesis ()"
::msgcat::mcset es "BRAKCOLOR" "Corchetes \\\[\\\]"
::msgcat::mcset es "BRACCOLOR" "Llaves {}"
::msgcat::mcset es "PUNCOLOR" "Puntuación"
::msgcat::mcset es "INTFCOLOR" "Función interna"
::msgcat::mcset es "COMMCOLOR" "Comando de Scilab"
::msgcat::mcset es "SCICCOLOR" "Función de Scicos"
::msgcat::mcset es "OPCOLOR" "Operación"
::msgcat::mcset es "QTXTCOLOR" "Cadena de caracteres"
::msgcat::mcset es "REMCOLOR" "Comentario"
::msgcat::mcset es "XMLCOLOR" "XML"
::msgcat::mcset es "NUMCOLOR" "Numero"
::msgcat::mcset es "SELCOLOR" "Color de selección"
::msgcat::mcset es "BREAKPOINTCOLOR" "Punto de límite"
::msgcat::mcset es "LFUNCOLOR" "Función de biblioteca"
::msgcat::mcset es "PDEFCOLOR" "Variable predefinida"
::msgcat::mcset es "FOUNDTEXTCOLOR" "Texto encontrado"
::msgcat::mcset es "CONTLINECOLOR" "Línea incompleta"

#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset es "&Directory:" "&Direcotrio"
::msgcat::mcset es "File &name:" "&Nombre del archivo:"
::msgcat::mcset es "Files of &type:" "Archivos de &tipo:"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
::msgcat::mcset es "&Open" "A&brir"
::msgcat::mcset es "Open" "Abrir"
::msgcat::mcset es "Save As" "Guardar como"
::msgcat::mcset es "&Cancel" "Ca&ncelar"
::msgcat::mcset es "&Selection:" "&Selección:"
::msgcat::mcset es "&Yes" "&Si"
::msgcat::mcset es "&No" "&No"
::msgcat::mcset es "&Red" "&Rojo"
::msgcat::mcset es "&Green" "&Verde"
::msgcat::mcset es "&Blue" "A&zul"
::msgcat::mcset es "Directory \"%1\$s\" does not exist."\
                   "El directorio \"%1\$s\" no existe."


