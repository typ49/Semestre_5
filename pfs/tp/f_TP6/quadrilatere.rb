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

class Lozenge < Parallelogram
    include LozengePerimeter

    def initialize(l0, alpha0)
        super(l0, l0, alpha0)
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
end


# ==================================================
# Tests

