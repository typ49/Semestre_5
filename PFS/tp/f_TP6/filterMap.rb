def filtermap(a0, regexp)
    a0.select { |item| item.to_s.match(regexp) }
      .map { |filtered_item| yield(filtered_item) }
  end
  
  # Exemple d'utilisation
  result = filtermap(['2023-11-09', '12', 11, 2023, '11-09-2023'], /2023/) do |x|
    [x]
  end
  puts result.inspect  # Devrait afficher [["2023-11-09"], ["11-09-2023"]]