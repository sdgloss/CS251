(define insert
  (lambda (v bst)
    (if (or (null? bst) (not (check_bst bst)))
      (list v)
      (if (= (entry bst) v)
        bst
        (if (and (< (entry bst) v) (= 0 (length (left bst))))
          (cons (list v) (cons (list (entry bst))) (right bst))
          (if (and (< (entry bst) v) not(= 0 (length (left bst))))
            (cons (insert v (left bst)) (right bst))
            (if (> (entry bst) v)
              (if (= 0 (length(right bst)))
                (cons (left bst) (cons (list (entry bst)) (list v)))
                (cons (left bst) (insert v (right bst)))))))))))

,tr (insert '5 '(6 () ()))