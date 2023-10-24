package fr.ufc.l3info.oprog.parser;

import fr.ufc.l3info.oprog.Network;
import org.junit.Test;

import java.io.File;
import java.io.IOException;
import java.util.List;

import static org.junit.Assert.*;

/**
 *  Quelques tests pour le package parser.
 */
public class NetworkParserTest {

    /** Chemin vers les fichiers de test */
    final String path = "./target/classes/data/";

    /** Instance singleton du parser de stations */
    final NetworkParser parser = NetworkParser.getInstance();

    @Test
    public void testTokenizer() throws NetworkParserException, IOException {
        List<Token> tokens = NetworkFileTokenizer.tokenize(new File(path + "metroOK.txt"));
        assertEquals(53, tokens.size());
        String[] expected = {
                "ligne", "\"line0\"", "{",
                "\"s0\"", ":", "0.0", ",", "\"s1\"", ":", "1", ",", "\"s2\"", ":", "4", ",", "\"s3\"", ":", "6", ",", "\"s4\"", ":", "7",
                "}",
                "ligne", "\"line1\"", "{",
                "\"s0\"", ":", "0.0", ",", "\"s3\"", ":", "3.0", ",", "\"s5\"", ":", "5.0",
                "}",
                "ligne", "\"line2\"", "{",
                "\"s1\"", ":", "0.0", ",", "\"s5\"", ":", "2", ",", "\"s4\"", ":", "5.0", "}"
        };
        for (int i=0; i < expected.length; i++) {
            assertEquals(expected[i], tokens.get(i).getValeur());
        }
        assertEquals(1, tokens.get(0).getLigne());
        assertEquals(1, tokens.get(0).getColonne());
        assertEquals(16, tokens.get(tokens.size()-1).getLigne());
        assertEquals(15, tokens.get(tokens.size()-1).getColonne());
    }


    @Test
    public void testParserOK() throws NetworkParserException, IOException {
        ASTNode n = parser.parse(new File(path + "metroOK.txt"));
        assertTrue(n instanceof ASTListeLignes);
        assertEquals(3, n.getNumChildren());

        assertEquals(6, n.getChild(0).getNumChildren());
        assertEquals(4, n.getChild(1).getNumChildren());
        assertEquals(4, n.getChild(2).getNumChildren());

        for (ASTNode n1 : n) {
            assertTrue(n1 instanceof ASTLigne);
            for (ASTNode nn1 : n1) {
                if (!(nn1 instanceof ASTChaine)) {
                    assertTrue(nn1 instanceof ASTDeclaration);
                    assertTrue(nn1.getChild(0) instanceof ASTChaine);
                    assertTrue(nn1.getChild(1) instanceof ASTNombre);
                }
            }
        }
    }


    @Test
    public void testNetworkBuilder() throws IOException, NetworkParserException {
        ASTNode n = parser.parse(new File(path + "metroOK.txt"));
        NetworkBuilder builder = new NetworkBuilder();
        n.accept(builder);
        Network net = builder.getNetwork();
        assertEquals(3, net.getLines().size());
        assertNotNull(net.getStationByName("s3"));
        assertTrue(net.getStationByName("s3").getLines().contains("line0"));
        assertTrue(net.getStationByName("s3").getLines().contains("line1"));
    }

    @Test(expected = NetworkParserException.class)
    public void testParserKO1() throws NetworkParserException, IOException { // slash instead of semicolon
        parser.parse(new File(path + "metro_slashInsteadOfSemicolon.txt"));
    }

    @Test(expected = NetworkParserException.class)
    public void testParserKO2() throws NetworkParserException, IOException { // missing comma
        parser.parse(new File(path + "metro_missingComma.txt"));
    }

    @Test(expected = NetworkParserException.class)
    public void testParserKO3() throws NetworkParserException, IOException { // simple quote instead of double quote
        parser.parse(new File(path + "metro_simpleQuoteInsteadOfDoubleQuote.txt"));
    }

    @Test(expected = NetworkParserException.class)
    public void testParserKO4() throws NetworkParserException, IOException { // missing double quote
        parser.parse(new File(path + "metro_missingDoubleQuote.txt"));
    }

    @Test(expected = NetworkParserException.class)
    public void testParserKO5() throws NetworkParserException, IOException { // missing opening brace
        parser.parse(new File(path + "metro_missingOpeningBrace.txt"));
    }

}