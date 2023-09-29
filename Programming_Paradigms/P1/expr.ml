();; 
(*null*)
(*- : unit = ()*) 

2 + 5 * 3;;
(*- : int = 17*)
(*CORRECT*)

1.25 *. 2.0;;
(*- : float = 2.5*)
(*CORRECT*)

(*2 - 2.0;;*)
(*Error: this expression has type float but an expression was expected of type int*)
(*CORRECT*)

(*3.0 + 2.0;;*)
(*Error: this expression has type float but an expression was expected of type int*)
(*CORRECT*)

5 / 3;;
(*- : int = 1*)
(*CORRECT*)

5 mod 3;;
(*- : int = 2*)
(*CORRECT*)

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
(*CORRECT*)

int_of_float;;
(*- : float -> int = <fun>*)
(*CORRECT*)

float_of_int;;
(*- : int -> float = <fun>*)
(*CORRECT*)

3.0 = float_of_int 3;;
(*- : true*)
(*- : bool = true*)

(*int_of_float -2.9;;*)
(*- : int = 2*)
(*Error: This expression has type float -> int but an expression was expected of type int*)

(*int_of_float 2.1 + int_of_float -2.9;;*)
(*Error: This expression has type float -> int but an expression was expected of type int*)
(*CORRECT*)

truncate;;
(*- : float -> int = <fun>*)
(*CORRECT*)

truncate 2.1 + truncate (-2.9);;
(*- : int = 0*)
(*CORRECT*)

floor;;
(*- : float -> int = <fun>*)
(*- : float -> float = <fun>*)

floor 2.1 +. floor (-2.9);;
(*- : float = 0.*)
(*- : float = -1.*)

ceil;;
(*- : float -> float = <fun>*)
(*CORRECT*)

ceil 2.1 +. ceil (-2.9);;
(*- : float = -1.*)
(*- : float = 1.*)

int_of_char;;
(*- : char -> int = <fun>*)
(*CORRECT*)

int_of_char 'A';;
(*- : int = 65*)
(*CORRECT*)

char_of_int;;
(*- : int -> char = <fun>*)
(*CORRECT*)

char_of_int 66;;
(*- : char = "66"*)
(*- : char = 'B'*)

Char.code;;
(*- : char -> int = <fun>*)
(*CORRECT*)

Char.code 'B';;
(*- : int = 66*)
(*CORRECT*)

Char.chr;;
(*- : char -> char = <fun>*)
(*- : int -> char = <fun>*)

Char.chr 67;;
(*'\067'*)
(*- : char = 'C'*)

'\067';;
(*- : char = 'C'*)
(*CORRECT*)

Char.chr (Char.code 'a' - Char.code 'A' + Char.code 'M');;
(*- : char = 'm'*)
(*CORRECT*)

Char.lowercase_ascii;;
(*- : char -> char = <fun>*)
(*CORRECT*)

Char.lowercase_ascii 'M';;
(*- : char = 'm'*)
(*CORRECT*)

Char.uppercase_ascii;;
(*- : char -> char = <fun>*)
(*CORRECT*)

Char.uppercase_ascii 'm';;
(*- : char = 'M'*)
(*CORRECT*)

"this is a string";;
(*- : string = "this is a string"*)
(*CORRECT*)

String.length;;
(*- : string -> int = <fun>*)
(*CORRECT*)

String.length "longitud";;
(*- : int = 8*)
(*CORRECT*)

(*"1999" + "1";;*)
(*Error: Expression has type string but an expression was expected of type int*)
(*CORRECT*)

"1990" ^ "1";;
(*- : string = "19901"*)
(*CORRECT*)

int_of_string;;
(*- : string -> int = <fun>*)
(*CORRECT*)

int_of_string "1999" + 1;;
(*- : int = 2000*)
(*CORRECT*)

"\065\066";;
(*- : string = "AB"*)
(*CORRECT*)

string_of_int;;
(*- : int -> string = <fun>*)
(*CORRECT*)

string_of_int 010;;
(*- : string = "10"*)
(*CORRECT*)

not true;;
(*- : bool = false*)
(*CORRECT*)

true && false;;
(*- : bool = false*)
(*CORRECT*)

true || false;;
(*- : bool = true*)
(*CORRECT*)

(1 < 2) = false;;
(*- : bool = false*)
(*CORRECT*)

"1" < "2";;
(*- : bool = true*)
(*CORRECT*)

2 < 12;;
(*- : bool = true*)
(*CORRECT*)

"2" < "12";;
(*- : bool = true*)
(*- : bool = false*)

"uno" < "dos";;
(*- : bool = false*)
(*CORRECT*)

if 3 = 4 then 0 else 4;;
(*- : int = 4*)
(*CORRECT*)

if 3 = 4 then "0" else "4";;
(*- : string = "4"*)
(*CORRECT*)

(*if 3 = 4 then 0 else "4";;*)
(*- : string = "4"*)
(*Error: This expression has type string but an expression was expected of type int*)

(if 3 < 5 then 8 else 10) + 4;;
(*- : int = 12*)
(*CORRECT*)
