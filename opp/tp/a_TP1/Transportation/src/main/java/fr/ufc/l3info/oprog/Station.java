package fr.ufc.l3info.oprog;

import java.util.Map;
import java.util.Set;
import java.util.HashMap;
import java.util.HashSet;

/**
 * Class representing a station.
 */
public class Station {
    private final String name;
    private final Map<String, LineInfo> lines;

    /**
     * Class representing a line associated to a station.
     */
    private static class LineInfo {
        final int number;
        final double distance;

        /**
         * Builds a line info.
         * @param _number
         * @param _distance
         */
        LineInfo(int _number, double _distance) {
            this.number = _number;
            this.distance = _distance;
        }
    }

    /**
     * Builds a station associated to no lines.
     * @param _name the name of the station.
     */
    public Station(String _name) {
        this.name = _name;
        this.lines = new HashMap<>();
    }

    /**
     * Builds a station, initially associated to a given line with a given number.
     * @param _name the name of the station
     * @param _line the name of the line associated to the station
     * @param _number the number of the station on the considered line
     * @param _dist the distance of the station on the considered line
     */
    public Station(String _name, String _line, int _number, double _dist) {
        this(_name);
        addLine(_line, _number, _dist);
    }

    /**
     * Adds a line to the current station, with the appropriate parameters.
     * If the line already exists, the previous information are overwritten.
     * @param _line the name of the line associated to the station
     * @param _number the number of the station on the considered line
     * @param _dist the distance of the station on the considered line
     */
    public void addLine(String _line, int _number, double _dist) {
        if (_line == null || _line.trim().isEmpty()) {
            return;
        }
        if (_number <= 0) {
            return;
        }
        if (_dist < 0) {
            return;
        }
        lines.put(_line, new LineInfo(_number, _dist));
    }

    /**
     * Removes a line from the station.
     * @param _line the line to remove.
     */
    public void removeLine(String _line) {
        if (_line == null || _line.trim().isEmpty()) {
            return;
        }
        lines.remove(_line);
    }

    /**
     * Retrieves the name of the station.
     * @return the name of the station
     */
    public String getName() {
        return name;
    }

    /**
     * Returns the number of the station on a given line.
     * @param l The name of the line
     * @return the # of the station for the given line,
     *         or 0 if the line does not exist at the station.
     */
    public int getNumberForLine(String l) {
        if (l == null || l.trim().isEmpty()) {
            return 0;
        }
        LineInfo info = lines.get(l);
        return info != null ? info.number : 0;
    }

    /**
     * Returns the distance of the station on a given line.
     * @param l The name of the line.
     * @return the distance of the station w.r.t. the beginning of the line.
     */
    public double getDistanceForLine(String l) {
        if (l == null || l.trim().isEmpty()) {
            return -1.0;
        }
        LineInfo info = lines.get(l);
        return info != null ? info.distance : -1.0;
    }

    /**
     * Computes the set of lines associated to the station.
     * @return a set containing the names of the lines that cross the station.
     */
    public Set<String> getLines() {
        return new HashSet<>(lines.keySet());
    }

    @Override
    public int hashCode() {
        return name.hashCode();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Station station = (Station) o;
        return name.equals(station.name);
    }
}
