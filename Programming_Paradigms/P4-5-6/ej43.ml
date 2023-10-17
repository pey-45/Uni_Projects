let rec reverse n = 
  let rec cifras n = if (if n > 0 then n else -n) >= 10 then 1 + (cifras (n/10)) else 1 in
  if n > 1 then n mod 10 * int_of_float (10.** float_of_int (cifras n - 1)) + reverse (int_of_float (float_of_int n /. 10.)) else 0;;

