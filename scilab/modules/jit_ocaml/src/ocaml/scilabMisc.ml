


let string_of_unicode s =
  let len = String.length s in
  let b = Buffer.create len in
  for i = 0 to len-1 do
    let c= s.[i] in
    if c <> '\000' then Buffer.add_char b c
  done;
  Buffer.contents b

let unicode_of_ascii s =
  let len = String.length s in
  let u = String.make (len*4) '\000' in
  for i = 0 to len-1 do
    let pos = i * 4 in
    u.[pos] <- s.[i];
  done;
  u
