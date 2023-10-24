package fr.ufc.l3info.oprog.parser;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * Noeud d'arbre de syntaxe abstraite.
 */
public abstract class ASTNode implements Iterable<ASTNode> {

    String value;
    List<ASTNode> children;
    int ligne, colonne;

    public ASTNode() {
        value = null;
        children = new ArrayList<>();
    }

    public ASTNode(String v, int l, int c) {
        value = v;
        children = new ArrayList<>();
        ligne = l;
        colonne = c;
    }

    public void addChild(ASTNode n) {
        children.add(n);
    }

    public ASTNode getChild(int i) {
        return children.get(i);
    }

    public int getNumChildren() {
        return children.size();
    }

    public Iterator<ASTNode> iterator() {
        return children.iterator();
    }

    public String getLCPrefix() {
        return "(" + ligne + ";" + colonne + ")";
    }

    public String toString() {
        return value;
    }

    public void print(String prefix) {
        String suffix = value.startsWith("[") ? "" : " (" + value + ")";
        System.out.println(prefix + this.getClass().getSimpleName() + suffix);
        for (ASTNode n : children) {
            n.print(prefix + "  ");
        }
    }

    public abstract Object accept(ASTNodeVisitor v);
}


class ASTListeLignes extends ASTNode {
    public ASTListeLignes(int l, int c) {
        super("[ListeLignes]", l, c);
    }
    @Override
    public Object accept(ASTNodeVisitor v) {
        return v.visit(this);
    }
}

class ASTLigne extends ASTNode {
    public ASTLigne(int l, int c) {
        super("[Ligne]", l, c);
    }
    @Override
    public Object accept(ASTNodeVisitor v) {
        return v.visit(this);
    }
}

class ASTDeclaration extends ASTNode {
    public ASTDeclaration(int l, int c) {
        super("[Declaration]", l, c);
    }
    @Override
    public Object accept(ASTNodeVisitor v) {
        return v.visit(this);
    }
}

class ASTChaine extends ASTNode {
    public ASTChaine(String s, int l, int c) {
        super(s, l, c);
    }
    @Override
    public Object accept(ASTNodeVisitor v) {
        return v.visit(this);
    }
}

class ASTNombre extends ASTNode {
    public ASTNombre(String n, int l, int c) {
        super(n, l, c);
    }
    public double getNumberValue() {
        return Double.parseDouble(value);
    }
    @Override
    public Object accept(ASTNodeVisitor v) {
        return v.visit(this);
    }
}