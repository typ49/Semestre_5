package fr.ufc.l3info.oprog;

import org.junit.*;

public class TestNetwork {
    Network n;
    Network n2;
    Station s;
    Station s2;
    Station noLine;

    @Before
    public void setup() {
        // Creating an object to test
        n = new Network();
        n2 = new Network();
        s = new Station("my Station");
        s2 = new Station("my Station2");
        noLine = new Station("no Line");
        s.addLine("Line1", 1, 10.0);
        s.addLine("Line2", 2, 20.0);
        s2.addLine("Line3", 3, 20.0);
        s2.addLine("Line2", 2, 20.0);
    }

    @Test
    public void testAddStation() {
        // Adding the station to the network
        n.addStation(s);

        // Check if the station was added successfully
        Assert.assertEquals(1, n.getStations().size());
        Assert.assertEquals(s, n.getStationByName("my Station"));
        n.addStation(s);
        Assert.assertEquals(1, n.getStations().size());
    }

    @Test
    public void testGetLines() {
        // Adding the station to the network
        n.addStation(s);
        Assert.assertEquals(2, n.getLines().size());

        n.addStation(s2);
        Assert.assertEquals(3, n.getLines().size());
    }

    @Test
    public void testGetStationByName() {
        n.addStation(s);

        Assert.assertEquals(s, n.getStationByName("my Station"));
        Assert.assertNull(n.getStationByName("my Station2"));
    }

    @Test
    public void testGetStationByLineAndNumber() {
        n.addStation(s);

        Assert.assertEquals(s, n.getStationByLineAndNumber("Line1", 1));
        Assert.assertEquals(s, n.getStationByLineAndNumber("Line2", 2));
        Assert.assertNull(n.getStationByLineAndNumber("Line1", 2));
        Assert.assertNull(n.getStationByLineAndNumber("Line2", 1));
        Assert.assertNull(n.getStationByLineAndNumber("Line3", 1));
    }

    /*
          Determines if the network is valid.
          A network is considered valid if all the following conditions are met:
               there is at least one line,
               each line is correctly structured
                   (each line contains at least one station,
                   the first station has number 1 and is at kilometer 0,
                   the numbering of the stations on the line does not have duplicate numbers
                   or missing numbers, and the kilometre positions are strictly increasing
                   throughout the line).
               Moreover, every station must be accessible from any other station
                   (in other words: there are no isolated lines on the network).
    */
    @Test
    public void testIsValidWithValidNetwork() {
        s.removeLine("Line1");
        s.addLine("Line4", 1, 0.0);
        n.addStation(s);
        n.addStation(s2);
        Assert.assertTrue(n.isValid());
    }

    @Test
    public void testIsValidWithEmptyNetwork() {
        Assert.assertFalse(n2.isValid());
    }

    @Test
    public void testIsValidWithStationWithoutLine() {
        n2.addStation(noLine);
        Assert.assertFalse(n2.isValid());
    }

    @Test
    public void testIsValidWithoutStation1() {
        n.addStation(s2);
        Assert.assertFalse(n.isValid());
    }

    @Test
    public void testIsValidWithoutStationAtKilometer0() {
        s.removeLine("Line4");
        s.addLine("Line1", 1, 10.0);
        n2.addStation(s);
        Assert.assertFalse(n2.isValid());
    }

    @Test
    public void testIsValidWithDuplicateOrMissingLine() {
        s.addLine("LineBis", 2, 20.0);
        n2.addStation(s);
        Assert.assertFalse(n2.isValid());

        n2.removeStation(s);
        s.removeLine("LineBis");
        s.addLine("Line10", 10, 100.0);
        n2.addStation(s);
        Assert.assertFalse(n2.isValid());
    }

    @Test
    public void testIsValidWithIsolatedStation() {
        // Setup: Creating two stations, each on a different line.
        // These stations are not connected to each other, so they are isolated.

        // Station 1 on "Line1"
        Station isolatedStation1 = new Station("Isolated Station 1");
        isolatedStation1.addLine("Line1", 1, 0.0);
        n.addStation(isolatedStation1);

        // Station 2 on "Line2"
        Station isolatedStation2 = new Station("Isolated Station 2");
        isolatedStation2.addLine("Line2", 1, 0.0);
        n.addStation(isolatedStation2);

        // Verification: The network should be invalid because it contains isolated stations.
        Assert.assertFalse(n.isValid());
    }

    @Test
    public void testDistanceSameStation() {
        n.addStation(s);
        Assert.assertEquals(0.0, n.distance("my Station", "my Station"), 0.01);
    }

    @Test
    public void testDistanceDifferentStationsSameLine() {
        n.addStation(s);
        Station s3 = new Station("my Station3");
        s3.addLine("Line1", 2, 15.0);
        n.addStation(s3);
        Assert.assertEquals(5.0, n.distance("my Station", "my Station3"), 0.01);
    }

    // Note: This test assumes that the functionality to calculate distance between stations on different lines is implemented.
    @Test
    public void testDistanceDifferentStationsDifferentLines() {
        n.addStation(s);
        n.addStation(s2);
        // Assuming some logic is implemented to calculate the shortest path between stations on different lines.
        double expectedDistance = 10.0; // Replace with expected distance value.
        Assert.assertEquals(expectedDistance, n.distance("my Station", "my Station2"), 0.01);
    }

    @Test
    public void testDistanceWithNonExistentStation() {
        n.addStation(s);
        Assert.assertEquals(-1.0, n.distance("my Station", "Non Existent Station"), 0.01);
    }

    @Test
    public void testDistanceOnInvalidNetwork() {
        n.addStation(s2); // Adding only s2 makes the network invalid as per previous tests.
        Assert.assertEquals(-1.0, n.distance("my Station", "my Station2"), 0.01);
    }
}
