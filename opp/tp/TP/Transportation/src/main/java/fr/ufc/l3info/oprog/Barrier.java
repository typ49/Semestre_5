package fr.ufc.l3info.oprog;

import java.util.Map;

/**
 * La classe Barrier représente une barrière de contrôle d'accès dans un réseau de métro.
 */
public class Barrier {

    // Réseau de métro associé à cette barrière
    final private Network associatedNetwork;

    // Nom de la station où se trouve cette barrière
    final private String stationName;

    // Structure de tarification associée à cette barrière
    final private Map<Double, Integer> tariffStructure;

    /**
     * Constructeur privé pour créer une instance de Barrier.
     *
     * @param associatedNetwork Le réseau de métro associé
     * @param stationName       Le nom de la station
     * @param tariffStructure   La structure de tarification
     */
    private Barrier(Network associatedNetwork, String stationName, Map<Double, Integer> tariffStructure) {
        this.associatedNetwork = associatedNetwork;
        this.stationName = stationName;
        this.tariffStructure = tariffStructure;
    }

    /**
     * Méthode statique pour instancier un objet Barrier en vérifiant la validité des paramètres.
     *
     * @param associatedNetwork Le réseau de métro
     * @param stationName       Le nom de la station
     * @param tariffStructure   La structure de tarification
     * @return Une instance de Barrier si les paramètres sont valides, sinon null
     */
    public static Barrier build(Network associatedNetwork, String stationName, Map<Double, Integer> tariffStructure) {
        // Vérifications de base
        if (associatedNetwork == null || stationName == null || tariffStructure == null || !associatedNetwork.isValid() || associatedNetwork.getStationByName(stationName) == null || tariffStructure.isEmpty()) {
            return null;
        }

        // Vérification des prix
        for (Map.Entry<Double, Integer> entry : tariffStructure.entrySet()) {
            if (entry.getKey() < 0 || entry.getValue() <= 0 || entry.getValue() % 10 != 0) {
                return null;
            }
        }

        // Vérification de la présence d'un tarif pour la distance 0
        if (!tariffStructure.containsKey(0.0)) {
            return null;
        }

        // vérification de l'augmentation croissante du tarif par rapport à la distance
        double previousDistance = -1.0; // Initialisation à une valeur négative pour s'assurer que la première distance est toujours supérieure
        int previousPrice = -1; // Initialisation à une valeur négative pour s'assurer que le premier prix est toujours supérieur
        for (Map.Entry<Double, Integer> entry : tariffStructure.entrySet()) {
            if (entry.getKey() <= previousDistance || entry.getValue() <= previousPrice) {
                return null;
            }
            previousDistance = entry.getKey();
            previousPrice = entry.getValue();
        }

        return new Barrier(associatedNetwork, stationName, tariffStructure);
    }


    /**
     * Méthode pour entrer dans le réseau de métro.
     *
     * @param providedTicket Le ticket utilisé pour l'entrée
     * @return true si l'entrée est autorisée, sinon false
     */
    public boolean enter(ITicket providedTicket) {
        // Vérifications de base pour l'entrée
        if (providedTicket == null || !providedTicket.isValid() || providedTicket.getEntryStation() != null || providedTicket.getAmount() <= 0) {
            return false;
        }

        // Enregistrement de la station d'entrée sur le ticket
        providedTicket.entering(this.stationName);
        return true;
    }

    /**
     * Méthode pour sortir du réseau de métro.
     *
     * @param providedTicket Le ticket utilisé pour la sortie
     * @return true si la sortie est autorisée, sinon false
     */
    public boolean exit(ITicket providedTicket) {
        // verifie que la station existe sur le réseau
        if (associatedNetwork.getStationByName(this.stationName) == null) {
            return false;
        }
        // Vérifications de base pour la sortie
        if (providedTicket == null || !providedTicket.isValid() || providedTicket.getEntryStation() == null) {
            return false;
        }

        // Calcul du coût du trajet
        double shortestDistance = associatedNetwork.distance(providedTicket.getEntryStation(), this.stationName);
        int adultFare = calculateFareForDistance(shortestDistance);
        int childFare = (int) Math.ceil(adultFare / 2.0);

        // Vérification du solde du ticket
        if (providedTicket.isChild()) {
            if (providedTicket.getAmount() < childFare) {
                return false; // Refuse la sortie si le solde est insuffisant
            }
        } else {
            if (providedTicket.getAmount() < adultFare) {
                return false; // Refuse la sortie si le solde est insuffisant
            }
        }
        providedTicket.invalidate();
        return true;
    }


    /**
     * Calcule le tarif associé à une distance parcourue en utilisant la structure de tarification.
     *
     * @param traveledDistance La distance parcourue par l'usager.
     * @return Le tarif associé à cette distance.
     */
    private int calculateFareForDistance(double traveledDistance) {

        // Étape 1 : Convertir l'ensemble des clés (distances) de la structure de tarification en un flux (stream).
        // Ceci permet d'effectuer des opérations en chaîne sur cet ensemble.
        // Étape 2 : Filtrer le flux pour ne conserver que les distances inférieures ou égales à la distance parcourue.
        // Étape 3 : Trouver la plus grande distance parmi celles filtrées. Cette distance sert de seuil pour définir le tarif.
        Double key = tariffStructure.keySet().stream()
                .filter(distance -> distance <= traveledDistance)
                .max(Double::compareTo)
                .orElse(null);

        // Étape 4 : Renvoyer le tarif associé à la distance/seuil trouvée.
        // Si aucune distance correspondante n'est trouvée, renvoyer une valeur par défaut (ici, Integer.MAX_VALUE).
        return key != null ? tariffStructure.get(key) : Integer.MAX_VALUE;
    }
}