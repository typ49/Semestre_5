#!/bin/bash

# Générer le message de commit
commit_message="save $(date +'%d-%b-%Y_%H%M' | tr '[:upper:]' '[:lower:]')"

# Exécuter les commandes Git
git add .
git commit -m "$commit_message"
git push
echo "git status :"
echo "-----------------------------------------------------------------------"
git status

# Affichez la taille du répertoire courant en gigaoctets
echo "Taille du répertoire courant :"
du -sh
