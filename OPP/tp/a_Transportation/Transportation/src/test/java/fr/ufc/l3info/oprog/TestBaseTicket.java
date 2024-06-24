package fr.ufc.l3info.oprog;

import org.junit.*;

/* pour pouvoir utiliser baseTicket.jar, il faut ajouter dans le pom.xml
<dependency>
      <groupId>fr.ufc.l3info.oprog</groupId>
      <artifactId>Ticket</artifactId>
      <version>1.0</version>
      <scope>system</scope>cket(false,
      <systemPath>${basedir}/target/BaseTickets.jar</systemPath>
    </dependency>
 */
// sujet de test 04

public class TestBaseTicket {

    BaseTicket ticketChild;
    BaseTicket ticketNegativeAmount;
    BaseTicket ticketAdult;
    BaseTicket ticketNoAmount;


    @Before
    public void setup() {
        ticketChild = new BaseTicket(true, 100);
        ticketNegativeAmount = new BaseTicket(false, -100);
        ticketAdult = new BaseTicket(false, 100);
        ticketNoAmount = new BaseTicket(false, 0);
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
    public void testEnteringAfterInvalidate() {
        ticketChild.invalidate();
        Assert.assertFalse(ticketChild.entering("station"));
    }


    BaseTicket ticketMinValue;
    BaseTicket ticketMaxValue;

    @Before
    public void additionalSetup() {
        ticketMinValue = new BaseTicket(false, Integer.MIN_VALUE);
        ticketMaxValue = new BaseTicket(false, Integer.MAX_VALUE);
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
        Assert.assertTrue(new BaseTicket(true, 50).isValid());
    }


    @Test
    public void testInvalidateEnterInvalidateSequence() {
        ticketChild.invalidate();
        ticketChild.entering("station1");
        ticketChild.invalidate();
        Assert.assertFalse(ticketChild.isValid());
    }


    @Test
    public void testSwitchingStationsWithoutInvalidation() {
        ticketChild.entering("station1");
        Assert.assertFalse(ticketChild.entering("station2"));
        Assert.assertNull(ticketChild.getEntryStation());
    }


    @Test
    public void testAmountJustAboveZero() {
        BaseTicket ticket = new BaseTicket(false, 1);
        Assert.assertEquals(1, ticket.getAmount());
    }

    @Test
    public void testAmountJustBelowZero() {
        BaseTicket ticket = new BaseTicket(false, -1);
        Assert.assertEquals(0, ticket.getAmount());  // Si l'implémentation ne permet pas les montants négatifs
    }







}