package fr.ufc.l3info.oprog;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;


/**
 * Test file for the Station class.
 */
public class TestStation {

    Station s;

    @Before     // indicates that the method should be executed before each test
    public void setup() {
        // creation of an object to test
        s = new Station("ma Station");

    }


    @Test   // indicates that this method is a test case
    public void testName() {
        // an observation that will cause the test to fail:
        Assert.assertEquals("ma Station", s.getName());
    }

    @Test
    public void TestGetLines() {
    	Assert.assertEquals(0, s.getLines().size());
    }


}