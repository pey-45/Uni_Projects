(*let p = function r -> 2. *. Float.pi *. r;;*)
let p r = 2. *. Float.pi *. r;;

(*let area = function r -> Float.pi *. r ** 2.;;*)
let area r = Float.pi *. r ** 2.;;

(*let absf = function n -> if n < 0. then (-.n) else n;;*)
let absf n = if n < 0. then (-.n) else n;;

(*let even = function n -> n mod 2 = 0;;*)
let even n = n mod 2 = 0;;

(*let next3 = function n -> int_of_float (ceil ((float_of_int n) /. 3.)) * 3;;*)
let next3 n = int_of_float (ceil ((float_of_int n) /. 3.)) * 3;;

(*let is_a_letter = function c -> Char.code c >= Char.code 'A' (*65*) && Char.code c <= Char.code 'z' (*122*);;*)
let is_a_letter c = Char.code c >= Char.code 'A' (*65*) && Char.code c <= Char.code 'z' (*122*);;

(*let string_of_bool = function true -> "verdadero" | false -> "falso";;*)
let string_of_bool b = if b then "verdadero" else "falso";;