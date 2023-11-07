let iseven n = n mod 2 = 0;;

let rec power x y = if y > 1 then x * power x (y-1) else x;;

let rec power' x y = 
  if y > 1 then
    if iseven y then 
      power' (x*x) (y/2) 
    else x * power' (x*x) (y/2)
  else x;;

(*x^n = (x*x)^(n/2) ya que convertir x en x*x es elevarlo
  al cuadrado, por tanto multiplicar el exponente por 2, 
  entonces se debe dividir la potencia entre 2
  para que el resultado sea el mismo*)

(*power' es mas eficiente porque requiere menos iteraciones, 
  pues en power 'y' decrece linealmente mientras que en power'
  decrece exponencialmente (rapido->lento). 
  De hecho en power el decrecimiento de 'y' es de una unidad por
  iteracion, y en power' este es el decrecimiento mínimo que puede 
  sufrir 'y' (cuando pasa de 2 o 3 a 1).*)

let rec powerf x y =
  if y > 1 then
    if iseven y then powerf (x*.x) (y/2) else x *. powerf (x*.x) (y/2)
  else x;; 

let pair_i p =
  let rec find i =
    if pair i = p then i
    else find (i+1)
  in find 1;;

let pair_i' p = 
  let rec row_n i n = 
    if i < n-1 then i + (row_n (i+1) n)
    else 1 in 
  let start = row_n 1 (fst p + snd p) in 
  let rec find i =
    if pair i = p then i
    else find (i+1)
  in find start;;

(*la nueva implementación es más rápida ya que 
  el crecimiento del indice de búsqueda en pair_i es constante,
  de x+1, y en pair_i' es de x+(1, 2, 3, ..., n) con incremento 
  del sumando en cada iteración.
  Una vez localizada la fila se busca desde el primer elemento de esa
  fila, ahorrando todas las iteraciones innecesarias anteriores*)