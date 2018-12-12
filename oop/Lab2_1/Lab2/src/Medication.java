public class Medication {

    private String name;
    private String pharm;
    private String group;
    private String analogs;
    private String versions;
    private String certificate;
    private String packagemy;
    private String dosage;

    public void setName(String name) { this.name = name;}
    public String getName() { return this.name;}

    public void setPharm(String pharm) { this.pharm = pharm;}
    public String getPharm() { return this.pharm;}

    public void setGroup(String group) { this.group = group;}
    public String getGroup() { return this.group;}

    public void setAnalogs(String analogs) {this.analogs = analogs;}
    public String getAnalogs() { return this.analogs;}

    public void setVersions(String versions) { this.versions = versions;}
    public String getVersions() { return this.versions;}

    public void setCertificate(String certificate) {
        this.certificate = certificate;
    }
    public String getCertificate() { return this.certificate;}

    public void setPackagemy(String prof) { this.packagemy = prof;}
    public String getPackagemy() { return this.packagemy;}

    public void setDosage(String duration) {
        this.dosage = duration;
    }
    public String getDosage() { return this.dosage;}

    public String toString() {
        StringBuilder sb = new StringBuilder("");
        sb.append("Name: ").append(this.name).append('\n');
        sb.append("Pharm: ").append(pharm).append('\n');
        sb.append("Group: ").append(group).append('\n');
        sb.append("Analogs: ").append(analogs).append('\n');
        sb.append("Versions: ").append(versions).append('\n');
        sb.append("Certificate: ").append(certificate).append('\n');
        sb.append("Package: ").append(packagemy).append('\n');
        sb.append("Dosage: ").append(dosage);
        return sb.toString();
    }
}