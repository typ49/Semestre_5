package fr.ufc.l3info.oprog.parser;

/**
 * Interface de visiteur pour les arbres de syntaxe abstraite utilisant des noeuds ASTNode.
 */
public interface ASTNodeVisitor {
    
    public Object visit(ASTNode n);
    public Object visit(ASTListeLignes n);
    public Object visit(ASTLigne n);
    public Object visit(ASTDeclaration n);
    public Object visit(ASTChaine n);
    public Object visit(ASTNombre n);

}
