/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.xcos.io.scicos;

import java.rmi.server.UID;
import static java.util.Arrays.asList;

import java.util.List;

import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.ScilabTypeCoder;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;

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
    protected static final List<String> DATA_FIELD_NAMES = asList("model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout", "state",
            "dstate", "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label", "nzcross", "nmode", "equations");
    protected static final List<String> DATA_FIELD_NAMES_FULL = asList("model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout",
            "state", "dstate", "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label", "nzcross", "nmode", "equations", "uid");

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
    public BlockModelElement(final JavaController controller, final XcosDiagram diag) {
        super(controller);

        this.diag = diag;
    }

    /**
     * Decode Scicos element into the block.
     *
     * This decode method doesn't coverage Port management because we need graphics information to handle it.
     *
     * @param element
     *            the scicos element
     * @param into
     *            the previously instantiated block.
     * @return the modified into block.
     * @throws ScicosFormatException
     *             on error.
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType, java.lang.Object)
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
        String[] functionName = new String[1];
        controller.getObjectProperty(into.getUID(), into.getKind(), ObjectProperties.SIM_FUNCTION_NAME, functionName);

        int[] functionType = new int[1];
        controller.getObjectProperty(into.getUID(), into.getKind(), ObjectProperties.SIM_FUNCTION_API, functionType);

        if (data.get(1) instanceof ScilabString) {
            functionName[0] = ((ScilabString) data.get(1)).getData()[0][0];
        } else if ((data.get(1) instanceof ScilabList)) {
            functionName[0] = ((ScilabString) ((ScilabList) data.get(1)).get(0)).getData()[0][0];
            functionType[0] = (int) ((ScilabDouble) ((ScilabList) data.get(1)).get(1)).getRealPart()[0][0];
        }

        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.SIM_FUNCTION_NAME, functionName[0]);
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.SIM_FUNCTION_API, functionType[0]);
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
                final BasicPort port = new ControlPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());

                // do not use BasicPort#addPort() to avoid the view update
                into.insert(port, baseIndex + i);
            }
        }

        if (dataNbCommandPort.getRealPart() != null) {
            final int baseIndex = into.getChildCount();

            int nbCommandPort = dataNbCommandPort.getHeight();
            for (int i = 0; i < nbCommandPort; i++) {
                final BasicPort port = new CommandPort(controller, controller.createObject(Kind.PORT), Kind.PORT, null, null, new UID().toString());

                // do not use BasicPort#addPort() to avoid the view update
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
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.STATE, toVectorOfDouble((ScilabDouble) data.get(field)));

        // dstate
        field++;
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.DSTATE, toVectorOfDouble((ScilabDouble) data.get(field)));

        // odstate
        field++;
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.ODSTATE, new ScilabTypeCoder().var2vec(data.get(field)));

        // rpar
        field++;
        if (data.get(field) instanceof ScilabMList) {
            try {
                new DiagramElement(controller).decode((ScilabMList) data.get(field), new XcosDiagram(controller, into.getUID(), into.getKind(), into.getId()));
            } catch (ScicosFormatException e) {}
        } else if (data.get(field) instanceof ScilabDouble ) {
            controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.RPAR, toVectorOfDouble((ScilabDouble) data.get(field)));
        }

        // ipar
        field++;
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.IPAR, toVectorOfInt((ScilabDouble) data.get(field)));

        // opar
        field++;
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.OPAR, new ScilabTypeCoder().var2vec(data.get(field)));

        // blocktype
        field++;
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.SIM_BLOCKTYPE, ((ScilabString) data.get(field)).getData()[0][0]);
    }

    /**
     * Fill the block with the firing parameters
     *
     * @param into
     *            the target instance
     */
    private void fillFiringParameters(BasicBlock into) {
        /*
         * A boolean can be used to indicate that no initial event will be emitted.
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
                CommandPort port = allCommandPorts.get(i);
                controller.setObjectProperty(port.getUID(), port.getKind(), ObjectProperties.FIRING, values[indexes[0]][indexes[1]]);

                incrementIndexes(indexes, isColumnDominant);
            }
        }
    }

    private VectorOfDouble toVectorOfDouble(ScilabDouble value) {
        VectorOfDouble ret = new VectorOfDouble(value.getHeight());
        for (int i = 0; i < value.getWidth(); i++) {
            ret.set(i, value.getRealElement(i, 0));
        }
        return ret;
    }

    private VectorOfInt toVectorOfInt(ScilabDouble value) {
        VectorOfInt ret = new VectorOfInt(value.getHeight());
        for (int i = 0; i < value.getWidth(); i++) {
            ret.set(i, (int) value.getRealElement(i, 0));
        }
        return ret;
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
        VectorOfInt v = new VectorOfInt(2);

        if (dependsOn.length == 1 && dependsOn[0].length == 2) {
            v.set(0, dependsOn[0][0] ? 1 : 0);
            v.set(1, dependsOn[0][1] ? 1 : 0);
        } else if (dependsOn.length == 2 && dependsOn[0].length == 1 && dependsOn[1].length == 1) {
            v.set(0, dependsOn[0][0] ? 1 : 0);
            v.set(1, dependsOn[1][0] ? 1 : 0);
        } else {
            throw new WrongStructureException(((ScilabString) data.get(0)).getData()[0][DEPENDU_INDEX]);
        }
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.SIM_DEP_UT, v);

        // label
        // do nothing
        field++;

        // nzcross
        field++;
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.NZCROSS, toVectorOfInt((ScilabDouble) data.get(field)));

        // nmode
        field++;
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.NMODE, toVectorOfInt((ScilabDouble) data.get(field)));

        // equation
        field++;
        controller.setObjectProperty(into.getUID(), into.getKind(), ObjectProperties.EQUATIONS, new ScilabTypeCoder().var2vec(data.get(field)));

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
     * This method doesn't pass the metrics because it perform many test. Therefore all these tests are trivial and the conditioned action only throw an exception.
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
}
// CSON: ClassDataAbstractionCoupling
