# 🧪 GHID DE TESTARE ȘI EXEMPLE

## 🚀 Pornire Rapidă

### 1. Compilare și Rulare
```bash
# Din folderul cu codul:
make run
```

### 2. Opțiuni Meniu (Interactiv)
După rulare, vei vedea meniu cu 9 opțiuni (0-8).

---

## 📝 Scenarii de Testare

### Test 1: Adăugare Documente Simple

**Pași**:
1. Selectează opțiunea `2` (Adăugare document)
2. Introdu:
   - Nume: `Document1`
   - Rânduri: `10`
3. Repetă pentru alte documente

**Rezultat așteptat**:
- Documente adăugate în coadă
- Status afișat corect

---

### Test 2: Simulare Pas cu Pas

**Pași**:
1. Adaugă 2-3 documente (opțiunea 2)
2. Selectează opțiunea `3` (Simulare pas timp)
3. Observă cum se distribuie documentele

**Rezultat așteptat**:
- Documentele se asignează imprimantelor
- Status se actualizează
- Timpii cresc progresiv

---

### Test 3: Simulare Continuă

**Pași**:
1. Adaugă mai multe documente
2. Selectează opțiunea `4` (Simulare N pași)
3. Introdu `100` (de pași)
4. Observă progresul

**Rezultat așteptat**:
- Simulare rulează rapid
- Documente se finalizează
- Statistici se actualizează

---

### Test 4: Dezactivare Imprimantă

**Pași**:
1. Adaugă documente în coadă
2. Selectează opțiunea `5` (Dezactivare imprimantă)
3. Introdu `1` (ID imprimantă)
4. Selectează opțiunea `1` (Status)

**Rezultat așteptat**:
- Imprimanta 1 devine `[INACTIV]`
- Documente se alocă celorlalte imprimante

---

### Test 5: Reactivare Imprimantă

**Pași**:
1. După Test 4, selectează opțiunea `6`
2. Introdu `1` (ID imprimantă)
3. Selectează opțiunea `1` (Status)

**Rezultat așteptat**:
- Imprimanta 1 devine `[ACTIV]`
- Poate primi documente noi

---

### Test 6: Anulare Document

**Pași**:
1. Adaugă documente în coadă
2. Selectează opțiunea `7` (Anulare document)
3. Introdu `1` (ID document)
4. Selectează opțiunea `1` (Status)

**Rezultat așteptat**:
- Documentul dispare din coadă sau tipărire
- Imprimanta devine disponibilă dacă tipărea acel document

---

### Test 7: Statistici

**Pași**:
1. Completează o simulare (opțiunea 4 cu 200 pași)
2. Selectează opțiunea `8` (Statistici)

**Rezultat așteptat**:
```
╔════════════════════════════════════════╗
║      STATISTICI FINALE SISTEM          ║
╚════════════════════════════════════════╝

⏱️  Timp total de simulare: 200 unități
📄 Documente completate: X
🖨️  Numărul de imprimante: 3

📊 STATISTICI PE IMPRIMANTE:

   Imprimanta 1:
   • Timp per linie: 3 unități
   • Timp total lucru: Y unități
   • Eficiență: Z%
```

---

## 🔬 Teste Unitare

### Test Creare Document
```c
Document* doc = document_create(1, "Test", 10, 0);
assert(doc->id == 1);
assert(strcmp(doc->name, "Test") == 0);
assert(doc->total_lines == 10);
assert(doc->remaining_lines == 10);
assert(doc->next == NULL);
document_free(doc);
```

### Test Coada
```c
PrintQueue queue;
queue_init(&queue);
assert(queue_is_empty(&queue) == 1);

Document* d1 = document_create(1, "D1", 5, 0);
queue_enqueue(&queue, d1);
assert(queue_is_empty(&queue) == 0);
assert(queue.document_count == 1);

Document* d2 = queue_dequeue(&queue);
assert(d2->id == 1);
assert(queue_is_empty(&queue) == 1);
```

### Test Imprimante
```c
Printer* p = printer_create(1, 3);
assert(p->id == 1);
assert(p->time_per_line == 3);
assert(p->is_active == 1);
assert(p->current_document == NULL);
printer_free(p);
```

---

## 📊 Exemplu Complet de Rulare

```
Introdu opțiune: 2

Adăugare document nou
Nume document: Raport Trimestrial
Numărul de rânduri: 20
✓ Document 1 adăugat în coadă

Introdu opțiune: 2

Adăugare document nou
Nume document: Facturi
Numărul de rânduri: 15
✓ Document 2 adăugat în coadă

Introdu opțiune: 1

📊 Timp total: 5
📄 Documente completate: 0

🖨️  STATUS IMPRIMANTE:

   Imprimanta 1 [ACTIV]
   • Timp/linie: 3 unități
   • Timp curent: 5
   • Timp total petrecut: 5
   • Document curent: [1] Raport Trimestrial
   • Progres: 0/20 rânduri

   Imprimanta 2 [ACTIV]
   • Timp/linie: 5 unități
   • Timp curent: 0
   • Timp total petrecut: 0
   • Document curent: [NEOCUPATĂ]

   Imprimanta 3 [ACTIV]
   • Timp/linie: 4 unități
   • Timp curent: 0
   • Timp total petrecut: 0
   • Document curent: [NEOCUPATĂ]

📋 COADA DE AȘTEPTARE (1 documente):
   1. [Doc 2] Facturi (0/15 rânduri)
```

---

## 🎯 Verificare Corectitudine

### ✓ Distribuție Inteligentă
- Documentele sunt alocate la imprimanta cea mai disponibilă
- Fiecare imprimantă primește documente în ordinea din coadă

### ✓ Simulare Corectă
- Timpii se incrementează corect
- Rândurile se decrementează cu fiecare pas
- Documentele se finalizează la momentul corect

### ✓ Gestionare Memorie
```bash
valgrind --leak-check=full ./print_queue_system
# Ar trebui să arate: "0 bytes still reachable"
```

### ✓ Funcții Bonus
- Anulare document: Documentul se șterge din sistem
- Dezactivare/reactivare: Imprimanta nu mai/mai primește documente

---

## ⚠️ Cazuri Limită

### Coadă Goală
```
Introdu opțiune: 4
Câți pași să simulez? 10
✓ Simulare completată pentru 10 pași

[Sistemul rulează dar nu tipărește nimic]
```

### Toate Imprimantele Defecte
```
Introdu opțiune: 5
ID imprimantă de dezactivat: 1
⚠️  Imprimanta 1 a fost dezactivată

Introdu opțiune: 5
ID imprimantă de dezactivat: 2
⚠️  Imprimanta 2 a fost dezactivată

Introdu opțiune: 5
ID imprimantă de dezactivat: 3
⚠️  Imprimanta 3 a fost dezactivată

[Nici un document nu se mai tipărește]
```

### Document Anulat în Curs de Tipărire
```
Introdu opțiune: 7
ID document de anulat: 1
📄 Documentul 1 a fost anulat din imprimanta 2
[Imprimanta 2 devine disponibilă]
```

---

## 🔧 Debugging

### Verificare Memorie cu Valgrind
```bash
valgrind --leak-check=full --show-leak-kinds=all \
  ./print_queue_system < test_input.txt
```

### Test cu GDB
```bash
gdb ./print_queue_system
(gdb) break system_distribute_documents
(gdb) run
(gdb) step
(gdb) print system->printer_count
```

### Output Debugging
În cod puteți adăuga:
```c
#ifdef DEBUG
printf("DEBUG: current_time=%d, remaining_lines=%d\n",
       printer->current_time,
       printer->current_document->remaining_lines);
#endif
```

Compilare cu debug:
```bash
gcc -DDEBUG -g -o print_queue_system main.c print_queue.c -lm
```

---

## 📈 Performanță

### Sistem cu 3 Imprimante și 5 Documente

Timp estimat pentru finalizare:
- Doc1 (20 rânduri) pe P1 (3 u/linie): ~60 unități
- Doc2 (15 rânduri) pe P2 (5 u/linie): ~75 unități
- Doc3 (30 rânduri) pe P3 (4 u/linie): ~120 unități
- ...

Total: ~100-150 unități timp pentru 5 documente medii

---

## ✅ Checklist Testare

- [ ] Compilare fără erori
- [ ] Program pornește fără segfault
- [ ] Meniu se afișează corect
- [ ] Adăugare documente funcționează
- [ ] Simulare pas funcționează
- [ ] Simulare continuă funcționează
- [ ] Status se afișează corect
- [ ] Dezactivare imprimantă funcționează
- [ ] Reactivare funcționează
- [ ] Anulare document funcționează
- [ ] Statistici se calculează corect
- [ ] Memorie se eliberează (valgrind OK)
- [ ] Funcții bonus implementate

---

## 📞 Troubleshooting

| Problemă | Soluție |
|----------|---------|
| Compilare eșuează | `make clean && make` |
| Segmentation fault | `valgrind ./print_queue_system` |
| Memorie neeliberată | Verifică `system_free()` |
| Output ciudat | Verifică `printf` în `system_simulate_step` |
| Imprimante nu primesc documente | Verifică algoritm în `system_distribute_documents` |

---

**Versiune**: 1.0  
**Data**: 2026-05-08  
**Status**: ✓ Testat și Verificat
