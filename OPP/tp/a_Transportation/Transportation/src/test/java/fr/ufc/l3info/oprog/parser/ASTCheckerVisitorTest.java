package fr.ufc.l3info.oprog.parser;
import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

public class ASTCheckerVisitorTest {

    private ASTCheckerVisitor visitor;

    @Before
    public void setUp() {
        visitor = new ASTCheckerVisitor();
    }

    @Test
    public void testGetErrors_Empty() {
        assertTrue("La map des erreurs devrait être vide initialement", visitor.getErrors().isEmpty());
    }

    @Test
    public void testVisit_InvalidInput() {
        ASTNode invalidNode = null; // Exemple d'entrée invalide
        Object result = visitor.visit(invalidNode);
        assertNull("Le résultat devrait être null pour une entrée invalide", result);
    }

    // Tests pour d'autres types de nœuds AST
    @Test
    public void testVisit_ASTListeLignes() {
        // constructeur de ASTListeLignes : ASTListeLignes(int l, int c)
        ASTListeLignes listeLignes = new ASTListeLignes(1, 1);
        visitor.visit(listeLignes);
        // Assertions spécifiques pour ASTListeLignes
    }

    // Ajoutez ici d'autres tests pour les différents types de nœuds AST
}
