#!r7rs

(import (scheme base) (scheme inexact) (scheme write))
;;  Importation des bibliothèques prédéfinies nécessaires. Pour l'instant,
;;  retenez simplement que vous aurez toujours besoin de "base". De même, nous
;;  importerons toujours "write" pour réaliser des affichages à l'écran. Nous
;;  vous signalerons l'importation d'autres bibliothèques le cas échéant : ici,
;;  "inexact", car nous utilisons quelques fonctions trigonométriques et
;;  logarithmiques. Comme vous l'avez deviné, ceci est un commentaire.

#|(write (+ 2023
          (*
           (+ 22 1)(/ 14
                      (- 9 2)))))  ;  => 2023 + (22+1) * (14 / (9-2)) = 2023 + 23 * (14/7) = 2023 + 23 * 2 = 2069
(newline)
(write "Hello world!")
(newline)
(display "Hello world!")
(newline)
(write ((lambda (x) (* x x)) 2))  ;  (**)
(newline)
|#

(define crash ;  (***)  Pour bien montrer que Scheme n'évalue pas sous la
  (lambda () (/ 0)))  ;  forme spéciale "lambda".

(define derive-wrt
  (lambda (f1 h)
    (lambda (x) (/ (- (f1 (+ x h)) (f1 x)) h))))


(write ((derive-wrt (lambda (x) (* x x)) 0.0001) 2))
(newline)
(write (exact ((derive-wrt + 0.0001) 2))) ;  (****) ; produit un résultat car la fonction + peut prendre 1 argument

(define fact
  (lambda (n) (if (zero? n) 1 (* n (fact (- n 1))))))

#|
(newline)
(write (fact 6))
(newline)
|#
(define pi (* 4 (atan 1)))  ;  "atan" est la fonction "Arc tangente" en Scheme.

(define power
  (lambda (U I f0) (* U I (cos (* 2 pi f0)))))

(define fq->power
  (lambda (f0)
    (let ((coefficient (cos (* 2 pi f0))))
      (lambda (U I) (* coefficient U I)))))

(newline)

;; test addition + multiplication

(write (+))
(newline)

(write (+ 6
          10
          2023
          1))
(newline)

(write (*))
(newline)

(write (* 12
          10
          2023
          2))
(newline)

(newline)


;; test soustraction + division

(write (- 1))
(newline)

(write (- 7 2 2023 6))
(newline)

(write (/ 6 3))
(newline)

(write (/ 3 2))
(newline)

(write (/ 3.0 2.0))
(newline)

(write (+ 1 1.1))
(newline)
       
(write (/ 3.0 2))
(newline)

(write (/ 4 3))
(newline)

(write (quotient 4 3))
(newline)

(write (remainder 4 3))
(newline)

(newline)

;; test pi

(write (+ (* 4.1 9.8) (/ (* 2 pi) 3)))
(newline)

(write (+ (exp pi) (log (sin 2.1)) (sin (+ pi (/ 1 pi)))))
(newline)
(newline)

;; test fonction

(write ((lambda (x) (* x x)) 2023))
(newline)

;;(write ((lambda (x y) (* x y y)) 2023)) => manque une valeur

(write ((lambda (x y) (* x y y)) 2023 2022))
(newline)

;; (write ((lambda (x y) (* x y y)) 2023 2022 2021)) => une valeur en trop

(write 2023)
(newline)

(write (lambda () 2023))
(newline)

(write ((lambda () 2023))) ; affiche les information de procédure
(newline)

(write +) ; affiche la procédure +
(newline)

(write (+)) ; fait le calucle 0+0
(newline)

;; (write (2023)) ; 2023 n'est pas une fonction ou une procédure

(write ((lambda () 2023)))
(newline)

;; (write (((lambda () 2023)))) ; une parenthèse n'est pas suivit d'une procédure ou d'une fonction donc erreur

(write (((lambda () (lambda () 2023 )))))
(newline)
(newline)

;; test definition

;; (write ((derive-wrt (lambda (x y) (+ (* 2 x) y)) 0.0001) 2)) ; provoque une erreur car le nombre d'argument est incorrecte


;; puissance électrique

(write (power 220 7 50))
(newline)

(define power-50 (fq->power 50))

(write (power-50 220 7))
(newline)

(write ((fq->power 50) 220 7))
(newline)
(newline)

;; dilatation des coprs physique

(define volume-at (lambda (t v0 lambdaAB) (* v0 ( + 1 (* lambdaAB t)))))

(write (volume-at 30 1 1e-5))
(newline)

(define body->volume-at
  (lambda (lambda-b)
    (lambda (t V0)
      (* V0 (+ 1 (* lambda-b t))))))

(define alpha (/ 1 273.5))

(define gas-volume-at (body->volume-at alpha))

(write (gas-volume-at 30 1))
(newline)


