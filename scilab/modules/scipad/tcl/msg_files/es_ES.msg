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

# Scipad texts in Spanish by Juan Nicolás Quesada
# in sync at least with v5.73


# commons
::msgcat::mcset es_ES "Cancel" "Cancelar"
::msgcat::mcset es_ES "Cance&l" "Cancel&ar"
::msgcat::mcset es_ES "OK" "Aceptar"
::msgcat::mcset es_ES "Scilab working" "Scilab trabajando"
::msgcat::mcset es_ES "All files" "Todos los archivos"
::msgcat::mcset es_ES "Add/Change" "Añadir/modificar"
::msgcat::mcset es_ES "Add/Chan&ge" "Añadir/&modificar"
::msgcat::mcset es_ES "&Remove" "&Remover"
::msgcat::mcset es_ES "Untitled" "Sin Título"
::msgcat::mcset es_ES "Close" "Cerrar"
::msgcat::mcset es_ES "&Close" "&Cerrar"


# buffernavigation.tcl
::msgcat::mcset es_ES "Goto Line?" "Ir a la línea?"
::msgcat::mcset es_ES "Go to:" "Ir a:"
::msgcat::mcset es_ES "&logical line" "linea &lógica"
::msgcat::mcset es_ES "&physical line" "linea fí&sica"
::msgcat::mcset es_ES "in &function" "en la &función"
::msgcat::mcset es_ES "in &current file" "en el archivo &actual"
::msgcat::mcset es_ES "Outside of function definition"\
                   "Fuera de la definición de la función"


# colorize.tcl
::msgcat::mcset es_ES "Wait seconds while recolorizing file"\
                   "Espere unos segundos mientras se le da nuevamente color al archivo"
::msgcat::mcset es_ES "You can't do that while colorization is in progress.\
                    Please try again when finished."\
                   "Usted no puede hacer eso mientras se da color al archivo.\
                    Por favor intente de nuevo cuando haya terminado el proceso"
::msgcat::mcset es_ES "Command forbidden during colorization"\
                   "Comando prohibido durante la colorización del archivo"


# completion.tcl
::msgcat::mcset es_ES "No possible completion" "No es posible completar"
::msgcat::mcset es_ES "possible completions" "completaciones posibles"
::msgcat::mcset es_ES "Possible completions" "Completaciones posibles"


# db_argsmanagement.tcl
::msgcat::mcset es_ES "Variable:" "Variable:"
::msgcat::mcset es_ES "Value:" "Valor:"
::msgcat::mcset es_ES "&Get current value from Scilab"\
                   "Obtener el &valor actual de Scilab"

# db_configureexec.tcl
::msgcat::mcset es_ES "Configure execution" "Configurar la Ejecución"
::msgcat::mcset es_ES "Function name:" "Nombre de la función:"
::msgcat::mcset es_ES "&Obtain" "&Obtener"
::msgcat::mcset es_ES "Input arguments:" "Argumentos de entrada:"
::msgcat::mcset es_ES "Function name or input arguments do not match definition\
                    of the function"\
                   "El nombre de la función o los argumentos de entrada\
                    no corresponden con la definición de la función"
::msgcat::mcset es_ES "in the file!\n\nCheck function name and arguments\
                    (names, order) in the configuration dialog. \nArguments\
                    order can be changed using drag and drop with right\
                    mouse button in the arguments listbox."\
                   "En el archivo!\n\n Revisar el nombre de la función y sus\
                    argumentos (nombre, orden) en el diálogo de configuración.\
                    \n El orden de los argumentos \
                    puede ser cambiado arrastrando con el botón derecho del ratón."
::msgcat::mcset es_ES "Error in selected function name or arguments"\
                   "Error en la función o argumentos seleccionados"
::msgcat::mcset es_ES "Debugger is not yet developed for sce scripts!"\
                   "El depurador para los guiones .sce todavía no está disponible "


# db_debugsession.tcl
::msgcat::mcset es_ES "Scilab is working, wait for the prompt to issue a\
                    debugger command."\
                   "Scilab está trabajando, espere por el aviso para \
                    usar el comando de depuración."
::msgcat::mcset es_ES "Cursor position is out of reach!"\
                   "La posición del cursor esta fuera de alcance!"
::msgcat::mcset es_ES "Cursor must be in a function"\
                   "El cursor debe estar en una función"
::msgcat::mcset es_ES "No effect - The debugged file is not stuck"\
                   "No hay efecto - El archivado depurado no esta atascado"
::msgcat::mcset es_ES "Scilab execution error"\
                   "Error de Scilab durante la Ejecución"
::msgcat::mcset es_ES "The shell reported an error while trying to execute "\
                   "La shell ha reportado un error tratando de ejecutar"
::msgcat::mcset es_ES "at line " "a la linea "
::msgcat::mcset es_ES " of " " de "
::msgcat::mcset es_ES "Execution aborted!" "Ejecución abortada!"


# db_init.tcl
::msgcat::mcset es_ES "Please wait..." "Por favor espere..."


# db_states.tcl
::msgcat::mcset es_ES "Currently no debug session"\
                   "No hay ninguna sesión de depuración en curso"
::msgcat::mcset es_ES "Ready to start debug" "Listo para empezar la depuración"
::msgcat::mcset es_ES "Debug in progress" "Depuración en curso"


# db_watch.tcl
::msgcat::mcset es_ES "Watch" "Cuidado"
::msgcat::mcset es_ES "Watch variables:" "Cuidado con las variables:"
::msgcat::mcset es_ES "Call stack:" "Llamar la pila del programa:"
::msgcat::mcset es_ES "Hide watch &variables" "Esconder las &variables"
::msgcat::mcset es_ES "Hide call &stack" "Esconder la &pila"
::msgcat::mcset es_ES "Undefined elements are not legal in variable"\
                   "Elementos no definidos no son validos en variables"
::msgcat::mcset es_ES ".\nThis variable will not be updated in Scilab."\
                   ".\nEsta variable no sera actualizada en Scilab"
::msgcat::mcset es_ES "Illegal undefined element found"\
                   "Elemento ilegal indefinido encontrado"


# filecommands.tcl
::msgcat::mcset es_ES "New script" "Nuevo guión"
::msgcat::mcset es_ES "The contents of" "Los contenidos de"
::msgcat::mcset es_ES "may have changed, do you wish to save your changes?"\
                   "puede haber cambiado, desea guardar los cambios?"
::msgcat::mcset es_ES "Save Confirm?" "Confirma Guardar?"
::msgcat::mcset es_ES "Open source of..." "Abrir la fuente de..."
::msgcat::mcset es_ES "Open source of:" "Abrir la fuente de:"
::msgcat::mcset es_ES "Weird drag&drop" "Arras&tre extraño"
::msgcat::mcset es_ES "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or you\
                    hit a bug of the dnd mechanism."\
                   "No es ni  nombre de archivo valido ni nombre de\
		   directorio valido.\
                    Puede ser que usred esté arrastrando un objeto de otro tipo, o usted\
                    encontro un error el en mecanismo dnd."
::msgcat::mcset es_ES "File not found" "Archivo no encontrado"
::msgcat::mcset es_ES "does not exist anymore. Do you want to create an empty\
                    file with the same name?"\
                   "no existe ya. Crear un archivo vacio con el mismo nombre?"
::msgcat::mcset es_ES "Open file" "Abrir archivo"
::msgcat::mcset es_ES "The file" "El Archivo"
::msgcat::mcset es_ES "is already opened! Save the current opened file to\
                    another name and reopen it from disk!" \
		   "Ya esta abierto! Guarde el archivo abierto actualmente\
		    con otro nombre y reabralo desde el disco!"
::msgcat::mcset es_ES "File" "Archivo"
::msgcat::mcset es_ES "saved" "guardado"
::msgcat::mcset es_ES "cannot be written!" "no puede ser escrito"
::msgcat::mcset es_ES "The contents of " "Los contenidos de"
::msgcat::mcset es_ES "has changed on disk, save it anyway?"\
                   "Ha cambiado en el disco, Guardar de todas maneras?"
::msgcat::mcset es_ES "has been modified outside of Scipad. Do you want to\
                    reload it?"\
                   "han sido cambiados fuera de Scipad. Desea recargarlo?"
::msgcat::mcset es_ES "File has changed!" "El archivo ha cambiado"
::msgcat::mcset es_ES "Save as" "Guardar como"
::msgcat::mcset es_ES "Scilab files" "Archivos de Scilab"
::msgcat::mcset es_ES "Scicos files" "Archivos de Scicos"
::msgcat::mcset es_ES "XML files" "Archivos XML"
::msgcat::mcset es_ES "Wait seconds while loading and colorizing file"\
                   "Espere unos segundos mientras se carga y da color al archivo"
::msgcat::mcset es_ES "The contents of this file may have changed, do you wish\
                    to save your changes?"\
                   "Los contenidos de este archivo pueden haber cambiado,\
                    desea guardar los cambios?"
::msgcat::mcset es_ES "Revert" "Regresar a la versión de"
::msgcat::mcset es_ES "to saved?" "al guardado?"
::msgcat::mcset es_ES "Revert Confirm?" "Confirma recuperación?"
::msgcat::mcset es_ES "Unreadable file" "Archivo ilegible!"
::msgcat::mcset es_ES "exists but is not readable!" "Existe pero no es legible!"



# findreplace.tcl
::msgcat::mcset es_ES "The string" "La secuencia \["
::msgcat::mcset es_ES "could not be found in" "\] no se pudo encotrar en"
::msgcat::mcset es_ES "could not be found in any of the opened files !"\
                   "\] no pudo ser encontrada en ninguno de los archivos abiertos!"
::msgcat::mcset es_ES "replacements done" "sustituciones efectuadas"
::msgcat::mcset es_ES "Find" "Buscar"
::msgcat::mcset es_ES "No match found in the selection for"\
                   "No se encontró nada en la selección\["
::msgcat::mcset es_ES "\nWould you like to look for it in the entire text?"\
                   "\] Quisiera buscarlo en el texto completo\?"
::msgcat::mcset es_ES "You are searching for an empty string!"\
                   "Está buscando una secuencia vacía!"
::msgcat::mcset es_ES "Re&place" "&Sustituir"
::msgcat::mcset es_ES "Find what:" "Encontrar:"
::msgcat::mcset es_ES "Replace with:" "Reemplazar con:"
::msgcat::mcset es_ES "Find &Next" "Buscar el siguie&nte"
::msgcat::mcset es_ES "Replace &All" "Reemplazar &todo"
::msgcat::mcset es_ES "&Upwards" "&Arriba"
::msgcat::mcset es_ES "&Downwards" "A&bajo"
::msgcat::mcset es_ES "Match &whole word only" "Buscar solo la palabra &entera"
::msgcat::mcset es_ES "Match &case" "Respetar &mayúsculas"
::msgcat::mcset es_ES "&Regular expression" "Expresión &regular (regexp)"
::msgcat::mcset es_ES "In all &opened files" "En todos los &archivos abiertos"
::msgcat::mcset es_ES "In &selection only" "Sólo en la selecc&ión"
::msgcat::mcset es_ES "In a director&y" "En un &directorio:"
::msgcat::mcset es_ES "Directory r&ecurse search" "Buscar re&cursivamente"
::msgcat::mcset es_ES "In files/file types:" "En archivos/Tipos de archivos"
::msgcat::mcset es_ES "In directory:" "En directorio:"
::msgcat::mcset es_ES "Wrapped around" "Continuado más allá"
#la anterior seguramente esta mal...
::msgcat::mcset es_ES "Back at the first file searched!"\
                   "Volver al primer archivo buscado!"
::msgcat::mcset es_ES "No more matches" "No hay más correspondencias"
::msgcat::mcset es_ES "The string to regexp search for cannot be compiled by\
                    Tcl.\nCheck again, maybe certain characters should be\
                    escaped."\
                   "La cadena de la busqueda regexp no puede ser compilada\
                    por Tcl. Por favor revísela. Quizá,\
                    deba quitar algún caracter especial \\."


# findinfiles.tcl
::msgcat::mcset es_ES "Directory access issue" "Problema al accesar el directorio"
::msgcat::mcset es_ES "The directory you specified cannot be reached. The\
                    search won't bring any result."\
                   "El directorio que especificó está fuera de alcance. La\
                    búsqueda no arrojó ningún resultado"
::msgcat::mcset es_ES "Some directories or files could not be open and were\
                    ignored during search.\n You might miss read access to\
                    them.\n\n The following ones were ignored"\
                   "Algunos directorios o archivos no pudieron ser abiertos\
                    y se ignoraron durante la búsqueda.\n Quiza no pueda leerlos\
                    Los siguientes se ignoraron:"
::msgcat::mcset es_ES "Ignored files" "Archivos ignorados"
::msgcat::mcset es_ES "Searching in file:" "Buscar en el archivo:"
::msgcat::mcset es_ES "End of file search:" "Fin de la búsqueda de archivos:"
::msgcat::mcset es_ES "matches found" "correspondecias encontradas"
::msgcat::mcset es_ES "Constructing file list:"\
                   "Construyendo la lista de archivos:"
::msgcat::mcset es_ES "&Previous" "&Anterior"
::msgcat::mcset es_ES "&Next" "&Siguiente"
::msgcat::mcset es_ES "Scipad search results" "Resultados de la busqueda de Scipad"


# helps.tcl
::msgcat::mcset es_ES "About" "Acerca de"
::msgcat::mcset es_ES "aboutme_message"\
                   "\n Originado de tknotepad de Joseph Acosta,\
                    joeja@mindspring.com.\n Modificado por el consorcio\
                    Scilab.\n Mejoras por Enrico Segre 2003-2006\n y por\
                    François Vogel (en particular el depurador) 2004-2008.\n\
                    Traducción: \n   noruego, Torbjrn Pettersen 2005; \n  \
                    polaco, Bartosz Kosiorek 2005; \n   sueco, Gunnar\
                    Lindholm 2005; \n   alemán, Dirk Reusch 2005; \n   español, Nicolás Quesada 2006."


# infomessages.tcl
::msgcat::mcset es_ES "Line:" "Línea:"
::msgcat::mcset es_ES "Column:" "Columna:"
::msgcat::mcset es_ES "Line" "Línea"
::msgcat::mcset es_ES "in" "en"
::msgcat::mcset es_ES "Logical line:" "Línea lógica:"
::msgcat::mcset es_ES " \[ReadOnly\]" " \[SoloLectura\]"
::msgcat::mcset es_ES " (modified)" " (modificado)"


# inputtext.tcl
::msgcat::mcset es_ES "No corresponding <" "No corresponde <"
#no estoy seguro de la anterior
::msgcat::mcset es_ES "> found!" "> encontrado!"
::msgcat::mcset es_ES "Code editing is not allowed during debug!"\
                   "La edición de código no está permitida durante la depuración!"


# menues.tcl
::msgcat::mcset es_ES "&File" "&Archivo"
::msgcat::mcset es_ES "&Edit" "&Editar"
::msgcat::mcset es_ES "&Search" "&Buscar"
::msgcat::mcset es_ES "E&xecute" "E&jecutar"
::msgcat::mcset es_ES "&Debug" "&Depurar"
::msgcat::mcset es_ES "S&cheme" "E&squema"
::msgcat::mcset es_ES "&Options" "&Opciones"
::msgcat::mcset es_ES "&Windows" "&Ventanas"
::msgcat::mcset es_ES "&Help" "A&yuda"

# menues.tcl - File menu
::msgcat::mcset es_ES "&New" "&Nuevo"
::msgcat::mcset es_ES "&Open..." "&Abrir..."
::msgcat::mcset es_ES "Open &in new" "Abr&ir en una nueva ventana"
::msgcat::mcset es_ES "&horizontal tile" "Cuadro &horizontal"
::msgcat::mcset es_ES "&vertical tile" "Cuadro &vertical"
::msgcat::mcset es_ES "Op&en source of..." "A&brir el código de..."
::msgcat::mcset es_ES "&Save" "&Guardar"
::msgcat::mcset es_ES "Save &as..." "Guardar &como..."
::msgcat::mcset es_ES "&Revert..." "&Recuperar..."
::msgcat::mcset es_ES "Import &Matlab file..." "Importar un archivo de &Matlab..."
::msgcat::mcset es_ES "Create help s&keleton..." "Crear un archivo de ayuda &XML..."
::msgcat::mcset es_ES "Compile as &help page" "Compilar como página de ay&uda"
::msgcat::mcset es_ES "Open &function source" "Abrir el codigo fuente de la &función"
::msgcat::mcset es_ES "Print Se&tup" "Configurar impresión"
::msgcat::mcset es_ES "&Print" "Im&primir"
::msgcat::mcset es_ES "E&xit" "&Salir"

# menues.tcl - Edit menu
::msgcat::mcset es_ES "&Undo" "&Deshacer"
::msgcat::mcset es_ES "&Redo" "&Rehacer"
::msgcat::mcset es_ES "Cu&t" "&Cortar"
::msgcat::mcset es_ES "&Copy" "C&opiar"
::msgcat::mcset es_ES "&Paste" "&Pegar"
::msgcat::mcset es_ES "&Delete" "&Eliminar"
::msgcat::mcset es_ES "Select &All" "Se&leccionar todo"
::msgcat::mcset es_ES "Insert &Time/Date" "Insertar dia/&hora"
::msgcat::mcset es_ES "Co&mment selection" "Co&mentar selección"
::msgcat::mcset es_ES "U&ncomment selection" "Descome&ntar la selección"
#no se si ese verbo exista, igual con desidentar
::msgcat::mcset es_ES "&Indent selection" "&Identar la selección"
::msgcat::mcset es_ES "Unin&dent selection" "Desiden&tar la selección"

# menues.tcl - Search menu
::msgcat::mcset es_ES "&Find..." "&Buscar..."
::msgcat::mcset es_ES "Find &Next" "Buscar siguie&nte"
::msgcat::mcset es_ES "&Replace..." "&Sustituir..."
::msgcat::mcset es_ES "&Goto Line..." "&Ir a la línea..."

# menues.tcl - Exec menu
::msgcat::mcset es_ES "&Load into Scilab" "&Cargar en Scilab"
::msgcat::mcset es_ES "&Evaluate selection" "&Evaluar la selección"

# menues.tcl - Debug menu
::msgcat::mcset es_ES "&Insert/Remove breakpoint" "&Insertar/remover un limite"
# notese que breakpoint=limite
::msgcat::mcset es_ES "Remove &all breakpoints" "Remover &todos los limites"
::msgcat::mcset es_ES "&Configure execution..." "&Configurar la ejecución..."
::msgcat::mcset es_ES "Go to next b&reakpoint" "Ir al si&guinete límite"
::msgcat::mcset es_ES "&Step by step" "Pa&so a paso"
::msgcat::mcset es_ES "Step &into" "Pasar  a&dentro" 
#ni 
::msgcat::mcset es_ES "Step o&ver" "Quedarse a&qui" 
#####???
::msgcat::mcset es_ES "Step &out" "Quedarse a&fuera"
::msgcat::mcset es_ES "Run to c&ursor" "Correr con el curso&r"
::msgcat::mcset es_ES "G&o on ignoring any breakpoint"\
                   "Co&ntinuar ignorando cualquier límite"
::msgcat::mcset es_ES "Show &watch" "Mostrar &observación"
::msgcat::mcset es_ES "&Break" "&Parar"
::msgcat::mcset es_ES "Cance&l debug" "C&ancelar depuración"

#    ::msgcat::mcset fr "language scheme" "Esquema de Lenguage"
# menues.tcl - Scheme menu
::msgcat::mcset es_ES "S&cilab" "&Scilab"
::msgcat::mcset es_ES "&XML" "&XML"
::msgcat::mcset es_ES "&none" "&ninguno"

# menues.tcl - Options menu
::msgcat::mcset es_ES "&Font size" "&Tamaño de la fuente"
::msgcat::mcset es_ES "&micro" "&micro"
::msgcat::mcset es_ES "&small" "&pequeña"
::msgcat::mcset es_ES "m&edium" "m&ediano"
::msgcat::mcset es_ES "&large" "&grande"
::msgcat::mcset es_ES "&Colors" "&Color"
::msgcat::mcset es_ES "Colorize \'&strings\'" "Dar color \'ca&dena\'"
::msgcat::mcset es_ES "Show c&ontinued lines" "Mostrar líneas continuadas"
::msgcat::mcset es_ES "Word &wrap" "&Regresar al campo visual"
# de esta tampoco estoy muy seguro y estoy basandome en la traducción del italiano
::msgcat::mcset es_ES "&Tabs and indentation" "&Tabulación e identación"
::msgcat::mcset es_ES "Tab inserts &spaces" "T&ab inserta espacios"
::msgcat::mcset es_ES "&Indentation spaces" "E&spacios de identacion"
::msgcat::mcset es_ES "Com&pletion" "A&utocompletación"
::msgcat::mcset es_ES "&Tab" "Ta&b"
::msgcat::mcset es_ES "&Control-Tab" "C&ontrol-Tab"
::msgcat::mcset es_ES "&Alt-Tab" "A&lt-Tab"
::msgcat::mcset es_ES "&Shift-Tab" "S&hift-Tab"
::msgcat::mcset es_ES "C&ontrol-Alt-Tab" "Co&ntrol-Alt-Tab"
::msgcat::mcset es_ES "S&hift-Control-Tab" "Sh&ift-Control-Tab"
::msgcat::mcset es_ES "Sh&ift-Alt-Tab" "Shi&ft-Alt-Tab"
::msgcat::mcset es_ES "Shi&ft-Control-Alt-Tab" "Shift-Control-Alt-Tab"
::msgcat::mcset es_ES "File&names" "Nombre de los arc&hivos"
::msgcat::mcset es_ES "&Full path" "Dirección com&pleta"
::msgcat::mcset es_ES "Full path if &ambiguous" "Toda la dirección si hay &ambiguedad"
::msgcat::mcset es_ES "&Unambiguous pruned path" "Dirección completa sin lugar a ambiguedad"
::msgcat::mcset es_ES "&Recent files" "Archivos recien&tes"
::msgcat::mcset es_ES "&Backup files depth" "&Versión de las copias de seguridad"
::msgcat::mcset es_ES "&Locale" "&Localización"

# menues.tcl - Windows menu
::msgcat::mcset es_ES "&Maximize" "&Maximizar"
::msgcat::mcset es_ES "&Split" "Dividir &horizontalmente"
::msgcat::mcset es_ES "S&plit (side by side)" "Dividir &verticalmente"
::msgcat::mcset es_ES "Tile all &vertically" "Todo en cuadros vert&icales"
::msgcat::mcset es_ES "Tile all &horizontally" "Todo en cu&adros horizontales"
::msgcat::mcset es_ES "Space sashes &evenly" "Espaciamiento &uniforme"
::msgcat::mcset es_ES "Opened Files" "Archivos abiertos"

# menues.tcl - Help menu
::msgcat::mcset es_ES "&Help..." "&Ayuda..."
::msgcat::mcset es_ES "&What's?..." "&Que es esto..."
::msgcat::mcset es_ES "&About" "Acerca &de"
::msgcat::mcset es_ES "Adding &translations..." "Añadiendo &Traducciones..."
::msgcat::mcset es_ES "Adding Scipad Translations..."\
                   "Añadiendo traducciones de Scipad:"
::msgcat::mcset es_ES "&Bugs \& wishlist" "&Errores \& Lista de Deseos"
::msgcat::mcset es_ES "Scipad Known bugs" "Errores conocidos"
::msgcat::mcset es_ES "Change&log" "Cronologia de &modificación"
::msgcat::mcset es_ES "Changes in the Scipad codebase"\
                   "Cambios en el código base de Scipad"


# popupmenus.tcl
::msgcat::mcset es_ES "Open the source of" "Abrir la fuente de"
::msgcat::mcset es_ES "AddWatch" "Añadir observación"
#La anterior probablemente este mala


# print.tcl
::msgcat::mcset es_ES "Print Setup" "Configurar la impresión"
::msgcat::mcset es_ES "Print Command: " "Comando de impresión:"


# scilabexec.tcl
::msgcat::mcset es_ES "No point in loading an empty file!"\
                   "Es inútil cargar un archivo vacío!"
::msgcat::mcset es_ES "Scilab is working, wait for the prompt to load file"\
                   "Scilab está trabajando, espere por el aviso para cargar el\
                    archivo"
::msgcat::mcset es_ES "Exec done" "Ejecución terminada"
::msgcat::mcset es_ES "Scilab is working, wait for the prompt to execute the\
                    selection."\
                   "Scilab esta trabajando, por favor espere el aviso para\
                    ejecutar la selección"
::msgcat::mcset es_ES "Scilab is working, wait for the prompt to convert a\
                    Matlab file."\
                   "Scilab esta trabajando, por favor espere el aviso para\
                    convertir un archivo Matlab"
::msgcat::mcset es_ES "Matlab files" "Archivos Matlab"
::msgcat::mcset es_ES "Matlab file to convert" "Archivo Matlab a convertir"
::msgcat::mcset es_ES "Scilab is converting, please hold on..."\
                   "Scilab esta convirtiendo, por favor espere..."
::msgcat::mcset es_ES "Matlab file import" "Importación de un archivo Matlab"
::msgcat::mcset es_ES "Conversion of the file failed, see the Scilab window for\
                    details -- Perhaps report the error text and the\
                    offending Matlab file to"\
                    "La conversión del archivo falló vea la ventana de\
                    Scilab para ver mas detalles -- Si desea reporte el error, \
                    incluyendo el archivo matlab:"


::msgcat::mcset es_ES "Please select destination path for the xml source of the\
                    help file"\
                   "Por favor seleccione la ubicación de la fuente del
		    archivo xml de ayuda"
::msgcat::mcset es_ES "Older version of" "Versión anteriorde "
::msgcat::mcset es_ES "found!" "encontrado!"
::msgcat::mcset es_ES "An old version of" "Una versión anterior de"
::msgcat::mcset es_ES "already exists: open the old file instead?"\
                   "Ya existe: desea abrir el archivo anterior?"
::msgcat::mcset es_ES "Existing file" "Archivo existente"
::msgcat::mcset es_ES "New skeleton" "Crear nuevo esqueleto"
::msgcat::mcset es_ES "Scilab is working, wait for the prompt to compile the\
                    help file."\
                   "Scilab esta trabajando, espere por el aviso para compilar\
                    el archivo de ayuda"
::msgcat::mcset es_ES "ScilabEval command cannot be passed to Scilab!"\
                   "Un comando ScilabEval no puede ser pasado  a\
                    Scilab!"
::msgcat::mcset es_ES "impossibleScilabEval_message"\
                   "ATENCIÓN : Un comando no ha podido ser pasado a\
                    Scilab."


# scipad.tcl
::msgcat::mcset es_ES "NOT CONNECTED TO SCILAB" "NO CONECTADO A SCILAB"


# textarea.tcl
::msgcat::mcset es_ES "Font size" "Tamaño de la fuente"


# whichfun.tcl
::msgcat::mcset es_ES "Being at line" "Estando en la línea"
::msgcat::mcset es_ES ", function" ", la función"
::msgcat::mcset es_ES "begins at" "comienza en"
::msgcat::mcset es_ES ", and there are" ", y allá estan"
#como raro
::msgcat::mcset es_ES "continued lines, i.e. we are at line"\
                   "líneas continuadas, por ejemplo estamos en la línea"
#no cuadra
::msgcat::mcset es_ES "of" "de"
::msgcat::mcset es_ES "Function" "Función"
::msgcat::mcset es_ES "line" "línea"
::msgcat::mcset es_ES "The cursor is not currently inside a function body."\
                   "El cursor no esta dentro de una función"
::msgcat::mcset es_ES "Which function?" "Cúal función?"


#preference variables description
::msgcat::mcset es_ES "BGCOLOR" "Fondo"
::msgcat::mcset es_ES "FGCOLOR" "Texto simple"
::msgcat::mcset es_ES "CURCOLOR" "Cursor"
::msgcat::mcset es_ES "PARCOLOR" "Paréntesis ()"
::msgcat::mcset es_ES "BRAKCOLOR" "Corchetes \\\[\\\]"
::msgcat::mcset es_ES "BRACCOLOR" "Llaves {}"
::msgcat::mcset es_ES "PUNCOLOR" "Puntuación"
::msgcat::mcset es_ES "INTFCOLOR" "Función interna"
::msgcat::mcset es_ES "COMMCOLOR" "Comando de Scilab"
::msgcat::mcset es_ES "SCICCOLOR" "Función de Scicos"
::msgcat::mcset es_ES "OPCOLOR" "Operación"
::msgcat::mcset es_ES "QTXTCOLOR" "Cadena de caracteres"
::msgcat::mcset es_ES "REMCOLOR" "Comentario"
::msgcat::mcset es_ES "XMLCOLOR" "XML"
::msgcat::mcset es_ES "NUMCOLOR" "Numero"
::msgcat::mcset es_ES "SELCOLOR" "Color de selección"
::msgcat::mcset es_ES "BREAKPOINTCOLOR" "Punto de límite"
::msgcat::mcset es_ES "LFUNCOLOR" "Función de biblioteca"
::msgcat::mcset es_ES "PDEFCOLOR" "Variable predefinida"
::msgcat::mcset es_ES "FOUNDTEXTCOLOR" "Texto encontrado"
::msgcat::mcset es_ES "CONTLINECOLOR" "Línea incompleta"

#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset es_ES "&Directory:" "&Direcotrio"
::msgcat::mcset es_ES "File &name:" "&Nombre del archivo:"
::msgcat::mcset es_ES "Files of &type:" "Archivos de &tipo:"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
::msgcat::mcset es_ES "&Open" "A&brir"
::msgcat::mcset es_ES "Open" "Abrir"
::msgcat::mcset es_ES "Save As" "Guardar como"
::msgcat::mcset es_ES "&Cancel" "Ca&ncelar"
::msgcat::mcset es_ES "&Selection:" "&Selección:"
::msgcat::mcset es_ES "&Yes" "&Si"
::msgcat::mcset es_ES "&No" "&No"
::msgcat::mcset es_ES "&Red" "&Rojo"
::msgcat::mcset es_ES "&Green" "&Verde"
::msgcat::mcset es_ES "&Blue" "A&zul"
::msgcat::mcset es_ES "Directory \"%1\$s\" does not exist."\
                   "El directorio \"%1\$s\" no existe."


