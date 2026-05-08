#include "print_queue.h"
#include <limits.h>

/* ==================== FUNCȚII PENTRU DOCUMENTE ==================== */

Document* document_create(int id, const char* name, int total_lines, int priority) {
    Document* doc = (Document*)malloc(sizeof(Document));
    if (doc == NULL) {
        fprintf(stderr, "Eroare: Nu s-a putut aloca memorie pentru document\n");
        return NULL;
    }
    
    doc->id = id;
    strncpy(doc->name, name, 99);
    doc->name[99] = '\0';
    doc->total_lines = total_lines;
    doc->remaining_lines = total_lines;
    doc->priority = priority;
    doc->next = NULL;
    
    return doc;
}

void document_free(Document* doc) {
    if (doc != NULL) {
        free(doc);
    }
}

/* ==================== FUNCȚII PENTRU IMPRIMANTE ==================== */

Printer* printer_create(int id, int time_per_line) {
    Printer* printer = (Printer*)malloc(sizeof(Printer));
    if (printer == NULL) {
        fprintf(stderr, "Eroare: Nu s-a putut aloca memorie pentru imprimantă\n");
        return NULL;
    }
    
    printer->id = id;
    printer->time_per_line = time_per_line;
    printer->lines_per_second = 1; /* De calculat în funcție de time_per_line */
    printer->total_time_spent = 0;
    printer->current_document = NULL;
    printer->current_time = 0;
    printer->is_active = 1;
    printer->next = NULL;
    
    return printer;
}

void printer_add(PrintSystem* system, Printer* printer) {
    if (system == NULL || printer == NULL) {
        return;
    }
    
    if (system->printers == NULL) {
        system->printers = printer;
    } else {
        Printer* temp = system->printers;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = printer;
    }
    
    system->printer_count++;
}

void printer_deactivate(PrintSystem* system, int printer_id) {
    if (system == NULL) {
        return;
    }
    
    Printer* temp = system->printers;
    while (temp != NULL) {
        if (temp->id == printer_id) {
            temp->is_active = 0;
            printf("⚠️  Imprimanta %d a fost dezactivată\n", printer_id);
            return;
        }
        temp = temp->next;
    }
    
    printf("⚠️  Imprimanta cu ID %d nu a fost găsită\n", printer_id);
}

void printer_reactivate(PrintSystem* system, int printer_id) {
    if (system == NULL) {
        return;
    }
    
    Printer* temp = system->printers;
    while (temp != NULL) {
        if (temp->id == printer_id) {
            temp->is_active = 1;
            printf("✓ Imprimanta %d a fost reactivată\n", printer_id);
            return;
        }
        temp = temp->next;
    }
    
    printf("⚠️  Imprimanta cu ID %d nu a fost găsită\n", printer_id);
}

void printer_free(Printer* printer) {
    if (printer != NULL) {
        free(printer);
    }
}

/* ==================== FUNCȚII PENTRU COADA ==================== */

void queue_init(PrintQueue* queue) {
    if (queue != NULL) {
        queue->front = NULL;
        queue->rear = NULL;
        queue->document_count = 0;
    }
}

void queue_enqueue(PrintQueue* queue, Document* doc) {
    if (queue == NULL || doc == NULL) {
        return;
    }
    
    doc->next = NULL;
    
    if (queue->rear == NULL) {
        queue->front = doc;
        queue->rear = doc;
    } else {
        queue->rear->next = doc;
        queue->rear = doc;
    }
    
    queue->document_count++;
}

Document* queue_dequeue(PrintQueue* queue) {
    if (queue == NULL || queue->front == NULL) {
        return NULL;
    }
    
    Document* doc = queue->front;
    queue->front = queue->front->next;
    
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    queue->document_count--;
    doc->next = NULL;
    
    return doc;
}

int queue_is_empty(PrintQueue* queue) {
    return (queue == NULL || queue->front == NULL);
}

int queue_cancel_document(PrintSystem* system, int doc_id) {
    if (system == NULL) {
        return 0;
    }
    
    /* Verifică dacă documentul este în coadă */
    Document* prev = NULL;
    Document* current = system->queue.front;
    
    while (current != NULL) {
        if (current->id == doc_id) {
            if (prev == NULL) {
                system->queue.front = current->next;
            } else {
                prev->next = current->next;
            }
            
            if (current == system->queue.rear) {
                system->queue.rear = prev;
            }
            
            system->queue.document_count--;
            printf("📄 Documentul %d a fost anulat din coadă\n", doc_id);
            document_free(current);
            return 1;
        }
        
        prev = current;
        current = current->next;
    }
    
    /* Verifică dacă documentul este în tipărire */
    Printer* printer = system->printers;
    while (printer != NULL) {
        if (printer->current_document != NULL && printer->current_document->id == doc_id) {
            printf("📄 Documentul %d a fost anulat din imprimanta %d\n", doc_id, printer->id);
            document_free(printer->current_document);
            printer->current_document = NULL;
            printer->current_time = 0;
            return 1;
        }
        printer = printer->next;
    }
    
    printf("⚠️  Documentul cu ID %d nu a fost găsit\n", doc_id);
    return 0;
}

void queue_display(PrintQueue* queue) {
    if (queue == NULL || queue_is_empty(queue)) {
        printf("   [Coada este goală]\n");
        return;
    }
    
    Document* current = queue->front;
    int position = 1;
    
    while (current != NULL) {
        printf("   %d. [Doc %d] %s (%d/%d rânduri)\n",
               position, current->id, current->name,
               current->total_lines - current->remaining_lines,
               current->total_lines);
        current = current->next;
        position++;
    }
}

/* ==================== FUNCȚII PENTRU SISTEM ==================== */

PrintSystem* system_create() {
    PrintSystem* system = (PrintSystem*)malloc(sizeof(PrintSystem));
    if (system == NULL) {
        fprintf(stderr, "Eroare: Nu s-a putut aloca memorie pentru sistem\n");
        return NULL;
    }
    
    system->printers = NULL;
    queue_init(&system->queue);
    system->printer_count = 0;
    system->total_time = 0;
    system->documents_completed = 0;
    
    return system;
}

void system_distribute_documents(PrintSystem* system) {
    if (system == NULL || queue_is_empty(&system->queue)) {
        return;
    }
    
    /* Algoritm Greedy: alocă documente la imprimanta cea mai disponibilă */
    while (!queue_is_empty(&system->queue)) {
        Printer* best_printer = NULL;
        int min_time = INT_MAX;
        
        /* Găsește imprimanta cea mai disponibilă (cu cel mai mic timp curent) */
        Printer* temp = system->printers;
        while (temp != NULL) {
            if (temp->is_active && (temp->current_document == NULL || temp->current_document->remaining_lines == 0)) {
                if (temp->current_time < min_time) {
                    min_time = temp->current_time;
                    best_printer = temp;
                }
            }
            temp = temp->next;
        }
        
        if (best_printer == NULL) {
            /* Nicio imprimantă disponibilă în prezent */
            break;
        }
        
        /* Alocă documentul la cea mai disponibilă imprimantă */
        Document* doc = queue_dequeue(&system->queue);
        if (doc != NULL) {
            best_printer->current_document = doc;
            best_printer->current_time = 0;
        }
    }
}

void system_simulate_step(PrintSystem* system) {
    if (system == NULL) {
        return;
    }
    
    system->total_time++;
    
    /* Simulează tipărirea pentru fiecare imprimantă */
    Printer* printer = system->printers;
    while (printer != NULL) {
        if (printer->is_active && printer->current_document != NULL &&
            printer->current_document->remaining_lines > 0) {
            
            printer->current_time++;
            
            /* Verifică dacă s-a finalizat tipărirea unei linii */
            if (printer->current_time >= printer->time_per_line) {
                printer->current_document->remaining_lines--;
                printer->total_time_spent += printer->current_time;
                printer->current_time = 0;
                
                /* Documentul este finalizat */
                if (printer->current_document->remaining_lines == 0) {
                    printf("✓ Documentul %d finalizat pe imprimanta %d la timp %d\n",
                           printer->current_document->id, printer->id, system->total_time);
                    document_free(printer->current_document);
                    printer->current_document = NULL;
                    system->documents_completed++;
                    
                    /* Distribuie documente din nou după finalizare */
                    system_distribute_documents(system);
                }
            }
        }
        
        printer = printer->next;
    }
}

void system_simulate(PrintSystem* system, int total_time) {
    if (system == NULL) {
        return;
    }
    
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║     SIMULARE GESTIONARE IMPRIMARE      ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    system_distribute_documents(system);
    
    for (int i = 0; i < total_time; i++) {
        system_simulate_step(system);
        
        /* Afișează status la fiecare 10 unități de timp */
        if ((i + 1) % 10 == 0) {
            printf("\n--- Status la timp %d ---\n", system->total_time);
            system_display_status(system);
        }
    }
    
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║     SIMULARE FINALIZATĂ               ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
}

void system_display_status(PrintSystem* system) {
    if (system == NULL) {
        return;
    }
    
    printf("📊 Timp total: %d\n", system->total_time);
    printf("📄 Documente completate: %d\n", system->documents_completed);
    printf("\n🖨️  STATUS IMPRIMANTE:\n");
    
    Printer* printer = system->printers;
    while (printer != NULL) {
        printf("\n   Imprimanta %d", printer->id);
        printf(" [%s]", printer->is_active ? "ACTIV" : "INACTIV");
        printf("\n   • Timp/linie: %d unități\n", printer->time_per_line);
        printf("   • Timp curent: %d\n", printer->current_time);
        printf("   • Timp total petrecut: %d\n", printer->total_time_spent);
        
        if (printer->current_document != NULL) {
            printf("   • Document curent: [%d] %s\n",
                   printer->current_document->id,
                   printer->current_document->name);
            printf("   • Progres: %d/%d rânduri\n",
                   printer->current_document->total_lines - printer->current_document->remaining_lines,
                   printer->current_document->total_lines);
        } else {
            printf("   • Document curent: [NEOCUPATĂ]\n");
        }
        
        printer = printer->next;
    }
    
    printf("\n📋 COADA DE AȘTEPTARE (%d documente):\n", system->queue.document_count);
    queue_display(&system->queue);
}

void system_display_statistics(PrintSystem* system) {
    if (system == NULL) {
        return;
    }
    
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      STATISTICI FINALE SISTEM          ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    printf("⏱️  Timp total de simulare: %d unități\n", system->total_time);
    printf("📄 Documente completate: %d\n", system->documents_completed);
    printf("🖨️  Numărul de imprimante: %d\n", system->printer_count);
    
    printf("\n📊 STATISTICI PE IMPRIMANTE:\n");
    
    Printer* printer = system->printers;
    
    while (printer != NULL) {
        printf("\n   Imprimanta %d:\n", printer->id);
        printf("   • Timp per linie: %d unități\n", printer->time_per_line);
        printf("   • Timp total lucru: %d unități\n", printer->total_time_spent);
        
        int efficiency = (system->total_time > 0) ?
            (printer->total_time_spent * 100) / system->total_time : 0;
        printf("   • Eficiență: %d%%\n", efficiency);
        
        printer = printer->next;
    }
    
    printf("\n");
}

void system_free(PrintSystem* system) {
    if (system == NULL) {
        return;
    }
    
    /* Eliberează toate imprimantele */
    Printer* printer = system->printers;
    while (printer != NULL) {
        Printer* temp = printer;
        printer = printer->next;
        
        if (temp->current_document != NULL) {
            document_free(temp->current_document);
        }
        
        printer_free(temp);
    }
    
    /* Eliberează toate documentele din coadă */
    Document* doc = system->queue.front;
    while (doc != NULL) {
        Document* temp = doc;
        doc = doc->next;
        document_free(temp);
    }
    
    free(system);
}
