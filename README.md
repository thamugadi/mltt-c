# mltt-c 
this is a WIP implementation of a language based on Martin-Löf type theory + W-types in C. it's still poorly tested.

don't hesitate to let me know if some cases wouldn't work :)

**todo:**
- fix ``tree`` and ``ind2`` type inference
- W-types recursors
- writing tests
- writing a good readme file
- embedded rec0, rec1, rec2
- J-rule
- removing memory leaks after erros
- decent error messages
- pretty printing (no de bruijn)
- implementing some features on the top (pattern matching?)

# Usage
```
mltt file_name
```

# Example

```
id_type0 : forall (t : Type 0) (x : bool), Type 0
id_type0 t x = t

id_type1 : forall (t : Type 1) (x : bool), Type 1
id_type1 t x = t

rec2_1 : forall (C : Type 1) (x : C) (y : C) (z : bool), C
rec2_1 C x y z = ind2 (id_type1 C) x y z

id_type0_void : forall (t : Type 0) (x : void), Type 0
id_type0_void t x = t

nat : Type 0
nat = W (b : bool), rec2_1 (Type 0) void one b

rec0_0 : forall (C : Type 0) (x : void), C
rec0_0 C x = ind0 (id_type0_void C) x

zero_subtr : forall (x : void), nat
zero_subtr x = rec0_0 nat x 

nat0 : nat
nat0 = tree false zero_subtr

one_subtr : forall (x : void), nat
one_subtr x = nat0

nat1 : nat
nat1 = tree true one_subtr

suc : forall (n : nat), nat
suc n = tree true (lambda x. n : (forall (u : void), nat))

nat5 : nat 
nat5 = suc (suc (suc (suc (suc nat0))))
