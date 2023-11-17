def fact(n)
    return 1 if n <= 1
    n * fact(n - 1)
  end
  
  puts fact(5)  # Exemple d'utilisation
  puts fact(10)