package fr.ufc.l3info.oprog;

import org.junit.*;

public class TestBaseTicket {

    BaseTicket10 ticketChild;
    BaseTicket10 ticketNegativeAmount;
    BaseTicket10 ticketAdult;
    BaseTicket10 ticketNoAmount;


    @Before
    public void setup() {
        ticketChild = new BaseTicket10(true, 100);
        ticketNegativeAmount = new BaseTicket10(false, -100);
        ticketAdult = new BaseTicket10(false, 100);
        ticketNoAmount = new BaseTicket10(false, 0);
    }

    /**
     * test isChild
     */
    @Test
    public void testIsChild_TRUE_KO() {
        Assert.assertTrue(ticketChild.isChild());
    }

    @Test
    public void testIsChild_FALSE() {
        Assert.assertFalse(ticketAdult.isChild());
    }

    /**
     * test getEntryStation
     */

    @Test
    public void testGetEntryStation_INVALID() {
        Assert.assertNull(ticketChild.getEntryStation());
    }

    @Test
    public void testGetEntryStation_VALID() {
        ticketChild.entering("station");
        Assert.assertEquals("station", ticketChild.getEntryStation());
    }

    /**
     * test entering
     */

    @Test
    public void testEntering_NULL() {
        Assert.assertFalse(ticketChild.entering(null));
    }

    @Test
    public void testEntering_EMPTY() {
        Assert.assertFalse(ticketChild.entering(""));
    }

    @Test
    public void testEntering_VALID() {
        Assert.assertTrue(ticketChild.entering("station"));
    }

    @Test
    public void testEntering_TWICE() {
        ticketChild.entering("station");
        Assert.assertFalse(ticketChild.entering("station"));
    }

    /**
     * test invalidate
     */

    @Test
    public void testInvalidate_DONE() {
        ticketChild.entering("station");
        ticketChild.invalidate();
        Assert.assertNull(ticketChild.getEntryStation());
    }

    @Test
    public void testInvalidate_FAIL() {
        ticketChild.invalidate();
        Assert.assertNull(ticketChild.getEntryStation());
    }


    /**
     * test getAmount
     */

    @Test
    public void testGetAmount_ADULT() {
        Assert.assertEquals(100, ticketAdult.getAmount());
    }

    @Test
    public void testGetAmount_CHILD() {
        Assert.assertEquals(100, ticketChild.getAmount());
    }

    @Test
    public void testGetAmount_NEGATIVE() {
        Assert.assertEquals(0, ticketNegativeAmount.getAmount());
    }

    @Test
    public void testGetAmount_NO_AMOUNT() {
        Assert.assertEquals(0, ticketNoAmount.getAmount());
    }

    /**
     * test isValid
     */

    @Test
    public void testIsValid_TRUE() {
        Assert.assertTrue(ticketChild.isValid());
    }

    @Test
    public void testIsValid_FALSE() {
        ticketChild.invalidate();
        Assert.assertFalse(ticketChild.isValid());
    }

    /**
     * test different scenarios
     */

    @Test
    public void testMultipleInvalidate() {
        ticketChild.invalidate();
        ticketChild.invalidate();
        Assert.assertNull(ticketChild.getEntryStation());
        Assert.assertFalse(ticketChild.isValid());
    }

    @Test
    public void testEnteringAfterInvalidate_KO() {
        ticketChild.invalidate();
        Assert.assertFalse(ticketChild.entering("station"));
    }


    BaseTicket10 ticketMinValue;
    BaseTicket10 ticketMaxValue;

    @Before
    public void additionalSetup() {
        ticketMinValue = new BaseTicket10(false, Integer.MIN_VALUE);
        ticketMaxValue = new BaseTicket10(false, Integer.MAX_VALUE);
    }

    @Test
    public void testGetAmount_MIN_VALUE() {
        Assert.assertEquals(0, ticketMinValue.getAmount());
    }

    @Test
    public void testGetAmount_MAX_VALUE() {
        Assert.assertEquals(Integer.MAX_VALUE, ticketMaxValue.getAmount());
    }


    @Test
    public void testEnteringThenInvalidate() {
        ticketChild.entering("station");
        ticketChild.invalidate();
        Assert.assertNull(ticketChild.getEntryStation());
    }

    @Test
    public void testIsValidAfterEntering() {
        ticketChild.entering("station");
        Assert.assertTrue(ticketChild.isValid());
    }

    @Test
    public void testIsValidAfterEnteringAndInvalidate() {
        ticketChild.entering("station");
        ticketChild.invalidate();
        Assert.assertFalse(ticketChild.isValid());
    }

    @Test
    public void testIsChildAfterInvalidate_KO() {
        ticketChild.invalidate();
        Assert.assertTrue(ticketChild.isChild());
    }

    @Test
    public void testGetAmountAfterEntering() {
        ticketChild.entering("station");
        Assert.assertEquals(100, ticketChild.getAmount());
    }

    @Test
    public void testEntering_LongName() {
        Assert.assertTrue(ticketChild.entering("stationWithAVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryLongName"));
    }

    @Test
    public void testEntering_SpecialChars() {
        Assert.assertTrue(ticketChild.entering("station@123!_-_-_-_-_--ç-àçà'è-(ç'_è(-c   (_'ç"));
    }

    @Test
    public void testIsValid_JustCreated() {
        Assert.assertTrue(new BaseTicket10(true, 50).isValid());
    }


    @Test
    public void testInvalidateEnterInvalidateSequence() {
        ticketChild.invalidate();
        ticketChild.entering("station1");
        ticketChild.invalidate();
        Assert.assertFalse(ticketChild.isValid());
    }


    @Test
    public void testSwitchingStationsWithoutInvalidation_KO() {
        ticketChild.entering("station1");
        Assert.assertFalse(ticketChild.entering("station2"));
        Assert.assertNull(ticketChild.getEntryStation());
    }


    @Test
    public void testAmountJustAboveZero() {
        BaseTicket10 ticket = new BaseTicket10(false, 1);
        Assert.assertEquals(1, ticket.getAmount());
    }

    @Test
    public void testAmountJustBelowZero() {
        BaseTicket10 ticket = new BaseTicket10(false, -1);
        Assert.assertEquals(0, ticket.getAmount());  // Si l'implémentation ne permet pas les montants négatifs
    }







}
