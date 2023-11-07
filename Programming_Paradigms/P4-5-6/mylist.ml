let hd = function 
  [] -> raise(Failure "hd") | 
  x::_ -> x;;

let tl = function 
  [] -> [] | 
  [x] -> [x] | 
  _::l -> l;;

let rec length = function 
  [] -> 0 | 
  _::l -> 1 + length l;;

let compare_lengths l1 l2 = length l1 - length l2;;

let compare_lengths_with l s = length l - s;;

let init n f = 
  let rec aux i n f = 
    if i < n then 
      (f i)::(aux (i+1) n f) 
    else [] in
  aux 0 n f;;

let nth l n = 
  let rec aux i l n = 
    if i < n then 
      aux (i+1) (tl l) n 
    else hd l in
  aux 0 l n;;

let rec append l1 l2 = match l1 with | 
  [] -> l2 | 
  hd1::tl1 -> hd1::(append tl1 l2);;

let rec rev_append l1 l2 = 
  if (length l1) > 1 then 
    rev_append (tl l1) ((hd l1)::l2) 
  else ((hd l1)::l2);;

let rev l = 
  let rec aux l a = match l with |
    [] -> a |
    hdl::tll -> aux tll (hdl::a) in
  aux l [];;

let rec concat ll = match ll with |
  [] -> [] |
  [x] -> x |
  hdll::tlll -> append hdll (concat tlll);;

let flatten = concat;;

let split lp = 
  let rec l1 lp = 
    if (length lp) > 1 then 
      fst (hd lp)::(l1 (tl lp)) 
    else (fst (hd lp))::[] in
  let rec l2 lp = 
    if (length lp) > 1 then 
      snd (hd lp)::(l1 (tl lp)) 
    else (snd (hd lp))::[] in
  (l1 lp, l2 lp);;

let combine l1 l2 =
  













let a = [1; 2; 3; 4; 5];;
let b = [10; 9; 8; 7; 6; 5; 4; 3; 2; 1; 0];;
let ll = [a; b];;

let lp = [(1, 2); (3, 4); (5, 6); (7, 8)];;

let n = hd a;;