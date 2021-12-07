(define insert-test
 (lambda (v bst)
  (if (or (null? bst) (= (entry bst) v))
    (cons (list v) bst)
    (if (< v (entry bst))
      (cons (insert v (left bst)) (list (entry bst)))
      (if (> v (entry bst))
        (cons (list (entry bst)) (insert v (right bst))))))))

,tr (insert-test '5 '(8 () ()))