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