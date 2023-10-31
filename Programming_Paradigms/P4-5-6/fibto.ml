let rec fib n = if n <= 1 then n else fib (n-1) + fib (n-2);;

let rec fibtoaux n limit =
  if ((fib n) <= limit) then 
    let _ = 
      Printf.printf "%d\n" (fib n) in
      fibtoaux (n + 1) limit;;

if (Array.length Sys.argv) > 2 
    then Printf.printf "Error"
    else fibtoaux 0 (int_of_string Sys.argv.(1));;