#!r7rs

;;;; Julien GAUTHIER

(import (scheme base) (scheme write))

;;;;  =========================================================================
;;;;
;;;;            Lab class examination --- Subject 2 ---
;;;;            Definitions given to students
;;;;
;;;;  =========================================================================
;;;;
;;;;  Author: J.-M. Hufflen
;;;;  Date: November 2023
;;;;
;;;;  =========================================================================

;;;  ==========================================================================
;;;  Displaying results

(define (writeln/return x)
  (write x)
  (newline)
  x)

;;;  ==========================================================================
;;;  Other definitions

(define rummy-hand-example
  '((Jack . club) Joker (8 . club) (Ace . spade) (Ace . club) (8 . spade)))

(define rummy-hand-example2
  '((Jack . club) (8 . club) (Ace . spade) (Ace . club) (8 . spade)))

(define (joker? card)
  (eq? card 'Joker))

(define value car)

(define rummy-alist
  '((Joker 50) (Ace 11) (King 10) (Queen 10) (Jack 10) (10) (9) (8) (7) (6) (5)
    (4) (3) (2)))


;;;  ==========================================================================
;;; 1 Preludio

;; exists?
; retourne une valeur logique vrai s'il existe un élément de la liste l qui satisfait le prédicat p1?
; la valeur logique faux sinon
(define (exists? p1? l)
  (cond ((null? l) #f)
        ((p1? (car l)) #t)
        (else (exists? p1? (cdr l)))))


;; no-duplicate?
; retourne une valeur logique vrai s'il n'existe pas deux éléments de la liste l qui sont égaux
; la valeur logique faux sinon
(define (no-duplicate? l)
    (cond ((null? l) #t)
                ((exists? (lambda (x) (equal? x (car l))) (cdr l)) #f)
                (else (no-duplicate? (cdr l)))))

;; list-length-min?
; retourne une valeur logique vrai si la liste l contient au moins nb éléments
; la valeur logique faux sinon
; on cherche à parcourir la liste un minimum de fois
(define (list-length-min? l nb)
  (cond ((null? l) (<= nb 0))
        ((<= nb 0) #t)
        (else (list-length-min? (cdr l) (- nb 1)))))


;;;  ==========================================================================
;;; 2 Jeu de rami

;;; 2.1 Cartes à jouer

;; colour
; retourne la couleur de la carte
(define colour cdr)

;; exists-colour?
; cherche si il existe un joker ou au moins une carte de la couleur c dans la liste l
(define (exists-colour? c l)
  (cond ((null? l) #f)
        ((joker? (car l)) #t)
        ((eq? c (colour (car l))) #t)
        (else (exists-colour? c (cdr l)))))

;; exists-figure?
; cherche si la liste l contient au moins une carte dont la valeur est une figure
; (ace, king, queen, jack)
(define (exists-figure? l)
  (cond ((null? l) #f)
        ((joker? (car l)) #t)
        ((exists? (lambda (x) (eq? x (value (car l)))) '(Ace King Queen Jack)) #t)
        (else (exists-figure? (cdr l)))))

;;; 2.2 Rami et points

;; small-hand?
; retourne une valeur logique vrai si la liste l n'as pas de cartes répétées et qu'il y a au moins 3 cartes
; la valeur logique faux sinon
(define (small-hand? l)
  (and (no-duplicate? l) (list-length-min? l 3)))

;; rummy-points
; prend en paramètre une liste d'association de la forme (figure valeur) ou (valeur)
; retourne le nombre de points associé à la valeur de la carte à jouer
; les points suivent la règle de la liste d'association rummy-alist
(define (rummy-points association)
    (if (null? (cdr association))
            (car association)
            (cdr (assoc (car association) rummy-alist))))

;; rummy-hand-points
; prend en paramètre une liste de cartes à jouer
; retourne le nombre de points associé à la main de cartes à jouer en utilisant la fonction rummy-points
; si une carte n'est pas répértoiriée dans la liste d'association rummy-alist, la fonction retourne #f
(define (rummy-hand-points hand)
    (if (null? hand)
            0
            (if (pair? (car hand))
                    (+ (rummy-points (car hand)) (rummy-hand-points (cdr hand)))
                    (let ((points (assoc (car hand) rummy-alist)))
                        (if (and points (pair? points))
                                (+ (car (cdr points)) (rummy-hand-points (cdr hand)))
                                (rummy-hand-points (cdr hand)))))))

                



;;; ==========================================================================
;;; test des fonctions

;; test pour exists?
(writeln/return "Testing exists?")
(writeln/return (exists? even? '(1 2 3 4 5)))
(writeln/return (exists? even? '(1 3 5)))

;; test pour no-duplicate?
(writeln/return "Testing no-duplicate?")
(writeln/return (no-duplicate? '(1 2 3 4 5)))
(writeln/return (no-duplicate? '(1 2 3 4 5 1)))

;; test pour list-length-min?
(writeln/return "Testing list-length-min?")
(writeln/return (list-length-min? '(1 2 3 4 5) 3))
(writeln/return (list-length-min? '(1 2 3 4 5) 6))

;; test pour joker?
(writeln/return "Testing joker?")
(writeln/return (joker? 'Joker))
(writeln/return (joker? '(Jack . club)))

;; test pour value et colour
(writeln/return "Testing value and colour")
(writeln/return (value '(Jack . club)))
(writeln/return (colour '(Jack . club)))

;; test pour exists-colour?
(writeln/return "Testing exists-colour?")
(writeln/return (exists-colour? 'spade rummy-hand-example)) ; #t for spade
(writeln/return (exists-colour? 'heart rummy-hand-example)) ; #f for joker
(writeln/return (exists-colour? 'diamond rummy-hand-example2 )) ; #f for diamond

;;test pour exists-figure?
(writeln/return "Testing exists-figure?")
(writeln/return (exists-figure? rummy-hand-example))
(writeln/return (exists-figure? '((2 . club) (3 . club) (4 . club) (5 . club))))

;; test pour small-hand?
(writeln/return "Testing small-hand?")
(writeln/return (small-hand? rummy-hand-example)) ; #t for rummy-hand-example
(writeln/return (small-hand? '((2 . club) (3 . club)))) ; #f du to less than 3 cards
(writeln/return (small-hand? '((2 . club) (3 . club) (4 . club) (5 . club) (5 . club)))) ; #f due to duplicate

;; test pour rummy-points
(writeln/return "Testing rummy-points")
(writeln/return (rummy-points '(Jack 10))) ; 10
(writeln/return (rummy-points '(9))) ; 9
(writeln/return (rummy-points '(Joker 50))) ; 50

;; test pour rummy-hand-points
; (writeln/return (rummy-hand-points rummy-hand-example)) ; 98
; (writeln/return (rummy-hand-points '(Joker))) ; 50