#!/bin/bash

# Afficher l'en-tête
echo "PID     CMD                 THREADS"

# Utiliser ps pour lister tous les processus et threads, puis awk pour filtrer les processus multithreadés
ps -eLf | awk '$4 > 1 {printf "%-8s %-20s %s\n", $4, $14, $5}'
