package fr.ufc.l3info.oprog;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class TestStation {

    Station s;

    @Before
    public void setup() {
        // Création d'un objet à tester
        s = new Station("ma Station");
    }

    @Test
    public void testName() {
        // Vérifie si la méthode getName renvoie le nom de la station attendu
        Assert.assertEquals("ma Station", s.getName());
    }

    @Test
    public void TestGetLines() {
        // Vérifie si getLines renvoie un ensemble vide lorsque aucune ligne n'a été ajoutée
        Assert.assertEquals(0, s.getLines().size());
    }

    @Test
    public void testAddLine() {
        // Ajoute une ligne à la station
        s.addLine("Ligne1", 1, 10.0);

        // Vérifie si la ligne a été ajoutée avec succès
        Assert.assertEquals(1, s.getLines().size());
        Assert.assertEquals(1, s.getNumberForLine("Ligne1"));
        Assert.assertEquals(10.0, s.getDistanceForLine("Ligne1"), 0.01);
    }

    @Test
    public void testRemoveLine() {
        // Ajoute une ligne à la station
        s.addLine("Ligne1", 1, 10.0);

        // Supprime la ligne
        s.removeLine("Ligne1");

        // Vérifie si la ligne a été supprimée avec succès
        Assert.assertEquals(0, s.getLines().size());
        Assert.assertEquals(0, s.getNumberForLine("Ligne1"));
        Assert.assertEquals(-1.0, s.getDistanceForLine("Ligne1"), 0.01);
    }
}
