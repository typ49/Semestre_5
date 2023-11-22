#!/bin/bash

# Vérifiez si le fichier a été fourni
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_file_path> <output_file_path>"
    exit 1
fi

input_file=$1
output_file=$2

echo "{" > "$output_file"
first_block=true

while IFS= read -r line || [[ -n "$line" ]]; do
    if [[ -z "$line" ]]; then
        if [ "$first_block" = false ]; then
            echo -n "}, " >> "$output_file"
        fi
        first_block=true
    else
        if [ "$first_block" = true ]; then
            echo -n "{" >> "$output_file"
            first_block=false
        else
            echo -n ", " >> "$output_file"
        fi
        echo -n "$line" >> "$output_file"
    fi
done < "$input_file"

# Fermer la dernière liste, s'il y en a une
if [ "$first_block" = false ]; then
    echo "}" >> "$output_file"
fi

echo "}" >> "$output_file"

echo "Fichier transformé avec succès en $output_file"

exit 0
