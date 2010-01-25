/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.model;

import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * The palette is organized trough multiples categories. As these categories are
 * fixed at compile time, the preferred implementation is an Enum.
 * 
 * We implement lazy loading of the bloc representation, thus the model just use
 * String reference and not raw instance of the icons. The icon loading is
 * performed when a category is selected (and cached after the first selection).
 * The BasicBlock loading is performed on each block selection or Drag'n'Drop
 * (not cached).
 */
public final class PaletteModel {
	private static PaletteModel[] values;
	
	private final String message;
	private final String[] blocks;
	private boolean enable;

	/**
	 * Default constructor
	 * @param message The associated localized message
	 * @param blocks Block contained in this  category
	 * @param enable activation state of the palette
	 */
	public PaletteModel(String message, String[] blocks, boolean enable) {
		this.message = message;
		this.blocks = blocks;
		this.enable = enable;
	}
	
	/** @return the instance descriptor */
	public String getMessage() {
		return message;
	}
	
	/** @return the associated block names */
	public String[] getBlocks() {
		return blocks;
	}

	/** @return The associated blocks */
	public String[] getBlockNames() {
		return blocks;
	}

	/**
	 * @param enable Enable of disable the palette.
	 */
	public void setEnable(boolean enable) {
		this.enable = enable;
	}

	/**
	 * @return true, if the palette is enable, false otherwise
	 */
	public boolean isEnable() {
		return enable;
	}
	
	/** @return a localized description by reflecting XcosMessages */
	@Override
	public String toString() {
		String value = "";
		try {
			value = (String) XcosMessages.class.getField(message).get(null);
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (NoSuchFieldException e) {
			e.printStackTrace();
		} finally {
			if (value.isEmpty()) {
				value = message;
			}
		}
		return value;
	}

	/**
	 * Load the Model from the configuration file. It follows the {@link Enum}
	 * semantics.
	 * 
	 * @return The model list
	 */
	public static PaletteModel[] values() {
		if (values == null) {
			values = ConfigXcosManager.getDefaultPalettes();
			if (values == null || values.length == 0) {
				// When there is not palette visible show them all
				// (configuration backward compatibility from 5.2.0)
				values = getHardCodedPalettes();
			}
		}
		return values;
	}
	
	/**
	 * @return hard coded values for palettes
	 */
	private static PaletteModel[] getHardCodedPalettes() {
		return new PaletteModel[] {
		// CSOFF: MultipleStringLiterals
		/** COMMONLY USED BLOCKS palette */
		new PaletteModel("COMMONUSED_PAL", new String[] {"ANDBLK",
				"BIGSOM_f", "CMSCOPE", "CONST_m", "CONVERT", "CSCOPXY", "DEMUX",
				"DOLLAR_f", "INTEGRAL_f", "IN_f", "LOGICAL_OP", "MUX", "NRMSOM_f",
				"OUT_f", "PRODUCT", "RELATIONALOP", "SATURATION", "SWITCH2_m",
				"TEXT_f"}, true),
		
		/** CONTINUOUS palette */
		new PaletteModel("CONTINUOUS_PAL", new String[] {
				"CLINDUMMY_f", "CLR", "CLSS", "DERIV", "INTEGRAL_f", "INTEGRAL_m",
				"PID", "TCLSS", "TIME_DELAY", "VARIABLE_DELAY", "PDE"}, true),
		
		/** DISCONTINUOUS palette */
		new PaletteModel("DISCONTINUOUS_PAL", new String[] {"BACKLASH",
				"DEADBAND", "DELAYV_f", "HYSTHERESIS", "RATELIMITER",
				"QUANT_f", "SATURATION"}, true),
		
		/** DISCRETE palette */
		new PaletteModel("DISCRETE_PAL", new String[] {"AUTOMAT", "DELAYV_f",
				"DELAY_f", "DLR", "DLRADAPT_f", "DLSS", "DOLLAR_f", "SAMPHOLD_m",
				"TCLSS"}, true),
		
		/** LOOKUP TABLES palette */
		new PaletteModel("LOOKUPTABLES_PAL", new String[] {"INTRP2BLK_f",
				"INTRPLBLK_f", "LOOKUP_f"}, true),
		
		/** EVENTS palette */
		new PaletteModel("EVENTS_PAL", new String[] {"ANDBLK", "ANDLOG_f",
				"CEVENTSCOPE", "CLKFROM", "CLKGOTO", "CLKGotoTagVisibility",
				"CLKOUTV_f", "CLKSOMV_f", "CLOCK_c", "EDGE_TRIGGER", "ENDBLK",
				"END_c", "ESELECT_f", "EVTDLY_c", "EVTGEN_f", "EVTVARDLY",
				"Extract_Activation", "HALT_f", "IFTHEL_f", "M_freq", "MCLOCK_f",
				"MFCLCK_f",	"REGISTER", "SampleCLK", "freq_div"}, true),
		
		/** MATH OPERATIONS palette */
		new PaletteModel("MATHSOPS_PAL", new String[] {"ABS_VALUE", "BIGSOM_f",
				"COSBLK_f", "EXPBLK_m", "GAINBLK_f", "INVBLK", "LOGBLK_f",
				"MATMAGPHI", "MATZREIM", "MAXMIN", "MAX_f", "MIN_f", "POWBLK_f",
				"PRODUCT", "PROD_f", "SIGNUM", "SINBLK_f", "SQRT", "SUMMATION",
				"SUM_f", "TANBLK_f", "TrigFun"}, true),
		
		/** MATRIX palette */
		new PaletteModel("MATRIX_PAL", new String[] {"CUMSUM", "EXTRACT",
				"EXTTRI", "MATBKSL", "MATCATH", "MATCATV", "MATDET", "MATDIAG",
				"MATDIV", "MATEIG", "MATEXPM", "MATINV", "MATLU", "MATMAGPHI",
				"MATMUL", "MATPINV", "MATRESH", "MATSING", "MATSUM", "MATTRAN",
				"MATZCONJ", "MATZREIM", "RICC", "ROOTCOEF", "SQRT", "SUBMAT"}, true),
		
		/** ELECTRICAL palette */
		new PaletteModel("ELECTRICAL_PAL", new String[] {"CCS", "CVS",
				"Capacitor", "ConstantVoltage", "CurrentSensor", "Diode", "Ground",
				"Gyrator", "IdealTransformer", "Inductor", "NMOS", "NPN", "OpAmp",
				"PMOS", "PNP", "PotentialSensor", "Resistor", "SineVoltage",
				"Switch", "VVsourceAC", "VariableResistor", "VoltageSensor",
				"VsourceAC"}, true),
		
		/** INTEGER palette */
		new PaletteModel("INTEGER_PAL", new String[] {"BITCLEAR", "BITSET",
				"CONVERT", "DFLIPFLOP", "DLATCH", "EXTRACTBITS", "INTMUL",
				"JKFLIPFLOP", "LOGIC", "SHIFT", "SRFLIPFLOP"}, true),
		
		/** PORT ACTION palette */
		new PaletteModel("PORTACTION_PAL", new String[] {"CLKINV_f", 
				"CLKOUTV_f", "IN_f", "INIMPL_f", "OUTIMPL_f", "OUT_f", "SUPER_f"}, true),
		
		/** THRESHOLD palette */
		new PaletteModel("THRESHOLD_PAL", new String[] {"GENERAL_f",
				"NEGTOPOS_f", "POSTONEG_f", "ZCROSS_f"}, true),
		
		/** Signal Routing palette */
		new PaletteModel("SIGNALROUTING_PAL", new String[] {"DEMUX",
				"EXTRACTOR", "FROM", "FROMMO", "GOTO", "GOTOMO",
				"GotoTagVisibility", "GotoTagVisibilityMO", "ISELECT_m",
				"MUX", "M_SWITCH", "NRMSOM_f", "RELAY_f", "SELECT_m", "SWITCH2_m",
				"SWITCH_f"}, true),
		
		/** SIGNAL PROCESSING palette */
		new PaletteModel("SIGNALPROCESSING_PAL", new String[] {"QUANT_f",
				"SAMPHOLD_m"}, true),
		
		/** IMPLICIT palette */
		new PaletteModel("IMPLICIT_PAL", new String[] {"CONSTRAINT_f",
				"DIFF_f"}, true),
		
		/** ANNOTATIONS palette */
		new PaletteModel("ANNOTATIONS_PAL", new String[] {"TEXT_f"}, true),
		
		/** SINKS palette */
		new PaletteModel("SINKS_PAL", new String[] {"AFFICH_m", "CANIMXY",
				"CANIMXY3D", "CFSCOPE", "CLKOUTV_f", "CMAT3D", "CMATVIEW",
				"CMSCOPE", "CSCOPE", "CSCOPXY", "CSCOPXY3D", "ENDBLK", "END_c",
				"HALT_f", "OUTIMPL_f", "OUT_f", "TOWS_c", "TRASH_f", "WFILE_f",
				"WRITEAU_f", "WRITEC_f"}, true),
		
		/** SOURCES palette */
		new PaletteModel("SOURCES_PAL", new String[] {"CLKINV_f", "CLOCK_c",
				"CONST_m", "CURV_f", "Counter", "FROMWSB", "GENSIN_f", "GENSQR_f",
				"INIMPL_f", "IN_f", "Modulo_Count", "RAMP", "RAND_m", "READAU_f",
				"READC_f", "RFILE_f", "STEP_FUNCTION", "SampleCLK",
				"Sigbuilder", "TIME_f", "TKSCALE"}, true),
		
		/** THERMO-HYDRAULICS palette */
		new PaletteModel("THERMOHYDRAULICS_PAL", new String[] {"Bache",
				"PerteDP", "PuitsP", "SourceP", "VanneReglante" }, true),
		
		/** DEMO-BLOCKS palette */
		new PaletteModel("DEMOBLOCKS_PAL", new String[] {"BOUNCE", "BOUNCEXY",
				"BPLATFORM"}, true),
		
		/** USER-DEFINED FUNCTIONS palette */
		new PaletteModel("USERDEFINEDFUNCTIONS_PAL", new String[] {
				"CBLOCK", "DEBUG_SCICOS", "EXPRESSION", "MBLOCK", "PDE", "SUPER_f",
				"c_block", "fortran_block", "generic_block3", "scifunc_block_m" }
		, true)
	// CSON: MultipleStringLiterals
		};
	}
}
