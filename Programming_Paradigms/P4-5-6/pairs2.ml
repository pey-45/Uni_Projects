let rec next (x, y) = 
  if iseven (x+y) then 
    if (x = 1) then (x, y+1) 
    else (x-1, y+1)
  else 
    if (y = 1) then (x+1, y) 
    else (x+1, y-1);;

let rec steps_from (x, y) n = 
  if n > 0 then 
    steps_from (next (x, y)) (n-1) 
  else (x, y);;

let pair n = 
  let rec aux (x, y) n = 
    if n > 1 then 
      aux (next (x, y)) (n-1)
    else (x, y) in
  aux (1, 1) n;;

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