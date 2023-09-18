#!r7rs

(import (scheme base) (scheme inexact) (scheme write))

;;;;  =========================================================================
;;;;
;;;;            L3 --- Functional programming
;;;;            Dichotomy method
;;;;
;;;;  =========================================================================
;;;;
;;;;  Author: J.-M. Hufflen
;;;;  Date: September 2022
;;;;
;;;;  =========================================================================

(define (writeln/return x)
  ;;  Fonction très utile pour pouvoir afficher un résultat suivi d'un passage
  ;;  à la ligne, tout en pouvant récupérer ce résultat dans une variable. Les
  ;;  trois expressions suivantes sont évaluées en séquence et la valeur de la
  ;;  dernière est retournée.
  (write x)
  (newline)
  x)

;;;  ==========================================================================
;;;  Starting

(define (average x y)
  (/ (+ x y) 2))

(define (close-enough? x y epsilon)
  (< (abs (- x y)) epsilon))

;;;  ==========================================================================
;;;  Dichotomy function

(define (dichotomy f1 r1 r2 epsilon)
  ;;
  (define (look-for-root negative-point positive-point)
    ;;  Local function.
    (let ((middle-point (average negative-point positive-point)))
      (if (close-enough? negative-point positive-point epsilon)
          middle-point
          (let ((middle-point-image (f1 middle-point)))
            (cond ((negative? middle-point-image)
                   (look-for-root middle-point positive-point))
                  ((positive? middle-point-image)
                   (look-for-root negative-point middle-point))
                  (else middle-point))))))
  ;;
  ;;  Launching:
  (let ((image-1 (f1 r1)))
    (cond ((>= (* image-1 (f1 r2)) 0)
           ;;  Implicit sequencing after a "cond" clause's test:
           (display "Can't apply the dichotomy method, sorry!")
           (newline)
           #f)
          ((negative? image-1) (look-for-root r1 r2))
          (else (look-for-root r2 r1)))))

;;;  ==========================================================================
;;;  Some examples

(writeln/return (dichotomy (lambda (x) (- (* x x) 2.0)) 1.0 2.0 1e-4))
(writeln/return (dichotomy (lambda (x) (- (* x x) 2.0)) 0.0 1.0 1e-4))
(writeln/return (dichotomy (lambda (x) (- (cos x) (/ x 4))) 1.0 2.0 1e-4))
