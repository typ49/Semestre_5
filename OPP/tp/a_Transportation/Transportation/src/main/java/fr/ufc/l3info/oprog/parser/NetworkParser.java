package fr.ufc.l3info.oprog.parser;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Parser pour lire un fichier décrivant des stations.
 */
public class NetworkParser {

    private int current = 0;
    private List<Token> tokens = null;

    /**
     * Constructeur privé pour empêcher d'instancier la classe. 
     */
    private NetworkParser() {    }

    private static NetworkParser INSTANCE = null;

    public static NetworkParser getInstance() {
        if (INSTANCE == null) {
            INSTANCE = new NetworkParser();
        }
        return INSTANCE;
    }

    /**
     * Réalise une analyse syntaxique du fichier passé en paramètre et construit l'arbre de syntaxe abstraite correspondant.
     * @param f le fichier format texte à analyser
     * @return la racine de l'arbre de syntaxe abstraite
     * @throws IOException si le fichier n'a pas pu être lu
     * @throws NetworkParserException si l'analyse a révélé des erreurs de parsing.
     */
    public ASTNode parse(File f) throws IOException, NetworkParserException {
        tokens = NetworkFileTokenizer.tokenize(f);
        current = 0;
        return listeStation();
    }

    /**
     * Lit le token courant et passe au suivant. 
     * @return le token courant
     * @throws NetworkParserException si la fin du fichier est rencontrée.
     */
    private Token nextToken() throws NetworkParserException {
        if (current >= tokens.size()) {
            int l = tokens.get(tokens.size()-1).getLigne();
            int c = tokens.get(tokens.size()-1).getColonne();
            throw new NetworkParserException("Fin de fichier inattendue", l, c);
        }
        Token t = tokens.get(current);
        current++;
        return t;
    }
    
    private ASTNode listeStation() throws NetworkParserException {
        ASTNode r = new ASTListeLignes(1, 1);
        while (current < tokens.size()) {
            ASTNode s = ligne();
            r.addChild(s);
        }
        return r;
    }

    private ASTNode ligne() throws NetworkParserException {

        Token tok = this.nextToken();
        if (! tok.getValeur().equalsIgnoreCase("ligne")) {
            throw new NetworkParserException("attendu : \"ligne\"", tok.getLigne(), tok.getColonne());
        }
        ASTNode r = new ASTLigne(tok.getLigne(), tok.getColonne());

        tok = nextToken();
        String nom = tok.getValeur();
        if (!(nom.length() > 1 && nom.charAt(0) == '"' && nom.charAt(nom.length()-1) == '"')) {
            throw new NetworkParserException("attendu : littéral de chaîne", tok.getLigne(), tok.getColonne());
        }
        r.addChild(new ASTChaine(nom, tok.getLigne(), tok.getColonne()));

        tok = nextToken();
        if (! tok.getValeur().equals("{")) {
            throw new NetworkParserException("attendu : {", tok.getLigne(), tok.getColonne());
        }

        do {
            ASTNode decl = declaration();
            r.addChild(decl);
            tok = nextToken();
        }
        while (tok.getValeur().equals(","));

        if (! tok.getValeur().equals("}")) {
            throw new NetworkParserException("attendu : } ou ,", tok.getLigne(), tok.getColonne());
        }

        return r;
    }

     private ASTNode declaration() throws NetworkParserException {
        Token tok = this.nextToken();
        ASTNode r = new ASTDeclaration(tok.getLigne(), tok.getColonne());

         String nom = tok.getValeur();
         if (!(nom.length() > 1 && nom.charAt(0) == '"' && nom.charAt(nom.length()-1) == '"')) {
             throw new NetworkParserException("attendu : littéral de chaîne", tok.getLigne(), tok.getColonne());
         }
         ASTChaine nomStation = new ASTChaine(tok.getValeur(), tok.getLigne(), tok.getColonne());
         r.addChild(nomStation);

         tok = this.nextToken();
         if (! tok.getValeur().equals(":")) {
            throw new NetworkParserException("attendu : \":\"", tok.getLigne(), tok.getColonne());
         }

         tok = this.nextToken();
         if (! tok.getValeur().matches("^\\-?\\d+(\\.\\d+)?$")) {
            throw new NetworkParserException("attendu : un nombre", tok.getLigne(), tok.getColonne());
         }
         ASTNombre num = new ASTNombre(tok.getValeur(), tok.getLigne(), tok.getColonne());
         r.addChild(num);

         return r;
     }

}



/**
 * Classe utilitaire permettant de découper le fichier en une suite de tokens. 
 */
class NetworkFileTokenizer {

    /**
     * Transforme le texte d'un fichier en une suite de tokens : identifiants, accolades, chaines, ponctuations, etc.
     * Seuls les espaces et les séparateurs classiques (tabulations, retours à la ligne, etc.) sont ignorés. 
     * @param f le fichier à découper
     * @return une liste de tokens représentant les tokens ainsi identifiés.
     * @throws IOException si le fichier n'a pas pu être lu.
     * @throws NetworkParserException si une erreur d'analyse lexicale a eu lieu.
     */
    public static List<Token> tokenize(File f) throws IOException, NetworkParserException {
        FileReader fr = new FileReader(f);
        List<Token> r = new ArrayList<>();
        int lu, nc = 0, nl = 1;
        Token token = new Token();
        boolean inQuotes = false;
        while ((lu = fr.read()) != -1) {
            char charLu = (char) lu;
            nc++;
            if (charLu == '\n') {
                nc = 0;
                nl++;
            }
            switch (charLu) {
                case '\t':
                case ' ':
                    if (inQuotes) {
                        token.append(charLu, nl, nc);
                        break;
                    }
                case '\n':
                case '\r':
                    if (inQuotes) {
                       throw new NetworkParserException("Unexpected end of String literal", nl, nc);
                    }
                    if (token.length() > 0) {
                        r.add(token);
                        token = new Token();
                    }
                    break;
                case '{':
                case ':':
                case ',':
                case '}':
                    if (token.length() > 0) {
                        r.add(token);
                        token = new Token();
                    }
                    token.append(charLu, nl, nc);
                    r.add(token);
                    token = new Token();
                    break;
                case '"':
                    if (inQuotes) {
                        token.append(charLu, nl, nc);
                        r.add(token);
                        token = new Token();
                        inQuotes = false;
                    }
                    else {
                        if (token.length() > 0) {
                            throw new NetworkParserException("Unexpected beginning of String literal", nl, nc);
                        }
                        token.append(charLu, nl, nc);
                        inQuotes = true;
                    }
                    break;
                default:
                    token.append(charLu, nl, nc);
                    break;
            }
        }
        if (token.length() > 0) {
            r.add(token);
        }
        return r;
    }

}

/**
 * Classe gérant les tokens.
 */
class Token {

    String valeur;
    int l, c;

    public Token() {
        valeur = "";
    }

    public int getLigne() {
        return l;
    }

    public int getColonne() {
        return c;
    }

    public void append(char c, int nl, int nc) {
        if (valeur.length() == 0) {
            this.l = nl;
            this.c = nc;
        }
        valeur += c;
    }

    public int length() {
        return valeur.length();
    }

    public String getValeur() {
        return valeur;
    }

    public String toString() { return "[" + valeur + "|(" + l + "," + c + ")]"; }
}

