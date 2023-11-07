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

let rec compare_lengths l1 l2 = match (l1, l2) with 
  ([], []) -> 0 |
  ([], _) -> 1 |
  (_, []) -> -1 |
  (hdl1::tll1, hdl2::tll2) -> compare_lengths tll1 tll2;;

let compare_length_with l s = 
  let len = length l in
  if len > s then 1
  else if len < s then -1
  else 0;;

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
      snd (hd lp)::(l2 (tl lp)) 
    else (snd (hd lp))::[] in
  (l1 lp, l2 lp);;

let rec combine l1 l2 =
  if (length l1) > 1 then
    (hd l1, hd l2)::(combine (tl l1) (tl l2))
  else (hd l1, hd l2)::[];;

let rec map f l = match l with
  [] -> [] |
  hdl::tll -> (f hdl)::(map f tll);;

let rec map2 f l1 l2 = 
  if (length l1) > 1 then
    (f (hd l1) (hd l2))::(map2 f (tl l1) (tl l2))
  else (f (hd l1) (hd l2))::[];;

let rev_map f l = rev (map f l);;

let rec for_all f l = match l with
  [] -> true |
  hdl::tll -> (f hdl) && (for_all f tll);;

let rec exists f l = match l with
  [] -> false |
  hdl::tll -> (f hdl) || (exists f tll);;

let rec mem x l = match l with 
  [] -> false |
  hdl::tll -> x = hdl || (mem x (tll));;

let rec find f l = 
  if (length l) = 1 then raise(Failure "Not_found")
  else if f (hd l) then hd l 
  else find f (tl l);;

let rec filter f l = match l with
  [] -> [] |
  hdl::tll -> if (f hdl) then hdl::(filter f tll)
              else filter f tll;;

let find_all = filter;;

let partition f l = 
  let rec filter_non f l = match l with
    [] -> [] |
    hdl::tll -> if not (f hdl) then hdl::(filter_non f tll)
                else filter_non f tll in
  (filter f l, filter_non f l);;
  
let rec fold_left f init l = 
  if (length l) != 1 then 
    fold_left f (f init (hd l)) (tl l)
  else f init (hd l);;

let rec fold_right f l init = 
  if (length l) != 1 then 
    f (hd l) (fold_right f (tl l) init)
  else f (hd l) init;;

let rec assoc x l = match l with 
  [] -> raise(Failure "Not_found") |
  hdl::tll -> if fst hdl = x then snd hdl 
              else assoc x tll;;

let rec mem_assoc x l = match l with 
  [] -> false |
  hdl::tll -> if fst hdl = x then true
  else mem_assoc x tll;;

let rec remove_assoc x l = match l with 
  [] -> [] |
  hdl::tll -> if fst hdl = x then remove_assoc x tll 
              else hdl::(remove_assoc x tll);;

(*
let a = [1; 2; 3; 4; 5];;
let b = [10; 9; 8; 7; 6; 5; 4; 3; 2; 1; 0];;
let c = [6; 8; 1; 10; 11];;
let par = [2; 4; 6; 8; 10];;
let impar = [1; 3; 5; 7; 9];;
let ll = [a; b];;

let lp = [(1, 2); (3, 4); (5, 6); (7, 8)];;
let lp2 = [(1, 19); (3, 35); (5, 7); (7, 11)];;
*)