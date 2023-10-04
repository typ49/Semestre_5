package fr.ufc.l3info.oprog;

public class AdjustedTicket implements ITicket {
    private final ITicket initialTicket;
    private final int additionalAmount;

    protected AdjustedTicket(ITicket initial, int amount) {
        if (initial == null) {
            throw new NullPointerException("Initial ticket cannot be null.");
        }

        this.initialTicket = initial;

        // Ensure the amount doesn't reduce the initial ticket's provision.
        this.additionalAmount = Math.max(amount, 0);
    }

    @Override
    public boolean isChild() {
        return initialTicket.isChild();
    }

    @Override
    public int getAmount() {
        return initialTicket.getAmount() + additionalAmount;
    }

    @Override
    public String getEntryStation() {
        return initialTicket.getEntryStation();
    }

    @Override
    public boolean entering(String station) {
        return initialTicket.entering(station);
    }

    @Override
    public void invalidate() {
        initialTicket.invalidate();
    }

    @Override
    public boolean isValid() {
        return initialTicket.isValid();
    }
}
