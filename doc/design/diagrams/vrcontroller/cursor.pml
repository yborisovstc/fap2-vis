@startuml
hide footbox

' :!plantuml cursor.pml

header DES based VR controller, ref ds_desvrc

(*) --> Test
Sw1 --> Cursor#Aqua
Cmp1 -->[Sel] Sw1
Const_SNil -->[Inp] Cmp1
Cursor -->[Inp2] Cmp1
ModelRoot -->[Inp] Sw1
Sw2 -->[Inp2] Sw1
NodeSelected -->[Inp1] Sw2
Cursor -->[Inp2] Sw2
Cmp_Eq_3 -->[Sec] Sw2
Const_SNil  --> Cmp_Eq_3
NodeSelected  --> Cmp_Eq_3

@enduml
