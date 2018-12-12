public enum XML {

    MEDICINE("Medicine"), MEDICATION("Medication"), NAME("Name"), PHARM("PHARM"), GROUP("Group"),
    ANALOGS("Analogs"), VERSIONS("Versions"), CERTIFICATE("Certificate"),
    PACKAGE("Package"), DOSAGE("Dosage");

    private String value;

    XML(String value) {
        this.value = value;
    }

    public boolean equalsTo(String name) {
        return value.equals(name);
    }

    public String value() {
        return value;
    }
}
