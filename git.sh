#!/bin/bash

# Vérifie si un message de commit a été fourni
if [ -z "$1" ]; then
  echo "Erreur : Aucun message de commit fourni."
  exit 1
fi

# Exécute les commandes Git
git add .
git commit -m "$1"
git push
git status
