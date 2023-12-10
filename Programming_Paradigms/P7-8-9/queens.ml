let queens n =
  let attacks (x1, y1) (x2, y2) = 
    (*comprueba que no estan en la misma fila, columna o diagonal*)
    x1 = x2 || y1 = y2 || abs (x1 - x2) = abs (y1 - y2) 
  in
  let rec attacks_any pair = function
    (*comprueba que el par no ataque a ningun elemento de la lista de pares*)
    | [] -> false
    | hd::tl -> attacks pair hd || attacks_any pair tl
  in
  let rec safe_positions n row col queens =
    (*devuelve una lista de posiciones seguras para una determinada fila*)
    (*en el parametro col siempre se pone 1 porque se empieza desde esa posicion*)
    if col > n then []
    (*cuando se terminan las columnas se termina la lista*)
    else 
      (*si la posicion actual ataca a alguna reina*)
      if attacks_any (row, col) queens then 
        (*se llama a la funcion avanzando una posicion*)
        safe_positions n row (col + 1) queens
      else 
        (*si no se añade a la lista que será devuelta y se avanza una posicion*)
        (row, col)::safe_positions n row (col + 1) queens
  in
  let rec place_queens n row queens =
    if row > n then [queens]
    (* Todas las filas han sido recorridas; devuelve la solución *)
    else
      (*se crea una lista con las posiciones seguras de esa fila*)
      let safe_pos = safe_positions n row 1 queens in
      (*para una posicion la añade a la lista de reinas y llama a la funcion principal en la siguiente fila*)
      let extend_solution pos = place_queens n (row + 1) (pos :: queens) in
      (*map: se aplica la funcion a todas las casillas seguras, creando listas independientes con las diferentes soluciones*)
      (*flatten: se almacenan todas las listas en una sola, la cual es devuelta*)
      List.flatten (List.map extend_solution safe_pos)
  in 
  if n < 0 then [[]] 
  else place_queens n 1 [];;


let is_queens_sol n sol =
  let attacks (x1, y1) (x2, y2) = 
    (*comprueba que no estan en la misma fila, columna o diagonal*)
    x1 = x2 || y1 = y2 || abs (x1 - x2) = abs (y1 - y2) 
  in
  let rec attacks_any pair = function
    (*comprueba que el par no ataque a ningun elemento de la lista de pares*)
    | [] -> false 
    | hd::tl -> attacks pair hd || attacks_any pair tl
  in
  let rec verify = function
    (*se comprueba que la posicion actual no se ataque con ninguna otra y que no se salga del tablero*)
    (*luego se llama a la función verify sin ese elemento*)
    | hd::tl -> not (attacks_any hd tl) && (fst hd) <= n && (snd hd) <= n && verify tl
    | [] -> true 
  in
  (*verifica si el tamaño de la solucion coincide con el dado, y confirma con la funcion anterior*)
  List.length sol = n && verify sol;;
