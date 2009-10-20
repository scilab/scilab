/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xcos.utils;

import org.scilab.modules.localization.Messages;


/**
 * All localized messages/menus labels used in Xcos
 * @author Vincent COUVERT
 *
 */
public final class XcosMessages {

    /**
     * Constructor
     */
    private XcosMessages() {

    }

    public static final String XCOS = Messages.gettext("Xcos");

    public static final String DOTS = "...";

    public static final String UNTITLED = "Untitled";

    /** File menu */
    public static final String FILE = Messages.gettext("File");
    public static final String NEW = Messages.gettext("New");
    public static final String NEW_DIAGRAM = Messages.gettext("New diagram") + DOTS;
    public static final String NEW_PALETTE = Messages.gettext("New palette") + DOTS;
    public static final String OPEN = Messages.gettext("Open") + DOTS;
    public static final String SAVE = Messages.gettext("Save");
    public static final String SAVE_AS = Messages.gettext("Save as") + DOTS;
    public static final String EXPORT = Messages.gettext("Export") + DOTS;
    public static final String SAVE_AS_INTERFACE_FUNCTION = Messages.gettext("Save as interface function") + DOTS;
    public static final String PRINT = Messages.gettext("Print") + DOTS;
    public static final String CLOSE = Messages.gettext("Close");
    public static final String QUIT = Messages.gettext("Quit");

    public static final String DUMP = Messages.gettext("Dump");
    public static final String VIEW_IN_SCICOS = Messages.gettext("View in Scicos");

    /** Edit Menu */
    public static final String EDIT = Messages.gettext("Edit");
    public static final String BLOCK_PARAMETERS = Messages.gettext("Block Parameters") + DOTS;
    public static final String REGION_TO_SUPERBLOCK = Messages.gettext("Region to superblock");
    public static final String SUPERBLOCK_MASK = Messages.gettext("Superblock mask");
    public static final String CREATE = Messages.gettext("Create");
    public static final String REMOVE = Messages.gettext("Remove");
    public static final String CUSTOMIZE = Messages.gettext("Customize") + DOTS;
    public static final String SAVE_BLOCK_GUI = Messages.gettext("Save block GUI");

    /** View Menu */
    public static final String VIEW = Messages.gettext("View");
    public static final String FIT_DIAGRAM_TO_VIEW = Messages.gettext("Fit diagram to view");
    public static final String NORMAL_100 = Messages.gettext("Normal 100%");
    public static final String PALETTE_BROWSER = Messages.gettext("Palette browser");
    public static final String DIAGRAM_BROWSER = Messages.gettext("Diagram browser");
    public static final String VIEWPORT = Messages.gettext("Viewport");
    public static final String GET_INFOS = Messages.gettext("Get infos");
    public static final String DETAILS = Messages.gettext("Details");
    public static final String BROWSER = Messages.gettext("Browser");

    /** Simulation menu */
    public static final String SIMULATION = Messages.gettext("Simulation");
    public static final String SETUP = Messages.gettext("Setup");
    public static final String COMPILE = Messages.gettext("Compile");
    public static final String START = Messages.gettext("Start");
    public static final String STOP = Messages.gettext("Stop");
    public static final String SET_CONTEXT = Messages.gettext("Set Context");

    /** Format menu */
    public static final String FORMAT = Messages.gettext("Format");
    public static final String RESIZE = Messages.gettext("Resize");
    public static final String ROTATE = Messages.gettext("Rotate");
    public static final String FLIP = Messages.gettext("Flip");
    public static final String SHOWHIDE_SHADOW = Messages.gettext("Show/Hide shadow");
    public static final String GRID = Messages.gettext("Grid");
    public static final String DIAGRAM_BACKGROUND = Messages.gettext("Diagram background") + DOTS;
    
    /** Tools menu */
    public static final String TOOLS = Messages.gettext("Tools");
    public static final String CODE_GENERATION = Messages.gettext("Code generation");

    /** Help menu */
    public static final String HELP = Messages.gettext("Help");
    public static final String XCOS_DOCUMENTATION = Messages.gettext("Xcos Help");
    public static final String BLOCK_DOCUMENTATION = Messages.gettext("Block Help");
    public static final String XCOS_DEMONSTRATIONS = Messages.gettext("Xcos Demos");
    public static final String ABOUT_XCOS = Messages.gettext("About Xcos") + DOTS;

    /** PALETTES */
    public static final String PALETTES = Messages.gettext("Palettes");
    public static final String SOURCES_PAL = Messages.gettext("Sources");
    public static final String CONTINUOUS_PAL = Messages.gettext("Continuous");
    public static final String DISCONTINUOUS_PAL = Messages.gettext("Discontinuous");
    public static final String LOOKUPTABLES_PAL = Messages.gettext("Lookup Tables");
    public static final String SIGNALPROCESSING_PAL = Messages.gettext("Signal Processing");
    public static final String THRESHOLD_PAL = Messages.gettext("Threshold");
    public static final String MATHSOPS_PAL = Messages.gettext("Math Operations");
    public static final String MODELICA_PAL = Messages.gettext("Modelica");
    public static final String INTEGER_PAL = Messages.gettext("Integer");
    public static final String MATRIX_PAL = Messages.gettext("Matrix");
    public static final String SINKS_PAL = Messages.gettext("Sinks");
    public static final String PORTACTION_PAL = Messages.gettext("Port Action");
    public static final String DISCRETE_PAL = Messages.gettext("Discrete");
    public static final String EVENTS_PAL = Messages.gettext("Events");
    public static final String SIGNALROUTING_PAL = Messages.gettext("Signal Routing");
    public static final String COMMONUSED_PAL = Messages.gettext("Commonly Used Blocks");
    public static final String USERDEFINEDFUNCTIONS_PAL = Messages.gettext("User-Defined Functions");
    public static final String DEMOBLOCKS_PAL = Messages.gettext("Demonstrations Blocks");
    public static final String ELECTRICAL_PAL = Messages.gettext("Electrical");
    public static final String THERMOHYDRAULICS_PAL = Messages.gettext("Thermo-Hydraulics");

    public static final String DIAGRAM_MODIFIED = Messages.gettext("Diagram has been modified since last save\nsave it?");
    public static final String FILE_DOESNT_EXIST = Messages.gettext("The file %s doesn't exist\n Do you want to create it?");

    public static final String OK = Messages.gettext("Ok");
    public static final String CANCEL = Messages.gettext("Cancel");
    public static final String SET_CONTEXT_LABEL_TEXT = "<html>"  + Messages.gettext("You may enter here scilab instructions to define "
	    + "symbolic parameters used in block definitions using Scilab instructions.<br/>These instructions are evaluated once confirmed."
	    + "(i.e. you click on OK, by Eval and every time diagram is loaded.)" ) + "</html>";

    public static final String FAIL_LOADING_DIAGRAM = Messages.gettext("Failed to load Diagram");
    public static final String FAIL_SAVING_DIAGRAM = Messages.gettext("Could not save diagram.");
    public static final String OVERWRITE_EXISTING_FILE = Messages.gettext("Do you want to overwrite existing file?");
    public static final String TRANSPARENT_BACKGROUND = Messages.gettext("Do you want a transparent background image?");
    public static final String NO_IMAGE_DATA = Messages.gettext("Image contains no data.");

    public static final String XCOS_ERROR = Messages.gettext("Xcos error");
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
}
