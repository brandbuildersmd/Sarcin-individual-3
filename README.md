# 📋 SISTEM GESTIONARE COADĂ TIPĂRIRE - DOCUMENTAȚIE COMPLETĂ

## 📑 Cuprins
1. [Descripția Proiectului](#descrierea-proiectului)
2. [Structuri de Date](#structuri-de-date)
3. [Algoritm de Distribuție](#algoritm-de-distribuție)
4. [Funcții Principale](#funcții-principale)
5. [Cum se Compilează](#cum-se-compilează)
6. [Cum se Folosește](#cum-se-folosește)
7. [Exemple de Utilizare](#exemple-de-utilizare)
8. [Complexitate Timp/Spațiu](#complexitate-timpspațiu)

---

## 📖 Descrierea Proiectului

Proiectul implementează un **sistem de gestionare a cozii de tipărire** pentru N imprimante
folosind **liste simplu înlănțuite în C**. Sistemul distribuie documentele de tipărit între
imprimante disponibile optimizând timpul total de așteptare.

### Caracteristici Principale:
- ✓ Gestionare eficientă a cozii cu liste înlănțuite
- ✓ Distribuție inteligentă (algoritm Greedy)
- ✓ Simulare a procesului de tipărire
- ✓ Anulare documente în curs
- ✓ Activare/dezactivare imprimante în timp real
- ✓ Statistici detaliate
- ✓ Interfață interactivă

---

## 🏗️ Structuri de Date

### 1. `Document`
```c
typedef struct Document {
    int id;                    // Identificator unic
    char name[100];            // Numele documentului
    int total_lines;           // Rânduri totale
    int remaining_lines;       // Rânduri rămase
    int priority;              // Prioritate (0=normal, 1=urgent)
    struct Document* next;     // Pointer la următorul
} Document;
```

### 2. `Printer`
```c
typedef struct Printer {
    int id;                    // Identificator unic
    int time_per_line;         // Timp pentru o linie
    int total_time_spent;      // Timp total lucru
    Document* current_document;// Document în curs
    int current_time;          // Cronometru
    int is_active;             // Status (1=activ, 0=defect)
    struct Printer* next;      // Pointer la următoarea
} Printer;
```

### 3. `PrintQueue`
```c
typedef struct {
    Document* front;           // Începutul cozii
    Document* rear;            // Sfârșitul cozii
    int document_count;        // Număr documente
} PrintQueue;
```

### 4. `PrintSystem`
```c
typedef struct {
    Printer* printers;         // Lista de imprimante
    PrintQueue queue;          // Coada de tipărire
    int printer_count;         // Număr imprimante
    int total_time;            // Timp total scurs
    int documents_completed;   // Documente finalizate
} PrintSystem;
```

---

## 🔄 Algoritm de Distribuție

### Strategie: Greedy (Lacom)

**Principiu**: Alocă fiecare document la imprimanta cea mai disponibilă (cu cel mai mic
timp curent de lucru).

**Pași**:
1. Verifică toate imprimantele active din coadă
2. Găsește imprimanta cu `current_time` minim
3. Asignează documentul primului din coadă la aceasta
4. Repetă până când:
   - Coada devine goală, SAU
   - Nicio imprimantă nu e disponibilă

**Complexitate**: O(N × M) unde N = imprimante, M = documente în coadă

**Exemplu**:
```
Inițial:
  Printer 1: tiempo=0 (disponibil)
  Printer 2: tiempo=5 (ocupat)
  Printer 3: tiempo=2 (ocupat)
  Coadă: [Doc1, Doc2, Doc3, ...]

Pas 1: Doc1 → Printer 1 (tiempo minim = 0)
Pas 2: Doc2 → Printer 3 (nuevo minimo = 2+timp_doc2)
Pas 3: Doc3 → Printer 2 (nuevo minimo)
...
```

---

## ⚙️ Funcții Principale

### Funcții pentru Documente

#### `Document* document_create(int id, const char* name, int total_lines, int priority)`
Creează un document nou și alocă memorie.
- **Parametri**: id, nume, nr. rânduri, prioritate
- **Returnează**: pointer la document sau NULL dacă eșec

#### `void document_free(Document* doc)`
Eliberează memoria unui document.

### Funcții pentru Imprimante

#### `Printer* printer_create(int id, int time_per_line)`
Creează o imprimantă nouă.
- **Parametri**: id, timp/linie
- **Returnează**: pointer la imprimantă sau NULL

#### `void printer_add(PrintSystem* system, Printer* printer)`
Adaugă o imprimantă la sistem.

#### `void printer_deactivate(PrintSystem* system, int printer_id)`
Dezactivează o imprimantă (simuleaza o defecțiune).

#### `void printer_reactivate(PrintSystem* system, int printer_id)`
Reactivează o imprimantă.

### Funcții pentru Coada

#### `void queue_enqueue(PrintQueue* queue, Document* doc)`
Adaugă document la sfârșitul cozii.

#### `Document* queue_dequeue(PrintQueue* queue)`
Scoate și returnează documentul din fața cozii.

#### `int queue_cancel_document(PrintSystem* system, int doc_id)`
Anulează un document din coadă sau din tipărire.

### Funcții pentru Sistem

#### `PrintSystem* system_create()`
Inițializează sistemul.

#### `void system_distribute_documents(PrintSystem* system)`
Distribuie documente din coadă către imprimante (algoritm Greedy).

#### `void system_simulate_step(PrintSystem* system)`
Simulează un pas de timp (tipărire unei linii).

#### `void system_simulate(PrintSystem* system, int total_time)`
Rulează simularea pentru `total_time` pași.

#### `void system_display_status(PrintSystem* system)`
Afișează starea curentă a sistemului.

#### `void system_display_statistics(PrintSystem* system)`
Afișează statistici finale.

---

## 🔨 Cum se Compilează

### Metoda 1: Folosind Makefile (Recomandată)
```bash
# Compilare
make

# Compilare și rulare
make run

# Curățare fișiere compilate
make clean

# Ajutor
make help
```

### Metoda 2: Compilare Manuală cu GCC
```bash
gcc -Wall -Wextra -std=c99 -g -o print_queue_system main.c print_queue.c -lm
```

### Metoda 3: Compilare cu Debugging
```bash
gcc -Wall -Wextra -std=c99 -g -O0 -o print_queue_system main.c print_queue.c -lm
gdb ./print_queue_system
```

---

## 🎮 Cum se Folosește

### Rulare Program
```bash
./print_queue_system
```

### Meniu Interactiv
```
╔══════════════════════════════════════════════╗
║       MENIU PRINCIPAL - SISTEM IMPRIMARE    ║
╠══════════════════════════════════════════════╣
║ 1. Afișare status sistem                     ║
║ 2. Adăugare document în coadă                ║
║ 3. Simulare pas timp                        ║
║ 4. Simulare pentru N pași timp               ║
║ 5. Dezactivare imprimantă                    ║
║ 6. Reactivare imprimantă                     ║
║ 7. Anulare document                         ║
║ 8. Afișare statistici finale                 ║
║ 0. Ieșire                                    ║
╚══════════════════════════════════════════════╝
```

---

## 📊 Exemple de Utilizare

### Exemplul 1: Demonstrație Automată
```c
// Decomentează funcția main_demo() din main.c
int main_demo() {
    PrintSystem* system = system_create();
    
    // Adaugă 3 imprimante
    printer_add(system, printer_create(1, 3));
    printer_add(system, printer_create(2, 5));
    printer_add(system, printer_create(3, 4));
    
    // Adaugă documente
    queue_enqueue(&system->queue, document_create(1, "Raport", 20, 0));
    queue_enqueue(&system->queue, document_create(2, "Contract", 15, 0));
    
    // Simulare 500 pași
    system_simulate(system, 500);
    
    // Statistici
    system_display_statistics(system);
    
    system_free(system);
    return 0;
}
```

### Exemplul 2: Utilizare Manuală
```c
// 1. Creează sistem cu imprimante
PrintSystem* sys = system_create();
printer_add(sys, printer_create(1, 3));
printer_add(sys, printer_create(2, 5));

// 2. Adaugă documente în coadă
queue_enqueue(&sys->queue, document_create(1, "Doc1", 10, 0));
queue_enqueue(&sys->queue, document_create(2, "Doc2", 15, 0));

// 3. Distribuie documente
system_distribute_documents(sys);

// 4. Simulare
for (int i = 0; i < 100; i++) {
    system_simulate_step(sys);
}

// 5. Afișare rezultate
system_display_status(sys);

// 6. Curățare
system_free(sys);
```

### Exemplul 3: Anulare Document
```c
// Simulare
for (int i = 0; i < 50; i++) {
    system_simulate_step(system);
}

// Anulare document cu ID 2
queue_cancel_document(system, 2);

// Continuă simulare
for (int i = 0; i < 50; i++) {
    system_simulate_step(system);
}
```

---

## 📈 Complexitate Timp/Spațiu

### Complexitate Timp
| Operație | Complexitate | Note |
|----------|-------------|-------|
| Creere document | O(1) | Alocare constantă |
| Adaugare în coadă | O(1) | Inserare la final |
| Scoatere din coadă | O(1) | Ștergere de la început |
| Distribuție | O(N×M) | N=imprimante, M=documente coadă |
| Pas simulare | O(N) | N=imprimante |
| Anulare document | O(N+M) | Căutare în imprimante și coadă |
| Afișare status | O(N+M) | Traversare structuri |

### Complexitate Spațiu
| Structură | Spațiu | Note |
|-----------|--------|-------|
| Document | O(1) | Constantă ~150 bytes |
| Printer | O(1) | Constantă ~100 bytes |
| Sistem | O(N+M) | N imprimante, M documente |
| **Total** | **O(N+M)** | Linear în funcție de imprimante+documente |

### Overhead Memorie
```
Document:     ~150 bytes
Printer:      ~100 bytes
PrintQueue:   ~24 bytes
PrintSystem:  ~40 bytes
Total:        O(N + M) bytes
```

---

## 🔍 Debugging și Testare

### Valgrind (verificare scurgeri memorie)
```bash
valgrind --leak-check=full ./print_queue_system
```

### GDB (debugger)
```bash
gdb ./print_queue_system
(gdb) break main
(gdb) run
(gdb) step
```

### Teste Personalizate
```c
// Adăugă în test_file.c:
#include "print_queue.h"

void test_document_creation() {
    Document* doc = document_create(1, "Test", 10, 0);
    assert(doc->id == 1);
    assert(doc->remaining_lines == 10);
    document_free(doc);
}

void test_queue_operations() {
    PrintQueue q;
    queue_init(&q);
    
    Document* d1 = document_create(1, "D1", 5, 0);
    queue_enqueue(&q, d1);
    
    Document* d2 = queue_dequeue(&q);
    assert(d2->id == 1);
    assert(queue_is_empty(&q));
}
```

---

## 💡 Îmbunătățiri Posibile

1. **Prioritate documente**: Implementare coadă cu prioritate (heap)
2. **Algoritm optimizat**: Algoritm Shortest Job First (SJF)
3. **Concurență**: Pthreads pentru simulare paralelă
4. **Interfață grafică**: GTK+ sau SDL2
5. **Persistență**: Salvare/încărcare din fișier
6. **Statistici avansate**: Calcul medie așteptare, mediana, etc.
7. **Predicție**: Estimare timp terminare document

---

## ✅ Checklist Implementare

- [x] Structuri Document și Printer
- [x] Liste simplu înlănțuite
- [x] Inițializare sistem
- [x] Adăugare documente
- [x] Distribuție inteligentă
- [x] Simulare tipărire
- [x] Afișare stare sistem
- [x] Eliberare memorie
- [x] Anulare documente (BONUS)
- [x] Activare/dezactivare imprimante (BONUS)
- [x] Interfață interactivă
- [x] Statistici finale
- [x] Documentație completă

---

## 📧 Contact / Suport

Pentru întrebări despre implementare:
- Verifică comentariile din cod
- Citește documentația online C
- Testează cu valgrind pentru erori memorie
- Folosește gdb pentru debugging

---

**Data**: 2026-05-08  
**Versiune**: 1.0  
**Status**: ✓ Complet și Testat
