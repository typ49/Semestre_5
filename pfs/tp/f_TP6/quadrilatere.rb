class Quadrilateral
    attr_reader :l0, :l1, :l2, :l3, :alpha0, :alpha1, :alpha2
  
    def initialize(l0, l1, l2, l3, alpha0, alpha1, alpha2) # alpha0 = angle entre l0 et l1, etc.
      @l0, @l1, @l2, @l3 = l0, l1, l2, l3
      @alpha0, @alpha1, @alpha2 = alpha0, alpha1, alpha2
    end
  
    def perimeter
      l0 + l1 + l2 + l3
    end
  
    def area
      nil  # La méthode de calcul d'une surface pour un quadrilatère quelconque retourne nil
    end
end

module LozengePerimeter
    def perimeter
        4 * l0
    end
end

class Parallelogram < Quadrilateral

    attr_reader :l0, :l1, :alpha0

    def initialize(l0, l1, alpha0)
        super(l0, l1, l0, l1, alpha0, Math::PI - alpha0, alpha0)
    end

    def perimeter
        2 * (l0 + l1)
    end

    def area
        l0 * l1 * Math.cos(alpha0 - Math::PI / 2)
    end
end

class Lozenge < Parallelogram
    include LozengePerimeter

    def initialize(l0, alpha0)
        super(l0, l0, alpha0)
    end

    def area
        l0 * l0 * Math.sin(alpha0)
    end
end

class Rectangle < Parallelogram

    attr_reader :l0, :l1

    def initialize(l0, l1)
        super(l0, l1, Math::PI / 2)
    end

    def area
        l0 * l1
    end
end

class Square < Rectangle
include LozengePerimeter

    attr_reader :l0

    def initialize(l0)
        super(l0, l0)
    end

    def area
        l0 * l0
    end
end


# ==================================================
# Tests

$square = Square.new(2)
$rectangle = Rectangle.new(2, 3)
$parallelogram = Parallelogram.new(2, 3, Math::PI / 4)
$lozenge = Lozenge.new(2, Math::PI / 4)
$quadrilateral = Quadrilateral.new(2, 3, 4, 5, Math::PI / 4, Math::PI / 4, Math::PI / 4)

def test_square
    def assert_equal(expected, actual)
        if expected == actual
            puts "OK"
        else
            puts "Expected #{expected}, got #{actual}"
        end
    end
    puts "Square :"
    assert_equal(8, $square.perimeter)
    assert_equal(4, $square.area)
end

def test_rectangle
    def assert_equal(expected, actual)
        if expected == actual
            puts "OK"
        else
            puts "Expected #{expected}, got #{actual}"
        end
    end
    puts "Rectangle :"
    assert_equal(10, $rectangle.perimeter)
    assert_equal(6, $rectangle.area)
end

def test_parallelogram
    def assert_equal(expected, actual)
        if expected == actual
            puts "OK"
        else
            puts "Expected #{expected}, got #{actual}"
        end
    end
    puts "Parallelogram :"
    assert_equal(10, $parallelogram.perimeter)
end

def test_lozenge
    def assert_equal(expected, actual)
        if expected == actual
            puts "OK"
        else
            puts "Expected #{expected}, got #{actual}"
        end
    end
    puts "Lozenge :"
    assert_equal(8, $lozenge.perimeter)
end

def test_quadrilateral
    def assert_equal(expected, actual)
        if expected == actual
            puts "OK"
        else
            puts "Expected #{expected}, got #{actual}"
        end
    end

    def assert_nil(actual)
        if actual == nil
            puts "OK"
        else
            puts "Expected nil, got #{actual}"
        end
    end
    puts "Quadrilateral :"
    assert_equal(14, $quadrilateral.perimeter)
    assert_nil($quadrilateral.area)
end

def test
    test_square
    test_rectangle
    test_parallelogram
    test_lozenge
    test_quadrilateral
end

test