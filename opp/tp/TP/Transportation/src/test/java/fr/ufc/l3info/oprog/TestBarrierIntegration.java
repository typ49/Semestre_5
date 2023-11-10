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

    @Test
    public void testExitWithChildTicketSufficientFundsLongDistance() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier barrier = Barrier.build(network, "Station1", prices);

        // Test
        ITicket childTicket = new BaseTicket(true, 30); // Child ticket with 30 units
        barrier.enter(childTicket);
        boolean result = barrier.exit(childTicket);
        assertTrue(result);
        assertFalse(childTicket.isValid());
    }

    @Test
    public void testExitWithAdultTicketInsufficientFundsLongDistance() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);

        Station station2 = new Station("Station2");
        station2.addLine("A", 2, 5.0); // Supposons que la distance entre Station1 et Station2 soit de 5.0
        network.addStation(station2);

        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier entryBarrier = Barrier.build(network, "Station1", prices);
        Barrier exitBarrier = Barrier.build(network, "Station2", prices);

        // Test
        ITicket adultTicket = new BaseTicket(false, 15); // Adult ticket with 15 units
        entryBarrier.enter(adultTicket);
        boolean result = exitBarrier.exit(adultTicket); // L'utilisateur tente de sortir à la Station2
        assertFalse(result);
        assertTrue(adultTicket.isValid());
    }


    @Test
    public void testBarrierBuildWithNonIncreasingPrices() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);

        // Test
        Map<Double, Integer> nonIncreasingPrices = new HashMap<>();
        nonIncreasingPrices.put(0.0, 20);
        nonIncreasingPrices.put(5.0, 10);
        Barrier barrier = Barrier.build(network, "Station1", nonIncreasingPrices);
        assertNull(barrier);
    }

    @Test
    public void testBarrierBuildWithNonExistentStation() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);

        // Test
        Barrier barrier = Barrier.build(network, "NonExistentStation", prices);
        assertNull(barrier);
    }

    @Test
    public void testEnterWithAlreadyEnteredTicket() {
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
        boolean result = barrier.enter(adultTicket);
        assertFalse(result);
    }

    @Test
    public void testExitWithAlreadyExitedTicket() {
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
        barrier.exit(adultTicket);
        boolean result = barrier.exit(adultTicket);
        assertFalse(result);
    }

    @Test
    public void testExitWithChildTicketInsufficientFundsLongDistance() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);

        Station station2 = new Station("Station2");
        station2.addLine("A", 2, 5.0);
        network.addStation(station2);

        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier entryBarrier = Barrier.build(network, "Station1", prices);
        Barrier exitBarrier = Barrier.build(network, "Station2", prices);

        // Test
        ITicket childTicket = new BaseTicket(true, 5); // Child ticket with 10 units
        entryBarrier.enter(childTicket);
        boolean result = exitBarrier.exit(childTicket);
        assertFalse(result);
        assertTrue(childTicket.isValid());
    }

    @Test
    public void testBarrierBuildWithNegativePrices() {
        // Initialisation
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);

        // Test with negative price
        Map<Double, Integer> pricesWithNegative = new HashMap<>();
        pricesWithNegative.put(0.0, -10);
        Barrier barrier2 = Barrier.build(network, "Station1", pricesWithNegative);
        assertNull(barrier2);
    }

    @Test
    public void testEnterAndExitWithDifferentStations() {
        // Initialisation avec plusieurs stations
        Network network = new Network();
        Station station1 = new Station("Station1");
        Station station2 = new Station("Station2");
        station1.addLine("A", 1, 0.0);
        station2.addLine("A", 2, 5.0);
        network.addStation(station1);
        network.addStation(station2);
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier entryBarrier = Barrier.build(network, "Station1", prices);
        Barrier exitBarrier = Barrier.build(network, "Station2", prices);
        ITicket adultTicket = new BaseTicket(false, 30);

        // Test
        assertTrue(entryBarrier.enter(adultTicket));
        assertTrue(exitBarrier.exit(adultTicket));
        assertFalse(adultTicket.isValid());
    }

    @Test
    public void testExitWhenTicketHasJustEnoughFunds() {
        // Initialisation
        Network network = setupBasicNetwork();
        Map<Double, Integer> prices = setupBasicPrices();
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket adultTicket = new BaseTicket(false, 10); // Just enough funds

        // Test
        barrier.enter(adultTicket);
        assertTrue(barrier.exit(adultTicket));
        assertFalse(adultTicket.isValid());
    }

    @Test
    public void testEnterExitWithMultipleUses() {
        // Initialisation
        Network network = setupBasicNetwork();
        Map<Double, Integer> prices = setupBasicPrices();
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket adultTicket = new BaseTicket(false, 50); // Multiple uses

        // Test
        assertTrue(barrier.enter(adultTicket));
        assertTrue(barrier.exit(adultTicket));
        assertFalse(barrier.enter(adultTicket)); // Second use
        assertFalse(barrier.exit(adultTicket));
        assertFalse(adultTicket.isValid()); // Should be invalid after second use
    }

    @Test
    public void testBarrierWithInvalidTariffStructure() {
        // Initialisation
        Network network = setupBasicNetwork();
        Map<Double, Integer> invalidPrices = new HashMap<>();
        invalidPrices.put(0.0, 0); // Invalid price
        invalidPrices.put(5.0, 20);

        // Test
        Barrier barrier = Barrier.build(network, "Station1", invalidPrices);
        assertNull(barrier);
    }

    // Helper methods to setup basic network and prices
    private Network setupBasicNetwork() {
        Network network = new Network();
        Station station1 = new Station("Station1");
        station1.addLine("A", 1, 0.0);
        network.addStation(station1);
        return network;
    }

    private Map<Double, Integer> setupBasicPrices() {
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        return prices;
    }

    @Test
    public void testFareCalculationAtExactTariffBoundaries() {
        Network network = setupBasicNetwork();
        Map<Double, Integer> prices = new HashMap<>();
        prices.put(0.0, 10);
        prices.put(5.0, 20);
        Barrier barrier = Barrier.build(network, "Station1", prices);
        ITicket ticket = new BaseTicket(false, 20); // Exact fare

        barrier.enter(ticket);
        assertTrue(barrier.exit(ticket));
        assertFalse(ticket.isValid());
    }

    @Test
    public void testEnterWithUsedTicket() {
        Network network = setupBasicNetwork();
        Barrier barrier = Barrier.build(network, "Station1", setupBasicPrices());
        ITicket ticket = new BaseTicket(false, 30);

        barrier.enter(ticket);
        barrier.exit(ticket);
        assertFalse(barrier.enter(ticket)); // Should be invalid after exit
    }

    @Test
    public void testEnterWithNegativeBalanceTicket() {
        Network network = setupBasicNetwork();
        Barrier barrier = Barrier.build(network, "Station1", setupBasicPrices());
        ITicket ticket = new BaseTicket(false, -10); // Negative balance

        assertFalse(barrier.enter(ticket));
    }


}
