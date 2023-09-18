#!/bin/bash

# Vérifier si un argument a été fourni
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 /chemin/vers/votre/dossier"
    exit 1
fi

# Utiliser l'argument comme chemin du dossier
DIR="$1"

# Aller dans le dossier
cd "$DIR" || { echo "Erreur : Impossible d'accéder au dossier '$DIR'"; exit 2; }

# Convertir les images dans l'ordre numérique en un PDF
convert $(ls -v *.png) output.pdf
