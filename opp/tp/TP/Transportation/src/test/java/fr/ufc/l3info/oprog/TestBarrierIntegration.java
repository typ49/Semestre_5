package fr.ufc.l3info.oprog;

import org.junit.Test;
import static org.junit.Assert.*;

import java.util.HashMap;
import java.util.Map;

public class TestBarrierIntegration {

    @Test
    public void testEnterWithValidAdultTicket() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket adultTicket = new BaseTicket(false, 30); // Adult ticket with 30 units

        // Test
        boolean result = barrier.enter(adultTicket);
        assertTrue(result);
        assertEquals("Station1", adultTicket.getEntryStation());
    }

    @Test
    public void testEnterWithValidChildTicket() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket childTicket = new BaseTicket(true, 15);  // Child ticket with 15 units

        // Test
        boolean result = barrier.enter(childTicket);
        assertTrue(result);
        assertEquals("Station1", childTicket.getEntryStation());
    }

    @Test
    public void testExitWithSufficientFunds() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket adultTicket = new BaseTicket(false, 30); // Adult ticket with 30 units

        // Test
        barrier.enter(adultTicket);
        boolean result = barrier.exit(adultTicket);
        assertTrue(result);
        assertFalse(adultTicket.isValid());
    }

    @Test
    public void testExitWithInsufficientFunds() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket childTicket = new BaseTicket(true, 10);  // Child ticket with 10 units

        // Test
        barrier.enter(childTicket);
        boolean result = barrier.exit(childTicket);
        assertTrue(result);
        assertFalse(childTicket.isValid());
    }

    @Test
    public void testEnterWithInvalidTicket() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket invalidTicket = new BaseTicket(false, 30); // Adult ticket with 30 units
        invalidTicket.invalidate();

        // Test
        boolean result = barrier.enter(invalidTicket);
        assertFalse(result);
    }

    @Test
    public void testEnterWithZeroBalanceTicket() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket zeroBalanceTicket = new BaseTicket(false, 0); // Adult ticket with 0 units

        // Test
        boolean result = barrier.enter(zeroBalanceTicket);
        assertFalse(result);
    }

    @Test
    public void testExitWithoutEntering() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket adultTicket = new BaseTicket(false, 30); // Adult ticket with 30 units

        // Test
        boolean result = barrier.exit(adultTicket);
        assertFalse(result);
    }

    @Test
    public void testBarrierBuildWithInvalidNetwork() {
        // Initialisation
        Network invalidNetwork = new Network(); // Empty network
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);

        // Test
        Barrier barrier = Barrier.build(invalidNetwork, "Station1", prices);
        assertNull(barrier);
    }

    @Test
    public void testBarrierBuildWithInvalidPrices() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> invalidPrices = new HashMap<>();
        invalidPrices.put(-1.0, 10); // Invalid distance

        // Test
        Barrier barrier = Barrier.build(network, "Station1", invalidPrices);
        assertNull(barrier);
    }
}
