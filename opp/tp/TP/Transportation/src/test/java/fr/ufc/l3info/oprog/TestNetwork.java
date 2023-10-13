package fr.ufc.l3info.oprog;

import org.junit.*;

public class TestNetwork {
    Network n;
    Network n2;
    Station s;

    Station s1;
    Station s2;
    Station s3;
    Station noLine;

    @Before
    public void setup() {
        // Creating objects to test
        n = new Network();
        n2 = new Network();
        s = new Station("my Station");
        s1 = new Station("my Station1");
        s2 = new Station("my Station2");
        s3 = new Station("my Station3");
        noLine = new Station("no Line");
        s.addLine("Line1", 1, 10.0);
        s.addLine("Line2", 2, 20.0);
        s2.addLine("Line3", 3, 20.0);
        s2.addLine("Line2", 2, 10.0);
    }

    @Test
    public void testAddStation() {
        // Adding the station to the network
        n.addStation(s);

        // Check if the station was added successfully
        Assert.assertEquals(s, n.getStationByName("my Station"));
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





    @Test
    public void testInvalidNetworkWithMissingNumber() {
        s1.addLine("L1", 1, 0);
        s2.addLine("L1", 3, 5);

        n.addStation(s1);
        n.addStation(s2);

        Assert.assertFalse(n.isValid());
    }

    @Test
    public void testInvalidNetworkWithDuplicateNumber() {
        s1.addLine("L1", 1, 0);
        s2.addLine("L1", 1, 5);

        n.addStation(s1);
        n.addStation(s2);

        Assert.assertFalse(n.isValid());
    }

    @Test
    public void testInvalidNetworkWithDecreasingKm() {
        s1.addLine("L1", 1, 5);
        s2.addLine("L1", 2, 0);

        n.addStation(s1);
        n.addStation(s2);

        Assert.assertFalse(n.isValid());
    }

    @Test
    public void testInvalidNetworkIsolatedLine() {
        s1.addLine("L1", 1, 0);
        s2.addLine("L1", 2, 5);
        s.addLine("L2", 1, 0);
        s3.addLine("L2", 2, 5);

        n.addStation(s1);
        n.addStation(s2);
        n.addStation(s);
        n.addStation(s3);

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

        @Test
        public void testDistanceDifferentStationsDifferentLines() {
            n.addStation(s);
            n.addStation(s2);
            double expectedDistance = 10.0;
            Assert.assertEquals(expectedDistance, n.distance("my Station", "my Station2"), 0.01);
        }

        @Test
        public void testDistanceWithNonExistentStation() {
            n.addStation(s);
            Assert.assertEquals(-1.0, n.distance("my Station", "Non Existent Station"), 0.01);
        }

        @Test
        public void testDistanceOnInvalidNetwork() {
            n.addStation(s2);
            Assert.assertEquals(-1.0, n.distance("my Station", "my Station2"), 0.01);
        }

    @Test
    public void testAddDuplicateStation() {
        n.addStation(s);
        n.addStation(s);
        // Since there's no direct method to get the size of stations, we'll use the indirect method of checking the size of lines.
        Assert.assertEquals(2, n.getLines().size());
    }

    // Removed testStationWithNameExists() as it uses the non-existent getStations() method.

    @Test
    public void testGetLinesWithoutStations() {
        Assert.assertTrue(n.getLines().isEmpty());
    }

    @Test
    public void testGetStationByNonexistentName() {
        n.addStation(s);
        Assert.assertNull(n.getStationByName("Nonexistent Station"));
    }

    @Test
    public void testGetStationByNonexistentLineAndNumber() {
        n.addStation(s);
        Assert.assertNull(n.getStationByLineAndNumber("Nonexistent Line", 1));
    }

    @Test
    public void testDistanceForStationWithNoLine() {
        n.addStation(noLine);
        n.addStation(s);
        Assert.assertEquals(-1.0, n.distance("no Line", "my Station"), 0.01);
    }

    @Test
    public void testDistanceForTwoStationsWithNoLines() {
        n.addStation(noLine);
        Station anotherNoLineStation = new Station("Another No Line");
        n.addStation(anotherNoLineStation);
        Assert.assertEquals(-1.0, n.distance("no Line", "Another No Line"), 0.01);
    }

    @Test
    public void testDistanceForStationsWithDiscontinuousKilometers() {
        Station s3 = new Station("Discontinuous Station");
        s3.addLine("Line1", 10, 100.0); // Adding a big jump in kilometers
        n.addStation(s);
        n.addStation(s3);
        Assert.assertEquals(90.0, n.distance("my Station", "Discontinuous Station"), 0.01);
    }

    @Test
    public void testIsValidWithOnlyOneLineStation() {
        s.removeLine("Line2");
        n.addStation(s);
        Assert.assertFalse(n.isValid()); // The network is invalid since there's only one station on the only line.
    }

    @Test
    public void testIsValidWithStationsHavingSameNumbersOnSameLine() {
        s2.addLine("Line1", 1, 30.0); // Another station with number 1 on Line1
        n.addStation(s);
        n.addStation(s2);
        Assert.assertFalse(n.isValid());
    }

    @Test
    public void testIsValidWithStationsHavingDecreasingKilometers() {
        s2.addLine("Line1", 2, 5.0); // Station 2 comes before Station 1 in terms of kilometers on Line1
        n.addStation(s);
        n.addStation(s2);
        Assert.assertFalse(n.isValid());
    }

    // Mutant test for addStation method
    @Test
    public void testAddStationMutant() {
        n.addStation(null);
        Assert.assertNull(n.getStationByName("Station1"));
    }

    // Mutant test for getLines method
    @Test
    public void testGetLinesMutant() {
        n.addStation(s1);
        Assert.assertTrue(n.getLines().isEmpty());
    }

    // Mutant test for isValid method
    @Test
    public void testIsValidMutant() {
        s1.addLine("Line1", 1, 5.0);
        s2.addLine("Line1", 2, 5.0);
        n.addStation(s1);
        n.addStation(s2);
        Assert.assertFalse(n.isValid());
    }

    // Mutant test for distance method
    @Test
    public void testDistanceMutant() {
        n.addStation(s1);
        n.addStation(s2);
        Assert.assertNotEquals(5.0, n.distance("Station1", "Station2"), 0.01);
    }

    // Scenario for an invalid network
    @Test
    public void testInvalidNetworkScenario() {
        s2.addLine("Line1", 1, 5.0);
        n.addStation(s1);
        n.addStation(s2);
        Assert.assertFalse(n.isValid());
    }

    // Scenario for a complex network
    @Test
    public void testComplexNetworkScenario() {
        s1.addLine("Line2", 2, 5.0);
        n.addStation(s1);
        n.addStation(s2);
        n.addStation(s3);
        Assert.assertFalse(n.isValid());
        Assert.assertEquals(-1.0, n.distance("Station1", "Station3"), 0.01); // They are on different lines
    }

    // Scenario for an empty network
    @Test
    public void testEmptyNetworkScenario() {
        Assert.assertFalse(n.isValid());
        Assert.assertEquals(-1.0, n.distance("Station1", "Station2"), 0.01);
    }
}
