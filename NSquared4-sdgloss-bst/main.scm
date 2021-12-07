;makes sure the bst meets the specifications
(define check_bst
  (lambda (bst)
    (if (and (and (= (length bst) 3) (list? (car (cdr bst)))) (list? (car (cdr (cdr bst)))))
      #t
      #f)))

;returns the first element of the bst
(define entry 
  (lambda (bst)
    (if (check_bst bst)
      (car bst)
      #f)))

;returns the left element of the bst 
(define left
  (lambda (bst)
    (if (check_bst bst)
        (car (cdr bst))
        #f)))

;returns the right element of the bst
(define right
  (lambda (bst)
    (if (check_bst bst)
        (car (cdr (cdr bst)))
        #f)))

;is given a value and two BSTs then turns it into one bigger BST. otherwise returns false
(define make-bst
  (lambda (elt left-bst right-bst)
    (if (and (and (integer? elt) (legal_bst left-bst)(legal_bst right-bst)))
      (list elt left-bst right-bst)
        #f)))

;is given a bst and returns whether it meets make-bst's rules
(define legal_bst
  (lambda (bst)
    (if (null? bst)
      #t
     (if (and (and (and (list? bst) (= 3 (length bst)))(list?(car (cdr bst)))) (list? (car (cdr (cdr bst)))))
      #t
      #f)))) 



;inserts the roof value, then goes left, then goes right
(define preorder
  (lambda (bst)
    (if (null? bst)
      '()
      (if (and (not (check_bst (left bst))) (not (check_bst (right bst))))
        (list (entry bst))
        (if (and (check_bst (left bst)) (not (check_bst (right bst))))
          (append (list (entry bst)) (preorder(left bst)))
          (if (and (not (check_bst (left bst))) (right bst))
            (append (list (entry bst)) (preorder(right bst)))
            (append (list (entry bst)) (preorder (left bst)) (preorder (right bst)))))))))


;traverses left, then takes the root, then traverses right
(define inorder
  (lambda (bst)
    (if (null? bst)
      '()
      (if (and (not (check_bst (left bst))) (not (check_bst (right bst))))
        (list (entry bst))
        (if (and (check_bst (left bst)) (not (check_bst (right bst))))
          (append (inorder (left bst)) (list (entry bst)) )
          (if (and (not (check_bst (left bst))) (right bst))
            (append (list (entry bst))(inorder (right bst)))
            (append (inorder (left bst)) (list (entry bst))  (inorder (right bst)))))))))
            
;traverses left, then the right, then the roof
(define postorder
  (lambda (bst)
    (if (null? bst)
      '()
      (if (and (not (check_bst (left bst))) (not (check_bst (right bst))))
        (list (entry bst))
        (if (and (check_bst (left bst)) (not (check_bst (right bst))))
          (append (postorder(left bst)) (list (entry bst)))
          (if (and (not (check_bst (left bst))) (right bst))
            (append (postorder(right bst)) (list (entry bst)))
            (append (postorder (left bst)) (postorder (right bst))(list (entry bst)))))))))

; traverses down to the element where the value of v is appropriate then inserts it 
(define insert
 (lambda (v bst)
  (if (null? bst)
    (list v '() '())
    (if (or (null? bst) (= (car bst) v))
      (list (car bst) '() '())
      (if (< v (car bst))
        (list (car bst) (insert v (left bst)) (right bst))
        (if (> v (car bst))
          (list (car bst) (left bst) (insert v (right bst)))))))))



