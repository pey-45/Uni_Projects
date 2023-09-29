let rec factorial n = if n > 1. then n *. factorial (n -. 1.) else 1. in 
let rec calc_e i = if i < 50. then 1. /. factorial i +. calc_e (i +. 1.) else 0. in 
Printf.printf "%.15f\n" (calc_e 0.)