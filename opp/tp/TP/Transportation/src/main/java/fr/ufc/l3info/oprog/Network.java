package fr.ufc.l3info.oprog;

import java.util.*;
import java.util.stream.Collectors;

public class Network {

    private final Map<String, Station> stations = new HashMap<>();

    public Network() {
    }

    public void addStation(Station s) {
        if (s != null) {
            stations.put(s.getName(), s);
        }
    }

    public void removeStation(Station s) {
        if (s != null) {
            stations.remove(s.getName());
        }
    }

    private boolean stationWithNameExists(String name) {
        return stations.containsKey(name);
    }

    public Set<String> getLines() {
        Set<String> lines = new HashSet<>();
        for (Station s : stations.values()) {
            lines.addAll(s.getLines());
        }
        return lines;
    }

    public Collection<Station> getStations() {
        return stations.values();
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
        if (stations.isEmpty()) return false;

        Set<String> lines = getLines();

        // Vérification: il existe au moins une ligne.
        if (lines.isEmpty()) return false;

        for (String line : lines) {
            boolean foundStationOne = false;
            double previousKm = -1;
            Set<Integer> seenNumbers = new HashSet<>();

            for (Station s : stations.values()) {
                if (s.getLines().contains(line)) {
                    int stationNumber = s.getNumberForLine(line);
                    double stationKm = s.getDistanceForLine(line); // Cette méthode doit être définie dans Station

                    // Vérification: la première station porte le numéro 1 et se trouve au kilomètre 0.
                    if (stationNumber == 1) {
                        foundStationOne = true;
                        if (stationKm != 0) return false;
                    }

                    // Vérification: les positions kilométriques sont strictement croissantes tout au long de la ligne.
                    if (stationKm <= previousKm) return false;
                    previousKm = stationKm;

                    // Vérification: la numérotation des stations de la ligne ne présente pas de numéros en double ou de numéro manquants.
                    if (seenNumbers.contains(stationNumber) || (stationNumber - seenNumbers.size() > 1)) {
                        return false;
                    }
                    seenNumbers.add(stationNumber);
                }
            }
            if (!foundStationOne) return false;
        }

        // À ce stade, chaque ligne est correctement structurée.
        // Reste à vérifier que chaque station est accessible depuis n'importe quelle autre station.

        // Cela nécessite un algorithme de parcours de graphe pour vérifier la connectivité.
        // Pour le moment, on suppose que c'est vrai. Cette partie pourrait être ajoutée ultérieurement.

        return true;
    }

    public double distance(String s1, String s2) {
        Station station1 = getStationByName(s1);
        Station station2 = getStationByName(s2);

        if (station1 == null || station2 == null) {
            return -1.0; // Retourner une valeur négative ou NaN plutôt que de lancer une exception.
        }

        // Calcul de distance entre deux stations sur la même ligne:
        for (String line : getLines()) {
            if (station1.getLines().contains(line) && station2.getLines().contains(line)) {
                return Math.abs(station1.getDistanceForLine(line) - station2.getDistanceForLine(line));
            }
        }
        return -1.0; // Retourner une valeur négative ou NaN plutôt que de lancer une exception.
    }
}