
# sum every where ignore
def sumeverywhereignore(x)
    goingeverywhere = Proc.new do |alreadysummed, y|
      if y.is_a?(Integer)
        alreadysummed + y
      elsif y.is_a?(Array)
        y.inject(alreadysummed) { |accum, element| goingeverywhere.call(accum, element) }
      else
        alreadysummed
      end
    end
    goingeverywhere.call(0, x)
  end

# Exemples d'utilisation de sumeverywhereignore
# puts sumeverywhereignore(2023) # ⇒ 2023
# puts sumeverywhereignore([30,11,2023]) # ⇒ 2064
# puts sumeverywhereignore([[30,11],2023]) # ⇒ 2064
# puts sumeverywhereignore([[30,[11,[2023],[]]]]) # ⇒ 2064

# ----------------------------------------------------------------------

# sum every where reject
def sumeverywherereject(x)
    goingeverywhere = Proc.new do |alreadysummed, y|
      return nil unless y.is_a?(Integer) || y.is_a?(Array)
  
      if y.is_a?(Integer)
        alreadysummed + y
      elsif y.is_a?(Array)
        y.inject(alreadysummed) { |accum, element| goingeverywhere.call(accum, element) }
      end
    end
    goingeverywhere.call(0, x)
  end
  
  # Exemples d'utilisation de sumeverywherereject
#   puts sumeverywherereject(2023) # ⇒ 2023
#   puts sumeverywherereject([30,11,2023]) # ⇒ 2064
#   puts sumeverywherereject([[30,11],2023]) # ⇒ 2064
#   puts sumeverywherereject([[30,[11,[2023],[]]]]) # ⇒ 2064
#   puts sumeverywherereject('L3') # ⇒ nil
#   puts sumeverywherereject([30,11,['today'],2023]) # ⇒ nil
  
# ----------------------------------------------------------------------

# sum every where perform
def sumeverywhereperform(x)
    goingeverywhere = Proc.new do |alreadysummed, y|
      if y.is_a?(Integer)
        alreadysummed + y
      elsif y.is_a?(Array)
        y.inject(alreadysummed) { |accum, element| goingeverywhere.call(accum, element) }
      else
        result = yield
        return result unless result.is_a?(Integer)
        result
      end
    end
    goingeverywhere.call(0, x)
  end  

  # Exemples d'utilisation de sumeverywhereperform
  def sumeverywhereignore_v2(x)
    sumeverywhereperform(x) { 0 }
  end
  
  def sumeverywherereject_v2(x)
    sumeverywhereperform(x) { return nil }
  end
  
#tests
puts sumeverywhereignore_v2(2023) # ⇒ 2023
puts sumeverywhereignore_v2([30,11,2023]) # ⇒ 2064
puts sumeverywhereignore_v2([[30,11],2023]) # ⇒ 2064
puts sumeverywhereignore_v2([[30,[11,[2023],[]]]]) # ⇒ 2064

puts sumeverywherereject_v2(2023) # ⇒ 2023
puts sumeverywherereject_v2([30,11,2023]) # ⇒ 2064
puts sumeverywherereject_v2([[30,11],2023]) # ⇒ 2064
puts sumeverywherereject_v2([[30,[11,[2023],[]]]]) # ⇒ 2064

result = 2024 + (sumeverywhereperform([30, 11, ['today'], 2023]) { nil } || 0)
# result ⇒ nil
puts result