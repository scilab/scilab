
package org.scilab.modules.xcos.simulink.patterns;

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;


/**
 * This object contains factory methods for each 
 * Java content interface and Java element interface 
 * generated in the org.scilab.modules.xcos.simulink.patterns package. 
 * <p>An ObjectFactory allows you to programatically 
 * construct new instances of the Java representation 
 * for XML content. The Java representation of XML 
 * content can consist of schema derived interfaces 
 * and classes representing the binding of schema 
 * type definitions, element declarations and model 
 * groups.  Factory methods for each of these are 
 * provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _Blocks_QNAME = new QName("", "blocks");

    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: org.scilab.modules.xcos.simulink.patterns
     * 
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link Int2StrValueMap }
     * 
     */
    public Int2StrValueMap createInt2StrValueMap() {
        return new Int2StrValueMap();
    }

    /**
     * Create an instance of {@link Str2IntParameters }
     * 
     */
    public Str2IntParameters createStr2IntParameters() {
        return new Str2IntParameters();
    }

    /**
     * Create an instance of {@link Int2StrParameters }
     * 
     */
    public Int2StrParameters createInt2StrParameters() {
        return new Int2StrParameters();
    }

    /**
     * Create an instance of {@link GeneralParameters }
     * 
     */
    public GeneralParameters createGeneralParameters() {
        return new GeneralParameters();
    }

    /**
     * Create an instance of {@link StringValueMap }
     * 
     */
    public StringValueMap createStringValueMap() {
        return new StringValueMap();
    }

    /**
     * Create an instance of {@link BlockPalette }
     * 
     */
    public BlockPalette createBlockPalette() {
        return new BlockPalette();
    }

    /**
     * Create an instance of {@link SimpleParameter }
     * 
     */
    public SimpleParameter createSimpleParameter() {
        return new SimpleParameter();
    }

    /**
     * Create an instance of {@link StringParameters }
     * 
     */
    public StringParameters createStringParameters() {
        return new StringParameters();
    }

    /**
     * Create an instance of {@link IntegerParameters }
     * 
     */
    public IntegerParameters createIntegerParameters() {
        return new IntegerParameters();
    }

    /**
     * Create an instance of {@link Str2IntValueMap }
     * 
     */
    public Str2IntValueMap createStr2IntValueMap() {
        return new Str2IntValueMap();
    }

    /**
     * Create an instance of {@link Block }
     * 
     */
    public Block createBlock() {
        return new Block();
    }

    /**
     * Create an instance of {@link IntegerValueMap }
     * 
     */
    public IntegerValueMap createIntegerValueMap() {
        return new IntegerValueMap();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link BlockPalette }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "", name = "blocks")
    public JAXBElement<BlockPalette> createBlocks(BlockPalette value) {
        return new JAXBElement<BlockPalette>(_Blocks_QNAME, BlockPalette.class, null, value);
    }

}
