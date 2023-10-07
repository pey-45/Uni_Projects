let rec sumto n = if (n > 0) then (n + sumto (n-1)) else 0;;

let rec exp10 n = if (n > 0) then (10 * exp10 (n-1)) else 1;;

let rec num_cifras n = if ((if n > 0 then n else -n) >= 10) then (1 + (num_cifras (n/10))) else 1;;

let abs n = if (n < 0) then -n else n;; (*auxiliar*)
let rec sum_cifras n = if (abs n >= 10) then ((abs n mod 10) + (sum_cifras (n/10))) else abs n;;