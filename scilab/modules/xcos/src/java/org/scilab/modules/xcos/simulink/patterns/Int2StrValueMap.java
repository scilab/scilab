
package org.scilab.modules.xcos.simulink.patterns;

import java.math.BigInteger;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for Int2StrValueMap complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="Int2StrValueMap">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="simVal" use="required" type="{http://www.w3.org/2001/XMLSchema}integer" />
 *       &lt;attribute name="xcosVal" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "Int2StrValueMap")
public class Int2StrValueMap {

    @XmlAttribute(required = true)
    protected BigInteger simVal;
    @XmlAttribute(required = true)
    protected String xcosVal;

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
     *     {@link String }
     *     
     */
    public String getXcosVal() {
        return xcosVal;
    }

    /**
     * Sets the value of the xcosVal property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setXcosVal(String value) {
        this.xcosVal = value;
    }

}
