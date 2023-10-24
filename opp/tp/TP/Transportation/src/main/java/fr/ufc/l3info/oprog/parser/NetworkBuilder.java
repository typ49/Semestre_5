package fr.ufc.l3info.oprog.parser;

import fr.ufc.l3info.oprog.Network;
import fr.ufc.l3info.oprog.Station;

import java.util.HashSet;
import java.util.Set;

/**
 *  Visiteur permettant de construire un réseau à partir d'un AST
 */
public class NetworkBuilder implements ASTNodeVisitor {

    private Network net;

    public Network getNetwork() {
        return net;
    }

    public NetworkBuilder() {
        this.reset();
    }

    public void reset() {
        net = new Network();
    }

    @Override
    public Object visit(ASTNode n) {
        return null;
    }

    @Override
    public Object visit(ASTListeLignes n) {
        for (ASTNode child : n) {
            child.accept(this);
        }
        return null;
    }

    @Override
    public Object visit(ASTLigne n) {
        String line = (String) n.getChild(0).accept(this);
        for (int i=1; i < n.getNumChildren(); i++) {
            Object[] decl = (Object[]) n.getChild(i).accept(this);
            String station = (String) decl[0];
            double km = (double) decl[1];
            Station s = net.getStationByName(station);
            if (s == null) {
                s = new Station(station);
                net.addStation(s);
            }
            s.addLine(line, i, km);
        }
        return null;
    }

    @Override
    public Object visit(ASTDeclaration n) {
        String key = (String) n.getChild(0).accept(this);
        Double value = (Double) n.getChild(1).accept(this);
        return new Object[] { key, value };
    }

    @Override
    public Object visit(ASTChaine n) {
        return n.toString().substring(1, n.toString().length()-1);
    }

    @Override
    public Object visit(ASTNombre n) {
        return n.getNumberValue();
    }
}
