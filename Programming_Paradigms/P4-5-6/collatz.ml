let f n = if n mod 2 = 0 then n / 2 else 3 * n + 1;;

let rec orbit n = (string_of_int n)^(if n = 1 then "" else ((", ")^(orbit (f n))));;

let rec length n = 
  if n = 1 then 0 else 1 + length (f n);;

let rec top n = 
  if n = 1 then 0 else max n (top (f n));;

let rec length'n'top n = 
  (*i create a function who receives two extra parameters and returns a pair, 
    as the main function only receives one, what leads to concurrent tasks*)
  let rec lengthntop2param n len top = 
    if n = 1 then (len, top)
    (*increases the length and compares the current top and the actual*)
    else lengthntop2param (f n) (len + 1) (max top n) in
    (*the length starts at 0 and we start comparing on n*)
    lengthntop2param n 0 n;;

let rec longest_in a b = 
  let rec length n = if n = 1 then 0 else 1 + length (f n) in
  let rec comparation increment = 
    (*comparo la longitud del actual con la del siguiente*)
    if (length a) < (length (a+increment)) then
      (*si el nuevo mayor encontrado se pasa del limite se devuelve el anterior mayor, 
        si no se sigue comparando con los siguientes*) 
      if a+increment <= b then longest_in (a+increment) b 
      else (a, length a)
    (*si el actual sigue siendo el maximo se compara con el siguiente*)
    else comparation (increment+1) in
  comparation 1;;

(*lo mismo con top*)
let rec highest_in a b = 
  let rec top n = if n = 1 then 0 else max n (top (f n)) in
  let rec comparation increment = 
    if (top a) < (top (a+increment)) then
      if a+increment <= b then highest_in (a+increment) b 
      else (a, top a)
    else comparation (increment+1) in
  comparation 1;;