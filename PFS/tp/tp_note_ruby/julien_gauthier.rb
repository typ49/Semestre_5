#### Julien gauthier
#### TP note ruby | PFS |2023-12-11

# 1 Tableaux

class Array
    def each_twice
        self.flat_map { |element| [element] * 2 }
    end

    def stammer
        result = []
        self.each_with_index do |element, index|
          result += [element] * (index + 1)
        end
        result
      end

      def no_stammer!
        result = []
        self.each do |element|
          result << element unless result.last == element
        end
        self.replace(result)
      end

end

# 2 Tables de hachage

class Hash
    def map_value
        result = {}
        self.each do |key, value|
          result[key] = yield(value)
        end
        result
    end

    def map_value_with(h0)
        result = {}
        self.each do |key, value|
          if h0.key?(key)
            result[key] = yield(value, h0[key])
          else
            result[key] = value
          end
        end
        result
      end
end

# 3 Chaînes de caractères et expressions régulières

class String
  def issn?
    match_data = self.match(/^\s*ISSN\s*(\d{4})-(\d{3})([\dX])\s*$/i)
    return false unless match_data

    digits = match_data[1..3].join.chars.map { |c| c == 'X' ? 10 : c.to_i }
    check_digit = digits.pop
    sum = digits.each_with_index.sum { |digit, index| digit * (8 - index) }
    check_digit == (11 - sum % 11) % 11
  end
end




### test
## Tableaux
puts "---------------------------------------------------------------------------------------------------------------------------------------------------"
puts "1 - Tableaux"
puts "---------------------------------------------------------------------------------------------------------------------------------------------------"

a = [11, 12, 2023, 24, 12, 2024]
b = [11, 12, 12, 2023, 24, 12, 12, 12, 2024]

# each_twice
puts "test each_twice :"
puts "#{a.inspect} => #{a.each_twice.inspect}" # expected [11, 11, 12, 12, 2023, 2023, 24, 24, 12, 12, 2024, 2024]
puts

# stammer
puts "test stammer :"
puts "#{a.inspect} => #{a.stammer.inspect}" # expected [11, 12, 12, 2023, 2023, 2023, 24, 24, 24, 24, 12, 12, 12, 12, 12, 2024, 2024, 2024, 2024, 2024]
puts

# no_stammer!
puts "test no_stammer! :"
puts "b => #{b.inspect}" # expected [11, 12, 12, 2023, 24, 12, 12, 12, 2024]
puts "b.no_stammer! => #{b.no_stammer!.inspect}" # expected [11, 12, 2023, 24, 12, 2024]
puts "b => #{b.inspect}" # expected [11, 12, 2023, 24, 12, 2024]
puts

## Tables de hachage
puts "---------------------------------------------------------------------------------------------------------------------------------------------------"
puts "2 - Tables de hachage"
puts "---------------------------------------------------------------------------------------------------------------------------------------------------"

h = { 11 => 12, 12 => 1, 2024 => 2025 }

# map_value
puts "test map_value :"
puts "#{h}.map_value { |value| 2 * value } => #{h.map_value { |value| 2 * value }}" # expected { 11 => 24, 12 => 2, 2024 => 4050 }
puts

# map_value_with
puts "test map_value_with :"
h0 = {12 => 2, 2024 => 2030, 2026 => 2027}
puts "#{h}.map_value_with(#{h0}) { |value, value0| value + value0 } => #{h.map_value_with(h0) { |value, value0| value + value0 }}" # expected { 11 => 12, 12 => 3, 2024 => 4055 }
puts

## Chaînes de caractères et expressions régulières
puts "---------------------------------------------------------------------------------------------------------------------------------------------------"
puts "3 - Chaînes de caractères et expressions régulières"
puts "---------------------------------------------------------------------------------------------------------------------------------------------------"

puts "test issn? :"
puts "ISSN 0395-2037 ? #{"ISSN 0395-2037".issn?}" # expected true
puts "ISSN 1273-9006 ? #{"ISSN 1273-9006".issn?}" # expected true
puts "ISSN 0029-4713 ? #{"ISSN 0029-4713".issn?}" # expected true
puts "ISSN 2416-8793 ? #{"ISSN 2416-8793".issn?}" # expected true
puts "ISSN 2607-415X ? #{"ISSN 2607-415X".issn?}" # expected true
puts "ISSN 2607-4151 ? #{"ISSN 2607-4151".issn?}" # expected false
puts "+ ISSN 2607-415X ? #{"+ ISSN 2607-415X".issn?}" # expected false
puts "ISSN 2607--415X ? #{"ISSN 2607--415X".issn?}" # expected false