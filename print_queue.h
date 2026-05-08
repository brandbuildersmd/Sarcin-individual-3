#ifndef PRINT_QUEUE_H
#define PRINT_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ========== STRUCTURI DE DATE ========== */

/* Structura pentru reprezentarea unui document */
typedef struct Document {
    int id;                          /* Identificator unic al documentului */
    char name[100];                  /* Numele documentului */
    int total_lines;                 /* Numărul total de rânduri */
    int remaining_lines;             /* Numărul de rânduri rămase de tipărit */
    int priority;                    /* Prioritate (0 = normal, 1 = urgent) */
    struct Document* next;           /* Pointer la următorul document */
} Document;

/* Structura pentru reprezentarea unei imprimante */
typedef struct Printer {
    int id;                          /* Identificator unic al imprimantei */
    int lines_per_second;            /* Linii pe secundă (inversul timpului/linie) */
    int time_per_line;               /* Timp necesar pentru o linie (în unități de timp) */
    int total_time_spent;            /* Timp total de tipărire */
    Document* current_document;      /* Documentul curent în tipărire */
    int current_time;                /* Cronometrul imprimantei */
    int is_active;                   /* 1 dacă este activ, 0 dacă e defect */
    struct Printer* next;            /* Pointer la următoarea imprimantă */
} Printer;

/* Structura pentru coada de tipărire */
typedef struct {
    Document* front;                 /* Pointer la primul document din coadă */
    Document* rear;                  /* Pointer la ultimul document din coadă */
    int document_count;              /* Numărul de documente din coadă */
} PrintQueue;

/* Structura pentru sistemul de imprimare */
typedef struct {
    Printer* printers;               /* Lista de imprimante */
    PrintQueue queue;                /* Coada de tipărire */
    int printer_count;               /* Numărul total de imprimante */
    int total_time;                  /* Timp total scurs */
    int documents_completed;         /* Documente finalizate */
} PrintSystem;

/* ========== FUNCȚII PENTRU DOCUMENTE ========== */

/**
 * Creează un document nou
 */
Document* document_create(int id, const char* name, int total_lines, int priority);

/**
 * Eliberează memoria unui document
 */
void document_free(Document* doc);

/* ========== FUNCȚII PENTRU IMPRIMANTE ========== */

/**
 * Creează o imprimantă nouă
 */
Printer* printer_create(int id, int time_per_line);

/**
 * Adaugă o imprimantă la sistemul de imprimare
 */
void printer_add(PrintSystem* system, Printer* printer);

/**
 * Scoate o imprimantă din serviciu (o dezactivează)
 */
void printer_deactivate(PrintSystem* system, int printer_id);

/**
 * Reactivează o imprimantă
 */
void printer_reactivate(PrintSystem* system, int printer_id);

/**
 * Eliberează memoria unei imprimante
 */
void printer_free(Printer* printer);

/* ========== FUNCȚII PENTRU COADA ========== */

/**
 * Inițializează coada de tipărire
 */
void queue_init(PrintQueue* queue);

/**
 * Adaugă un document în coada de tipărire
 */
void queue_enqueue(PrintQueue* queue, Document* doc);

/**
 * Scoate și returnează documentul din fața cozii
 */
Document* queue_dequeue(PrintQueue* queue);

/**
 * Verifică dacă coada este goală
 */
int queue_is_empty(PrintQueue* queue);

/**
 * Anulează un document din coadă sau din tipărire
 */
int queue_cancel_document(PrintSystem* system, int doc_id);

/**
 * Afișează coada de tipărire
 */
void queue_display(PrintQueue* queue);

/* ========== FUNCȚII PENTRU SISTEM ========== */

/**
 * Inițializează sistemul de imprimare
 */
PrintSystem* system_create();

/**
 * Distribuie documentele din coadă către imprimante disponibile (algoritm Greedy)
 */
void system_distribute_documents(PrintSystem* system);

/**
 * Simulează un pas de timp în sistem (tipărirea unei linii)
 */
void system_simulate_step(PrintSystem* system);

/**
 * Rulează simularea pentru un timp specificat
 */
void system_simulate(PrintSystem* system, int total_time);

/**
 * Afișează starea curentă a sistemului
 */
void system_display_status(PrintSystem* system);

/**
 * Afișează statistici finale
 */
void system_display_statistics(PrintSystem* system);

/**
 * Eliberează toată memoria alocată pentru sistem
 */
void system_free(PrintSystem* system);

#endif /* PRINT_QUEUE_H */
