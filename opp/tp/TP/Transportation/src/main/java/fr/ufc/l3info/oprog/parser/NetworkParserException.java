package fr.ufc.l3info.oprog.parser;

/**
 * Classe d'exception symbolisant une erreur d'analyse lexicale ou syntaxique.
 */
public class NetworkParserException extends Exception {
    public NetworkParserException(String msg, int l, int c) {
        super("Erreur de parsing (" + l + ":" + c + ") : " + msg);
    }
}
