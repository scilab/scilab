/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import static java.util.Arrays.asList;

import java.util.List;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BasicBlock.SimulationFunctionType;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;

import com.mxgraph.model.mxCell;

/**
 * Protected class which decode model fields of a block.
 *
 * This class is intentionally package-protected to prevent external use.
 */
// CSOFF: ClassDataAbstractionCoupling
final class BlockModelElement extends BlockPartsElement {
    /*
     * "uid" have been added on the 5.5.0 cycle. It is not checked to be compatible with older versions.
     */
    protected static final List<String> DATA_FIELD_NAMES = asList("model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout", "state", "dstate",
            "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label", "nzcross", "nmode", "equations");
    protected static final List<String> DATA_FIELD_NAMES_FULL = asList("model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout", "state", "dstate",
            "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label", "nzcross", "nmode", "equations", "uid");

    private static final int CTRL_PORT_INDEX = DATA_FIELD_NAMES.indexOf("evtin");
    private static final int CMD_PORT_INDEX = DATA_FIELD_NAMES.indexOf("evtout");
    private static final int STATE_INDEX = DATA_FIELD_NAMES.indexOf("state");
    private static final int FIRING_INDEX = DATA_FIELD_NAMES.indexOf("firing");
    private static final int DEPENDU_INDEX = DATA_FIELD_NAMES.indexOf("dep_ut");

    /** Mutable field to easily get the data through methods */
    private ScilabMList data;

    /** In-progress decoded diagram */
    private final XcosDiagram diag;

    /**
     * Default constructor
     */
    public BlockModelElement(final XcosDiagram diag) {
        this.diag = diag;
    }

    /**
     * Decode Scicos element into the block.
     *
     * This decode method doesn't coverage Port management because we need
     * graphics information to handle it.
     *
     * @param element
     *            the scicos element
     * @param into
     *            the previously instantiated block.
     * @return the modified into block.
     * @throws ScicosFormatException
     *             on error.
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType,
     *      java.lang.Object)
     */
    @Override
    public BasicBlock decode(ScilabType element, BasicBlock into) throws ScicosFormatException {

        if (into == null) {
            throw new IllegalArgumentException();
        }

        data = (ScilabMList) element;
        BasicBlock local = into;

        validate();

        local = beforeDecode(element, local);

        /*
         * fill the data
         */
        fillSimulationFunction(local);
        fillControlCommandPorts(local);
        fillFirstRawParameters(local);
        fillFiringParameters(local);
        fillSecondRawParameters(local);

        local = afterDecode(element, local);

        return local;
    }

    /**
     * Fill the simulation data into the block
     *
     * @param into
     *            the target instance
     */
    private void fillSimulationFunction(BasicBlock into) {
        String functionName = into.getSimulationFunctionName();
        SimulationFunctionType functionType = into.getSimulationFunctionType();

        if (data.get(1) instanceof ScilabString) {
            functionName = ((ScilabString) data.get(1)).getData()[0][0];
        } else if ((data.get(1) instanceof ScilabList)) {
            functionName = ((ScilabString) ((ScilabList) data.get(1)).get(0)).getData()[0][0];
            functionType = SimulationFunctionType.convertScilabValue((int) ((ScilabDouble) ((ScilabList) data.get(1)).get(1)).getRealPart()[0][0]);
        }

        into.setSimulationFunctionName(functionName);
        into.setSimulationFunctionType(functionType);
    }

    /**
     * Fill the block with the control and command ports
     *
     * @param into
     *            the target instance
     */
    private void fillControlCommandPorts(BasicBlock into) {
        ScilabDouble dataNbControlPort = (ScilabDouble) data.get(CTRL_PORT_INDEX);
        ScilabDouble dataNbCommandPort = (ScilabDouble) data.get(CMD_PORT_INDEX);

        if (dataNbControlPort.getRealPart() != null) {
            final int baseIndex = into.getChildCount();

            int nbControlPort = dataNbControlPort.getHeight();
            for (int i = 0; i < nbControlPort; i++) {
                final BasicPort port = new ControlPort();

                // do not use BasicPort#addPort() to avoid the view update
                port.setOrdering(i + 1);
                into.insert(port, baseIndex + i);
            }
        }

        if (dataNbCommandPort.getRealPart() != null) {
            final int baseIndex = into.getChildCount();

            int nbCommandPort = dataNbCommandPort.getHeight();
            for (int i = 0; i < nbCommandPort; i++) {
                final BasicPort port = new CommandPort();

                // do not use BasicPort#addPort() to avoid the view update
                port.setOrdering(i + 1);
                into.insert(port, baseIndex + i);
            }
        }
    }

    /**
     * Fill the block with the first raw parameters
     *
     * @param into
     *            the target instance
     */
    private void fillFirstRawParameters(BasicBlock into) {
        // state
        int field = STATE_INDEX;
        into.setState(data.get(field));

        // dstate
        field++;
        into.setDState(data.get(field));

        // odstate
        field++;
        into.setODState(data.get(field));

        // rpar
        field++;
        into.setRealParameters(data.get(field));

        // ipar
        field++;
        into.setIntegerParameters(data.get(field));

        // opar
        field++;
        into.setObjectsParameters(data.get(field));

        // blocktype
        field++;
        into.setBlockType(((ScilabString) data.get(field)).getData()[0][0]);
    }

    /**
     * Fill the block with the firing parameters
     *
     * @param into
     *            the target instance
     */
    private void fillFiringParameters(BasicBlock into) {
        /*
         * A boolean can be used to indicate that no initial event will be
         * emitted.
         */
        if (data.get(FIRING_INDEX) instanceof ScilabBoolean) {
            return;
        }

        /*
         * Normal behavior
         */
        final ScilabDouble firing = (ScilabDouble) data.get(FIRING_INDEX);

        if (!isEmptyField(firing)) {
            final List<CommandPort> allCommandPorts = BasicBlockInfo.getAllTypedPorts(into, false, CommandPort.class);

            final boolean isColumnDominant = firing.getHeight() >= firing.getWidth();
            final double[][] values = firing.getRealPart();
            final int[] indexes = { 0, 0 };

            for (int i = 0; i < allCommandPorts.size(); i++) {
                allCommandPorts.get(i).setInitialState(values[indexes[0]][indexes[1]]);
                incrementIndexes(indexes, isColumnDominant);
            }
        }
    }

    /**
     * Fill the block with the second raw parameters
     *
     * @param into
     *            the target instance
     * @throws WrongStructureException
     *             on wrong value
     */
    private void fillSecondRawParameters(BasicBlock into) throws WrongStructureException {
        // dep-ut
        int field = DEPENDU_INDEX;
        final boolean[][] dependsOn = ((ScilabBoolean) data.get(field)).getData();

        if (dependsOn.length == 1 && dependsOn[0].length == 2) {
            into.setDependsOnU(dependsOn[0][0]);
            into.setDependsOnT(dependsOn[0][1]);
        } else if (dependsOn.length == 2 && dependsOn[0].length == 1 && dependsOn[1].length == 1) {
            into.setDependsOnU(dependsOn[0][0]);
            into.setDependsOnT(dependsOn[1][0]);
        } else {
            throw new WrongStructureException(((ScilabString) data.get(0)).getData()[0][DEPENDU_INDEX]);
        }

        // label
        // do nothing
        field++;

        // nzcross
        field++;
        into.setNbZerosCrossing(data.get(field));

        // nmode
        field++;
        into.setNmode(data.get(field));

        // equation
        field++;
        into.setEquations(data.get(field));

        // uid
        // compatibility check, for pre-5.5.0 diagrams
        field++;
        if (field >= data.size()) {
            return;
        }
        final ScilabType uid = data.get(field);
        if (uid instanceof ScilabString) {
            final String id = ((ScilabString) uid).getData()[0][0];
            if (id != null && !id.isEmpty()) {
                into.setId(id);
            }
        }
    }

    /**
     * Validate the current data.
     *
     * This method doesn't pass the metrics because it perform many test.
     * Therefore all these tests are trivial and the conditioned action only
     * throw an exception.
     *
     * @throws ScicosFormatException
     *             when there is a validation error.
     */
    // CSOFF: CyclomaticComplexity
    // CSOFF: NPathComplexity
    // CSOFF: JavaNCSS
    // CSOFF: MethodLength
    private void validate() throws ScicosFormatException {
        if (!canDecode(data)) {
            throw new WrongElementException();
        }

        int field = 0;

        // we test if the structure as enough field
        if (data.size() < DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }

        /*
         * Checking the MList header
         */

        // Check the first field
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }
        final String[] header = ((ScilabString) data.get(field)).getData()[0];

        // Checking for the field names
        if (header.length < DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }
        for (int i = 0; i < DATA_FIELD_NAMES.size(); i++) {
            if (!header[i].equals(DATA_FIELD_NAMES.get(i))) {
                throw new WrongStructureException(DATA_FIELD_NAMES);
            }
        }

        /*
         * Checking the data
         */

        // sim : String or list(String, int)
        field++;
        if (!(data.get(field) instanceof ScilabString) && !(data.get(field) instanceof ScilabList)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // in
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // in2
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // intyp
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // out
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // out2
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // outtyp
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // evtin
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // evtout
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // state
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // dstate
        // TODO: the ScilabString value is undocumented
        field++;
        if (!(data.get(field) instanceof ScilabDouble) && !(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // odstate
        field++;
        if (!(data.get(field) instanceof ScilabDouble) && !(data.get(field) instanceof ScilabList)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // rpar
        // SuperBlocks store all "included" data in rpar field.
        field++;
        if (!(data.get(field) instanceof ScilabDouble) && !(data.get(field) instanceof ScilabMList) && !(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // ipar
        // !! WARNING !! scifunc_block_m ipar = list(...)
        field++;
        if (!(data.get(field) instanceof ScilabDouble) && !(data.get(field) instanceof ScilabList)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // opar
        field++;
        if (!(data.get(field) instanceof ScilabDouble) && !(data.get(field) instanceof ScilabList)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // blocktype
        field++;
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // firing
        field++;
        if (!(data.get(field) instanceof ScilabDouble) && !(data.get(field) instanceof ScilabBoolean)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // dep-ut
        field++;
        if (!(data.get(field) instanceof ScilabBoolean)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // label
        field++;
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // nzcross
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // nmode
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // equations
        field++;
        if (!(data.get(field) instanceof ScilabTList) && !isEmptyField(data.get(field))) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // uid not checked, introduced in Scilab 5.5.0
    }

    // CSON: CyclomaticComplexity
    // CSON: NPathComplexity
    // CSON: JavaNCSS
    // CSON: MethodLength

    /**
     * Check if the element can be decoded.
     *
     * @param element
     *            the Scicos element
     * @return true, if the Scicos types match.
     * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
     */
    @Override
    public boolean canDecode(ScilabType element) {
        data = (ScilabMList) element;

        final String type = ((ScilabString) data.get(0)).getData()[0][0];
        return type.equals(DATA_FIELD_NAMES.get(0));
    }

    /**
     * Encode the instance into the element
     *
     * @param from
     *            the source instance
     * @param element
     *            must be null.
     * @return the element parameter
     * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object,
     *      org.scilab.modules.types.ScilabType)
     */
    // CSOFF: JavaNCSS
    // CSOFF: NPathComplexity
    @Override
    public ScilabType encode(BasicBlock from, ScilabType element) {
        data = (ScilabMList) element;
        int field = 0;
        ScilabType property;

        if (data == null) {
            data = allocateElement();
        } else {
            throw new IllegalArgumentException("The element parameter must be null.");
        }

        data = (ScilabMList) beforeEncode(from, data);

        /*
         * Fill the element
         */
        field++; // sim
        data.set(field, from.getSimulationFunctionNameAndType());

        /*
         * Fields managed by specific elements : - in - in2 - intyp - out - out2
         * - outyp see InputPortElement and OutputPortElement.
         */
        field++; // in
        field++; // in2
        field++; // intyp
        field++; // out
        field++; // out2
        field++; // outtyp

        /*
         * Event ports
         */
        field++; // evtin
        final List<ControlPort> ctrlPorts = BasicBlockInfo.getAllTypedPorts(from, false, ControlPort.class);
        data.set(field, BasicBlockInfo.getAllPortsDataLines(ctrlPorts));
        field++; // evtout
        final List<CommandPort> cmdPorts = BasicBlockInfo.getAllTypedPorts(from, false, CommandPort.class);
        data.set(field, BasicBlockInfo.getAllPortsDataLines(cmdPorts));

        /*
         * State
         */
        field++; // state
        data.set(field, from.getState());
        field++; // dstate
        data.set(field, from.getDState());
        field++; // odstate
        data.set(field, from.getODState());

        /*
         * Parameters
         */
        field++; // rpar
        property = from.getRealParameters();
        if (property == null) {
            property = new ScilabDouble();
        }
        data.set(field, property);

        field++; // ipar
        property = from.getIntegerParameters();
        if (property == null) {
            property = new ScilabDouble();
        }
        data.set(field, property);

        field++; // opar
        property = from.getObjectsParameters();
        if (property == null) {
            property = new ScilabDouble();
        }
        data.set(field, property);

        field++; // blocktype
        data.set(field, new ScilabString(from.getBlockType()));

        field++; // firing
        property = from.getAllCommandPortsInitialStates();
        if (property == null) {
            property = new ScilabDouble();
        }
        data.set(field, property);

        field++; // dep_ut
        boolean[][] dependsOnUandT = { { from.isDependsOnU(), from.isDependsOnT() } };
        data.set(field, new ScilabBoolean(dependsOnUandT));

        field++; // label
        final XcosDiagram parent = from.getParentDiagram();
        if (parent != null) {
            final mxCell identifier = parent.getCellIdentifier(from);
            if (identifier != null && identifier.getValue() != null) {
                data.set(field, new ScilabString(identifier.getValue().toString()));
            }
        }

        field++; // nzcross
        property = from.getNbZerosCrossing();
        if (property == null) {
            property = new ScilabDouble();
        }
        data.set(field, property);

        field++; // nmode
        property = from.getNmode();
        if (property == null) {
            property = new ScilabDouble();
        }
        data.set(field, property);

        field++; // equations
        property = from.getEquations();
        if (property == null) {
            property = new ScilabList();
        }
        data.set(field, property);

        field++; // uid
        property = new ScilabString(from.getId());
        data.set(field, property);

        data = (ScilabMList) afterEncode(from, data);

        return data;
    }

    // CSON: NPathComplexity
    // CSON: JavaNCSS

    /**
     * Allocate a new element
     *
     * @return the new element
     */
    private ScilabMList allocateElement() {
        ScilabMList element = new ScilabMList(DATA_FIELD_NAMES_FULL.toArray(new String[0]));
        element.add(new ScilabList()); // sim
        addSizedPortVector(element, ScilabDouble.class, getInSize()); // in
        addSizedPortVector(element, ScilabDouble.class, getInSize()); // in2
        addSizedPortVector(element, ScilabDouble.class, getInSize()); // intyp
        addSizedPortVector(element, ScilabDouble.class, getOutSize()); // out
        addSizedPortVector(element, ScilabDouble.class, getOutSize()); // out2
        addSizedPortVector(element, ScilabDouble.class, getOutSize()); // outtyp
        addSizedPortVector(element, ScilabDouble.class, getEinSize()); // evtin
        addSizedPortVector(element, ScilabDouble.class, getEoutSize()); // evtout
        element.add(new ScilabDouble()); // state
        element.add(new ScilabDouble()); // dstate
        element.add(new ScilabDouble()); // ostate
        element.add(new ScilabDouble()); // rpar
        element.add(new ScilabDouble()); // ipar
        element.add(new ScilabDouble()); // opar
        element.add(new ScilabString()); // blocktype
        element.add(new ScilabDouble()); // firing
        element.add(new ScilabDouble()); // dep_ut
        element.add(new ScilabString("")); // label
        element.add(new ScilabDouble()); // nzcross
        element.add(new ScilabDouble()); // nmode
        element.add(new ScilabList()); // equations
        element.add(new ScilabString("")); // uid
        return element;
    }
}
// CSON: ClassDataAbstractionCoupling
