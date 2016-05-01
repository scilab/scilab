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

import static java.util.Arrays.asList;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.LinkPortMap;
import org.scilab.modules.xcos.port.BasicPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;
import java.rmi.server.UID;

/**
 * Perform a link transformation between Scicos and Xcos.
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: FanOutComplexity
public final class LinkElement extends AbstractElement<BasicLink> {
    protected static final List<String> DATA_FIELD_NAMES = asList("Link", "xx", "yy", "id", "thick", "ct", "from", "to");

    private static final int XX_INDEX = DATA_FIELD_NAMES.indexOf("xx");
    private static final int YY_INDEX = DATA_FIELD_NAMES.indexOf("yy");
    private static final int CT_INDEX = DATA_FIELD_NAMES.indexOf("ct");
    private static final int FROM_INDEX = DATA_FIELD_NAMES.indexOf("from");
    private static final int TO_INDEX = DATA_FIELD_NAMES.indexOf("to");

    private static final Logger LOG = Logger.getLogger(LinkElement.class.getName());

    /** Mutable field to easily get the data through methods */
    private ScilabMList data;
    private BasicPort start;
    private BasicPort end;

    /** The already decoded block map */
    private final Map<Integer, BasicBlock> blocks;

    /**
     * Default constructor
     *
     * @param blocks
     *            the already decoded blocks
     */
    public LinkElement(final JavaController controller, Map<Integer, BasicBlock> blocks) {
        super(controller);

        this.blocks = blocks;
    }

    /**
     * Decode the element into the link.
     *
     * @param element
     *            The current Scilab data
     * @param into
     *            the target, if null a new instance is allocated and returned.
     * @return the decoded block.
     * @throws ScicosFormatException
     *             when e decoding error occurred.
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType, java.lang.Object)
     */
    @Override
    public BasicLink decode(ScilabType element, BasicLink into) throws ScicosFormatException {
        data = (ScilabMList) element;
        BasicLink link = into;

        validate();

        if (into == null) {
            link = allocateLink();
        }

        link = beforeDecode(element, link);

        searchForPorts(link);
        List<mxPoint> points = getPoints();

        /*
         * Fill the data
         */
        link.setTerminal(start, true);
        link.setTerminal(end, false);

        mxGeometry geom = link.getGeometry();
        if (geom == null) {
            geom = new mxGeometry();
        }
        geom.setPoints(points);
        link.setGeometry(geom);

        link = afterDecode(element, link);

        return link;
    }

    /**
     * @return a new typed link
     */
    private BasicLink allocateLink() {
        BasicLink link = null;
        final int type = (int) ((ScilabDouble) data.get(CT_INDEX)).getRealPart()[0][1];

        String id = new UID().toString();

        try {
            Class<? extends BasicLink> klass = LinkPortMap.getLinkClass(type);
            Constructor<? extends BasicLink> cstr = klass.getConstructor(JavaController.class, Long.TYPE, Kind.class, Object.class, mxGeometry.class, String.class, String.class);
            link = cstr.newInstance(controller, controller.createObject(Kind.LINK), Kind.LINK, null, null, null, id);
        } catch (ReflectiveOperationException e) {
            LOG.severe(e.toString());
        }

        return link;
    }

    /**
     * @return the link points
     */
    private List<mxPoint> getPoints() {
        List<mxPoint> points = new ArrayList<mxPoint>();

        final double[][] xAxis = ((ScilabDouble) data.get(XX_INDEX)).getRealPart();
        final double[][] yAxis = ((ScilabDouble) data.get(YY_INDEX)).getRealPart();
        final boolean isColumnDominant = data.get(XX_INDEX).getHeight() >= data.get(XX_INDEX).getWidth();

        /*
         * Pre-condition (start and end should have been set)
         */
        if (start == null || end == null) {
            return points;
        }

        /*
         * The first, last and common indexes.
         */
        final int min = 1;
        final int max;
        final int[] indexes = { 0, 0 };
        if (isColumnDominant) {
            max = data.get(1).getHeight() - 1;
            indexes[0] = min;
        } else {
            max = data.get(1).getWidth() - 1;
            indexes[1] = min;
        }

        /*
         * Loop all over the points
         */
        for (int i = min; i < max; i++) {
            double x = xAxis[indexes[0]][indexes[1]];
            double y = yAxis[indexes[0]][indexes[1]];

            // invert the y-axis value
            y = -y;

            points.add(new mxPoint(x, y));

            incrementIndexes(indexes, isColumnDominant);
        }

        return points;
    }

    /**
     * Fill the {@link #start} and {@link #end} parameters.
     *
     * @param link
     *            the link instance
     */
    // CSOFF: JavaNCSS
    private void searchForPorts(final BasicLink link) {

        /*
         * Retrieve data from encoded instance
         */
        final ScilabDouble from = (ScilabDouble) data.get(FROM_INDEX);
        final ScilabDouble to = (ScilabDouble) data.get(TO_INDEX);

        final double[][] fromReal = from.getRealPart();
        final double[][] toReal = to.getRealPart();

        final int[] indexes = { 0, 0 };
        final boolean isColumnDominant = from.getHeight() >= from.getWidth();

        final int startBlockIndex = (int) fromReal[indexes[0]][indexes[1]];
        final int endBlockIndex = (int) toReal[indexes[0]][indexes[1]];

        final BasicBlock startBlock = blocks.get(startBlockIndex - 1);
        final BasicBlock endBlock = blocks.get(endBlockIndex - 1);

        incrementIndexes(indexes, isColumnDominant);

        final int startPortIndex = (int) fromReal[indexes[0]][indexes[1]];
        if (startPortIndex == 0) {
            LOG.severe("Link has an invalid start port");
            LOG.severe(data.toString());
            return;
        }
        final int endPortIndex = (int) toReal[indexes[0]][indexes[1]];
        if (endPortIndex == 0) {
            LOG.severe("Link has an invalid end port");
            LOG.severe(data.toString());
            return;
        }

        incrementIndexes(indexes, isColumnDominant);

        final boolean startPortIsStart;
        if (canGet(from, indexes)) {
            startPortIsStart = fromReal[indexes[0]][indexes[1]] == 0.0;
        } else {
            // fallback start value
            startPortIsStart = true;
        }

        final boolean endPortIsStart;
        if (canGet(to, indexes)) {
            endPortIsStart = toReal[indexes[0]][indexes[1]] == 0.0;
        } else {
            // fallback end value
            endPortIsStart = false;
        }

        final List<BasicPort> startPorts = BasicBlockInfo.getAllPortsAtPosition(startBlock, startPortIndex);
        final List<BasicPort> endPorts = BasicBlockInfo.getAllPortsAtPosition(endBlock, endPortIndex);

        /*
         * Get the ordered ports, this is the normal case.
         */
        lookForOrderedPorts(link, startPorts, startPortIsStart, endPorts, endPortIsStart);

        /*
         * Fallback to handle an inverted link.
         */
        if (start == null || end == null) {
            // implicit links can be inverted but this is exceptional so trace
            // them
            if (LOG.isLoggable(Level.FINEST)) {
                final BasicBlock block;
                final int index;
                if (start == null) {
                    block = startBlock;
                    index = startPortIndex;
                } else {
                    block = endBlock;
                    index = endPortIndex;
                }

                if (block != null) {
                    String[] v = new String[1];
                    controller.getObjectProperty(block.getUID(), block.getKind(), ObjectProperties.SIM_FUNCTION_NAME, v);
                    LOG.warning("Unable to get " + v[0] + "[" + index + "]" + block.toString());
                } else {
                    return;
                }
            }

            /*
             * Look for inverted ports in all cases (check inverted first).
             */
            lookForOrderedPorts(link, startPorts, !startPortIsStart, endPorts, !endPortIsStart);
            if (start != null && end != null) {
                return;
            }
            lookForOrderedPorts(link, startPorts, startPortIsStart, endPorts, !endPortIsStart);
            if (start != null && end != null) {
                return;
            }
            lookForOrderedPorts(link, startPorts, !startPortIsStart, endPorts, endPortIsStart);
            if (start != null && end != null) {
                return;
            }
        }
    }

    /**
     * Look for the ordered ports.
     *
     * This method assume that the startPorts and endPorts are sorted according to {@link BasicBlock#sortChildren()} and have the right ordering.
     *
     * @param link
     *            the link to connect
     * @param startPorts
     *            the possible starts
     * @param startIsStart
     *            boolean to indicate the link direction (from start ? )
     * @param endPorts
     *            the possible ends
     * @param endIsStart
     *            boolean to indicate the link direction (from end ? )
     */
    private void lookForOrderedPorts(final BasicLink link, final List<BasicPort> startPorts, final boolean startIsStart, final List<BasicPort> endPorts,
                                     final boolean endIsStart) {
        Class<? extends BasicPort> startKlass = LinkPortMap.getPortClass(link.getClass(), startIsStart);
        Class<? extends BasicPort> endKlass = LinkPortMap.getPortClass(link.getClass(), endIsStart);

        /*
         * Clear the state
         */
        start = null;
        end = null;

        /*
         * Iterate over start
         */
        for (BasicPort p : startPorts) {
            if (startKlass.isInstance(p)) {
                // only link the same port once, fallback should be provided by the caller method
                if (p.getEdgeCount() == 0) {
                    start = p;
                }
                break;
            }
        }

        /*
         * Iterate over end
         */
        for (BasicPort p : endPorts) {
            if (endKlass.isInstance(p)) {
                // only link the same port once, fallback should be provided by the caller method
                if (p.getEdgeCount() == 0) {
                    end = p;
                }
                break;
            }
        }
    }

    // CSON: JavaNCSS

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
    private void validate() throws ScicosFormatException {
        if (!canDecode(data)) {
            throw new WrongElementException();
        }

        int field = 0;

        // we test if the structure as enough field
        if (data.size() != DATA_FIELD_NAMES.size()) {
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
        if (header.length != DATA_FIELD_NAMES.size()) {
            throw new WrongStructureException(DATA_FIELD_NAMES);
        }
        for (int i = 0; i < header.length; i++) {
            if (!header[i].equals(DATA_FIELD_NAMES.get(i))) {
                throw new WrongStructureException(DATA_FIELD_NAMES);
            }
        }

        // xx
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // yy
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // id
        field++;
        if (!(data.get(field) instanceof ScilabString)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // thick
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // ct
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // from
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }

        // to
        field++;
        if (!(data.get(field) instanceof ScilabDouble)) {
            throw new WrongTypeException(DATA_FIELD_NAMES, field);
        }
    }

    // CSON: CyclomaticComplexity
    // CSON: NPathComplexity

    /**
     * Test if the current implementation can be used to decode the element.
     *
     * @param element
     *            the element to test
     * @return true, when the current implementation is the right one
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
// CSON: FanOutComplexity
