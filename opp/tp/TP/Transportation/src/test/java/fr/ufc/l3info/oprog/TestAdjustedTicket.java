package fr.ufc.l3info.oprog;

import org.junit.*;

public class TestAdjustedTicket {

    ITicket baseTicket;
    AdjustedTicket adjustedTicket;
    AdjustedTicket adjustedChild;
    AdjustedTicket adjustedNegative;
    AdjustedTicket adjustedAdult;
    AdjustedTicket adjustedNoAmount;

    @Before
    public void setup() {
        baseTicket = new BaseTicket(true, 100);
        adjustedTicket = new AdjustedTicket(baseTicket, 25);
        adjustedChild = new AdjustedTicket(baseTicket, 20);
        adjustedNegative = new AdjustedTicket(baseTicket, -20);
        adjustedAdult = new AdjustedTicket(new BaseTicket(false, 100), 20);
        adjustedNoAmount = new AdjustedTicket(new BaseTicket(false, 0), 20);
    }

    @Test(expected = NullPointerException.class)
    public void testNullTicketConstruction() {
        new AdjustedTicket(null, 25);
    }

    @Test
    public void testAdjustedAmount() {
        Assert.assertEquals(125, adjustedTicket.getAmount());
    }

    @Test
    public void testNoAdjustForNegativeAmount() {
        ITicket negativeAdjust = new AdjustedTicket(baseTicket, -25);
        Assert.assertEquals(baseTicket.getAmount(), negativeAdjust.getAmount());
    }

    @Test
    public void testChildStatus() {
        Assert.assertEquals(baseTicket.isChild(), adjustedTicket.isChild());
    }

    @Test
    public void testEnteringStation() {
        String station = "TestStation";
        adjustedTicket.entering(station);
        Assert.assertEquals(station, adjustedTicket.getEntryStation());
    }

    @Test
    public void testInvalidate() {
        adjustedTicket.entering("TestStation");
        adjustedTicket.invalidate();
        Assert.assertNull(adjustedTicket.getEntryStation());
        Assert.assertFalse(adjustedTicket.isValid());
    }

    @Test
    public void testIsValid() {
        Assert.assertTrue(adjustedTicket.isValid());
        adjustedTicket.invalidate();
        Assert.assertFalse(adjustedTicket.isValid());
    }

    @Test
    public void testIsChild_TRUE_KO() {
        Assert.assertTrue(adjustedChild.isChild());
    }

    @Test
    public void testIsChild_FALSE() {
        Assert.assertFalse(adjustedAdult.isChild());
    }

    @Test
    public void testGetEntryStation_INVALID() {
        Assert.assertNull(adjustedChild.getEntryStation());
    }

    @Test
    public void testGetEntryStation_VALID() {
        adjustedChild.entering("station");
        Assert.assertEquals("station", adjustedChild.getEntryStation());
    }

    @Test
    public void testEntering_NULL() {
        Assert.assertFalse(adjustedChild.entering(null));
    }

    @Test
    public void testEntering_EMPTY() {
        Assert.assertFalse(adjustedChild.entering(""));
    }

    @Test
    public void testEntering_VALID() {
        Assert.assertTrue(adjustedChild.entering("station"));
    }

    @Test
    public void testEntering_TWICE() {
        adjustedChild.entering("station");
        Assert.assertFalse(adjustedChild.entering("station"));
    }

    @Test
    public void testInvalidate_DONE() {
        adjustedChild.entering("station");
        adjustedChild.invalidate();
        Assert.assertNull(adjustedChild.getEntryStation());
    }

    @Test
    public void testInvalidate_FAIL() {
        adjustedChild.invalidate();
        Assert.assertNull(adjustedChild.getEntryStation());
    }

    @Test
    public void testGetAmount_ADULT() {
        Assert.assertEquals(120, adjustedAdult.getAmount());
    }

    @Test
    public void testGetAmount_CHILD() {
        Assert.assertEquals(120, adjustedChild.getAmount());
    }

    @Test
    public void testGetAmount_NEGATIVE() {
        Assert.assertEquals(100, adjustedNegative.getAmount());  // The adjusted amount should not decrease the original
    }

    @Test
    public void testGetAmount_NO_AMOUNT() {
        Assert.assertEquals(20, adjustedNoAmount.getAmount());
    }

    @Test
    public void testIsValid_TRUE() {
        Assert.assertTrue(adjustedChild.isValid());
    }

    @Test
    public void testIsValid_FALSE() {
        adjustedChild.invalidate();
        Assert.assertFalse(adjustedChild.isValid());
    }

    @Test
    public void testNegativeAdjustedAmountEqualsBaseAmount() {
        Assert.assertEquals(baseTicket.getAmount(), adjustedNegative.getAmount());
    }

    @Test
    public void testZeroAdjustment() {
        AdjustedTicket zeroAdjustTicket = new AdjustedTicket(baseTicket, 0);
        Assert.assertEquals(baseTicket.getAmount(), zeroAdjustTicket.getAmount());
    }

    @Test
    public void testMultipleAdjustments() {
        AdjustedTicket multipleAdjusted = new AdjustedTicket(adjustedTicket, 50);
        Assert.assertEquals(175, multipleAdjusted.getAmount());  // Base: 100 + First Adjust: 25 + Second Adjust: 50
    }

    @Test
    public void testInheritedInvalidation() {
        baseTicket.invalidate();
        Assert.assertFalse(adjustedTicket.isValid());
    }

    @Test
    public void testInheritedEntering() {
        baseTicket.entering("TestStation2");
        Assert.assertEquals("TestStation2", adjustedTicket.getEntryStation());
    }

    @Test
    public void testInvalidationPropagation() {
        adjustedTicket.invalidate();
        Assert.assertFalse(baseTicket.isValid());
    }

    @Test
    public void testEnteringPropagation() {
        adjustedTicket.entering("TestStation3");
        Assert.assertEquals("TestStation3", baseTicket.getEntryStation());
    }

    @Test
    public void testAdjustmentToZeroAmount() {
        AdjustedTicket adjustToZero = new AdjustedTicket(adjustedNoAmount, -20);
        Assert.assertEquals(adjustedNoAmount.getAmount(), adjustToZero.getAmount());
    }

    @Test
    public void testEnteringAfterInvalidate() {
        adjustedTicket.entering("station1");
        adjustedTicket.invalidate();
        Assert.assertFalse(adjustedTicket.entering("station2"));
    }

    @Test
    public void testInvalidateAfterEntering() {
        adjustedTicket.entering("station1");
        adjustedTicket.invalidate();
        Assert.assertFalse(adjustedTicket.isValid());
    }

}

