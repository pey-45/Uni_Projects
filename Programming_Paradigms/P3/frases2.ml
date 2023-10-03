(*x - y;;*)
(*- : int = x - y*)
(*Error: Unbound value x*)

let x = 1;;
(*- : val x = 1*)
(*val x : int = 1*)

(*x - y;;*)
(*Error: Unbound value y*)

let y = 2;;
(*val y : int = 2*)

x - y;;
(*- : int = -1*)

(*let x = y in x - y;;*)
(function x -> x - y) y;;
(*- : int = 0*)

x - y;;
(*- : int = -1*)

(*z;;*)
(*val z*)
(*Error: Unbound value z*)

let z = x + y;;
(*val z: int = 3*)

z;;
(*- : int = 3*)

let x = 5;;
(*val x : int = 5*)

x + y;;
(*- : int = 7*)

z;;
(*- : int = 3*)

(*let y = 5 in x + y;;*)
(function y -> x + y) 5;;
(*- : int = 10*)

x + y;;
(*- : int = 7*)

(*let x = x + y in let y = x * y in x + y + z;;*)
(function (x, y) -> x + y + z) (x + y, (x + y) * y);;
(*- : int = 24*)

x + y + z;;
(*- : int = 10*)

function x -> 2 * x;;
(*- : int -> int = <fun>*)

(function x -> x * 2) (2 + 1);;
(*- : int = 6*)

(function x -> x * 2) 2 + 1;;
(*- : int = 5*)

let f = function x -> 2 * x;;
(*val f : int -> int = <fun>*)

f;;
(*Error: Expected value?*)
(*- : int -> int = <fun>*)

f (2 + 1);;
(*- : int = 6*)

f 2 + 1;;
(*- : int = 5*)

f x;;
(*- : int = 10*)

let x = 100;;
(*val x : int = 100*)

f x;;
(*- : int = 200*)

let m = 1000;;
(*val m : int = 1000*)

let g = function x -> x + m;;
(*- : int -> int = <fun>*)
(*val g : int -> int = <fun>*)

g;;
(*- : int -> int = <fun>*)

g 3;;
(*- : int = 1003*)

(*g 3.0;;*)
(*Error: Operation has type float but a value was expected of type int*)
(*Error: This expression has type float but an expression was expected of type int*)

let m = 7;;
(*val m : int = 7*)

g 3;;
(*- : int = 10*)
(*- : int = 1003*)
(*LA FUNCION FUE DECLARADA CON EL VALOR DE M = 3, ES COMO SI SE HUBIERA DECLARADO COMO X + 3, ES INMUTABLE*)

let istrue = function true -> true;;
(*val istrue : bool -> bool = <fun>*)

istrue;;
(*- : bool -> bool = <fun>*)

istrue (1 < 2);;
(*- : bool = true*)

istrue (2 < 1);;
(*- : bool = false*)
(*Exception: Match_failure ("//toplevel//", 1, 13)*)

(*istrue 0;;*)
(*Exception: Match_failure ("//toplevel//", 1, 13)*)
(*Error: This expression has type int but an expression was expected of type bool*)

let iscero_v1 = function 0 -> true;;
(*val iscero_v1 : int -> bool = <fun>*)

iscero_v1 0;;
(*- : bool = true*)

(*iscero_v1 0.;;*)
(*Error: This expression has type float but an expression was expected of type int*)

iscero_v1 1;;
(*Exception: Match_failure ("//toplevel//", 1, 13)*)
(*Exception: Match_failure ("//toplevel//", 1, 16)*)


let iscero_v2 = function 0 -> true | _ -> false;;
(*val iscero_v2 : int -> bool | val -> bool = <fun>*)
(*val iscero_v2 : int -> bool = <fun>*)

iscero_v2 0;;
(*- : bool = true*)

iscero_v2 1;;
(*- : bool = false*)

(*iscero_v2 0.;;*)
(*Error: This expression has type float but an expression was expected of type int*)

let all_to_true = function true -> true | false -> true;;
(*val all_to_true : bool -> bool = <fun>*)

all_to_true (1 < 2);;
(*- : bool = true*)

all_to_true(2 < 1);;
(*- : bool = true*)

(*all_to_true 0;;*)
(*Error: This expression has type int but an expression was expected of type bool*)

let first_all_to_true = all_to_true;;
(*val first_all_to_true : bool -> bool = <fun>*)

let all_to_true = function x -> true;;
(*val all_to_true : int -> bool = <fun>*)
(*val all_to_true : 'a -> bool = <fun>*)

all_to_true (1 < 2);;
(*- : bool = true*)

all_to_true (2 < 1);;
(*- : bool = true*)

all_to_true 0;;
(*- : bool = true*)

(*first_all_to_true 0;;*)
(*Error: This expression has type int but an expression was expected of type bool*)