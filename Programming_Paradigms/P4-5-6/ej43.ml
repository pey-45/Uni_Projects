let rec reverse n = 
  let abs n =  if n > 0 then n  else -n in
  let rec cifras n = 
    if (abs n) >= 10 then 
      1 + (cifras (n/10)) 
    else 1 in
  if n >= 1 then 
    n mod 10 * int_of_float (10.** float_of_int (cifras n - 1)) + reverse (int_of_float (float_of_int n /. 10.)) 
  else 0;;

let rec palindromo s = 
  if String.length s > 1 then 
    if s.[0] = s.[String.length s - 1] then 
      palindromo (String.sub s 1 (String.length s - 2)) 
    else false s
  else true;;

let rec mcd(x, y) = 
  if x = 0 then y 
  else mcd(y mod x, x);;