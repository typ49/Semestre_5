#!r7rs

(import (scheme base) (scheme cxr) (scheme write))

(define (writeln/return x)
  (write x)
  (newline)
  x)

;;;  ==========================================================================
;;;  Intrada

(define (retain p1? l)
  (if (null? l)
      '()
      (let ((first (car l))
            (more (retain p1? (cdr l))))
        (if (p1? first) (cons first more) more))))

;; fonction complement
(define (complement p1?)
  (lambda (x)
    (not (p1? x))))

;; test de complement :
(define (odd? n)
  (= (remainder n 2) 1))

(define even? (complement odd?))

(writeln/return (even? 2023)) ; Affiche #f (faux)
(writeln/return (even? 2022)) ; Affiche #t (vrai)

;; fonction filter
(define (filter pred lst)
  (if (null? lst)
      '()
      (if (pred (car lst))
          (cons (car lst) (filter pred (cdr lst)))
          (filter pred (cdr lst)))))


;; Fonction remove
(define (remove p1? l)
  (filter (complement p1?) l))

;; test de remove
(define (is-even? n)
  (= (remainder n 2) 0))

(writeln/return (remove is-even? '(1 2 3 4 5 6))) ; Affiche (1 3 5)



;;;  ==========================================================================
;;;  Multiple values

(writeln/return (call-with-values (lambda () (values 'L 3)) cons))

(writeln/return (let ((a 'sure)
                      (b 'here)
                      (x 'little)
                      (y 'players))
                  (let-values (((a b) (values x y)) ; échange les valeur a b avec x y
                               ((x y) (values a b))) ; échange simultanément avec la ligne du dessus x y avec a b
                    (list a b x y))))

(writeln/return (let ((a 'sure)
                      (b 'here)
                      (x 'little)
                      (y 'players))
                  (let*-values (((a b) (values x y)) ; échange a b avec x y
                                ((x y) (values a b))) ; a déjà redefini a b donc, affecte les nouvelles valeurs de a b, soit x, y
                    (list a b x y))))

(writeln/return (let-values (((first) (car '(very little players))) ; car => very
                             ((the-quotient the-remainder) (truncate/ 2000 9))) ; 222 2
                  (cons the-quotient (cons the-remainder first)))) ; resultat 222 2 . very

(define (take n l)
  (if (zero? n) '() (cons (car l) (take (- n 1) (cdr l)))))

(define (drop n l)
  (if (zero? n) l (drop (- n 1) (cdr l))))

(define (take-while p1? l)
  (if (null? l)
      '()
      (let ((first (car l)))
        (if (p1? first) (cons first (take-while p1? (cdr l))) '()))))

(define (drop-while p1? l)
  (if (or (null? l) (not (p1? (car l)))) l (drop-while p1? (cdr l))))

(define (split-at n l)
  (values (take n l) (drop n l)))

;; split-at recursive
(define (split-at-rec n l)
  (if (<= n 0)
      (values '() l)
      (let-values (((front rest) (split-at-rec (- n 1) (cdr l))))
        (values (cons (car l) front) rest))))

(define (span p1? l)
  (values (take-while p1? l) (drop-while p1? l)))

;; span recursive
(define (span-rec p1? l)
  (if (null? l)
      (values '() '())
      (if (p1? (car l))
          (let-values (((front rest) (span-rec p1? (cdr l))))
            (values (cons (car l) front) rest))
          (values '() l))))

(define (partition p1? l)
  (values (retain p1? l) (remove p1? l)))


;; partition efficace
(define (partition-opti p1? l)
  (let loop ((l l) (true-list '()) (false-list '()))
    (cond
      ((null? l) (values (reverse true-list) (reverse false-list)))
      ((p1? (car l)) (loop (cdr l) (cons (car l) true-list) false-list))
      (else (loop (cdr l) true-list (cons (car l) false-list))))))

;; test partition
(let-values (((true-list false-list) (partition odd? '(19 21 10 2023 2024 2025))))
  (writeln/return true-list)
  (writeln/return false-list))

(define (partition-tr p1? l)
  (let loop ((l l) (true-list '()) (false-list '()))
    (cond
      ((null? l) (values (reverse true-list) (reverse false-list)))
      ((p1? (car l)) (loop (cdr l) (cons (car l) true-list) false-list))
      (else (loop (cdr l) true-list (cons (car l) false-list))))))

; Test partition-tr
(let-values (((true-list false-list) (partition-tr odd? '(19 21 10 2023 2024 2025))))
  (display true-list)
  (newline)
  (display false-list)
  (newline))



(define (map-3-functions f1 g1 h1 l)
  (let loop ((l l) (res1 '()) (res2 '()) (res3 '()))
    (if (null? l)
        (values (reverse res1) (reverse res2) (reverse res3))
        (loop (cdr l)
              (cons (f1 (car l)) res1)
              (cons (g1 (car l)) res2)
              (cons (h1 (car l)) res3)))))

; Test
(let-values (((list1 list2 list3) 
              (map-3-functions (lambda (x) (+ x 1))
                               (lambda (x) (+ x 2))
                               (lambda (x) (* x 2))
                               '(19 21 10 2023 2024 2025))))
  (display list1)
  (newline)
  (display list2)
  (newline)
  (display list3)
  (newline))







;;;  ==========================================================================
;;;  Displaying multiple values

(define-syntax write-values
  (syntax-rules ()
    ((write-values the-values) (let ((nothing-string "*nothing*")
                                     (and-string " *and* "))
                                 (call-with-values (lambda () the-values)
                                   (lambda arg-list
                                     (if (null? arg-list)
                                         (display nothing-string)
                                         (begin
                                           (write (car arg-list))
                                           (for-each (lambda (arg)
                                                       (display and-string)
                                                       (write arg))
                                                     (cdr arg-list))))
                                     (newline)
                                     #t))))))

;;;  ==========================================================================
;;;  Mini data-bases

(define unknown '??)

(define (unknown? x)
  (eq? x unknown))

(define (mk-recording key-0 title-0 author-list-0 year-0 publisher-0 
		      key-list-0)
  ;;  SYMBOL,STRING,LIST-of[SYMBOL}^,INTEGER,SYMBOL^,LIST-of[SYMBOL]^ -> JAZZ-R
  (list key-0 title-0 author-list-0 year-0 publisher-0 key-list-0))

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

(define key 
  ;;  JAZZ-R -> SYMBOL
  car)

(define title
  ;;  JAZZ-R -> STRING
  cadr)

(define authors
  ;;  JAZZ-R -> LIST-of[SYMBOL]^
  caddr)

(define year
  ;;  JAZZ-R -> INTEGER
  cadddr)

(define (publisher jazz-r0)
  ;;  JAZZ-R -> SYMBOL^
  (car (cddddr jazz-r0)))

(define (cds jazz-r0)
  ;;  JAZZ-R -> LIST-of[SYMBOL]^
  (cadr (cddddr jazz-r0)))

;;;  ==========================================================================
;;;  3 Mélanges


(define (length lst)
  (if (null? lst) 0 (+ 1 (length (cdr lst)))))

(define (extract-max-author-nb/max-year j-list)
  (define (helper lst max-authors max-year)
    (if (null? lst)
        (values max-authors max-year)
        (let* ((current-entry (car lst))
               (current-authors (if (unknown? (authors current-entry)) '() (authors current-entry))) ; Traitement des auteurs inconnus
               (current-year (year current-entry))
               (new-max-authors (max max-authors (length current-authors)))
               (new-max-year (max max-year current-year)))
          (helper (cdr lst) new-max-authors new-max-year))))


  
  (if (null? j-list)
      (values 0 0)
      (helper j-list 0 0)))

(let-values (((max-authors max-year) (extract-max-author-nb/max-year miles-davis-r)))
  (writeln/return (list max-authors max-year))) ; doit afficher (2 1972) et retourner (4 1993)

(let-values (((max-authors max-year) (extract-max-author-nb/max-year stan-getz-r)))
  (writeln/return (list max-authors max-year))) ; doit afficher (2 1980) et retourner (1 1994)


;;;  ==========================================================================
;;;  4 Listes d'associations


(define (assq-split x alist)
  (let ((found-assoc (assq x alist)))
    (if found-assoc
        (values found-assoc (remove (lambda (pair) (eq? (car pair) x)) alist))
        (values #f alist))))


(let-values (((result remaining) (assq-split 'L '((M . 2) (L . 3) (Nautilus . -20000)))))
  (writeln/return (list result remaining)))
; Résultat attendu : ((L . 3) ((M . 2) (Nautilus . -20000)))

(let-values (((result remaining) (assq-split 'Titanic '((M . 2) (L . 3) (Nautilus . -20000)))))
  (writeln/return (list result remaining)))
; Résultat attendu : (#f ((M . 2) (L . 3) (Nautilus . -20000)))


(define (merge-alists alist alist0)
  (let ((merged '())
        (diffs '()))
    (for-each
     (lambda (pair)
       (let ((match (assq (car pair) alist0)))
         (cond
          ((and match (equal? (cdr pair) (cdr match))) (set! merged (cons pair merged)))
          (match (set! diffs (cons (car pair) diffs)))
          (else (set! merged (cons pair merged))))))
     alist)
    (for-each
     (lambda (pair)
       (unless (or (assq (car pair) alist) (assq (car pair) merged))
         (set! merged (cons pair merged))))
     alist0)
    (values merged diffs)))

; Tests
(let-values (((result-keys result-diff) (merge-alists '((players . little) (L . 3) (here . ufc))
                                                     '((L . 3) (there . ubfc) (players . master)))))
  (writeln/return (list result-keys result-diff)))

; Expected result: (((L . 3) (here . ufc) (there . ubfc)) (players))


;;;  ==========================================================================
;;;  5 Révisions

(define (included-into? l0 l1)
  (if (null? l0)
      #t
      (if (member (car l0) l1)
          (included-into? (cdr l0) l1)
          #f)))

(let-values (((result) (included-into? '(19 10 2023) '(2023 19 21 10))))
  (writeln/return result)) ; Expected output: #t

(let-values (((result) (included-into? '(19 10 2023) '(2023 20 21 10))))
  (writeln/return result)) ; Expected output: #f

(define (same-elts? l0 l1)
  (and (included-into? l0 l1) (included-into? l1 l0)))


(let-values (((result) (same-elts? '(19 10 2023) '(2023 19 10))))
  (writeln/return result)) ; Expected output: #t

(let-values (((result) (same-elts? '(19 10 2023) '(2023 20 21 10))))
  (writeln/return result)) ; Expected output: #f

(let-values (((result) (same-elts? '(19 10 2023) '(2023 19 21 10))))
  (writeln/return result)) ; Expected output: #f


(define (find-tail p1? l)
  (cond
    ((null? l) #f)
    ((p1? (car l)) l)
    (else (find-tail p1? (cdr l)))))

; Test cases:
(display (find-tail even? '(19 21 10 2023))) ; Expected: (10 2023)
(newline)
(display (find-tail zero? '(19 21 10 2023))) ; Expected: #f
(newline)


; memq-rd function
(define (memq-rd item lst)
  (cond
    ((null? lst) #f)
    ((eq? item (car lst)) (if (memq-rd item (cdr lst)) (memq-rd item (cdr lst)) lst))
    (else (memq-rd item (cdr lst)))))

; member-rd function
(define (member-rd pred lst)
  (cond
    ((null? lst) #f)
    ((pred (car lst)) (if (member-rd pred (cdr lst)) (member-rd pred (cdr lst)) lst))
    (else (member-rd pred (cdr lst)))))

; assq-ff function
(define (assq-ff pred alist)
  (cond
    ((null? alist) #f)
    ((pred (caar alist)) (car alist))
    (else (assq-ff pred (cdr alist)))))

; Test cases:
(display (memq-rd 'a '(b a c a d))) ; Expected: (a d)
(newline)
(display (member-rd (lambda (x) (> x 2)) '(1 2 3 4 3 2))) ; Expected: (3 2)
(newline)
(display (assq-ff (lambda (x) (> x 2)) '((1 . a) (3 . b) (2 . c)))) ; Expected: (3 . b)
(newline)
(let-values (((result) (assq-ff (lambda (key) (eq? key 'L)) '((M . 2) (L . 3) (Nautilus . -20000)))))
  (writeln/return result)) ; Expected output: (L . 3)

(let-values (((result) (assq-ff (lambda (key) (eq? key 'Titanic)) '((M . 2) (L . 3) (Nautilus . -20000)))))
  (writeln/return result)) ; Expected output: #f
