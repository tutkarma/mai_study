(defun get-matrix (n)
  (let  ((matrix (make-array (list n n)))
        (cnt (ceiling n 2))
        (num 1))

  (dotimes (i cnt)

    (loop for j from 0 upto (1- n)
      do (setf (aref matrix j (* i 2)) num)
         (setf num (1+ num))
    )

    ; условие, чтобы корректно обрабатывались матрицы с нечетным размером
    (when (or (evenp n) (< i (1- cnt)))
      (loop for j from (1- n) downto 0
        do (setf (aref matrix j (1+ (* i 2))) num)
           (setf num (1+ num))
      )
    )
  )

    matrix)
)


(defun matrix-tl-bl (n)
  (get-matrix n)
)


(print (matrix-tl-bl 1))
(print (matrix-tl-bl 5))
(print (matrix-tl-bl 8))