package fr.ufc.l3info.oprog;
import java.util.Map;
import java.util.Set;
import java.util.HashMap;
import java.util.HashSet;

public class Station {
    private final String name;
    private final Map<String, LineInfo> lines;

    private static class LineInfo {
        final int number;
        final double distance;

        LineInfo(int _number, double _distance) {
            this.number = _number;
            this.distance = _distance;
        }
    }

    public Station(String _name) {
        this.name = _name;
        this.lines = new HashMap<>();
    }

    public Station(String _name, String _line, int _number, double _dist) {
        this(_name);
        addLine(_line, _number, _dist);
    }

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

    public void removeLine(String _line) {
        if (_line == null || _line.trim().isEmpty()) {
            return;
        }
        lines.remove(_line);
    }

    public String getName() {
        return name;
    }

    public int getNumberForLine(String l) {
        if (l == null || l.trim().isEmpty()) {
            return 0;
        }
        LineInfo info = lines.get(l);
        return info != null ? info.number : 0;
    }

    public double getDistanceForLine(String l) {
        if (l == null || l.trim().isEmpty()) {
            return -1.0;
        }
        LineInfo info = lines.get(l);
        return info != null ? info.distance : -1.0;
    }

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
