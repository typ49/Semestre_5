#!r7rs

(import (scheme base) (scheme inexact) (scheme write))

;; exercice finale

(define (average x y)
  (/ (+ x y) 2))

(define (close-enough? x y e)
  (< (abs (- x y)) e))


;; fontion look-for-root
(define (look-for-root f1 negative-point positive-point epsilon)
  (let ((mid-point (/ (+ negative-point positive-point) 2.0)))
    (if (< (- positive-point negative-point) epsilon)
        mid-point ; Si l'intervalle est suffisamment petit, renvoyer le point médian comme solution approximative
        (let ((f-mid (f1 mid-point)))
          (cond ((zero? f-mid) mid-point) ; Cas (i)
                ((> f-mid 0) (look-for-root f1 negative-point mid-point epsilon)) ; Cas (ii)
                (else (look-for-root f1 mid-point positive-point epsilon))))))) ; Cas (iii)

;; fonction de test
(define (f x)
  (- (* x x) 2.0))

(define (g x)
  (- (cos x) (/ x 4)))


;; test look-for-root
(write (look-for-root f 1.0 2.0 0.0001))
(newline)
(newline)


;; fonction dichotomy
(define (dichotomy f1 r1 r2 epsilon)
  (define (look-for-root negative-point positive-point)
    (let ((mid-point (/ (+ negative-point positive-point) 2.0)))
      (if (< (- positive-point negative-point) epsilon)
          mid-point
          (let ((f-mid (f1 mid-point)))
            (cond ((zero? f-mid) mid-point)
                  ((> f-mid 0) (look-for-root negative-point mid-point))
                  (else (look-for-root mid-point positive-point)))))))

  (let ((f-r1 (f1 r1))
        (f-r2 (f1 r2)))
    (cond ((and (< f-r1 0) (> f-r2 0)) (look-for-root r1 r2))
          ((and (> f-r1 0) (< f-r2 0)) (look-for-root r2 r1))
          (else (write "Dichotomy not applicable.") #f))))

;; test dichotomy

(write (dichotomy f 1.0 2.0 0.0001))
(newline)
(write (dichotomy f 0.0 1.0 0.0001))
(newline)
(write (dichotomy g 1.0 2.0 0.0001))
(newline)

#|
Oui, lorsque vous rendez look-for-root locale à dichotomy, certains arguments formels de look-for-root peuvent être omis car ils sont déjà accessibles depuis l'environnement englobant de dichotomy.

En particulier, les arguments f1 et epsilon peuvent être supprimés de look-for-root car ils sont déjà fournis à dichotomy et sont donc accessibles à look-for-root sans avoir besoin de les passer explicitement.
|#