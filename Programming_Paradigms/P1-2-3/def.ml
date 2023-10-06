let f x = 2 * x + 1;;

let v = f 5;;

let w = 4. *. 3. +. 5. -. 8. /. 2.;;

let x = if (4 < 3) then 'x' else 'd';;

let y = (function true -> false | false -> true) (5+3 > 8);;

let z = "Hola Mund" ^ (String.make 1 (Char.chr (11 + int_of_char 'd')));;