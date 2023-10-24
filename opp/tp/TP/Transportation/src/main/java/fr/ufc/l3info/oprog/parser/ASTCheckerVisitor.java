package fr.ufc.l3info.oprog.parser;

public interface ASTCheckerVisitor {

    public Object visit(ASTNode n);

    public Object visit(ASTListeLignes n);

    public Object visit(ASTLigne n);

    public Object visit(ASTDeclaration n);

    public Object visit(ASTChaine n);

    public Object visit(ASTNombre n);
}
