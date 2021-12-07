;;dave's code
(define mult
  (lambda (a)
    (lambda (b)
      (* a b))))


;;takes a function and three inputs then functions those inputs
(define curry3
  (lambda (f)
    (lambda (x)
      (lambda (y)
        (lambda (z)
          (f x y z))))))

;;takes a function and then 3 numbers and returns them as a list of three elements
(define uncurry3
  (lambda (fxn)
    (lambda (x y z)
      (((fxn x) y) z))))

;;dave's code
(define get-all-but-first-input
  (lambda args
    (cdr args)))

;; we couldn't figure this out. :( 
(define uncurry
  (lambda (fxn)
    (lambda lst
      (fxn (uncurry-helper lst)))))

;;takes in a list for uncurry then recursively returns it in the proper form. 
(define uncurry-helper
  (lambda lst
    (if (= (length lst) 1)
      lst
      (cons (car lst) (uncurry-helper (cdr lst))))))

(define test-uncurry-helper
  (lambda (fn lst)
    (if (= (length lst) 1)
      (car lst)
      (cons (cons (car lst)) (test-uncurry-helper fn (cdr lst))))))

;;returns a list where the procedure has been applied
(define my-filter
  (lambda (fxn lst)
    (if (null? lst)
      '()
      (if (fxn (car lst))
        (cons (car lst) (my-filter fxn (cdr lst)))
        (my-filter fxn (cdr lst))))))

;; union
;; parameters: (list) lst1, (list) lst2
;; return: return the union of lst1 and lst2     
(define union
  (lambda (lst1 lst2)
    (append lst1 (my-filter (lambda (x) (not (member x lst1))) lst2))))

;;sees if an element is in both lists and returns all the elements that are!
(define intersect
  (lambda (lst1 lst2)
       (my-filter (lambda (x) (member x lst1)) lst2)))


;; exists
;; parameters: (function) fxn, (list) lst
;; return: Returns #t if any of the item in lst meets the requriement of the fxn
(define exists 
  (lambda (fxn lst)
    (if (null? (my-filter fxn lst))
      #f
      #t)))




