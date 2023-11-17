def powerfultwice(x = 2023)
    if block_given?
      yield(yield(x))
    else
      x
    end
  end
  
  # Exemples d'utilisation
  puts powerfultwice                         # Devrait afficher 2023
  puts powerfultwice(2022)                   # Devrait afficher 2022
  puts powerfultwice { |x| x + 1 }           # Devrait afficher 2025
  puts powerfultwice(2024) { |x| x + 1 }     # Devrait afficher 2026
  