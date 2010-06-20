
package org.scilab.modules.xcos.simulink.patterns;

import java.math.BigInteger;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for IntegerValueMap complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="IntegerValueMap">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="simVal" use="required" type="{http://www.w3.org/2001/XMLSchema}integer" />
 *       &lt;attribute name="xcosVal" use="required" type="{http://www.w3.org/2001/XMLSchema}integer" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "IntegerValueMap")
public class IntegerValueMap {

    @XmlAttribute(required = true)
    protected BigInteger simVal;
    @XmlAttribute(required = true)
    protected BigInteger xcosVal;

    /**
     * Gets the value of the simVal property.
     * 
     * @return
     *     possible object is
     *     {@link BigInteger }
     *     
     */
    public BigInteger getSimVal() {
        return simVal;
    }

    /**
     * Sets the value of the simVal property.
     * 
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *     
     */
    public void setSimVal(BigInteger value) {
        this.simVal = value;
    }

    /**
     * Gets the value of the xcosVal property.
     * 
     * @return
     *     possible object is
     *     {@link BigInteger }
     *     
     */
    public BigInteger getXcosVal() {
        return xcosVal;
    }

    /**
     * Sets the value of the xcosVal property.
     * 
     * @param value
     *     allowed object is
     *     {@link BigInteger }
     *     
     */
    public void setXcosVal(BigInteger value) {
        this.xcosVal = value;
    }

}
