/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.xcos.utils;

import java.awt.event.MouseEvent;

import javax.swing.SwingUtilities;

import org.scilab.modules.localization.Messages;

/**
 * All localized messages/menus labels used in Xcos
 */
// CSOFF: JavadocVariable
// CSOFF: LineLength
// CSOFF: MultipleStringLiterals
public final class XcosMessages {

    public static final String XCOS = Messages.gettext("Xcos");

    public static final String DOTS = "...";

    public static final String UNTITLED = Messages.gettext("Untitled");

    /* File menu */
    public static final String FILE = Messages.gettext("File");
    public static final String NEW = Messages.gettext("New");
    public static final String NEW_DIAGRAM = Messages.gettext("New diagram") + DOTS;
    public static final String NEW_PALETTE = Messages.gettext("New palette") + DOTS;
    public static final String OPEN = Messages.gettext("Open") + DOTS;
    public static final String OPEN_IN = Messages.gettext("Open file in Scilab current directory");
    public static final String SAVE = Messages.gettext("Save");
    public static final String SAVE_AS = Messages.gettext("Save as") + DOTS;
    public static final String EXPORT = Messages.gettext("Export") + DOTS;
    public static final String EXPORT_ALL = Messages.gettext("Export all diagrams");
    public static final String SAVE_AS_INTERFACE_FUNCTION = Messages.gettext("Save as interface function") + DOTS;
    public static final String PRINT = Messages.gettext("Print") + DOTS;
    public static final String CLOSE = Messages.gettext("Close");
    public static final String QUIT = Messages.gettext("Quit Xcos");
    public static final String RECENT_FILES = Messages.gettext("Recent files");

    /* Edit Menu */
    public static final String EDIT = Messages.gettext("Edit");
    public static final String BLOCK_PARAMETERS = Messages.gettext("Block Parameters") + DOTS;
    public static final String REGION_TO_SUPERBLOCK = Messages.gettext("Selection to superblock");
    public static final String SUPERBLOCK_MASK = Messages.gettext("Superblock mask");
    public static final String CREATE = Messages.gettext("Create");
    public static final String RENAME = Messages.gettext("Rename");
    public static final String REMOVE = Messages.gettext("Remove");
    public static final String CUSTOMIZE = Messages.gettext("Customize") + DOTS;
    public static final String SAVE_BLOCK_GUI = Messages.gettext("Save block GUI");

    /* Palette menu in palette browser */
    public static final String LOAD_AS_PAL = Messages.gettext("Load as palette") + DOTS;
    public static final String USER_DEFINED = Messages.gettext("User-Defined");
    public static final String REMOVE_USER_DEFINED = REMOVE;
    public static final String CREATE_CATEGORY = Messages.gettext("Create a category");
    public static final String ADDTO_CATEGORY = Messages.gettext("Add to a new category");
    public static final String DEFAULT_CATEGORY_NAME = Messages.gettext("Category");
    public static final String ASK_FOR_A_NAME = Messages.gettext("Enter a name");

    public static final String DUMP = Messages.gettext("Dump");
    public static final String VIEW_IN_SCICOS = Messages.gettext("View in Scicos");

    /* View Menu */
    public static final String VIEW = Messages.gettext("View");
    public static final String FIT_DIAGRAM_TO_VIEW = Messages.gettext("Fit diagram or blocks to view");
    public static final String NORMAL_100 = Messages.gettext("Normal 100%");
    public static final String PALETTE_BROWSER = Messages.gettext("Palette browser");
    public static final String OLD_DIAGRAM_BROWSER = Messages.gettext("tree_show(scs_m)");
    public static final String DIAGRAM_BROWSER = Messages.gettext("Diagram browser");
    public static final String VIEWPORT = Messages.gettext("Viewport");
    public static final String GET_INFOS = Messages.gettext("Get infos");
    public static final String DETAILS = Messages.gettext("Details");
    public static final String SHOW_PARENT = Messages.gettext("Show parent diagram");

    /* Simulation menu */
    public static final String SIMULATION = Messages.gettext("Simulation");
    public static final String SETUP = Messages.gettext("Setup");
    public static final String COMPILE = Messages.gettext("Compile");
    public static final String START = Messages.gettext("Start");
    public static final String STOP = Messages.gettext("Stop");
    public static final String SET_CONTEXT = Messages.gettext("Set Context");

    /* Format menu */
    public static final String FORMAT = Messages.gettext("Format");
    public static final String RESIZE = Messages.gettext("Resize");
    public static final String ROTATE = Messages.gettext("Rotate");
    public static final String FLIP = Messages.gettext("Flip");
    public static final String MIRROR = Messages.gettext("Mirror");
    public static final String SHOWHIDE_SHADOW = Messages.gettext("Show/Hide shadow");
    public static final String GRID = Messages.gettext("Grid");
    public static final String DIAGRAM_BACKGROUND = Messages.gettext("Diagram background") + DOTS;

    /* Format - Edit dialog */
    public static final String BOLD = Messages.gettext("Bold");
    public static final String ITALIC = Messages.gettext("Italic");
    public static final String FONT_SIZE = Messages.gettext("Font size");
    public static final String FONT_NAME = Messages.gettext("Font name");
    public static final String FONT_STYLE = Messages.gettext("Font style");
    public static final String TEXT_SETTINGS = Messages.gettext("Text settings");
    public static final String IMAGE_PATH = Messages.gettext("Image path");

    /* Tools menu */
    public static final String TOOLS = Messages.gettext("Tools");
    public static final String CODE_GENERATION = Messages.gettext("Code generation");

    /* Help menu */
    public static final String HELP = Messages.gettext("?");
    public static final String XCOS_DOCUMENTATION = Messages.gettext("Xcos Help");
    public static final String BLOCK_DOCUMENTATION = Messages.gettext("Block Help");
    public static final String XCOS_DEMONSTRATIONS = Messages.gettext("Xcos Demonstrations");
    // For the moment, "About Xcos" points to "About Scilab", we may update this when we get a nice splash image
    public static final String ABOUT_XCOS = Messages.gettext("About Scilab") + DOTS;

    /* PALETTES */
    public static final String PALETTES = Messages.gettext("Palettes");
    public static final String SOURCES_PAL = Messages.gettext("Sources");
    public static final String CONTINUOUS_PAL = Messages.gettext("Continuous time systems");
    public static final String IMPLICIT_PAL = Messages.gettext("Implicit");
    public static final String DISCONTINUOUS_PAL = Messages.gettext("Discontinuities");
    public static final String LOOKUPTABLES_PAL = Messages.gettext("Lookup Tables");
    public static final String SIGNALPROCESSING_PAL = Messages.gettext("Signal Processing");
    public static final String THRESHOLD_PAL = Messages.gettext("Zero crossing detection");
    public static final String MATHSOPS_PAL = Messages.gettext("Mathematical Operations");
    public static final String INTEGER_PAL = Messages.gettext("Integer");
    public static final String MATRIX_PAL = Messages.gettext("Matrix");
    public static final String SINKS_PAL = Messages.gettext("Sinks");
    public static final String PORTACTION_PAL = Messages.gettext("Port & Subsystem");
    public static final String ANNOTATIONS_PAL = Messages.gettext("Annotations");
    public static final String DISCRETE_PAL = Messages.gettext("Discrete time systems");
    public static final String EVENTS_PAL = Messages.gettext("Event handling");
    public static final String SIGNALROUTING_PAL = Messages.gettext("Signal Routing");
    public static final String COMMONUSED_PAL = Messages.gettext("Commonly Used Blocks");
    public static final String USERDEFINEDFUNCTIONS_PAL = Messages.gettext("User-Defined Functions");
    public static final String DEMOBLOCKS_PAL = Messages.gettext("Demonstrations Blocks");
    public static final String ELECTRICAL_PAL = Messages.gettext("Electrical");
    public static final String THERMOHYDRAULICS_PAL = Messages.gettext("Thermo-Hydraulics");

    public static final String ENABLE = Messages.gettext("Enable");
    public static final String PAL_NAME = Messages.gettext("Palette name");

    public static final String DIAGRAM_MODIFIED = Messages.gettext("Diagram has been modified since last save.<br/> Do you want to save it?");
    public static final String FILE_DOESNT_EXIST = Messages.gettext("The file %s doesn't exist\n Do you want to create it?");

    public static final String IMAGE_URL_DOESNT_EXIST = Messages.gettext("Unable to decode the URI : %s .");

    public static final String OK = Messages.gettext("Ok");
    public static final String CANCEL = Messages.gettext("Cancel");
    public static final String RESET = Messages.gettext("Reset to default");

    public static final String SET_CONTEXT_LABEL_TEXT = "<html>"
            + Messages
            .gettext("You may enter here scilab instructions to define symbolic parameters used in block definitions using Scilab instructions.<br/>These instructions are evaluated once confirmed (i.e. you click on OK and every time the diagram is loaded).")
            + "</html>";

    public static final String FAIL_LOADING_DIAGRAM = Messages.gettext("Failed to load Diagram");
    public static final String FAIL_SAVING_DIAGRAM = Messages.gettext("Could not save diagram.");
    public static final String OVERWRITE_EXISTING_FILE = Messages.gettext("Do you want to overwrite existing file?");
    public static final String TRANSPARENT_BACKGROUND = Messages.gettext("Do you want a transparent background image?");
    public static final String NO_IMAGE_DATA = Messages.gettext("Image contains no data.");
    public static final String UNKNOW_VERSION = Messages.gettext("Unknown Diagram Version : ");
    public static final String TRY_TO_CONTINUE = Messages.gettext("Will try to continue...");

    public static final String XCOS_ERROR = Messages.gettext("Xcos error");
    public static final String UNABLE_TO_DELETE = Messages.gettext("Unable to delete ");
    public static final String NO_BLOCK_SELECTED = Messages.gettext("No block selected");

    public static final String EXPORT_TO_XML = Messages.gettext("Export to XML") + DOTS;
    public static final String IMPORT_FROM_XML = Messages.gettext("Import from XML") + DOTS;

    public static final String ADDTO = Messages.gettext("Add to");
    public static final String ADDTO_NEW_DIAGRAM = Messages.gettext("Add to new diagram");

    public static final String LINK_ERROR_EXPLICIT_IN = Messages.gettext("Explicit data input port must be connected to explicit data output port");
    public static final String LINK_ERROR_IMPLICIT_IN = Messages.gettext("Implicit data input port must be connected to implicit data output port");
    public static final String LINK_ERROR_EXPLICIT_OUT = Messages.gettext("Explicit data output port must be connected to explicit data input port");
    public static final String LINK_ERROR_IMPLICIT_OUT = Messages.gettext("Implicit data output port must be connected to implicit data input port");
    public static final String LINK_ERROR_EVENT_IN = Messages.gettext("Command port must be connected to control port");
    public static final String LINK_ERROR_EVENT_OUT = Messages.gettext("control port must be connected to command port");
    public static final String LINK_ERROR_ALREADY_CONNECTED = Messages.gettext("Port is already connected, please select an unconnected port or a valid link.");

    public static final String ALIGN_BLOCKS = Messages.gettext("Align Blocks");
    public static final String ALIGN_LEFT = Messages.gettext("Left");
    public static final String ALIGN_RIGHT = Messages.gettext("Right");
    public static final String ALIGN_CENTER = Messages.gettext("Center");
    public static final String ALIGN_TOP = Messages.gettext("Top");
    public static final String ALIGN_BOTTOM = Messages.gettext("Bottom");
    public static final String ALIGN_MIDDLE = Messages.gettext("Middle");

    public static final String BORDER_COLOR = Messages.gettext("Border Color");
    public static final String FILL_COLOR = Messages.gettext("Fill Color");
    public static final String TEXT_COLOR = Messages.gettext("Text Color");

    public static final String LINK_STYLE = Messages.gettext("Link Style");
    public static final String LINK_STYLE_STRAIGHT = Messages.gettext("Straight");
    public static final String LINK_STYLE_HORIZONTAL = Messages.gettext("Horizontal");
    public static final String LINK_STYLE_VERTICAL = Messages.gettext("Vertical");

    public static final String DEBUG_LEVEL_LABEL = "<html>" + Messages.gettext("Set debugging level (0,1,2,3) <br/> it performs scicos_debug(n)") + "</html>";
    public static final String SET_DEBUG = Messages.gettext("Execution trace and Debug");

    /* Setup box */
    public static final String FINAL_INTEGRATION_TIME = Messages.gettext("Final integration time");
    public static final String REAL_TIME_SCALING = Messages.gettext("Real time scaling");
    public static final String INTEGRATOR_ABSOLUTE_TOLERANCE = Messages.gettext("Integrator absolute tolerance");
    public static final String INTEGRATOR_RELATIVE_TOLERANCE = Messages.gettext("Integrator relative tolerance");
    public static final String TOLERANCE_ON_TIME = Messages.gettext("Tolerance on time");
    public static final String MAX_INTEGRATION_TIME_INTERVAL = Messages.gettext("Max integration time interval");
    public static final String SOLVER_CHOICE = Messages.gettext("Solver kind");
    public static final String MAXIMUN_STEP_SIZE = Messages.gettext("Maximum step size (0 means no limit)");
    public static final String DEFAULT = Messages.gettext("Default");
    public static final String SETUP_TITLE = Messages.gettext("Set Parameters");

    /* About box */
    public static final String COPYRIGHT_INRIA = "Copyright (c) 1989-2009 (INRIA)";
    public static final String SCICOS_BASED = Messages.gettext("Based on Scicos");

    /* InfoBar messages */
    public static final String EMPTY_INFO = "";
    public static final String SAVING_DIAGRAM = Messages.gettext("Saving diagram") + DOTS;
    public static final String LOADING_DIAGRAM = Messages.gettext("Loading diagram");
    public static final String LOADING_PALETTES = Messages.gettext("Loading palettes") + DOTS;
    public static final String LOADING_USER_DEFINE = Messages.gettext("Loading user defined palettes") + DOTS;
    public static final String GENERATING_C_CODE = Messages.gettext("Generating C Code for SuperBlock") + DOTS;
    public static final String ERROR_GENERATING_C_CODE = Messages.gettext("A SuperBlock must be selected to generate code");
    public static final String SIMULATION_IN_PROGRESS = Messages.gettext("Simulation in progress");
    public static final String COMPILATION_IN_PROGRESS = Messages.gettext("Compilation in progress, results will be stored in the \'cpr\' variable");
    public static final String GENERATE_SUPERBLOCK = Messages.gettext("Generate SuperBlock, please wait") + DOTS;
    public static final String DRAW_LINK = Messages.gettext("Click on diagram to add link point or on a compatible target to finish");

    /* Debug level messages */
    public static final String DEBUGLEVEL_0 = Messages.gettext("No trace nor debug printing");
    public static final String DEBUGLEVEL_1 = Messages.gettext("Light Simulation trace (Discrete and Continuous part switches)");
    public static final String DEBUGLEVEL_2 = Messages.gettext("Per block execution trace and Debug block calls");
    public static final String DEBUGLEVEL_3 = Messages.gettext("Debug block calls without trace");

    /* File description */
    public static final String ALL_SUPPORTED_FORMATS = Messages.gettext("All supported formats");
    public static final String FILE_COSF = Messages.gettext("Scicos file");
    public static final String FILE_COS = FILE_COSF;
    public static final String FILE_XCOS = Messages.gettext("Xcos file");
    public static final String FILE_ZCOS = Messages.gettext("Xcos (zip) file");
    public static final String FILE_XMI = Messages.gettext("XMI (Eclipse EMF) file");
    public static final String FILE_SOD = Messages.gettext("Scilab Open Data file");

    /* Superblock mask editor */
    public static final String MASK_TITLE = Messages.gettext("Masked SuperBlock editor");
    public static final String MASK_INSERT = Messages.gettext("Insert");
    public static final String MASK_DELETE = Messages.gettext("Delete");
    public static final String MASK_MOVEUP = Messages.gettext("Move Up");
    public static final String MASK_MOVEDOWN = Messages.gettext("Move Down");
    public static final String MASK_ROWS = Messages.gettext("Rows");
    public static final String MASK_VARSETTINGS = Messages.gettext("Variable settings");
    public static final String MASK_DEFAULTVALUES = Messages.gettext("Default values");
    public static final String MASK_WINTITLEVAR = Messages.gettext("WinTitle");
    public static final String MASK_WINTITLE = Messages.gettext("Window title");
    public static final String MASK_VARNAME = Messages.gettext("Variable names");
    public static final String MASK_VARDESC = Messages.gettext("Variable descriptions");
    public static final String MASK_EDITABLE = Messages.gettext("Editable");
    public static final String MASK_VARVALUES = Messages.gettext("Values");
    public static final String MASK_DEFAULTWINDOWNAME = Messages.gettext("Set block parameters");

    /* Configuration Error */
    public static final String ERR_CONFIG_PALETTE_INVALID = Messages
            .gettext("The user palette configuration file (palettes.xml) is invalid.<BR> Switching to the default one.");
    public static final String ERR_CONFIG_INVALID = Messages.gettext("The user configuration file (xcos.xml) is invalid.<BR> Switching to the default one.");

    /* Modelica Compiler */
    public static final String INITIALIZING_MODELICA_COMPILER = Messages.gettext("Setting up Modelica Compiler.");
    public static final String INIT_MODELICA = Messages.gettext("Modelica initialize");

    /* Blocks values */
    public static final String BLOCK_FROM = "From %s";
    public static final String BLOCK_GOTO = "Goto %s";

    /* Errors */
    public static final String ERROR_UNABLE_TO_COMPILE_THIS_SUPER_BLOCK = Messages.gettext("Error: unable to compile this SuperBlock");
    public static final String WRONG_PORT_NUMBER = Messages.gettext("Evaluation problem: wrong port number.");
    public static final String EXPECTING_NUMBER = Messages.gettext("Expecting '%d'.");
    public static final String LINK_NOT_CONNECTED = Messages
            .gettext("<html><body>Compilation error: link ignored because it is not connected. <br/>Please reconnect it.</body></html>");

    /* Tooltips */
    public static final String TOOLTIP_BLOCK = Messages.gettext("Block name: ");
    public static final String TOOLTIP_BLOCK_SIMULATION = Messages.gettext("Simulation: ");
    public static final String TOOLTIP_BLOCK_UID = Messages.gettext("UID: ");
    public static final String TOOLTIP_BLOCK_STYLE = Messages.gettext("Style: ");
    public static final String TOOLTIP_BLOCK_RPAR = Messages.gettext("Rpar: ");
    public static final String TOOLTIP_BLOCK_IPAR = Messages.gettext("Ipar: ");
    public static final String TOOLTIP_BLOCK_OPAR = Messages.gettext("Opar: ");
    public static final String TOOLTIP_PORT_DATATYPE = Messages.gettext("Datatype: ");
    public static final String TOOLTIP_PORT_IMPLICIT = Messages.gettext("Implicit: ");
    public static final String TOOLTIP_PORT_STYLE = Messages.gettext("Style: ");
    public static final String TOOLTIP_LINK_SRC_DATATYPE = Messages.gettext("Source datatype: ");
    public static final String TOOLTIP_LINK_TRG_DATATYPE = Messages.gettext("Target datatype: ");
    public static final String TOOLTIP_LINK_LABEL = Messages.gettext("Label: ");
    public static final String TOOLTIP_LINK_STYLE = Messages.gettext("Style: ");

    // CSON: JavadocVariable
    // CSON: LineLength
    // CSON: MultipleStringLiterals

    /** This class is a static singleton, thus it must not be instantiated */
    private XcosMessages() {
    }

    /**
     * This function checks for the popup menu activation under MacOS with Java
     * version 1.5 Related to Scilab bug #5190
     *
     * @param e
     *            Click event
     * @return true if Java 1.5 and MacOS and mouse clic and ctrl activated
     */
    public static boolean isMacOsPopupTrigger(MouseEvent e) {
        return (SwingUtilities.isLeftMouseButton(e) && e.isControlDown() && (System.getProperty("os.name").toLowerCase().indexOf("mac") != -1) && (System
                .getProperty("java.specification.version").equals("1.5")));
    }
}
