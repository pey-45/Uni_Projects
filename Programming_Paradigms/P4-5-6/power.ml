let rec power x y = if y > 1 then x * power x (y-1) else x;;

let rec power' x y = 
  let iseven n = n mod 2 = 0 in
  if y > 1 then
    if iseven y then power' (x*x) (y/2) else x * power' (x*x) (y/2)
  else x;;

(*pow' es mas eficiente porque requiere menos iteraciones, 
  pues en pow y decrece linealmente mientras que en pow'
  decrece exponencialmente (rapido->lento)*)

let rec powerf x y =
  let iseven n = n mod 2 = 0 in
  if y > 1 then
    if iseven y then powerf (x*.x) (y/2) else x *. powerf (x*.x) (y/2)
  else x;; 
  