class Integer
    def bit_length
      return 1 if self == 0
      (Math.log(self + 1, 2)).ceil
    end
  end
  
#   puts 0.bit_length  # Affiche 1
#   puts 2023.bit_length  # Affiche 11


  def fibonacci(n)
    a, b = 0, 1
    n.times do
      a, b = b, a + b
    end
    a
  end
  
  puts fibonacci(0)  # Affiche 0
  puts fibonacci(1)  # Affiche 1
  puts fibonacci(2)  # Affiche 1
  puts fibonacci(3)  # Affiche 2
  puts fibonacci(4)  # Affiche 3
  puts fibonacci(5)  # Affiche 5
  puts fibonacci(6)  # Affiche 8
  