(*let g n = (n >= 0 && n mod 2 = 0) || n mod 2 = -1;;*)
let g n = 
  if n mod 2 = -1 then true else
    if n >= 0 then
      if n mod 2 = 0 then true else false
    else false;;

let g2 n = (function true -> true | false -> (function false -> false | true -> (function n -> n mod 2 = 0) n) (n >= 0)) (n mod 2 = -1);;