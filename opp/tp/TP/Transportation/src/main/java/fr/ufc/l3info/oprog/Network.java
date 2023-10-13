package fr.ufc.l3info.oprog;

import java.util.*;

public class Network {

    private final Map<String, Station> stations = new HashMap<>();

    public Network() {
    }

    public void addStation(Station s) {
        if (s != null) {
            stations.put(s.getName(), s);
        }
    }

    public Set<String> getLines() {
        Set<String> lines = new HashSet<>();
        for (Station s : stations.values()) {
            lines.addAll(s.getLines());
        }
        return lines;
    }

    public Station getStationByName(String name) {
        return stations.get(name);
    }

    public Station getStationByLineAndNumber(String line, int number) {
        for (Station s : stations.values()) {
            if (s.getLines().contains(line) && s.getNumberForLine(line) == number) {
                return s;
            }
        }
        return null;
    }

    public boolean isValid() {
        if (stations.isEmpty()) return false; // An empty network is invalid

        Set<String> lines = getLines();

        for (String line : lines) {
            boolean foundStationOne = false;
            double previousKm = -1;
            int expectedNumber = 1;

            List<Station> stationsOnLine = new ArrayList<>();
            for (Station s : stations.values()) {
                if (s.getLines().contains(line)) {
                    stationsOnLine.add(s);
                }
            }
            stationsOnLine.sort(Comparator.comparingInt(s -> s.getNumberForLine(line)));

            for (Station s : stationsOnLine) {
                int stationNumber = s.getNumberForLine(line);
                double stationKm = s.getDistanceForLine(line);

                if (stationNumber == 1) {
                    foundStationOne = true;
                    if (stationKm != 0) return false;
                }

                if (stationKm <= previousKm) return false;
                previousKm = stationKm;

                if (stationNumber != expectedNumber) return false;
                expectedNumber++;
            }
            if (!foundStationOne) return false; // There should be a station with number 1 for each line
        }

        return true;
    }

    public double distance(String s1, String s2) {
        Station station1 = getStationByName(s1);
        Station station2 = getStationByName(s2);

        if (station1 == null || station2 == null) {
            return -1.0;
        }

        double shortestDistance = Double.MAX_VALUE;
        boolean found = false;

        for (String line : getLines()) {
            if (station1.getLines().contains(line) && station2.getLines().contains(line)) {
                double currentDistance = Math.abs(station1.getDistanceForLine(line) - station2.getDistanceForLine(line));
                if (currentDistance < shortestDistance) {
                    shortestDistance = currentDistance;
                    found = true;
                }
            }
        }

        return found ? shortestDistance : -1.0;
    }
}