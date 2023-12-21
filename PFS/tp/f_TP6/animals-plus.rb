#  ============================================================================
#  Examples of classes using Ruby --- January 2017

class Animal
  #
  attr_reader :name
  #
  def initialize(name_0)
    @name = name_0   #  "@...":  instance variable,
    @@animalnb += 1  #  "@@...": static   .........
  end
  #

  def eat
    puts 'I\'m eating.'
  end
  #

  @@animalnb = 0
  #

  def self.nb
    @@animalnb
  end
  #

  CrDate = '2016/12/13'
  #

end # class Animal

class Dog < Animal
  #
  attr_reader :ownername
  #
  def initialize(name_0,ownername_0)
    super(name_0)
    @ownername = ownername_0
  end
  #
  def bark
    puts 'I\'m barking.'
  end
  #
  class << self
    undef_method :nb
  end
  #
end

class Monkey < Animal
  #
  def climb
    puts 'I\'m climbing.'
  end
  #
  class << self
    undef_method :nb
  end
  #
end

#  "$<variable-name>": global variable.
$a = Animal.new('Baloo')
p $a.name
$a.eat


$d = Dog.new('Rintintin','Bonanza')
p $d.name
$d.eat
$d.bark
p $d.ownername

$m = Monkey.new('Snow Flake')
p $m.name
$m.eat
$m.climb


p Animal.nb

$kingkong = Monkey.new('King-Kong')

def $kingkong.skullisland
  puts 'I\'m at home.'
end


class << $kingkong
  #
  def empirestatebuilding
    puts 'That\'s the end, my friend.'
  end
  #
end

class Dog
  #
  def bite
    puts 'Biting U!'
  end
  #
  def do_with_names_and(x)
    yield(@name,@ownername,x)
  end
  #
end

$d.bite

puts Animal::CrDate

class Animal
  #
  def gimme
    puts 'Gimme, gimme ' + yield
  end
  #
  def do_with_name
    puts "I'm #{@name}"
    yield(@name) if block_given?
  end
  #
end


$kingkong.gimme do 'A man after midnight' end # Gimme, gimme A man after midnight
$kingkong.do_with_name # I'm King-Kong
$kingkong.do_with_name do |name| puts "#{name} and Jane" end # I'm King-Kong \nKing-Kong and Jane


$d.do_with_names_and('John Wayne') do |name, owner, x| puts "#{name}, #{owner}, played by #{x}" end # => "Rintintin, Bonanza, played by John Wayne"


class Animal
  #
  def with(another_animal)
    if self.class == another_animal.class
      puts "#{self.name} with #{another_animal.name} for #{yield}"
      true
    else
      false
    end
  end
  #
end # class Animal


result = $kingkong.with($m) do 'getting Jane' end
p result

result = $a.with($d) do 'anything' end
p result
