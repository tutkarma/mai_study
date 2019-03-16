(defun castle-moves (k l m n)
  (cond ((or (= k m)
              (= l n)) t)
        (t (values k n))))