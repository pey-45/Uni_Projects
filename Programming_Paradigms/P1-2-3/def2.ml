let p = function r -> 2. *. Float.pi *. r;;

let area = function r -> Float.pi *. r ** 2.;;

let absf = function n -> if n < 0. then n -. 2. *. n else n;;

let even = function n -> n mod 2 = 0;;

let next3 = function n -> int_of_float (ceil ((float_of_int n) /. 3.)) * 3;;

let is_a_letter = function c -> Char.code c >= 65 && Char.code c <= 122;;

let string_of_bool = function b -> if b = false then "falso" else "verdadero";;
