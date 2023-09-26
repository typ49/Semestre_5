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

    public void removeStation(Station s) {
        stations.remove(s);
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
        if (stations.isEmpty()) return false;

        Map<String, List<Station>> stationsByLine = new HashMap<>();
        for (Station s : stations) {
            for (String line : s.getLines()) {
                if (!stationsByLine.containsKey(line)) {
                    stationsByLine.put(line, new ArrayList<Station>());
                }
                stationsByLine.get(line).add(s);
            }
        }

        for (Map.Entry<String, List<Station>> entry : stationsByLine.entrySet()) {
            final String line = entry.getKey();
            List<Station> lineStations = entry.getValue();

            Collections.sort(lineStations, new Comparator<Station>() {
                @Override
                public int compare(Station s1, Station s2) {
                    return Integer.compare(s1.getNumberForLine(line), s2.getNumberForLine(line));
                }
            });

            if (lineStations.get(0).getNumberForLine(line) != 1 || lineStations.get(0).getDistanceForLine(line) != 0.0) {
                return false;
            }

            for (int i = 1; i < lineStations.size(); i++) {
                if (lineStations.get(i).getNumberForLine(line) - lineStations.get(i - 1).getNumberForLine(line) != 1 ||
                        lineStations.get(i).getDistanceForLine(line) <= lineStations.get(i - 1).getDistanceForLine(line)) {
                    return false;
                }
            }
        }

        // TODO: Check if every station is accessible from any other station.
        // This requires a more complex graph traversal algorithm.

        return true;
    }


    public double distance(String s1, String s2) {
        if (!isValid()) return -1.0;

        Station station1 = getStationByName(s1);
        Station station2 = getStationByName(s2);

        if (station1 == null || station2 == null) return -1.0;

        // If they are on the same line, calculate the distance
        for (String line : station1.getLines()) {
            if (station2.getLines().contains(line)) {
                return Math.abs(station1.getDistanceForLine(line) - station2.getDistanceForLine(line));
            }
        }

        // TODO: Calculate shortest distance between stations on different lines.
        // This requires a more complex graph traversal algorithm.

        return -1.0;
    }
}
