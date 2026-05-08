#include "print_queue.h"

/* ==================== FUNCȚIA MAIN - DEMONSTRAȚIE ==================== */

void print_menu() {
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║       MENIU PRINCIPAL - SISTEM IMPRIMARE    ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║ 1. Afișare status sistem                     ║\n");
    printf("║ 2. Adăugare document în coadă               ║\n");
    printf("║ 3. Simulare pas timp                        ║\n");
    printf("║ 4. Simulare pentru N pași timp              ║\n");
    printf("║ 5. Dezactivare imprimantă                   ║\n");
    printf("║ 6. Reactivare imprimantă                    ║\n");
    printf("║ 7. Anulare document                         ║\n");
    printf("║ 8. Afișare statistici finale                ║\n");
    printf("║ 0. Ieșire                                   ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("Alegeți opțiune: ");
}

int main() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║  SISTEM GESTIONARE COADĂ TIPĂRIRE - N IMPRIMANTE     ║\n");
    printf("║  Implementare cu Liste Simplu Înlănțuite în C        ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");
    
    /* Inițializare sistem */
    PrintSystem* system = system_create();
    if (system == NULL) {
        fprintf(stderr, "Eroare la crearea sistemului\n");
        return 1;
    }
    
    /* Adăugare imprimante inițiale */
    printf("🔧 Inițializare imprimante...\n\n");
    
    Printer* p1 = printer_create(1, 3);  /* 3 unități timp per linie */
    Printer* p2 = printer_create(2, 5);  /* 5 unități timp per linie */
    Printer* p3 = printer_create(3, 4);  /* 4 unități timp per linie */
    
    printer_add(system, p1);
    printer_add(system, p2);
    printer_add(system, p3);
    
    printf("✓ Adăugate %d imprimante:\n", system->printer_count);
    printf("  • Imprimanta 1: 3 unități timp/linie\n");
    printf("  • Imprimanta 2: 5 unități timp/linie\n");
    printf("  • Imprimanta 3: 4 unități timp/linie\n\n");
    
    /* Meniu interactiv */
    int choice;
    int document_id = 1;
    int running = 1;
    
    while (running) {
        print_menu();
        scanf("%d", &choice);
        getchar(); /* Consumă newline */
        
        switch (choice) {
            case 1: {
                /* Afișare status */
                system_display_status(system);
                break;
            }
            
            case 2: {
                /* Adăugare document */
                char doc_name[100];
                int lines;
                
                printf("\nAdăugare document nou\n");
                printf("Nume document: ");
                fgets(doc_name, sizeof(doc_name), stdin);
                doc_name[strcspn(doc_name, "\n")] = 0; /* Elimină newline */
                
                printf("Numărul de rânduri: ");
                scanf("%d", &lines);
                getchar();
                
                if (lines > 0) {
                    Document* new_doc = document_create(document_id++, doc_name, lines, 0);
                    if (new_doc != NULL) {
                        queue_enqueue(&system->queue, new_doc);
                        printf("✓ Document %d adăugat în coadă\n", new_doc->id);
                        
                        /* Încearcă distribuire imediată */
                        system_distribute_documents(system);
                    }
                } else {
                    printf("⚠️  Numărul de rânduri trebuie să fie pozitiv\n");
                }
                break;
            }
            
            case 3: {
                /* Simulare un pas */
                printf("\nSimulare un pas de timp...\n");
                system_simulate_step(system);
                system_display_status(system);
                break;
            }
            
            case 4: {
                /* Simulare N pași */
                int steps;
                printf("\nCâți pași să simulez? ");
                scanf("%d", &steps);
                getchar();
                
                if (steps > 0) {
                    for (int i = 0; i < steps; i++) {
                        system_simulate_step(system);
                    }
                    printf("✓ Simulare completată pentru %d pași\n", steps);
                    system_display_status(system);
                } else {
                    printf("⚠️  Introduceți un număr pozitiv\n");
                }
                break;
            }
            
            case 5: {
                /* Dezactivare imprimantă */
                int printer_id;
                printf("\nID imprimantă de dezactivat: ");
                scanf("%d", &printer_id);
                getchar();
                
                printer_deactivate(system, printer_id);
                break;
            }
            
            case 6: {
                /* Reactivare imprimantă */
                int printer_id;
                printf("\nID imprimantă de reactivat: ");
                scanf("%d", &printer_id);
                getchar();
                
                printer_reactivate(system, printer_id);
                break;
            }
            
            case 7: {
                /* Anulare document */
                int doc_id;
                printf("\nID document de anulat: ");
                scanf("%d", &doc_id);
                getchar();
                
                queue_cancel_document(system, doc_id);
                break;
            }
            
            case 8: {
                /* Statistici finale */
                system_display_statistics(system);
                break;
            }
            
            case 0: {
                /* Ieșire */
                printf("\n👋 La revedere!\n\n");
                running = 0;
                break;
            }
            
            default: {
                printf("⚠️  Opțiune invalidă. Introduceți 0-8\n");
            }
        }
    }
    
    /* Afișare statistici finale înainte de eliberare */
    system_display_statistics(system);
    
    /* Eliberare memorie */
    printf("🧹 Curățare memorie...\n");
    system_free(system);
    printf("✓ Memorie eliberată cu succes\n\n");
    
    return 0;
}

/* ==================== DEMONSTRAȚIE AUTOMATĂ ==================== */

/**
 * Funcție alternativă care rulează o demonstrație automată
 * Comentează main() și decomentează aceasta pentru a testa
 */
/*
int main_demo() {
    printf("\n📋 DEMONSTRAȚIE AUTOMATĂ - Sistem de Imprimare\n");
    printf("════════════════════════════════════════════════\n\n");
    
    // Creează sistem
    PrintSystem* system = system_create();
    
    // Adaugă imprimante
    printer_add(system, printer_create(1, 3));
    printer_add(system, printer_create(2, 5));
    printer_add(system, printer_create(3, 4));
    
    printf("✓ Sistem creat cu 3 imprimante\n\n");
    
    // Adaugă documente în coadă
    int doc_id = 1;
    queue_enqueue(&system->queue, document_create(doc_id++, "Raport Trimestrial", 20, 0));
    queue_enqueue(&system->queue, document_create(doc_id++, "Contract de Vânzare", 15, 0));
    queue_enqueue(&system->queue, document_create(doc_id++, "Facturi", 30, 0));
    queue_enqueue(&system->queue, document_create(doc_id++, "Invoice 2024", 10, 0));
    queue_enqueue(&system->queue, document_create(doc_id++, "Corespondență", 25, 0));
    
    printf("✓ Adăugate 5 documente în coadă\n\n");
    
    // Rulează simulare
    system_simulate(system, 500);
    
    // Afișează statistici
    system_display_statistics(system);
    
    // Eliberare memorie
    system_free(system);
    
    return 0;
}
*/
