let rec orbit n = 
  let f n = if n mod 2 = 0 then n / 2 else 3 * n + 1 in
  (string_of_int n)^(if n = 1 then "" else ((", ")^(orbit (f n))));;

let rec length n = 
  let f n = if n mod 2 = 0 then n / 2 else 3 * n + 1 in
  if n = 1 then 0 else 1 + length (f n);;

let rec top n = 
  let f n = if n mod 2 = 0 then n / 2 else 3 * n + 1 in
  if n = 1 then 0 else max n (top (f n));;

let rec length'n'top n = 
  let f n = if n mod 2 = 0 then n / 2 else 3 * n + 1 in
  let rec length n = if n = 1 then 0 else 1 + length (f n) in
  let rec top n = if n = 1 then 0 else max n (top (f n)) in
  (length n, top n);;

let rec longest_in a b =
  let f n = if n mod 2 = 0 then n / 2 else 3 * n + 1 in
  let rec length n = if n = 1 then 0 else 1 + length (f n) in
  

let rec highest_in a b =
  let f n = if n mod 2 = 0 then n / 2 else 3 * n + 1 in
  let rec top n = if n = 1 then 0 else max n (top (f n)) in
  if a = b then 0 else max (top a) (highest_in (a+1) b);;