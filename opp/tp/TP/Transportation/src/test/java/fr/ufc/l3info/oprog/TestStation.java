package fr.ufc.l3info.oprog;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class TestStation {

    Station s;

    @Before
    public void initialisation() {
        s = new Station("ma Station");
    }

    @Test
    public void testNomDeLaStation() {
        Assert.assertEquals("ma Station", s.getName());
    }

    @Test
    public void testObtenirLignesSansAjout() {
        Assert.assertEquals(0, s.getLines().size());
    }

    @Test
    public void testMutantAjoutLigneAvecNumeroZero() {
        s.addLine("Ligne2", 0, 5.0);
        Assert.assertEquals(0, s.getLines().size());
    }

    @Test
    public void testMutantDistancePourLigneInexistante() {
        double distance = s.getDistanceForLine("Ligne3");
        Assert.assertEquals(-1.0, distance, 0.01);
    }

    @Test
    public void testAjoutMultipleDeLignes() {
        s.addLine("LigneA", 1, 5.0);
        s.addLine("LigneB", 2, 10.0);
        s.addLine("LigneC", 3, 15.0);
        Assert.assertEquals(3, s.getLines().size());
    }

    @Test
    public void testSuppressionDeLigne() {
        s.addLine("LigneB", 2, 10.0);
        s.removeLine("LigneB");
        Assert.assertEquals(0, s.getLines().size());
    }

    @Test
    public void testAjoutLigneAvecNomVide() {
        s.addLine("", 1, 5.0);
        Assert.assertEquals(0, s.getLines().size());
    }

    @Test
    public void testAjoutLigneAvecNumeroNegatif() {
        s.addLine("LigneD", -1, 5.0);
        Assert.assertEquals(0, s.getLines().size());
    }

    @Test
    public void testAjoutLigneAvecDistanceNegative() {
        s.addLine("LigneE", 1, -5.0);
        Assert.assertEquals(0, s.getLines().size());
    }

    @Test
    public void testAjoutLignesAvecNomsSimilaires() {
        s.addLine("LigneA", 1, 5.0);
        s.addLine("LigneA", 2, 10.0);
        Assert.assertEquals(1, s.getLines().size());
        Assert.assertEquals(2, s.getNumberForLine("LigneA"));
        Assert.assertEquals(10.0, s.getDistanceForLine("LigneA"), 0.01);
    }

    @Test
    public void testSuppressionLigneInexistante() {
        s.removeLine("LigneInexistante");
        Assert.assertEquals(0, s.getLines().size());
    }

    @Test
    public void testAjoutLigneAvecNomNull() {
        s.addLine(null, 1, 5.0);
        Assert.assertEquals(0, s.getLines().size());
    }

    @Test
    public void testRecuperationInfosAvecNomNull() {
        Assert.assertEquals(0, s.getNumberForLine(null));
        Assert.assertEquals(-1.0, s.getDistanceForLine(null), 0.01);
    }

    @Test
    public void testEgaliteDeuxStations() {
        Station s2 = new Station("ma Station");
        Assert.assertTrue(s.equals(s2));
    }

    @Test
    public void testNonEgaliteDeuxStations() {
        Station s2 = new Station("autre Station");
        Assert.assertFalse(s.equals(s2));
    }

    @Test
    public void testEgaliteStationAvecAutreObjet() {
        Object o = new Object();
        Assert.assertFalse(s.equals(o));
    }

    @Test
    public void testEgaliteStationAvecNull() {
        Assert.assertFalse(s.equals(null));
    }

    @Test
    public void testHashCodeStation() {
        int expectedHashCode = "ma Station".hashCode();
        Assert.assertEquals(expectedHashCode, s.hashCode());
    }
}
