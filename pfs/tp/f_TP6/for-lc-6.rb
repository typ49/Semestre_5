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

$a1 = [21,10,2023,2024]
$a1.each do |x| p x end
$a1.each_index do |index| p index end
$a1.each_with_index do |x,index| p [x,index] end
p($a1.select do |x| x.odd? end)
p([].inject(0) do |x,y| x + y end)
p($a1.inject(0) do |x,y| x + y end)
p((1..10).inject(1) do |x,y| x * y end)

def twice(x)
  yield(yield(x))
end

p(twice(2023) do |y| y + 1 end)

def commutativefor(x,y)
  yield(x,y) == yield(y,x)
end

p(commutativefor(2023,2022) do |x,y| x + y end)
p(commutativefor(2023,2022) do |x,y| x - y end)
