(defun whitespace-char-p (char)
    (member char '(#\Space #\Tab #\Newline)))


(defun word-list (string)
    ;; Разбить строки на слова, разделённые знаками whitespace
    ;; A la (split-seq-if #'whitespace-char-p string)
    (loop with len = (length string)
          for left = 0 then (1+ right)
            for right = (or (position-if #'whitespace-char-p string
                                          :start left)
                            len)
          unless (= right left)   ; исключить пустые слова
            collect (subseq string left right)
      while (< right len)
    )
)


(defun remove-odd-and-reverse-even (list-words)
    (if list-words (concatenate 'string (reverse (cadr list-words)) " " (remove-odd-and-reverse-even (cddr list-words))))
)


(defun remove-and-reverse-words (string)
    (string-right-trim " " (remove-odd-and-reverse-even (word-list string)))
)


(print (remove-and-reverse-words "во что бы то ни стало"))
(print (remove-and-reverse-words "Как-то в полночь, в час угрюмый, утомившись от раздумий, задремал я над страницей фолианта одного"))
(print (remove-and-reverse-words "eighteen till i die"))
(print (remove-and-reverse-words "съешь ещё этих мягких французских булок, да выпей чаю"))
