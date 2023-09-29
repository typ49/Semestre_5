#!r7rs

(import (scheme base) (scheme case-lambda) (scheme char) (scheme cxr)
        (scheme write))

(define (writeln/return x)
  ;;  Fonction très utile pour pouvoir afficher un résultat suivi d'un passage
  ;;  à la ligne, tout en pouvant récupérer ce résultat dans une variable. Les
  ;;  trois expressions suivantes sont évaluées en séquence et la valeur de la
  ;;  dernière est retournée.
  (write x)
  (newline)
  x)

;;;  ==========================================================================
;;;  Start

#|
(writeln/return (cons 0 1)) ; => (0 . 1)
(writeln/return (cons 0.1 0.1)) ; => (0.1 . 0.1)
(writeln/return (cons (cons 0.1 0.1) 1)) ; => ((0.1 . 0.1) . 1)
(writeln/return (cons 0.1 '())) ; => (0.1)
(writeln/return (cons '() 0.1)) ; => (() . 0.1)
(writeln/return (cons '() '())) ; => (())
|#

(define current-year 2023)
(define next-year (+ current-year 1))
(define academic-year (cons current-year next-year))
(define academic-year-0 (cons current-year 'next-year))
(define academic-year-1 (cons 'current-year 'next-year))
(define operations (cons + -))
(define symbols (cons '+ '-))
(define two-numbers (cons 0 1))
(define something (cons (cons 0.1 0.1) 1))

#|
  (writeln/return academic-year) ; => (2023 . 2024)
  (writeln/return (quote academic-year)) ; => academic-year
  (writeln/return 'academic-year) ; => academic-year
  (writeln/return 'undefined) ; => undefined
;;  (writeln/return undefined) ; #error
  (writeln/return +) ; => #<prcedure:+>
  (writeln/return '+) ; => +
;;  (writeln/return (0 . 1)) ; #error
  (writeln/return '(0 . 1)) ; => (0 . 1)
;;  (writeln/return (+ . 1)) ; => #error
  (writeln/return '(+ . 1)) ; => (+ . 1)
|#


;;; car renvoie le premier élément d'une paire
;;; cdr renvoie tous les élément d'une paire sauf le premier
#|
  (writeln/return (car academic-year)) ; => 2023
  (writeln/return (cdr academic-year)) ; => 2024
;;  (writeln/return (car next-year)) ; => #error , pas une paire
  (writeln/return academic-year-0) ; => (2023 . newt-year)
  (writeln/return (cdr academic-year-0)) ; => next-year
  (writeln/return academic-year-1) ; => (current-year . next-year)
|#
#|
  (writeln/return ((car operations) (car two-numbers) (cdr two-numbers))) ; => + 0 1 => 1
;;  (writeln/return ((cdr symbols) (cdr two-numbers) (car two-numbers))) ; => #error : '- n'est pas une procédure
  (writeln/return (cons (cdr symbols) (car two-numbers))) ; => (- . 0)
  (writeln/return (cons (cdr operations) (cdr two-numbers))) ; => (#<procedure:-> . 1)
  (writeln/return ((cdr operations) (cdr two-numbers))) ; => -1
  (writeln/return (car something)) ; => (0.1 . 0.1)
  (writeln/return (cdr (car something))) ; => 0.1
;;  (writeln/return (car (cdr something))) ; => #error : 1 n'est pas une paire
|#

(define movie-title '((Mad . Max) Beyond the (Thunderdome . ())))
#|
  (writeln/return (caar movie-title)) ; => Mad
  (writeln/return (cdar movie-title)) ; => Max
  (writeln/return (cadr movie-title)) ; => Beyond
  (writeln/return (cddr movie-title)) ; => (the (Thunderdome))
  (writeln/return (caddr movie-title)) ; => the
  (writeln/return (cdddr movie-title)) ; => ((thunderdome))
  (writeln/return (cadddr movie-title)) ; => (thunderdome)
  (writeln/return (cddddr movie-title)) ; => ()
|#

;;;  ==========================================================================
;;;  From Pairs to Lists

;  ****

;;;  ==========================================================================
;;;  Lists

(define (free-family? v w)
  (let ((v1 (car v))
        (v2 (cdr v))
        (w1 (car w))
        (w2 (cdr w)))
    (not (= 0 (- (* v1 w2) (* v2 w1))))))

#|
(writeln/return '(Sometime (to) (() . miss) (now)))
(writeln/return (list (cons 'Everybody '()) (cons '() 'wants) (cons 'somebody '())))
(writeln/return '(Thats (() why) (() I (need (you . baby)))))
(writeln/return (cons 'Aint (list 'nobody 'else 'around)))
(writeln/return (cons 'Someone (list 'to 'squeeze)))
(writeln/return (cons (list 'I 'need 'you) (list 'you 'you)))
(writeln/return (cons (list 'And 'I) (list (list 'need) 'you 'you)))
(writeln/return (cons 'I-m (append (list 'so 'glad) (list 'to 'be 'here 'tonight))))
|#


;;;  ==========================================================================
;;;  Mini data bases

(define unknown '??)

(define (unknown? x)
  (eq? x unknown))

(define (mk-recording key-0 title-0 author-list-0 year-0 publisher-0 key-list-0)
  (list key-0 title-0 author-list-0 year-0 publisher-0 key-list-0))

(define (key jazz-r)
  (let ((k (car jazz-r)))
    (if (unknown? k)
        (error "La clé ne peut pas être une information inconnue.")
        k)))

(define (title jazz-r)
  (cadr jazz-r))

(define (authors jazz-r)
  (caddr jazz-r))

(define (year jazz-r)
  (cadddr jazz-r))

(define (publisher jazz-r)
  (car (cddddr jazz-r)))

(define (cds jazz-r)
  (cadr (cddddr jazz-r)))

(define (unique-keys? j-list)
  (define (keys j-list)
    (map key j-list))

  (define (duplicates? lst)
    (if (null? lst)
        #f
        (if (member (car lst) (cdr lst))
            #t
            (duplicates? (cdr lst)))))

  (not (duplicates? (keys j-list))))

(define (get-information-e key0 sel0 j-list)
  (cond
    ((null? j-list) #f)
    ((equal? (key (car j-list)) key0) (sel0 (car j-list)))
    (else (get-information-e key0 sel0 (cdr j-list)))))

(define (get-key x0 sel0 j-list)
  (cond
    ((null? j-list) #f)
    ((equal? (sel0 (car j-list)) x0) (key (car j-list)))
    (else (get-key x0 sel0 (cdr j-list)))))

(define (filter pred lst)
  (cond ((null? lst) '())
        ((pred (car lst)) (cons (car lst) (filter pred (cdr lst))))
        (else (filter pred (cdr lst)))))

(define (those-that p1? j-list f1)
  (map f1 (filter p1? j-list)))

(define (top-years j-list)
  ;; Construire la liste d'associations
  (define (build-alist j-list)
    (if (null? j-list)
        '()
        (let ((yr (year (car j-list))))
          (cons (cons yr (count yr j-list)) (build-alist (cdr j-list))))))

  ;; Compter le nombre d'occurrences d'une année dans j-list
  (define (count yr j-list)
    (if (null? j-list)
        0
        (if (= yr (year (car j-list)))
            (+ 1 (count yr (cdr j-list)))
            (count yr (cdr j-list)))))

  ;; Trouver les années avec le nombre maximum d'occurrences
  (define (max-years alist max-count)
    (if (null? alist)
        '()
        (if (= (cdr (car alist)) max-count)
            (cons (car (car alist)) (max-years (cdr alist) max-count))
            (max-years (cdr alist) max-count))))

  ;; Trouver le nombre maximum d'occurrences dans a-list
  (define (max-count alist)
    (if (null? alist)
        0
        (max (cdr (car alist)) (max-count (cdr alist)))))

  ;; Appeler les fonctions auxiliaires pour obtenir le résultat
  (let ((alist (build-alist j-list)))
    (max-years alist (max-count alist))))








  ;;  SYMBOL,STRING,LIST-of[SYMBOL}^,INTEGER,SYMBOL^,LIST-of[SYMBOL]^ -> JAZZ-R
 ;; (list key-0 title-0 author-list-0 year-0 publisher-0 key-list-0)

(define miles-davis-r
  (list (mk-recording 'd0 "On the Corner" unknown 1972 unknown '(c40))
	(mk-recording 'd1 "New-York Girl" unknown 1972 unknown '(c40))
	(mk-recording 'd2 "Thinkin' One Thing and Doin' Another" unknown 1972
		      unknown '(c40))
	(mk-recording 'd3 "One and One" unknown 1972 unknown '(c40))
	(mk-recording 'd4 "Well You Needn't" '(Thelonius-Monk) 1954
		      'Blue-Ribbon-Music '(c41))
	(mk-recording 'd5 "Love for Sale" '(Cole-Porter) 1958 'Chappell '(c41))
	(mk-recording 'd6 "Something Else" '(Miles-Davis) 1958 'EMI '(c41))
	(mk-recording 'd7 "Dear Old Stockholm" '(Stan-Getz P-Golly) 1952
		      'Windswept-Pacific-Music '(c41))
	(mk-recording 'd8 "Black Satin" unknown 1972 unknown '(c40))
	(mk-recording 'd9 "Mr. Freedom X" unknown 1972 unknown '(c40))
	(mk-recording 'd10 "Helen Butte" unknown 1972 unknown '(c40))
	(mk-recording 'd11 "Boplicity" '(Cleo-Henry) 1949 'Campbell '(c40))
	(mk-recording 'd12 "Ray's Idea" '(Ray-Bonner W-G-Fuller) 1953 
		      'Bosworth '(c41))
	(mk-recording 'd13 "Yesterdays" '(Kern Harbach) 1952 'Universal '(c42))
	(mk-recording 'd14 "Vote for Miles" unknown 1972 unknown '(c40))
	(mk-recording 'd15 "Deception" '(Miles-Davis) 1950 'Sony '(c41))
	(mk-recording 'd16 "Israel" '(John-Carisi) 1949 'EMI '(c41))
	(mk-recording 'd17 "How Deep is the Ocean" '(Irving-Berlin) 1952 'EMI
		      '(c41))
	(mk-recording 'd18 "Kelo" '(Jay-Jay-Johnson) 1953 'Kensington-Music
		      '(c41))
	(mk-recording 'd19 "Woody 'n' You" '(Dizzy-Gillepsie) 1952 'Chappell
		      '(c41))
	(mk-recording 'd20 "Chance It" '(Oscar-Petitford) 1952 'Orpheus-Music
		      '(c41))))

(define stan-getz-r
  (list (mk-recording 'g30 "Autumn Leaves" '(Kosma Prevert) 1980
		      'Peter-Maurice-Co-Ltd '(c44))
	(mk-recording 'g31 "Nature Boy" '(Abbon) 1980 'Chappell '(c44))))

;  ****

;;;  ==========================================================================
;;;  Operations on mini data bases

;;(writeln/return (map (lambda (x) (+ x 1)) '(28 9 2023)))

  
  (writeln/return (unique-keys? miles-davis-r))
  (writeln/return (unique-keys? stan-getz-r))

  (writeln/return (get-information-e 'd14 title miles-davis-r))
  (writeln/return (get-information-e 'd13 authors miles-davis-r))
  (writeln/return (get-information-e 'g30 year stan-getz-r))
  (writeln/return (get-information-e 'd14 publisher miles-davis-r))

  (writeln/return (get-key "Kelo" title miles-davis-r))
  (writeln/return (get-key "Blue Moon" title stan-getz-r))

;; 1. Liste des noms des œuvres enregistrées par Miles Davis en 1972.
(define miles-1972-titles
  (those-that 
   (lambda (rec) (= (year rec) 1972))
   miles-davis-r
   title))

;; 2. Liste des clés des enregistrements de Stan Getz concernant les pièces ayant plusieurs auteurs.
(define stan-multiple-authors
  (those-that 
   (lambda (rec) 
     (and (not (unknown? (authors rec))) 
          (> (length (authors rec)) 1)))
   stan-getz-r
   key))

;; 3. Liste des clés des enregistrements de Miles Davis concernant les pièces ayant un seul auteur.
(define miles-single-author
  (those-that 
   (lambda (rec) 
     (and (not (unknown? (authors rec))) 
          (= (length (authors rec)) 1)))
   miles-davis-r
   key))

;; 4. Liste des noms des pièces enregistrées par Miles Davis ou Stan Getz pour lesquelles l’éditeur est inconnu.
(define unknown-publisher-titles
  (append 
   (those-that 
    (lambda (rec) (unknown? (publisher rec)))
    miles-davis-r
    title)
   (those-that 
    (lambda (rec) (unknown? (publisher rec)))
    stan-getz-r
    title)))

;; 5. Liste des clés des œuvres enregistrées par Miles Davis dont l’éditeur est EMI.
(define miles-emi-keys
  (those-that 
   (lambda (rec) (eq? (publisher rec) 'EMI))
   miles-davis-r
   key))

;; 6. Liste des noms des œuvres de Miles Davis qu’il a lui-même enregistrées.
(define miles-self-recorded
  (those-that 
   (lambda (rec) 
     (and (not (unknown? (authors rec)))
          (member 'Miles-Davis (authors rec))))
   miles-davis-r
   title))

;; 7. Liste des clés des œuvres enregistrées par Stan Getz et reprises sur plusieurs compact discs.
(define stan-multiple-cds
  (those-that 
   (lambda (rec) 
     (> (length (cds rec)) 1))
   stan-getz-r
   key))

;; Affichage des résultats
(writeln/return miles-1972-titles)
(writeln/return stan-multiple-authors)
(writeln/return miles-single-author)
(writeln/return unknown-publisher-titles)
(writeln/return miles-emi-keys)
(writeln/return miles-self-recorded)
(writeln/return stan-multiple-cds)


(writeln/return (top-years miles-davis-r))
(writeln/return (top-years stan-getz-r))




;;;  ==========================================================================
;;;  Merge Sort

(define cp-list-example
  '((5 . 5) (6 . 0) (3 . 3) (6 . 1) (7 . 7) (1 . 1) (6 . 2)))

(define (pretty-writeln/return x)
  ;;  Writes "x", followed by an end-of-line character, and returns "x".  If
  ;;  "x" is a linear list, successive lines are used for successive elements.
  (if (and (pair? x) (list? x))
      (begin
	(write-char #\()
	(write (car x))
	(for-each (lambda (x0)
		    (newline)
		    (write-char #\space)
		    (write x0))
		  (cdr x))
	(write-char #\)))
      (write x))
  (newline)
  x)


(define (take lst n)
  (if (or (null? lst) (= n 0))
      '()
      (cons (car lst) (take (cdr lst) (- n 1)))))

(define (drop lst n)
  (if (or (null? lst) (= n 0))
      lst
      (drop (cdr lst) (- n 1))))

(define (merge-sort less? lst)
  (if (<= (length lst) 1)
      lst
      (let ((half (quotient (length lst) 2)))
        (merge less? 
               (merge-sort less? (take lst half))
               (merge-sort less? (drop lst half))))))

(define (merge less? lst1 lst2)
  (cond ((null? lst1) lst2)
        ((null? lst2) lst1)
        ((less? (car lst1) (car lst2))
         (cons (car lst1) (merge less? (cdr lst1) lst2)))
        (else
         (cons (car lst2) (merge less? lst1 (cdr lst2))))))




(define (compare-couples c1 c2)
  (or (< (car c1) (car c2))
      (and (= (car c1) (car c2))
           (<= (cdr c1) (cdr c2)))))


(writeln/return (merge-sort compare-couples cp-list-example))





;;;  ==========================================================================
;;;  Apotheosis

(define (compare-years-strict r1 r2)
  (< (year r1) (year r2)))

(define (compare-years-large r1 r2)
  (<= (year r1) (year r2)))

(define sorted-miles-davis-strict
  (merge-sort compare-years-strict miles-davis-r))

(define sorted-miles-davis-large
  (merge-sort compare-years-large miles-davis-r))

(pretty-writeln/return sorted-miles-davis-strict)
(newline)
(newline)
(pretty-writeln/return sorted-miles-davis-large)
(newline)
(newline)


(define (compare-years-then-titles r1 r2)
  (if (= (year r1) (year r2))
      (string-ci<=? (title r1) (title r2))
      (< (year r1) (year r2))))

(define sorted-miles-davis-years-titles
  (merge-sort compare-years-then-titles miles-davis-r))

(pretty-writeln/return sorted-miles-davis-years-titles)



(define (position x l)
  (let thru ((l0 l)
	     (current-position 0))
    (cond ((null? l0) #f)
	  ((equal? (car l0) x) current-position)
	  (else (thru (cdr l0) (+ current-position 1))))))

(define ordered-publishers
  '(Blue-Ribbon-Music Bosworth Chappell EMI Kensington-Music Sony Universal))

(define (publisher<=? s1 s2)
  (let ((pos1 (position s1 ordered-publishers))
        (pos2 (position s2 ordered-publishers)))
    (if (or (not pos1) (eq? s1 '??))
        (set! pos1 (length ordered-publishers)))
    (if (or (not pos2) (eq? s2 '??))
        (set! pos2 (length ordered-publishers)))
    (<= pos1 pos2)))

(define (publisher<? s1 s2)
  (and (publisher<=? s1 s2) (not (equal? s1 s2))))

(define (compare-publishers r1 r2)
  (publisher<=? (publisher r1) (publisher r2)))

(define sorted-miles-davis-publishers
  (merge-sort compare-publishers miles-davis-r))

(define sorted-stan-getz-publishers
  (merge-sort compare-publishers stan-getz-r))

(pretty-writeln/return sorted-miles-davis-publishers)
(pretty-writeln/return sorted-stan-getz-publishers)


(define add-something
  (case-lambda
   ((x) (+ x 1))
   ((x y) (+ x y))))




(define mergesort-plus
  (case-lambda
    [(lst) (mergesort-plus lst <= (lambda (x) x))]
    [(lst rel2?) (mergesort-plus lst rel2? (lambda (x) x))]
    [(lst rel2? k1)
     (letrec ((merge (lambda (lst1 lst2)
                       (cond
                         ((null? lst1) lst2)
                         ((null? lst2) lst1)
                         ((rel2? (cdr (car lst1)) (cdr (car lst2)))
                          (cons (car lst1) (merge (cdr lst1) lst2)))
                         (else (cons (car lst2) (merge lst1 (cdr lst2)))))))
              (msort (lambda (lst)
                       (if (<= (length lst) 1)
                           lst
                           (let ((half (quotient (length lst) 2)))
                             (merge (msort (take lst half))
                                    (msort (drop lst half))))))))
       (map car (msort (map (lambda (x) (cons x (k1 x))) lst))))]))

; Exemples d'utilisation :
(define sorted-by-publisher-1 (mergesort-plus miles-davis-r publisher<=? publisher))
(define sorted-by-publisher-2 (mergesort-plus stan-getz-r publisher<=? publisher))

(pretty-writeln/return sorted-by-publisher-1)
(pretty-writeln/return sorted-by-publisher-2)
