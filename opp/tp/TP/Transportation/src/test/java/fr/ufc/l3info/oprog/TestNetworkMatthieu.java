package fr.ufc.l3info.oprog;

import org.junit.Assert;
import org.junit.Test;

public class TestNetworkMatthieu {

    @Test
    public void testConstruct() {
        Network n = new Network();
        Assert.assertEquals(0, n.getLines().size());
    }

    @Test
    public void testAddStation() {
        Network n = new Network();
        Station s1 = new Station("S1", "L1", 1, 0.0);
        Station s2 = new Station("S2", "L1", 2, 0.0);
        n.addStation(s1);
        n.addStation(s2);
        Assert.assertEquals(s1, n.getStationByName("S1"));
        Assert.assertEquals(s2, n.getStationByName("S2"));
        Assert.assertEquals(1, n.getLines().size());
    }

    @Test
    public void testGetLines(){
        Network n = new Network();
        Station s1 = new Station("S1");
        Station s2 = new Station("S2");
        s2.addLine("L1", 1, 0.0);
        n.addStation(s1);
        Assert.assertEquals(0, n.getLines().size());
        n.addStation(s2);
        Assert.assertEquals(1, n.getLines().size());
    }

    @Test
    public void testGetStationByName(){
        Network n = new Network();
        Station s1 = new Station("S1");
        Station s2 = new Station("S2");
        s2.addLine("L1", 1, 4.2);
        n.addStation(s1);
        n.addStation(s2);

        Station new_s1 = n.getStationByName("S1");
        Station new_s2 = n.getStationByName("S2");
        Station new_s3 = n.getStationByName("S3");

        Assert.assertEquals(s1, new_s1);
        Assert.assertEquals(0, new_s1.getLines().size());
        Assert.assertEquals(s2, new_s2);
        Assert.assertEquals(4.2, new_s2.getDistanceForLine("L1"), 0.0);
        Assert.assertNull(new_s3);
    }

    @Test
    public void testGetStationByLineAndNumber(){
        Network n = new Network();
        Station s1 = new Station("S2", "L1", 1, 4.2);
        s1.addLine("L2", 2, 42.0);
        n.addStation(s1);

        Station s_L1 = n.getStationByLineAndNumber("L1", 1);
        Station s_L2 = n.getStationByLineAndNumber("L2", 2);
        Station s_L3 = n.getStationByLineAndNumber("L2", 3);
        Station s_L4 = n.getStationByLineAndNumber("L3", 1);

        Assert.assertEquals(s1, s_L1);
        Assert.assertEquals(4.2, s_L1.getDistanceForLine("L1"), 0.0);
        Assert.assertEquals(s1, s_L2);
        Assert.assertEquals(42.0, s_L2.getDistanceForLine("L2"), 0.0);
        Assert.assertNull(s_L3);
        Assert.assertNull(s_L4);
    }

    @Test
    public void testIsValid1() {
        Network n0 = new Network();
        Station s0 = new Station("S0");
        n0.addStation(s0);
        Assert.assertFalse(n0.isValid());
    }
    @Test
    public void testIsValid2() {
        Network n1 = new Network();
        Station s1 = new Station("S1", "L1", 3, 0.0);
        n1.addStation(s1);
        Assert.assertFalse(n1.isValid());
    }
    @Test
    public void testIsValid3() {
        Network n2 = new Network();
        Station s2 = new Station("S2", "L1", 1, 0.0);
        Station s3 = new Station("S3", "L2", 1, 0.0);
        n2.addStation(s2);
        n2.addStation(s3);
        Assert.assertFalse(n2.isValid());
    }
    @Test
    public void testIsValid4() {
        Network n3 = new Network();
        Station s4 = new Station("S4");
        Station s5 = new Station("S5");
        s4.addLine("L1", 1, 0.0);
        s4.addLine("L2", 2, 4.2);
        s5.addLine("L1", 2, 42);
        s5.addLine("L2", 1, 0.0);
        n3.addStation(s4);
        n3.addStation(s5);
        Assert.assertTrue(n3.isValid());
    }
    @Test
    public void testIsValid5() {
        Network n4 = new Network();
        Station s6 = new Station("S6", "L1", 1, 0.0);
        Station s7 = new Station("S7", "L1", 3, 4.2);
        n4.addStation(s6);
        n4.addStation(s7);
        Assert.assertFalse(n4.isValid());
    }
    @Test
    public void testIsValid6() {
        Network n5 = new Network();
        Station s8 = new Station("S6", "L1", 1, 0.0);
        Station s9 = new Station("S7", "L1", 2, 0.0);
        n5.addStation(s8);
        n5.addStation(s9);
        Assert.assertFalse(n5.isValid());
    }
    @Test
    public void testIsValid7() {
        Network n7 = new Network();
        Station s12 = new Station("S2", "L1", 1, 4.2);
        Station s13 = new Station("S3", "L1", 2, 42.0);
        n7.addStation(s12);
        n7.addStation(s13);
        Assert.assertFalse(n7.isValid());
    }
    @Test
    public void testIsValidNoStationNoLine() {
        Network n7 = new Network();
        Assert.assertFalse(n7.isValid());
    }
    @Test
    public void testIsValidSingleStation() {
        Network n7 = new Network();
        Station s12 = new Station("S2", "L1", 1, 0.0);
        n7.addStation(s12);
        Assert.assertTrue(n7.isValid());
    }
    @Test
    public void testIsValidNotSorted() {
        Network n7 = new Network();
        Station s12 = new Station("S2", "L1", 1, 0.0);
        Station s13 = new Station("S3", "L1", 2, 42.0);
        Station s14 = new Station("S4", "L1", 3, 4.2);
        n7.addStation(s12);
        n7.addStation(s13);
        n7.addStation(s14);
        Assert.assertFalse(n7.isValid());
    }
    @Test
    public void testIsValidLinkedLines() {
        Network n7 = new Network();
        Station s12 = new Station("S2", "L1", 1, 0.0);
        Station s13 = new Station("S3", "L1", 2, 4.2);
        s13.addLine("L2", 1, 0.0);
        Station s14 = new Station("S4", "L2", 2, 4.2);
        n7.addStation(s12);
        n7.addStation(s13);
        n7.addStation(s14);
        Assert.assertTrue(n7.isValid());
    }
    @Test
    public void testIsValidNotTwoSameStationOnSameLine() {
        Network n7 = new Network();
        Station s12 = new Station("S2", "L1", 1, 0.0);
        Station s13 = new Station("S3", "L1", 2, 4.2);
        Station s14 = new Station("S3", "L1", 3, 42.0);
        n7.addStation(s12);
        n7.addStation(s13);
        n7.addStation(s14);
        Assert.assertFalse(n7.isValid());
    }
    @Test
    public void testIsValidNotTwoSameStationOnDifferentLines() {
        Network n7 = new Network();
        Station s12 = new Station("S2", "L1", 1, 0.0);
        Station s13 = new Station("S3", "L1", 2, 4.2);
        s13.addLine("L2", 1, 0.0);
        Station s14 = new Station("S3", "L2", 3, 4.2);
        n7.addStation(s12);
        n7.addStation(s13);
        n7.addStation(s14);
        Assert.assertFalse(n7.isValid());
    }

    

    @Test
    public void testDistance(){
        // invalid network
        Network n1 = new Network();
        Station s1 = new Station("S1", "L1", 1, 0.0);
        Station s2 = new Station("S2", "L1", 3, 4.2);
        n1.addStation(s1);
        n1.addStation(s2);
        Assert.assertTrue(0 > n1.distance("S1", "S2"));

        Network n2 = new Network();
        Station s5 = new Station("S1", "L1", 1, 0.0);
        s5.addLine("L2", 5, 18.0);
        s5.addLine("L3", 5, 26.0);
        Station s6 = new Station("S2", "L1", 2, 8.0);
        s6.addLine("L3", 1,  0.0);
        s6.addLine("L4", 3,  7.0);
        Station s7 = new Station("S3", "L2", 4, 14.0);
        s7.addLine("L3", 4,  22.0);
        s7.addLine("L4", 4,  17.0);
        Station s8 = new Station("S4", "L1", 5,  27.0);
        s8.addLine("L2", 3,  6.0);
        s8.addLine("L3", 2,  5.0);
        s8.addLine("L4", 2,  2.0);
        Station s9 = new Station("S5", "L1", 3,  16.0);
        s9.addLine("L2", 2,  4.0);
        s9.addLine("L4", 1,  0.0);
        Station s10 = new Station("S6", "L1", 4,  20.0);
        s10.addLine("L2", 1,  0.0);
        s10.addLine("L3", 3,  12.0);

        n2.addStation(s5);
        n2.addStation(s6);
        n2.addStation(s7);
        n2.addStation(s8);
        n2.addStation(s9);
        n2.addStation(s10);


        Assert.assertEquals(0.0, n2.distance("S1", "S1"), 0.0);
        Assert.assertEquals(8.0, n2.distance("S1", "S2"), 0.0);
        Assert.assertEquals(4.0, n2.distance("S1", "S3"), 0.0);
        Assert.assertEquals(12.0, n2.distance("S1", "S4"), 0.0);
        Assert.assertEquals(14.0, n2.distance("S1", "S5"), 0.0);
        Assert.assertEquals(14.0, n2.distance("S1", "S6"), 0.0);

        Assert.assertEquals(8.0, n2.distance("S2", "S1"), 0.0);
        Assert.assertEquals(0.0, n2.distance("S2", "S2"), 0.0);
        Assert.assertEquals(10.0, n2.distance("S2", "S3"), 0.0);
        Assert.assertEquals(5.0, n2.distance("S2", "S4"), 0.0);
        Assert.assertEquals(7.0, n2.distance("S2", "S5"), 0.0);
        Assert.assertEquals(11.0, n2.distance("S2", "S6"), 0.0);

        Assert.assertEquals(4.0, n2.distance("S3", "S1"), 0.0);
        Assert.assertEquals(10.0, n2.distance("S3", "S2"), 0.0);
        Assert.assertEquals(0.0, n2.distance("S3", "S3"), 0.0);
        Assert.assertEquals(8.0, n2.distance("S3", "S4"), 0.0);
        Assert.assertEquals(10.0, n2.distance("S3", "S5"), 0.0);
        Assert.assertEquals(10.0, n2.distance("S3", "S6"), 0.0);

        Assert.assertEquals(12.0, n2.distance("S4", "S1"), 0.0);
        Assert.assertEquals(5.0, n2.distance("S4", "S2"), 0.0);
        Assert.assertEquals(8.0, n2.distance("S4", "S3"), 0.0);
        Assert.assertEquals(0.0, n2.distance("S4", "S4"), 0.0);
        Assert.assertEquals(2.0, n2.distance("S4", "S5"), 0.0);
        Assert.assertEquals(6.0, n2.distance("S4", "S6"), 0.0);

        Assert.assertEquals(14.0, n2.distance("S5", "S1"), 0.0);
        Assert.assertEquals(7.0, n2.distance("S5", "S2"), 0.0);
        Assert.assertEquals(10.0, n2.distance("S5", "S3"), 0.0);
        Assert.assertEquals(2.0, n2.distance("S5", "S4"), 0.0);
        Assert.assertEquals(0.0, n2.distance("S5", "S5"), 0.0);
        Assert.assertEquals(4.0, n2.distance("S5", "S6"), 0.0);

        Assert.assertEquals(14.0, n2.distance("S6", "S1"), 0.0);
        Assert.assertEquals(11.0, n2.distance("S6", "S2"), 0.0);
        Assert.assertEquals(10.0, n2.distance("S6", "S3"), 0.0);
        Assert.assertEquals(6.0, n2.distance("S6", "S4"), 0.0);
        Assert.assertEquals(4.0, n2.distance("S6", "S5"), 0.0);
        Assert.assertEquals(0.0, n2.distance("S6", "S6"), 0.0);

        Assert.assertTrue(0 > n2.distance("S1", "S7"));
        Assert.assertTrue(0 > n2.distance("S7", "S1"));
        Assert.assertTrue(0 > n2.distance(null, "S1"));
        Assert.assertTrue(0 > n2.distance("S1", null));

    }


}
