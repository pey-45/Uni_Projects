let dijkstra w = 
  let valid_array = ref true in
  let n = Array.length w
  in
  for i = 0 to n - 1 do
    for j = 0 to n - 1 do 
      if w.(i).(j) < 0 then
        valid_array := false
    done;
    if Array.length w.(i) <> n then
      valid_array := false
  done;
  if !valid_array then
    let dijkstra_aux w dist n =
      let not_visited = Array.make n true in
      for i = 0 to n - 1 do
        for j = 0 to n - 1 do
          dist.(i).(j) <- w.(i).(j);
          not_visited.(j) <- true
        done;
        not_visited.(i) <- false;
    
        for j = 0 to n - 3(**) do
          let node = ref (-1) in
          for k = 0 to n - 1 do
            if not_visited.(k) && (!node = -1 || dist.(i).(k) < dist.(i).(!node)) then
              node := k
          done;
          if !node != -1 then
            not_visited.(!node) <- false;
    
          for k = 0 to n - 1 do
            if not_visited.(k) && dist.(i).(k) > dist.(i).(!node) + w.(!node).(k) then
              dist.(i).(k) <- dist.(i).(!node) + w.(!node).(k)
          done
        done
      done;
      dist
    in
    let dist = Array.make_matrix n n 0 in
    let n = Array.length w in
    dijkstra_aux w dist n
  else invalid_arg "dijkstra";;

    

