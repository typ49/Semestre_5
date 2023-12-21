#!r7rs

(import (scheme base) (scheme inexact) (scheme write))

(define (writeln/return x)
  ;;  Fonction très utile pour pouvoir afficher un résultat suivi d'un passage
  ;;  à la ligne, tout en pouvant récupérer ce résultat dans une variable. Les
  ;;  trois expressions suivantes sont évaluées en séquence et la valeur de la
  ;;  dernière est retournée.
  (write x)
  (newline)
  x)

(define pi (* 4 (atan 1)))

;; renvoie la somme des décimale d'un nombre entrée en paramètre
(define (first-mystery n)
  ;;  Rappelons que les fonctions "quotient" et "remainder" retournent
  ;;  respectivement le quotient entier et le reste d'une division euclidienne,
  ;;  à condition, bien sûr, que le diviseur ne soit pas nul.}}
  (if (< n 10) n (+ (first-mystery (quotient n 10)) (remainder n 10))))

;; renvoie 1 si n est divisible par p
(define (second-mystery n p)
  ;;
  (define (rec-mystery n0 p0)
    ;;  Rappelons également qu'une forme spéciale "define" interne à une forme
    ;;  spéciale "lambda" est équivalente à l'utilisation d'une forme spéciale
    ;;  "letrec*".
    (if (zero? (remainder n0 p0)) (rec-mystery (quotient n0 p0) p0) n0))
  ;;
  ;;  Principal call:
  (if (or (< p 2) (zero? n)) n (rec-mystery n p)))


(define (second-mystery-simple n p)
  (define (rec-mystery n0)
    (if (zero? (remainder n0 p)) 
        (rec-mystery (quotient n0 p)) 
        n0))
  (if (or (< p 2) (zero? n)) 
      n 
      (rec-mystery n)))



;;; exercice n°1

;; fonction handler
(define (simple-surface r f1) ; f1 est une fonction
  (f1 r))

;; surface d'un cercle
(define (circle-surface p)
  (* pi (* p p)))

;; surface d'une sphere
(define (sphere-surface p)
  (* 4 pi (* p p)))

;; surface d'un carré
(define (square-surface p)
  (* p p))

;; surface d'un cube
(define (cube-surface p)
  (* p p p))

(write "Exercice n°1 :")
(newline)
(write (simple-surface 5 circle-surface)) ; 78.53981633974483
(newline)
(write (simple-surface 5 sphere-surface)) ; => 314.1592653589793
(newline)
(write (simple-surface 5 square-surface)) ; => 25
(newline)
(write (simple-surface 5 cube-surface)) ; => 125
(newline)
(newline)

;;; exercice n°2
;; 2.1

;; convertit des miles en inches
(define (in-inches miles0 yards0 feet0 inches0)
  (let* ((yards (+ yards0 (* 1760 miles0)))
         (feet (+ feet0 (* 3 yards)))
         (inches (+ inches0 (* 12 feet))))
    inches))

;; convertie des inches en metres
(define (inches-to-meters inches0)
  (let* ((mm (* 25.4 inches0))
         (meters (* 0.001 mm)))
  meters))

;; convertie des miles en metres
(define (british-to-metrics miles0 yards0 feet0 inches0)
  (let* ((inches (in-inches miles0 yards0 feet0 inches0))
         (meters (inches-to-meters inches)))
    meters))
  


(write "Exercice n°2 :")
(write "2.1 :")
(newline)
(write (in-inches 1 0 0 0)) ; => 63360
(newline)
(write (inches-to-meters 63360)) ; => 1609344000.0
(newline)
(write (british-to-metrics 3 61 2 10)) ; => 4884.674
(newline)
(newline)

;; 2.2
;; 2.3

(define (to-metric nb1 nb2 nb3 metric0)
  (lambda (unit1 unit2 unit3 unit4)
    (*
     (+
      (*
       (+
        (*
         (+
          (* nb1 unit1)
          unit2)
         nb2)
        unit3)
       nb3)
      unit4)
     metric0)))

(define british-to-metric-V2 (to-metric 1760 3 12 0.0254))

(define gallons-to-litres (to-metric 4 2 4 0.1421))

(define stones-to-grammes (to-metric 14 16 437.5 0.065))

(define pounds-to-euros (to-metric 10 2 12 1.15))

(define days-to-seconds (to-metric 60 60 60 1))

(write "2.2 / 2.3")
(newline)
(write (british-to-metric-V2 3 61 2 10)) ; => 4884.674
(newline)
(write (gallons-to-litres 3 3 3 3)) ; => 19.183500000000002
(newline)
(write (stones-to-grammes 2 10 11 320)) ; => 17623.6125
(newline)
(write (pounds-to-euros 12 3 1 6)) ; => 3415.4999999999995
(newline)
(write (days-to-seconds 1 3 47 48)) ; => 229668
(newline)
(newline)

;;; Exercice n°3

(write "Exercice n°3 :")
(newline)
(write(first-mystery 2021)) ; => 5
(newline)
(write (first-mystery 2009)) ; => 11
(newline)

(define (one-digit n)
  (if (< n 10)
      n
      (one-digit (first-mystery n))))

(write (one-digit 2021))
(newline)
(write (one-digit 2009))
(newline)
(newline)

;;; Exercice n°4

(define (hamming-number? n)
  (let ((result (second-mystery (second-mystery (second-mystery n 2) 3) 5)))
    (= result 1)))

(write "Exercice n°4 :")
(newline)
(write (hamming-number? 1))
(newline)
(write (hamming-number? 6))
(newline)
(write (hamming-number? 14))
(newline)
(newline)

;;; Exercice n°5

(define (alternate-digit-sum n positive?)
  (if (zero? n)
      0
      (let ((digit (remainder n 10))
            (next-sum (alternate-digit-sum (quotient n 10) (not positive?))))
        (if positive?
            (+ digit next-sum)
            (- next-sum digit)))))

(define (divisible-by-11? i)
  (let ((result (alternate-digit-sum (abs i) #t)))
    (if (< result 10)
        (= result 0)
        (divisible-by-11? result))))


(write "Exercice n°5")
(newline)
(write (divisible-by-11? 154))
(newline)
(write (divisible-by-11? -1749))
(newline)
(write (divisible-by-11? 1751))
(newline)
(newline)

;;; Exercice n°6

(define (diagonal n)
  (if (= n 0)
      (cons 0 0)
      (let* ((prev (diagonal (- n 1)))
             (p (car prev))
             (q (cdr prev)))
        (if (= q 0)
            (cons 0 (+ p 1))
            (cons (+ p 1) (- q 1))))))


(write "Exercice n°6")
(newline)
;; les couple : (0.0) (0.1) (1.0) (1.1) (2.0) ...
(write (diagonal 5)) ; => (2.0)
(newline)