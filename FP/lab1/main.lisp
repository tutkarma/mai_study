(defun castle-moves (k l m n)
  (cond ((or (= k m)
              (= l n)) "T")
        (t (values k n))))

(castle-moves 1 1 1 1)
(castle-moves 4 5 7 8)
(castle-moves 7 7 2 4)
(castle-moves 2 6 6 6)