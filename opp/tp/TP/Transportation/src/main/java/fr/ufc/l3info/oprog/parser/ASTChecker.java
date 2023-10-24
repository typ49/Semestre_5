package fr.ufc.l3info.oprog.parser;

import java.util.HashMap;
import java.util.Map;

/**
 * Vérification du fichier parsé :
 *  - liste de lignes non vide
 *  - nom de ligne vide
 *  - nom de stations non vides
 *  - noms de stations uniques sur une ligne
 *  - distance kilométrique positive
 *  - distance kilométrique strictement croissante à partir de 0
 *  - toutes les lignes sont accessibles
 */
public class ASTChecker implements ASTNodeVisitor {

    private HashMap<String, ERROR_KIND> errors;

    public ASTChecker() {
        errors = new HashMap<>();
    }

    public Map<String, ERROR_KIND> getErrors() {
        return new HashMap<>(errors);
    }

    @Override
    public Object visit(ASTNode n) {
        return null;
    }

    @Override
    public Object visit(ASTListeLignes n) {
        return null;
    }

    @Override
    public Object visit(ASTLigne n) {
        return null;
    }

    @Override
    public Object visit(ASTDeclaration n) {
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