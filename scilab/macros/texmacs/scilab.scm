
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; MODULE      : scilab.scm
;; DESCRIPTION : Special support for scilab
;; COPYRIGHT   : (C) 1999  Joris van der Hoeven
;;
;; This software falls under the GNU general public license and comes WITHOUT
;; ANY WARRANTY WHATSOEVER. See the file $TEXMACS_PATH/LICENSE for details.
;; If you don't have this file, write to the Free Software Foundation, Inc.,
;; 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Initialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




(define (scilab-input-var-row r)
  (if (not (null? r))
      (begin
	(display ", ")
	(cas-input (car r))
	(scilab-input-var-row (cdr r)))))

(define (scilab-input-row r)
  ;;(display "[")
  (cas-input (car r))
  (scilab-input-var-row (cdr r))
  ;;(display "]")
)

(define (scilab-input-var-rows t)
  (if (not (null? t))
      (begin
	(display "; ")
	(scilab-input-row (car t))
	(scilab-input-var-rows (cdr t)))))

(define (scilab-input-rows t)
  (display "[")
  (scilab-input-row (car t))
  (scilab-input-var-rows (cdr t))
  (display "]"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Initialization
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(connection-declare "scilab" "scilab --texmacs")
(connection-format "scilab" "verbatim" "generic")
(set! session-menu (menu-merge session-menu
  '(("Scilab" (make-session "scilab" "default")))))
(define (in-scilab?) (equal? (get-env "prog language") "scilab"))

(cas-supports-math-input-set! "scilab")

(set! cas-input-current-cas "scilab")
(cas-input-set! 'rows scilab-input-rows)
(display "")
