(defclass polynom ()
    ((polunom-symbol :initarg :var1 :reader var1)
    ;; Разреженный список термов в порядке убывания степени
     (term-list :initarg :terms :reader terms)
    )
)


(defun make-term (&key order coeff)
    (list order coeff)
)


(defun order (term) (first term))
(defun coeff (term) (second term))


(defun list-coefs (p)
    (if p (cur-coef (first p) (second p) (list-coefs (rest p))))
)


(defun cur-coef (cur next tail)
    (cond ((null next) (if (= 0 (order cur)) 
                            (cons (coeff cur) tail)
                            (cons (coeff cur) (append (get-zeros (order cur)) tail)))
          )
          ((= (order cur) (1+ (order next))) (cons (coeff cur) tail))
          (t (cons (coeff cur) (append (get-zeros (1- (- (order cur) (order next)))) tail)))
    )
)


(defun get-zeros (n)
    (make-list n :initial-element '0)
)


(defun get-d (p a)
    (let  ((d (list (first a))))
        (loop for i in (rest a) 
            do (nconc d (list (+ i (sum-mult-d-S d (remove-last-el a)))))
        )
    (reverse d))
)


; сумма всех слагаемых, кроме первого (a_i)
(defun sum-mult-d-S (d a)
    (sum-list (list-d-s 0 d a (list-length d)))
)

; получить все пары d_j и S, перемножить их, получить список конечных слагаемых
(defun list-d-s (j d a i)
    (if d (cons (d-mult-S (first d) (get-last-n-elems (- (list-length a) j) a) i) (list-d-s (1+ j) (rest d) a (1- i))))
)


; d_j * S -- одно слагаемое
(defun d-mult-S (d a j)
    (cond
        ((oddp j) (* d (sum-list (mult-list (combinations j a)))))
        (t (* -1 (* d (sum-list (mult-list (combinations j a))))))
    )
)

; последних, потому что список коэффициентов перевернут (an ... a1)
(defun get-last-n-elems (count list)
    (last list count)
)


(defun combinations (count list)
    (cond
        ((zerop count) '(()))
        ((endp list)   '())
        (t (nconc (mapcar (let ((item (first list)))
                                (lambda (comb) (cons item comb)))
                                (combinations (1- count) (rest list)))
              (combinations count (rest list))))
    )
)


(defun mult-list (list)
    (mapcar #'(lambda(x) (reduce '* x)) list)
)


(defun sum-list (list)
    (reduce '+ list)
)


(defun remove-last-el (list)
    (loop for i on list
        while (rest i)
            collect (first i)
    )
)


(defun main ()
    (let ((p1 (make-instance 'polynom
          :var1 'x
          :terms (list (make-term :order 2 :coeff 5)
                       (make-term :order 1 :coeff 3.3)
                       (make-term :order 0 :coeff -7))))
         (p2 (make-instance 'polynom
          :var1 'x
          :terms (list (make-term :coeff 1 :order 3)
                       (make-term :coeff 2 :order 1)
                       (make-term :coeff 1 :order 0))))
         (p3 (make-instance 'polynom
          :var1 'x
          :terms (list (make-term :order 5 :coeff -2)
                       (make-term :order 3 :coeff 4)
                       (make-term :order 1 :coeff -6)))))

    (print (get-d p1 (list-coefs (terms p1))))
    (print (get-d p2 (list-coefs (terms p2))))
    (print (get-d p3 (list-coefs (terms p3))))
    (values))
)
