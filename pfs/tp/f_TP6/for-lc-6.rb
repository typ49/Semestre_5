#  ============================================================================
#
#            Sixth lab class --- Some Ruby definitions given to students
#
#  ============================================================================
#
#  Author: J.-M. Hufflen
#  Date: October 2023
#
#  ============================================================================

# $a1 = [21,10,2023,2024]
# $a1.each do |x| p x end
# $a1.each_index do |index| p index end
# $a1.each_with_index do |x,index| p [x,index] end # [21,0] [10,1] [2023,2] [2024,3]
# p($a1.select do |x| x.odd? end) # 21, 2023

# p([].inject(0) do |x,y| x + y end) # 0
# p($a1.inject(0) do |x,y| x + y end) # 2023 + 21 + 10 + 2024 = 4078

# p((1..10).inject(1) do |x,y| x * y end) # 1*2*3*4*5*6*7*8*9*10 = 3628800

def twice(x)
  yield(yield(x))
end

p(twice(2023) do |y| y + 1 end) # 2025

def commutativefor(x,y)
  yield(x,y) == yield(y,x)
end

p(commutativefor(2023,2022) do |x,y| x + y end)
p(commutativefor(2023,2022) do |x,y| x - y end)
