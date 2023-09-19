package fr.ufc.l3info.oprog;


import java.util.Set;


/**
 * Class representing a station.
 */
public class Station {


    /** Builds a station associated to no lines.
     * @param _name the name of the station.
     */
    public Station(String _name) {
        // TODO
    }

    /**
     * Builds a station, initially associated to a given line with a given number.
     * @param _name the name of the station
     * @param _line the name of the line associated to the station
     * @param _number the number of the station on the considered line
     * @param _dist the distance of the station on the considered line
     */
    public Station(String _name, String _line, int _number, double _dist) {
        // TODO
    }


    /**
     * Adds a line to the current station, with the appropriate parameters.
     * If the line already exists, the previous information are overwritten.
     * @param _line the name of the line associated to the station
     * @param _number the number of the station on the considered line
     * @param _dist the distance of the station on the considered line
     */
    public void addLine(String _line, int _number, double _dist) {
        // TODO
    }


    /**
     * Removes a line from the station.
     * @param _line the line to remove.
     */
    public void removeLine(String _line) {
        // TODO
    }


    /**
     * Retrieves the name of the station.
     * @return the name of the station
     */
    public String getName() {
        return null;  // TODO
    }

    /**
     * Returns the number of the station on a given line.
     * @param l The name of the line
     * @return the # of the station for the given line,
     *         or 0 if the line does not exist at the station.
     */
    public int getNumberForLine(String l) {
        return 0;   // TODO
    }


    /**
     * Returns the distance of the station on a given line.
     * @param l The name of the line.
     * @return the distance of the station w.r.t. the beginning of the line.
     */
    public double getDistanceForLine(String l) {
        return 0.0; // TODO
    }

    /**
     * Computes the set of lines associated to the station.
     * @return a set containing the names of the lines that cross the station.
     */
    public Set<String> getLines() {
        return null;    // TODO
    }


    @Override
    public int hashCode() {
        return 0;   // TODO
    }

    @Override
    public boolean equals(Object o) {
        return false;  // TODO
    }
}