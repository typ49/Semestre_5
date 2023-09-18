#!/bin/bash

# Vérifiez si les arguments nécessaires ont été fournis
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 /chemin/vers/le/dossier numero_de_depart"
    exit 1
fi

DIR="$1"
COUNTER="$2"

# Vérifiez si le chemin fourni est un dossier
if [ ! -d "$DIR" ]; then
    echo "Erreur: $DIR n'est pas un dossier valide."
    exit 2
fi

# Vérifiez si le deuxième argument est un nombre
if ! [[ "$COUNTER" =~ ^[0-9]+$ ]]; then
    echo "Erreur: Le deuxième argument doit être un nombre."
    exit 3
fi

# Parcourez tous les fichiers du dossier
for file in "$DIR"/*; do
    # Récupérez l'extension du fichier
    EXT="${file##*.}"
    # Construisez le nouveau nom du fichier
    NEW_NAME="$DIR/$COUNTER.$EXT"
    # Renommez le fichier
    mv "$file" "$NEW_NAME"
    # Incrémente le compteur
    COUNTER=$((COUNTER+1))
done

echo "Les fichiers ont été renommés avec succès!"
