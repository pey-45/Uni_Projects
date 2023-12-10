let rec insert x = function
  | [] -> [x]
  | hd::tl -> 
      if x <= hd then 
        x::hd::tl
      else 
        hd::insert x tl;;


let rec isort = function
  | [] -> []
  | hd::tl -> insert hd (isort tl);;


let bigl = 
  let rec aux i l =
    if i < 500000 then 
      aux (i+1) (i::l)
    else 
      i::l 
  in 
  aux 1 [];;


let insert_t x l =
  let rec aux new_list = function
    | [] -> [x]
    | hd::tl -> 
        if x <= hd then 
          List.rev_append new_list (x::hd::tl)
        else 
          aux (hd::new_list) tl
  in
  aux [] l;;


let isort_t l = 
  let rec aux ordlist = function
    | [] -> []
    | hd::tl -> aux (insert_t hd ordlist) tl 
  in
  aux [] l;;


let rlist n = 
  let rec aux i l = 
    if i < n then 
      aux (i+1) (Random.int 50000::l)
    else 
      Random.int 50000::l 
  in
  aux 1 [];;


(*funciones auxiliares para crear listas ascendentes y descendentes de n elementos*)
let alist n = 
  let rec aux i l =
    if i > 1 then
      aux (i-1) (i::l)
    else 
      i::l
  in
  aux n [];;


let dlist n = 
  let rec aux i l =
    if i < n then 
      aux (i+1) (i::l)
    else 
      i::l
  in
  aux 1 [];;


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


let isort_g ord l = 
  let insert_g x l =
    let rec aux_insert new_list = function
      | [] -> [x]
      | hd::tl -> 
          if ord x hd then 
            List.rev_append new_list (x::hd::tl)
          else 
            aux_insert (hd::new_list) tl 
    in
    aux_insert [] l 
  in
  let rec aux ordlist = function
    | [] -> ordlist
    | hd::tl -> aux (insert_g hd ordlist) tl 
  in
  aux [] l;;
  

(*
# crono isort lc1;;
- : float = 0.000853999999999993653
# crono isort lc2;;
- : float = 0.00492800000000004346

# crono isort_t lc1;;
- : float = 1.911823
# crono isort_t lc2;;
- : float = 9.61364499999999822

# crono isort ld1;;
- : float = 1.75080200000000019
# crono isort ld2;;
- : float = 8.38300199999999762

# crono isort_t ld1;;
- : float = 0.00299500000000030298
# crono isort_t ld2;;
- : float = 0.00308800000000175601

# crono isort lr1;;
- : float = 0.853597999999998081
# crono isort lr2;;
- : float = 4.0418420000000026

# crono isort_t lr1;;
- : float = 0.924044000000002086
# crono isort_t lr2;;
- : float = 4.35940100000000541  

Relaciones entre las listas 2 y 1 (l_2/l_1):
isort lc -> 5.77
isort_t lc -> 5.02
isort ld -> 4.79
isort_t ld -> 1.03
isort lr -> 4.73
isort_t lr -> 4.71

Sobre todo cuando la lista está ordenada de forma descendente, se aprecia 
que el impacto en el tiempo de ejecución al duplicar los elementos se ve 
reducido cuando la implementación es recursiva terminal.

Esto se debe a que en la implementación no terminal la pila se va llenando
en cada iteración mientras que en la terminal se vacía en cada una, y esto 
supone una penalización del tiempo de ejecución si contiene muchos elementos.

El tiempo de ejecución de isort lr2 es ligeramente menor que el de  isort_t lr2 
ya que, pese a que la recursividad terminal ayuda a vaciar la pila y no generar un
stack overflow, en este caso tiene un mayor coste de cpu que la no terminal. 
*)


let rec split l = match l with
  | h1::h2::t -> 
      let t1, t2 = split t 
      in 
      h1::t1, h2::t2 
  | _ -> l, [];;


let rec merge (l1,l2) = match l1, l2 with
  | [], l | l, [] -> l
  | h1::t1, h2::t2 -> 
      if h1 <= h2 then 
        h1::merge (t1, l2)
      else 
        h2::merge (l1, t2);;


let rec msort l = match l with
  | [] | [_] -> l
  | _ -> 
      let l1, l2 = split l 
      in
      merge (msort l1, msort l2);;


let bigl2 = bigl;;


let split_t l =
  let rec aux pair = function
    | h1::h2::tl -> aux (h1::fst pair, h2::snd pair) tl
    (*se les da la vuelta a las listas del par al final para que estén correctamente ordenadas*)
    | [x] -> List.rev (x::fst pair), List.rev (snd pair) (*si queda un elemento se añade a fst*)
    | [] -> List.rev (fst pair), List.rev (snd pair) 
  in
  aux ([],[]) l;;


let merge_t pair = 
  let rec aux l = function 
    | h1::t1, h2::t2 -> 
        if h1 <= h2 then 
          aux (h1::l) (t1, h2::t2) 
        else 
          aux (h2::l) (h1::t1, t2)
    (*si solo queda una lista se añade todo*)
    | [], rest | rest, [] -> List.rev_append rest l
  in
  (*se da la vuelta al final para que queden ordenados*)
  List.rev (aux [] pair);;


let rec msort' l = match l with 
  | [] | [_] -> l
  | _ -> 
      let l1, l2 = split_t l 
      in
      merge_t (msort' l1, msort' l2);;


let bigl3 = [];;


(*
No produce stack overflow porque el algoritmo utiliza el método divide y vencerás, 
por tanto ambas llamadas recursivas msort' l1 y msort' l2 se resuelven antes de hacer
el merge_t, lo que evita la acumulación excesiva en la pila.
*)

(*
# crono msort' lc1;;
- : float = 0.0166809999999983916
# crono msort' lc2;;
- : float = 0.0313839999999991903
# crono msort' ld1;;
- : float = 0.0147649999999970305
# crono msort' ld2;;
- : float = 0.027905999999997988
# crono msort' lr1;;
- : float = 0.0148020000000030905
# crono msort' lr2;;
- : float = 0.0413710000000016
   
El tiempo al duplicar la lista prácticamente se duplica, menos con la lista randomizada, que 
aumenta algo más. Pero en general los tiempos no varían mucho entre distintas ordenaciones
iniciales.
*)


let rec msort_g ord l = 
  let split_t l =
    let rec aux pair = function
      | h1::h2::tl -> aux (h1::fst pair, h2::snd pair) tl
      | [x] -> List.rev (x::fst pair), List.rev (snd pair)
      | [] -> List.rev (fst pair), List.rev (snd pair) 
    in
    aux ([],[]) l
  in
  let merge_t pair = 
    let rec aux l = function 
      | h1::t1, h2::t2 -> 
          if ord h1 h2 then 
            aux (h1::l) (t1, h2::t2) 
          else 
            aux (h2::l) (h1::t1, t2)
      | [], rest | rest, [] -> List.rev_append rest l
    in
    List.rev (aux [] pair)
  in
  match l with 
    | [] | [_] -> l
    | _ -> 
        let l1, l2 = split_t l 
        in
        merge_t (msort_g ord l1, msort_g ord l2);;
