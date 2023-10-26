def match_french_department_number(str)
    return true if str.match(/\A(0[1-9]|[1-8]\d|9[0-5]|2[AB]|97[1-4]|976)\z/)
    false
  end

  def match_1811_french_department_number(str)
    return true if str.match(/\A(0[1-9]|[1-8]\d|9[1-9]|1[0-2]\d|13[01])\z/) && str != "90"
    false
  end
  
  def process_unit(str)
    if str.match(/\A([a-zA-Z]+(\(\d+\))?(\.[a-zA-Z]+(\(-\d+\))?)*)\z/)
      latex_str = str.gsub(/([a-zA-Z]+)(\((\-?\d+)\))?/) do
        "\\mathrm{#{$1}}^{#{$3}}"
      end
      return "$#{latex_str}$"
    end
    nil
  end
  

  def american_date_2_french_date(str)
    str.gsub(/(\d{4})-(\d{2})-(\d{2})|(\d{2})-(\d{2})-(\d{4})/) { "#{$3 || $5}/#{$2 || $4}/#{$1 || $6}" }
  end
  
  def american_date_2_french_date!(str)
    new_str = american_date_2_french_date(str)
    return nil if new_str == str
    str.replace(new_str)
  end
  

  def update_your_department(str)
    str.gsub(/(?<![0-9])24(?![0-9])/, '25')
  end
  
  def target_p(str)
    match = str.match(/(\((?:[^()]*+|\((?:[^()]*+|\([^()]*+\))*+\))*+\)\.)/)
    match ? match[1] : nil
  end
  
  



  # test
# Test pour match_french_department_number
puts "Test pour match_french_department_number"
puts match_french_department_number('01')  # Doit retourner true
puts match_french_department_number('2A')  # Doit retourner true
puts match_french_department_number('225') # Doit retourner false
puts match_french_department_number('971') # Doit retourner true
puts "-------------------------"

# Test pour match_1811_french_department_number
puts "Test pour match_1811_french_department_number"
puts match_1811_french_department_number('131') # Doit retourner true
puts match_1811_french_department_number('141') # Doit retourner false
puts match_1811_french_department_number('90')  # Doit retourner false
puts "-------------------------"

# Test pour process_unit
puts "Test pour process_unit"
puts process_unit('N.m')       # Doit retourner "$\\mathrm{N}.\\mathrm{m}$"
puts process_unit('m.s(-1)')   # Doit retourner "$\\mathrm{m}.\\mathrm{s}^{-1}$"
puts process_unit('invalid')   # Doit retourner nil
puts "-------------------------"

# Test pour american_date_2_french_date et american_date_2_french_date!
puts "Test pour american_date_2_french_date et american_date_2_french_date!"
puts american_date_2_french_date('2022-09-04')  # Doit retourner "04/09/2022"
puts american_date_2_french_date('09-04-2022')  # Doit retourner "04/09/2022"

str = '2022-09-04'
puts american_date_2_french_date!(str)  # Doit retourner "04/09/2022"
puts str  # Doit être modifié en "04/09/2022"
puts "-------------------------"

# Test pour update_your_department
puts "Test pour update_your_department"
puts update_your_department('24th Department')           # Doit retourner "25th Department"
puts update_your_department('124th Department')          # Doit retourner "124th Department"
puts update_your_department('The 24th place in France')  # Doit retourner "The 25th place in France"
puts "-------------------------"

# Test pour target_p
puts "Test pour target_p"
puts target_p('(He Could Stop the World). (The Man of Bronze) (Meteor Menace). Doc Savage.')  # Devrait afficher "(He Could Stop the World). (The Man of Bronze) (Meteor Menace)."
puts target_p('(The Polar Treasure) (The Lost Oasis). (Fear Cay)')  # Devrait afficher nil
puts target_p('(Yeah! (I (really) enjoy (programming in) Ruby). Don\'t you?')  # Devrait afficher "(I (really) enjoy (programming in) Ruby)."
