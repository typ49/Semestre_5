package fr.ufc.l3info.oprog;

import java.util.*;

public class Network {

    private final Set<Station> stations = new HashSet<>();

    public Network() {
    }

    public void addStation(Station s) {
        if (s != null && !stationWithNameExists(s.getName())) {
            stations.add(s);
        }
    }

    private boolean stationWithNameExists(String name) {
        for (Station station : stations) {
            if (station.getName().equals(name)) {
                return true;
            }
        }
        return false;
    }

    public Set<String> getLines() {
        Set<String> lines = new HashSet<>();
        for (Station s : stations) {
            lines.addAll(s.getLines());
        }
        return lines;
    }

    public Set<Station> getStations() {
        return new HashSet<>(stations);
    }

    public Station getStationByName(String name) {
        for (Station s : stations) {
            if (s.getName().equals(name)) {
                return s;
            }
        }
        return null;
    }

    public Station getStationByLineAndNumber(String line, int number) {
        for (Station s : stations) {
            if (s.getLines().contains(line) && s.getNumberForLine(line) == number) {
                return s;
            }
        }
        return null;
    }

    public boolean isValid() {
        // TODO : Implement this method
    return false;
    }


    public double distance(String s1, String s2) {
        // TODO : Implement this method
        return 0.0;
    }
}
