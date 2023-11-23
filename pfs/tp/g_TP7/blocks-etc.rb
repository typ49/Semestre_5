#  Coding: utf-8
#  ============================================================================
#
#            Some advanced notions within Ruby
#
#  ============================================================================
#
#  Author: J.-M. Hufflen
#  Date: October 2023
#
#  ============================================================================

#  ============================================================================
#  Blocks, procedures, methods

$a0 = [7,11,2023]
p($a0.each do |x| p x end)
p($a0.each &(method(:p)))
p($a0.map do |x| x + 1 end)

def plusone(x)
  x + 1
end

p plusone(2023)
$m = method(:plusone)
p $m.class       #  Prints "Method".
                 #  Note that "new" cannot be invoked for this class.
p $m.arity       #  1
p $m.call(2023)  #  The "call" method can be invoked by means of "[...]".
p $a0.map(&$m)

def plussomething(x,y = 0)
  x + y + 1
end

p plussomething(2023)
p plussomething(2023,2)
$m0 = method(:plussomething)
p $m0.class       #  Method
p $m0.arity       #  -2 = - 1 - 1
p $m0.call(2023)   
p $m0.call(2023,2)
p $a0.map(&$m0)

def mk_average_with(x)
  Proc.new do |y| (x + y) / 2 end
end

p(mk_average_with(10)[12])
$average_with_10 = mk_average_with(10)
p $average_with_10.arity  #  Results in:  1
p $a0.map(&$average_with_10)
p $a0.map(&(mk_average_with(10)))

class Integer
  #
  def plustwice(x)
    #  See some examples below.
    self + 2 * x
  end
  #
end

p 2023.plustwice(2)
$m1 = 2023.method(:plustwice)
p $m1.class  #  Method
p $m1.arity  #  1
p $m1.call(2)
p $a0.map(&$m1)

$p0 = :+.to_proc
p $p0.class  #  Proc
p $p0.arity  #  -1 (dispatch)
p $p0.call(2023,12)
p $p0.call('master ','players')

def factorial(x)
  x < 0 ? nil : (1..x).inject(1,&(:*.to_proc))
end

#  ============================================================================
#  Playing with exceptions

def testexceptions(s)
  begin
    begin
      eval s
    rescue ZeroDivisionError
      p 0
      1
    else
      p 2
      3
    ensure
      p 4
      5
    end
  rescue StandardError => e
    p e.message
    p 6
    7
  else
    p 8
    9
  ensure
    p 10
    11
  end
end

p(testexceptions('2023'))
p(testexceptions('1/0'))
p(testexceptions "open('nonexistingfile')")

def testexceptionsv2
  begin
    begin
      yield
    rescue ZeroDivisionError
      p 0
      1
    else
      p 2
      3
    ensure
      p 4
      5
    end
  rescue StandardError => e
    p e.message
    p 6
    7
  else
    p 8
    9
  ensure
    p 10
    11
  end
end

p(testexceptionsv2 do p 'OK'
  2022
end)

p(testexceptionsv2 do p 'Division by zero'
  1 / 0
end)

p(testexceptionsv2 do p 'Opening file failed'
  open 'nonexistingfile'
end)

#  ============================================================================
#  Escaping

class Array
  #
  public
    #
    def find_one
      catch :exit do
        rec_find_one do |x| yield(x) end
      end
    end
    #
    def select_one
      catch :exit do
        [rec_find_one do |x| yield(x) end]
      end
    end
    #
  private
    #
    def rec_find_one
      result = []
      each do |x|
        if yield(x)
          if result.empty?
            result.push(x)
          else
            throw :exit,nil
          end
        end
      end
      if result.empty?
        throw :exit  #  "...,nil" est la convention par défaut.
      else
        result.first
      end
    end
    #
end 

def sort_both_fixnums_strings_only(a)
  catch :exit do
    r_for_fixnum = /\A\s*[-+]?[0-9]+\s*\z/
    a.map do |x|
      x_class = x.class
      if x_class == Integer
        [0,x]
      elsif x_class != String
        throw :exit
      elsif x =~ r_for_fixnum
        [0,x.to_i]
      else
        [1,x]
      end
    end.
   sort! do |c,c0|  #  "c" et "c0" sont des tableaux à deux éléments.
     first_test = c.first <=> c0.first
     first_test == 0 ? c.last <=> c0.last : first_test
   end.
   map! do |c| c.last end
  end
end

p sort_both_fixnums_strings_only(['Tuesday',15,11,2022,'November',-273,'Fall'])
  
def findanywhere(x,y)
  catch :exit do
    search = Proc.new do |y0|
      if y0.instance_of?(Array)
        #  La méthode générale  "instance_of?" teste si un objet est bien de la
        #  classe donnée, sans tenir compte des super-classes, comme le font
        #  les deux méthodes synonymes "is_a?" et "kind_of?".
        y0.each do |y1| search[y1] end
      elsif y0 == x 
        throw :exit,true
      end
    end
    search[y]
    false
  end
end

p findanywhere(2023,[5,[11,[2022,2023,2024]]])
p findanywhere(2023,[5,[11,[2022,2033,2034]]])

def findanywhere_v2(x,y)
  search = Proc.new do |y0|
    if y0.instance_of?(Array)
      y0.each &search
    elsif y0 == x
      return true
    end
  end
  search[y]
  false
end

def palindrome(s)
  forwardindex = 0
  backwardindex = s.size - 1
  while forwardindex < backwardindex
    return false if s[forwardindex] != s[backwardindex]
    forwardindex += 1
    backwardindex -= 1
  end
  true
end

p findanywhere_v2(2023,[5,[11,[2022,2023,2024]]])
p findanywhere_v2(2023,[5,[11,[2022,2033,2034]]])

##  Comparaison entre échappements dynamiques et lexicaux

p((catch (:escape_from_New_York) do
     #  Exemple d'utilisation d'échappements dynamiques. 
     f0 = Proc.new do |x| throw :escape_from_New_York,x end
     1 + (catch (:escape_from_New_York) do f0[2] end)
   end))

def escape_from_LA
  #  Exemple d'utilisation d'échappements lexicaux.
  Proc.new do 
    f1 = Proc.new do |x| return x end
    1 + (Proc.new do
          f1[2] 
          2022
      end)[]
  end[]
end

p escape_from_LA

