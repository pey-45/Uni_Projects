type 'a bintree = Empty | Node of 'a * 'a bintree * 'a bintree

let in_order tree =
  let rec aux tree list = match tree with
    (*si el no hay nodo devuelve la lista*)
    | Empty -> list
    | Node (root, left, right) ->
        (*va recorriendo por la izquierda, cuando llega al final devuelve la lista acumulativa*)
        let list_left = aux left list in 
        (*se añade la raiz a la lista*)
        let list_root = root :: list_left in 
        (*se llama a la funcion con el nodo de la derecha y la lista con la raíz añadida*)
        aux right list_root
  in
  (*se le da la vuelta para que este en el orden que queremos*)
  List.rev (aux tree []);;


let rec insert ord tree x = match tree with
  (*si el árbol está vacío se devuelve un arbol con un solo elemento de valor x*)
  | Empty -> Node (x, Empty, Empty)
  | Node (root, left, right) ->
      if ord x root then
        (*si el elemento cumple el criterio se inserta por la izquierda*)
        Node (root, insert ord left x, right)
      else
        (*si no por la derecha*)
        Node (root, left, insert ord right x);;


let bst ord l = 
  let rec aux btree ord = function
    | [] -> btree
    | hd::tl -> aux (insert ord btree hd) ord tl
  in
  aux Empty ord l;;


let qsort ord l = in_order (bst ord l);;


