public class Main {
    public static void main(String[] args) throws Exception {

        String xmlFileName = "input.xml";
        System.out.println("Input ==> " + xmlFileName);


        DOM dom = new DOM(xmlFileName);
        dom.parse(true);
        Medicine medicine = dom.getMedicine();

        Sorter.sortMedicationByName(medicine);

        String outputXmlFile = "output.dom.xml";
        DOM.saveToXML(medicine, outputXmlFile);
        System.out.println("Output ==> " + outputXmlFile);


        SAX sax = new SAX(xmlFileName);
        sax.parse(true);
        medicine = sax.getMedicine();

        Sorter.sortMedicationByAnalogs(medicine);

        outputXmlFile = "output.sax.xml";

        DOM.saveToXML(medicine, outputXmlFile);
        System.out.println("Output ==> " + outputXmlFile);


        STAX stax = new STAX(xmlFileName);
        stax.parse();
        medicine = stax.getMedicine();

        Sorter.sortMedicationByVersions(medicine);

        outputXmlFile = "output.stax.xml";
        DOM.saveToXML(medicine, outputXmlFile);
        System.out.println("Output ==> " + outputXmlFile);

        System.out.println("====================================");
        System.out.println("~~~~~~~~~~DOM.start~~~~~~~~");
        DOM.start();
        System.out.println("~~~~~~~~~~SAX.start~~~~~~~~");
        SAX.start();
        System.out.println("~~~~~~~~~~STAX.start~~~~~~~");
        STAX.start();
        System.out.println("~~~~~~~~~~~~~Sorter.start~~~~~~~~~~~~");
        Sorter.main(args);
    }

}