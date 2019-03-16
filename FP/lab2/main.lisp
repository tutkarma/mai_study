(defun map-set (func X)
    (cond
        ((null X) nil)
        (t (mycons (funcall func (car X)) (map-set func (cdr X)) ))
    )
)


(defun mycons (X Y)
    (cond ((null (member X Y)) (cons X Y))
        (t Y))
)


(print (map-set #'abs '(1 2 -3 -2)))
(print (map-set #'identity (list 20 20 30 30 40 40)))
(print (map-set #'abs (list )))
(print (map-set #'abs (list 1 -1 1 -1 1 -1)))
(print (map-set #'sqrt (list 4 9 16 25 25 16 9 4)))
(print (map-set (lambda (x) (* x x)) (list 1 -2 3 -4 5 -6 7)))