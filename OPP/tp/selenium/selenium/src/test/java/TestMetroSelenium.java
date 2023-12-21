import junit.framework.TestCase;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.*;
import org.openqa.selenium.Dimension;
import org.openqa.selenium.Point;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.chrome.ChromeOptions;

import java.awt.*;
import java.util.concurrent.TimeUnit;


public class TestMetroSelenium extends TestCase {

    MetroAdapter2 metro;

    @Before
    public void setUp() {
        metro = new MetroAdapter2();
        metro.startTest();
    }

    @After
    public void tearDown() {
        metro.endTest();
    }


    @Test
    public void testScenario1() {
        String c = metro.addCharacter();
        metro.buyTicket(1, 0, 200, 200);
        metro.passBarrier(true, 1, true);
        metro.passBarrier(false, 1, true);
        metro.removeCharacter(c);
    }

}


class MetroAdapter {

    /** Web driver used to send the commands **/
    private WebDriver driver;

    /**
     * Constructor for Metro Adapter.
     */
    public MetroAdapter() {
        System.setProperty("webdriver.chrome.driver", "/home/juliengauthier410/l3/s5/opp/chromedriver-linux64/chromedriver-linux64/chromedriver");
        ChromeOptions chromeOptions = new ChromeOptions();
        // chromeOptions.addArguments("--headless");
        driver = new ChromeDriver(chromeOptions);

        /*
        System.setProperty("webdriver.gecko.driver", "/chemin/absolu/vers/geckodriver");
        driver = new FirefoxDriver();
         */

        // open browser window
        driver.manage().window().setPosition(new Point(0,0));
        java.awt.Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
        driver.manage().window().setSize(new Dimension(dim.width, dim.height));
        driver.manage().timeouts().implicitlyWait(3, TimeUnit.SECONDS);

    }

    /**
     * Start a test by opening the URL.
     */
    public void startTest() {
        driver.get("https://fdadeau.github.io/chikatetsu");
    }

    /**
     * Ends a test by closing the tab.
     */
    public void endTest() {
        driver.close();
        driver.quit();  // à enlever avec le plugin Firefox
    }


    /**
     *  Adds a character to the scene.
     *  @return  the identifier of the new character.
     */
    public String addCharacter(){
        driver.findElement(By.id("btnAddCharacter")).click();
        wait(1000);
        return driver.findElement(By.cssSelector(".sprite.selected")).getAttribute("id");
    }


    /**
     *  Go to ticket vending machine and buy the requested number of tickets
     *  @param nbAdult number of adult tickets to buy
     *  @param nbChild number of child tickets to buy
     *  @param fare fare amount to buy
     *  // TODO: could be split into several sub-methods...
     */
    public void buyTicket(int nbAdult, int nbChild, int fare, int payment) {
        // click on machine
        driver.findElement(By.cssSelector(".TVM:nth-child(8)")).click();
        // wait for the machine to be displayed
        WebElement machine = driver.findElement(By.cssSelector("aside .machine"));
        while (! machine.isDisplayed()) {
            wait(200);
        }
        // click on "English" button
        driver.findElement(By.id("lblJPEN")).click();
        // clicks on adults button
        for (int i=0; i < nbAdult; i++) {
            driver.findElement(By.id("btnOneAdult")).click();
            wait(1000);
        }
        // clicks on children button
        for (int j=0; j < nbChild; j++) {
            driver.findElement(By.id("btnOneChild")).click();
            wait(1000);
        }
        // clicks on the requested fare
        driver.findElement(By.cssSelector(".machine .right label[data-fare='" + fare + "']")).click();
        wait(1000);

        // check that the amount corresponds to the computed total
        Assert.assertEquals(""+payment,
                driver.findElement(By.cssSelector("[data-total]")).getAttribute("data-total"));

        // pay the requested amount using coins
        driver.findElement(By.cssSelector(".machine .coins")).click();
        wait(500);
        Alert a = driver.switchTo().alert();
        a.sendKeys(""+payment);
        a.accept();
        wait(500);

        // get the tickets
        driver.findElement(By.cssSelector(".machine .tickets .ticket")).click();
        wait(500);

        // close the interface
        driver.findElement(By.cssSelector("aside .btnClose")).click();
        wait(500);
    }


    /**
     * Reaches a barrier and attempts to enter a ticket.
     * @param isEntry indicates if the barrier is an entry (true) or exit (false)
     * @param ticketIndex index of the ticket to be used (index in CSS start at 1)
     * @param shouldOpen indicates if the barrier is expected to open (true) or not (false)
     */
    public void passBarrier(boolean isEntry, int ticketIndex, boolean shouldOpen) {

        WebElement barrier = driver.findElement(By.cssSelector(isEntry ? ".barrier:nth-child(5)" : ".barrier:nth-child(2)"));
        WebElement tickets = driver.findElement(By.id("bcTickets"));

        // go to barrier
        barrier.click();

        // wait for ticket request
        while (! tickets.isDisplayed()) {
            wait(200);
        }

        // select requested ticket
        driver.findElement(By.cssSelector("#bcTickets .ticket:nth-child(" + ticketIndex + ")")).click();
        wait(100);

        // check that the barrier is opened
        Assert.assertTrue(barrier.getAttribute("class").contains(shouldOpen ? "ok" : "ko"));

        // wait until the character has passed the barrier
        while (barrier.getAttribute("class").contains(shouldOpen ? "ok" : "ko")) {
            wait(200);
        }
    }


    /**
     * Removes the character from the scene.
     * @param id the identifier of the character to exit the scene.
     */
    public void removeCharacter(String id) {
        driver.findElement(By.id("btnExit")).click();
        wait(3500);
        try {
            driver.findElement(By.id(id));
            Assert.fail("Client should not exist anymore.");
        }
        catch (NoSuchElementException e) {
            // expected
        }
    }


    /**
     * Utility function to pause the test execution.
     * (to wait for loading, or others actions to end)
     * @param ms the waiting time in milliseconds.
     */
    private static void wait(int ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {
            Assert.fail("Test case interrupted");
        }

    }


}
