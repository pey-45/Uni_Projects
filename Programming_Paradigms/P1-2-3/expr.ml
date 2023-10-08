();; 
(*null*)
(*- : unit = ()*) 

2 + 5 * 3;;
(*- : int = 17*)

1.25 *. 2.0;;
(*- : float = 2.5*)

(*2 - 2.0;;*)
(*Error: this expression has type float but an expression was expected of type int*)
(*Error de tipo porque '-' es un operador de tipo int y 2.0 es de tipo float*)

(*3.0 + 2.0;;*)
(*Error: this expression has type float but an expression was expected of type int*)
(*Error de tipo porque '+' es un operador de tipo int y 3.0 y 2.0 son de tipo float*)

5 / 3;;
(*- : int = 1*)

5 mod 3;;
(*- : int = 2*)

2.0 *. 3.0 ** 2.0;;
(*Error: this expression has type float but an expression was expected of type int*)
(*- : float = 18.*)

2.0 ** 3.0 ** 2.0;;
(*- : float = 64.*)
(*- : float = 512.*)

sqrt;;
(*Error: expected expression*)
(*- : float -> float = <fun>*)

(*sqrt 4;;*)
(*Error: this expression has type int but an expression was expected of type float*)
(*Error de tipo porque "sqrt" es una funcion que recibe valores de tipo float y 4 es de tipo int*)

int_of_float;;
(*- : float -> int = <fun>*)

float_of_int;;
(*- : int -> float = <fun>*)

3.0 = float_of_int 3;;
(*- : true*)
(*- : bool = true*)

(*int_of_float -2.9;;*)
(*- : int = -2*)
(*Error: This expression has type float -> int but an expression was expected of type int*)
(*Error de tipo porque (-2.9) deberia ir entre parentesis, entonces el compilador lo toma como tipo int e "int_of_float" es una funcion que recibe valores de tipo float*)

(*int_of_float 2.1 + int_of_float -2.9;;*)
(*Error: This expression has type float -> int but an expression was expected of type int*)
(*Error de tipo porque (-2.9) deberia ir entre parentesis, entonces el compilador lo toma como tipo int e "int_of_float" es una funcion que recibe valores de tipo float*)

truncate;;
(*- : float -> int = <fun>*)

truncate 2.1 + truncate (-2.9);;
(*- : int = 0*)

floor;;
(*- : float -> int = <fun>*)
(*- : float -> float = <fun>*)

floor 2.1 +. floor (-2.9);;
(*- : float = 0.*)
(*- : float = -1.*)

ceil;;
(*- : float -> float = <fun>*)

ceil 2.1 +. ceil (-2.9);;
(*- : float = -1.*)
(*- : float = 1.*)

int_of_char;;
(*- : char -> int = <fun>*)

int_of_char 'A';;
(*- : int = 65*)

char_of_int;;
(*- : int -> char = <fun>*)

char_of_int 66;;
(*- : char = "66"*)
(*- : char = 'B'*)

Char.code;;
(*- : char -> int = <fun>*)

Char.code 'B';;
(*- : int = 66*)

Char.chr;;
(*- : char -> char = <fun>*)
(*- : int -> char = <fun>*)

Char.chr 67;;
(*'\067'*)
(*- : char = 'C'*)

'\067';;
(*- : char = 'C'*)

Char.chr (Char.code 'a' - Char.code 'A' + Char.code 'M');;
(*- : char = 'm'*)

Char.lowercase_ascii;;
(*- : char -> char = <fun>*)

Char.lowercase_ascii 'M';;
(*- : char = 'm'*)

Char.uppercase_ascii;;
(*- : char -> char = <fun>*)

Char.uppercase_ascii 'm';;
(*- : char = 'M'*)

"this is a string";;
(*- : string = "this is a string"*)

String.length;;
(*- : string -> int = <fun>*)

String.length "longitud";;
(*- : int = 8*)

(*"1999" + "1";;*)
(*Error: Expression has type string but an expression was expected of type int*)
(*Error de tipo porque '+' es un operador de tipo int y "1999" y "1" son de tipo string*)

"1990" ^ "1";;
(*- : string = "19901"*)

int_of_string;;
(*- : string -> int = <fun>*)

int_of_string "1999" + 1;;
(*- : int = 2000*)

"\065\066";;
(*- : string = "AB"*)

string_of_int;;
(*- : int -> string = <fun>*)

string_of_int 010;;
(*- : string = "10"*)

not true;;
(*- : bool = false*)

true && false;;
(*- : bool = false*)

true || false;;
(*- : bool = true*)

(1 < 2) = false;;
(*- : bool = false*)

"1" < "2";;
(*- : bool = true*)

2 < 12;;
(*- : bool = true*)

"2" < "12";;
(*- : bool = true*)
(*- : bool = false*)

"uno" < "dos";;
(*- : bool = false*)

if 3 = 4 then 0 else 4;;
(*- : int = 4*)

if 3 = 4 then "0" else "4";;
(*- : string = "4"*)

(*if 3 = 4 then 0 else "4";;*)
(*- : string = "4"*)
(*Error: This expression has type string but an expression was expected of type int*)
(*Error de tipo porque en la subexpresion "then 0" se le aclara al compilador que el condicional es de tipo int y "4" es de tipo string*)

(if 3 < 5 then 8 else 10) + 4;;
(*- : int = 12*)