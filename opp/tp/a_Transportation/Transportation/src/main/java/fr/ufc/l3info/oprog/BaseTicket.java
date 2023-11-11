package fr.ufc.l3info.oprog;

/**
 * Black-box testing --> wrong implementations to detect based on the specification
 */
public class BaseTicket implements ITicket {

    /** is this a child ticket? */
    private final boolean child;

    /** amount of money on the ticket for the trip */
    private final int amount;

    /** entry station */
    private String entry = "should not be returned";

    /** state of the ticket: 0 - issued, 1 - entry set, 2 - invalid */
    private int state;
    final private int ISSUED = 0;
    final private int ENTERED = 1;
    final private int INVALID = 2;


    /**
     * Constructor for the Ticket class.
     * Creates a ticket with the initial childhood properties and amount.
     * @param child indicates if the ticket concerns a child or not.
     * @param amount initial amount of money on the card.
     */
    protected BaseTicket(boolean child, int amount) {
        this.child = child;
        this.amount = Math.max(amount, 0);
        this.state = ISSUED;
    }


    /**
     * Checks if the ticket is a child ticket.
     * @return true if the ticket is for a child traveller.
     */
    public boolean isChild() {
        return this.child;
    }


    /**
     * Returns the entry station stored on the ticket.
     * @return The station name, or null if the ticket is invalid,
     *          or if the traveller didn't pass the barrier to enter.
     */
    public String getEntryStation() {
        return (state == ENTERED) ? entry : null;
    }


    /**
     * Sets the entry station on the ticket. Should be done only once.
     * @param name the name of the station.
     * @return true if the station has been properly stored.
     */
    public boolean entering(String name) {
        if (this.state == ISSUED && name != null && !name.trim().isEmpty()) {
            entry = name;
            this.state = ENTERED;
            return true;
        }
        invalidate();
        return false;
    }


    /**
     * Signals the exiting of the station.
     */
    public void invalidate() {
        state = INVALID;
    }


    /**
     * Provides the amount of money that is on the ticket.
     * @return the balance of the ticket.
     */
    public int getAmount() {
        return amount;
    }


    /**
     * Determines if the ticket is valid.
     * @return true if the ticket has not been invalidated.
     */
    public boolean isValid() {
        return state != INVALID;
    }


}