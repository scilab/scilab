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

package org.scilab.modules.xcos.palette;

import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * The palette is organized trough multiple categories.
 */
public enum Category {
// As each block can be in a different panel it is multiple times referenced
// CSOFF: MultipleStringLiterals
	/** COMMONLY USED BLOCKS palette */
	COMMONUSED(XcosMessages.COMMONUSED_PAL, new String[] {"ANDBLK",
			"BIGSOM_f", "CMSCOPE", "CONST_m", "CONVERT", "CSCOPXY", "DEMUX",
			"DOLLAR_f", "INTEGRAL_f", "IN_f", "LOGICAL_OP", "MUX", "NRMSOM_f",
			"OUT_f", "PRODUCT", "RELATIONALOP", "SATURATION", "SWITCH2_m",
			"TEXT_f"}),
	
	/** CONTINUOUS palette */
	CONTINUOUS(XcosMessages.CONTINUOUS_PAL, new String[] {
			"CLINDUMMY_f", "CLR", "CLSS", "DERIV", "INTEGRAL_f", "INTEGRAL_m",
			"PID", "TCLSS", "TIME_DELAY", "VARIABLE_DELAY", "PDE"}),
	
	/** DISCONTINUOUS palette */
	DISCONTINUOUS(XcosMessages.DISCONTINUOUS_PAL, new String[] {"BACKLASH",
			"DEADBAND", "DELAYV_f", "HYSTHERESIS", "RATELIMITER",
			"QUANT_f", "SATURATION"}),
	
	/** DISCRETE palette */
	DISCRETE(XcosMessages.DISCRETE_PAL, new String[] {"AUTOMAT", "DELAYV_f",
			"DELAY_f", "DLR", "DLRADAPT_f", "DLSS", "DOLLAR_f", "SAMPHOLD_m",
			"TCLSS"}),
	
	/** LOOKUP TABLES palette */
	LOOKUPTABLES(XcosMessages.LOOKUPTABLES_PAL, new String[] {"INTRP2BLK_f",
			"INTRPLBLK_f", "LOOKUP_f"}),
	
	/** EVENTS palette */
	EVENTS(XcosMessages.EVENTS_PAL, new String[] {"ANDBLK", "ANDLOG_f",
			"CEVENTSCOPE", "CLKFROM", "CLKGOTO", "CLKGotoTagVisibility",
			"CLKOUTV_f", "CLKSOMV_f", "CLOCK_c", "EDGE_TRIGGER", "ENDBLK",
			"END_c", "ESELECT_f", "EVTDLY_c", "EVTGEN_f", "EVTVARDLY",
			"Extract_Activation", "HALT_f", "IFTHEL_f", "M_freq", "MCLOCK_f",
			"MFCLCK_f",	"REGISTER", "SampleCLK", "freq_div"}),
	
	/** MATH OPERATIONS palette */
	MATHSOPS(XcosMessages.MATHSOPS_PAL, new String[] {"ABS_VALUE", "BIGSOM_f",
			"COSBLK_f", "EXPBLK_m", "GAINBLK_f", "INVBLK", "LOGBLK_f",
			"MATMAGPHI", "MATZREIM", "MAXMIN", "MAX_f", "MIN_f", "POWBLK_f",
			"PRODUCT", "PROD_f", "SIGNUM", "SINBLK_f", "SQRT", "SUMMATION",
			"SUM_f", "TANBLK_f", "TrigFun"}),
	
	/** MATRIX palette */
	MATRIX(XcosMessages.MATRIX_PAL, new String[] {"CUMSUM", "EXTRACT",
			"EXTTRI", "MATBKSL", "MATCATH", "MATCATV", "MATDET", "MATDIAG",
			"MATDIV", "MATEIG", "MATEXPM", "MATINV", "MATLU", "MATMAGPHI",
			"MATMUL", "MATPINV", "MATRESH", "MATSING", "MATSUM", "MATTRAN",
			"MATZCONJ", "MATZREIM", "RICC", "ROOTCOEF", "SQRT", "SUBMAT"}),
	
	/** ELECTRICAL palette */
	ELECTRICAL(XcosMessages.ELECTRICAL_PAL, new String[] {"CCS", "CVS",
			"Capacitor", "ConstantVoltage", "CurrentSensor", "Diode", "Ground",
			"Gyrator", "IdealTransformer", "Inductor", "NMOS", "NPN", "OpAmp",
			"PMOS", "PNP", "PotentialSensor", "Resistor", "SineVoltage",
			"Switch", "VVsourceAC", "VariableResistor", "VoltageSensor",
			"VsourceAC"}),
	
	/** INTEGER palette */
	INTEGER(XcosMessages.INTEGER_PAL, new String[] {"BITCLEAR", "BITSET",
			"CONVERT", "DFLIPFLOP", "DLATCH", "EXTRACTBITS", "INTMUL",
			"JKFLIPFLOP", "LOGIC", "SHIFT", "SRFLIPFLOP"}),
	
	/** PORT ACTION palette */
	PORTACTION(XcosMessages.PORTACTION_PAL, new String[] {"CLKINV_f", 
			"CLKOUTV_f", "IN_f", "INIMPL_f", "OUTIMPL_f", "OUT_f", "SUPER_f"}),
	
	/** THRESHOLD palette */
	THRESHOLD(XcosMessages.THRESHOLD_PAL, new String[] {"GENERAL_f",
			"NEGTOPOS_f", "POSTONEG_f", "ZCROSS_f"}),
	
	/** Signal Routing palette */
	SIGNALROUTING(XcosMessages.SIGNALROUTING_PAL, new String[] {"DEMUX",
			"EXTRACTOR", "FROM", "FROMMO", "GOTO", "GOTOMO",
			"GotoTagVisibility", "GotoTagVisibilityMO", "ISELECT_m",
			"MUX", "M_SWITCH", "NRMSOM_f", "RELAY_f", "SELECT_m", "SWITCH2_m",
			"SWITCH_f"}),
	
	/** SIGNAL PROCESSING palette */
	SIGNALPROCESSING(XcosMessages.SIGNALPROCESSING_PAL, new String[] {"QUANT_f",
			"SAMPHOLD_m"}),
	
	/** IMPLICIT palette */
	IMPLICIT(XcosMessages.IMPLICIT_PAL, new String[] {"CONSTRAINT_f",
			"DIFF_f"}),
	
	/** ANNOTATIONS palette */
	ANNOTATIONS(XcosMessages.ANNOTATIONS_PAL, new String[] {"TEXT_f"}),
	
	/** SINKS palette */
	SINKS(XcosMessages.SINKS_PAL, new String[] {"AFFICH_m", "CANIMXY",
			"CANIMXY3D", "CFSCOPE", "CLKOUTV_f", "CMAT3D", "CMATVIEW",
			"CMSCOPE", "CSCOPE", "CSCOPXY", "CSCOPXY3D", "ENDBLK", "END_c",
			"HALT_f", "OUTIMPL_f", "OUT_f", "TOWS_c", "TRASH_f", "WFILE_f",
			"WRITEAU_f", "WRITEC_f"}),
	
	/** SOURCES palette */
	SOURCES(XcosMessages.SOURCES_PAL, new String[] {"CLKINV_f", "CLOCK_c",
			"CONST_m", "CURV_f", "Counter", "FROMWSB", "GENSIN_f", "GENSQR_f",
			"INIMPL_f", "IN_f", "Modulo_Count", "RAMP", "RAND_m", "READAU_f",
			"READC_f", "RFILE_f", "SAWTOOTH_f", "STEP_FUNCTION", "SampleCLK",
			"Sigbuilder", "TIME_f", "TKSCALE"}),
	
	/** THERMO-HYDRAULICS palette */
	THERMOHYDRAULICS(XcosMessages.THERMOHYDRAULICS_PAL, new String[] {"Bache",
			"PerteDP", "PuitsP", "SourceP", "VanneReglante" }),
	
	/** DEMO-BLOCKS palette */
	DEMOBLOCKS(XcosMessages.DEMOBLOCKS_PAL, new String[] {"BOUNCE", "BOUNCEXY",
			"BPLATFORM"}),
	
	/** USER-DEFINED FUNCTIONS palette */
	USERDEFINEDFUNCTIONS(XcosMessages.USERDEFINEDFUNCTIONS_PAL, new String[] {
			"CBLOCK", "DEBUG_SCICOS", "EXPRESSION", "MBLOCK", "PDE", "SUPER_f",
			"c_block", "fortran_block", "generic_block3", "scifunc_block_m" });
// CSON: MultipleStringLiterals

	private final String message;
	private final String[] blocks;

	/**
	 * Default constructor
	 * @param message The associated localized message
	 * @param blocks Block contained in this  category
	 */
	private Category(String message, String[] blocks) {
		this.message = message;
		this.blocks = blocks;
	}

	/** @return The associated blocks */
	public String[] getBlockNames() {
		return blocks;
	}

	/** @return a localized description */
	@Override
	public String toString() {
		return message;
	}
}
