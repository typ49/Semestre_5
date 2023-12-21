class Array
    def halfapply
      result = []
      self.each_with_index do |element, index|
        result.push(yield(element)) if index.even?
      end
      result
    end
  end
  
  # Exemple d'utilisation
  $a1 = [21, 10, 2023, 2024]
  puts $a1.halfapply { |x| x + 1 }.inspect  # Devrait afficher [22, 2024]
  