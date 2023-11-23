# coding: utf-8
#  ============================================================================
#
#            Definitions for Lab Class #7
#
#  ============================================================================
#
#  Author: J.-M. Hufflen
#  Date: November 2023
#
#  ============================================================================

#  Total size of a directory's contents

require 'find'

class File
  #
  def self.whole_size(path)
    if File.directory?(path)
      size_accumulator = 0
      Dir.glob(File.join(path,'**','*')) do |path_0|
        size_accumulator += File.size(path_0) if File.file?(path_0)
      end
      size_accumulator
    else
      File.size(path)
    end
  end
  #
end

#  Checking that the 'UTF-8' encoding is known.

$s = "l'été"
p $s.encoding
p Encoding.class
p Encoding.find('UTF-8')
p Encoding.find('locale')

class String
  #
  ACCENTED_LETTER_MAPPING = { 
    'A' => [192,193,194,195,196,197],'C' => [199],'E' => [200,201,202,203],
    'I' => [204,205,206,207],'N' => [209],'O' => [210,211,212,213,214,216],
    'U' => [217,218,219,220],'Y' => [221],'a' => [224,225,226,227,228,229,230],
    'c' => [231],'e' => [232,233,234,235],'i' => [236,237,238,239],
    'n' => [241],'o' => [242,243,244,245,246,248],'u' => [249,250,251,252],
    'y' => [253,255],'AE' => [306],'ae' => [346],'OE' => [188],'oe' => [189]
  }
  #
  def removeaccents    
    str = String.new(self)
    String::ACCENTED_LETTER_MAPPING.each do |letter,accents|
      packed = accents.pack('U*')
      rxp = Regexp.new("[#{packed}]",nil)
      str.gsub!(rxp,letter)
    end
    str
  end
  # 
end

# traduit une chaine de caracètre en entier avec une expression régulière
def string_to_int(s)
  if s =~ /\A\s*[-+]?[0-9]+\s*\z/
    s.to_i
  else
    nil
  end
end

# trie un tableau d'entier et de chaine de caractère
# en mettant les entiers en premier, dans l'ordre croissant (les chaine de caractère convertible en entier sont aussi pris en compte)
# puis les chaines de caractère, dans l'ordre lexicographique
def sort_both_integers_strings(a0)
  # Séparer les entiers et les chaînes de caractères
  integers = a0.select { |item| item.is_a?(Integer) || (item.is_a?(String) && item =~ /\A\s*[-+]?[0-9]+\s*\z/) }
  strings = a0.select { |item| item.is_a?(String) && item !~ /\A\s*[-+]?[0-9]+\s*\z/ }

  # Convertir les chaînes de caractères en entiers
  integers.map! { |item| item.is_a?(String) ? string_to_int(item) : item } # Fix: Use ternary operator to handle non-string items

  # Trier les entiers et les chaînes de caractères
  integers.sort!
  strings.sort!

  # Combiner les entiers et les chaînes de caractères
  integers + strings
end

# retourne une tableau de toutes les adresses de fichiers pdf dans le répertoire path ou appartenant à un sous-répertoire de path 
# en utilisant find du module Find
def look_4_pdf_files(path)
  pdf_files = []
  Find.find(path) do |path_0|
    pdf_files << path_0 if File.extname(path_0) == '.pdf'
    # avec une expression régulière
    # pdf_files << path_0 if path_0 =~ /\.pdf\z/
  end
  pdf_files
end

# retourne le tableau de tous les sous-répertoires, trié par ordre croissant de la place occupé par ces divers sous-répertoires
def sort_by_size(path)
  subdirectories = []
  Find.find(path) do |path_0|
    subdirectories << path_0 if File.directory?(path_0)
  end
  subdirectories.sort_by { |subdirectory| File.whole_size(subdirectory) }
end
  


#  ============================================================================
# Tests

def test_sort_both_integers_strings
  a0 = ['Tuesday',15,11,2022,'November','-273', '36 30', 'Cyberpunk 2077','Fall']
  a1 = sort_both_integers_strings(a0)
  p a0
  p a1
end

def test_look_4_pdf_files
  p look_4_pdf_files('/home/juliengauthier410/l3/s5/pfs/tp')
end

def test_sort_by_size
  p sort_by_size('/home/juliengauthier410/l3/s5/pfs/tp')
end

test_sort_both_integers_strings

test_look_4_pdf_files

test_sort_by_size

puts "File.size : #{File.size('/home/juliengauthier410/l3/s5/pfs/tp/')}"
puts "File.whole.size : #{File.whole_size('/home/juliengauthier410/l3/s5/pfs/tp/')}"