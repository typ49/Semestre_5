package fr.ufc.l3info.oprog;

/**
 * Interface describing tickets.
 */
public interface ITicket {

    /**
     * Checks if the ticket is a child ticket.
     * @return true if the ticket is for a child traveller.
     */
    public boolean isChild();

    /**
     * Returns the entry station stored on the ticket.
     * @return The station name, or null if the ticket is invalid,
     *          or if the traveller didn't pass the barrier to enter.
     */
    public String getEntryStation();

    /**
     * Sets the entry station on the ticket.
     * @param name the name of the station.
     * @return true if the station has been properly stored.
     */
    public boolean entering(String name);

    /**
     * Signals the exiting of the station.
     */
    public void invalidate();

    /**
     * Provides the amount of money that is on the ticket.
     * @return the balance of the ticket.
     */
    public int getAmount();

    /**
     * Determines if the ticket is valid.
     * @return true if the ticket has not been invalidated.
     */
    public boolean isValid();

}