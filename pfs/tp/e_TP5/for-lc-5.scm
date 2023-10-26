#!r7rs

(import (scheme base) (scheme write))

(define (writeln/return x)
  (write x)
  (newline)
  x)

;;;  ==========================================================================

(define (make-average-f x)
  (lambda (y)
    (/ (+ x y) 2.0)))

; Exemples d'utilisation
(define average-with-10 (make-average-f 10.0))
(display '(> test de la fonction make-average-f <))
(newline)
(display (average-with-10 12.0)) ; Affiche 11.0
(newline)
(display ((make-average-f 12.0) 13.0)) ; Affiche 12.5
(newline)

;;;  ==========================================================================

(define (twice-f f1)
  (lambda (x)
    (f1 (f1 x))))

; Exemple d'utilisation
(display '(> test de la fonction twice-f <))
(newline)
(display ((twice-f (lambda (x) (+ x 1))) 2023)) ; Affiche 2025
(newline)