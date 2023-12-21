package fr.ufc.l3info.oprog.parser;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class ASTCheckerVisitor implements ASTNodeVisitor {

    private final Map<String, ERROR_KIND> errors = new HashMap<>();
    private final Set<String> lineNames = new HashSet<>();
    private final Map<String, Set<String>> stationsPerLine = new HashMap<>();
    private double lastNumber = -1;

    public Map<String, ERROR_KIND> getErrors() {
        return new HashMap<>(errors);
    }

    @Override
    public Object visit(ASTNode n) {
        return null;
    }

    @Override
    public Object visit(ASTListeLignes n) {
        if (n.getNumChildren() == 0) {
            errors.put("ListeLignes", ERROR_KIND.EMPTY_LINE_LIST);
        }
        return null;
    }

    @Override
    public Object visit(ASTLigne n) {
        String lineName = n.value.trim();
        if (lineName.isEmpty()) {
            errors.put("Ligne", ERROR_KIND.EMPTY_LINE_NAME);
        } else if (lineNames.contains(lineName)) {
            errors.put("Ligne", ERROR_KIND.DUPLICATE_LINE_NAME);
        } else {
            lineNames.add(lineName);
            stationsPerLine.put(lineName, new HashSet<>());
        }
        lastNumber = -1;  // Reset for each new line
        return null;
    }

    @Override
    public Object visit(ASTDeclaration n) {
        String lineName = n.getChild(0).value.trim();  // Assuming the line name is the first child
        String stationName = n.getChild(1).value.trim();  // Assuming the station name is the second child
        double number = ((ASTNombre) n.getChild(2)).getNumberValue();  // Assuming the number is the third child

        if (stationName.isEmpty()) {
            errors.put("Declaration", ERROR_KIND.EMPTY_STATION_NAME);
        } else if (stationsPerLine.get(lineName).contains(stationName)) {
            errors.put("Declaration", ERROR_KIND.DUPLICATE_STATION_NAME);
        } else {
            stationsPerLine.get(lineName).add(stationName);
        }

        if (number <= lastNumber || number < 0) {
            errors.put("Declaration", ERROR_KIND.WRONG_NUMBER_VALUE);
        }
        lastNumber = number;

        return null;
    }

    @Override
    public Object visit(ASTChaine n) {
        return null;
    }

    @Override
    public Object visit(ASTNombre n) {
        return null;
    }
}

enum ERROR_KIND {
    EMPTY_LINE_LIST,
    EMPTY_LINE_NAME,
    DUPLICATE_LINE_NAME,
    EMPTY_STATION_NAME,
    DUPLICATE_STATION_NAME,
    WRONG_NUMBER_VALUE,
    UNREACHABLE_LINE
}
