let rec insert x = function
  [] -> [x] |
  h::t -> if x <= h 
            then x :: h :: t
            else h :: insert x t;;


let rec isort = function
  [] -> [] |
  h::t -> insert h (isort t);;


let bigl = 
  let rec aux l i = match l with
    [] -> aux (i::[]) (i+1) |
    hd::tl -> if i < 500000
                then aux (i::hd::tl) (i+1)
                else i::hd::tl in 
  aux [] 0;;


let rec insert_t x = function 
  [] -> [x] |
  h::t -> if x <= h 
            then x :: h :: t
            else insert_t x (h::t);;


let isort_t l = 
  let rec aux ordlist l = match l with
    [] ->  l |
    hd::tl -> aux (insert_t hd ordlist) tl in
  aux [] l;;


let rlist n = 
  let randomness = 999 in
  let rec aux l i n = match l with 
    [] -> aux ((Random.int randomness)::[]) (i+1) n |
    hd::tl -> if i < n 
                then aux ((Random.int randomness)::hd::tl) (i+1) n
                else (Random.int randomness)::hd::tl in
  aux [] 1 n;;

(*funciones auxiliares para crear listas ascendentes y descendentes de n elementos*)

let alist n = 
  let rec aux l i n = match l with 
    [] -> aux (i::[]) (i-1) n |
    hd::tl -> if i > n
                then aux (i::hd::tl) (i-1) n
                else i::hd::tl in
  aux [] n 1;;


let dlist n = 
  let rec aux l i n = match l with 
    [] -> aux (i::[]) (i+1) n |
    hd::tl -> if i < n
                then aux (i::hd::tl) (i+1) n
                else i::hd::tl in
  aux [] 1 n;;


let lc1 = alist 10000;;
let lc2 = alist 20000;;
let ld1 = dlist 10000;;
let ld2 = dlist 20000;;
let lr1 = rlist 10000;;
let lr2 = rlist 20000;;

let crono f x =
  let t = Sys.time () in
  let _ = f x in
  Sys.time () -. t;;

