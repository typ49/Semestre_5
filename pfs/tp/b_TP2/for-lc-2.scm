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

(define (first-mystery n)
  ;;  Rappelons que les fonctions "quotient" et "remainder" retournent
  ;;  respectivement le quotient entier et le reste d'une division euclidienne,
  ;;  à condition, bien sûr, que le diviseur ne soit pas nul.}}
  (if (< n 10) n (+ (first-mystery (quotient n 10)) (remainder n 10))))

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
