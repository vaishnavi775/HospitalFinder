    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <string.h>
    #include <limits.h>
    #include <float.h>
    #include <stdbool.h>

    #define MAX_LOCATIONS 100
    #define MAX_NAME_LENGTH 500
    #define MAX 100
    #define MAX_COMMENTS 100


    struct Location {
        char name[MAX_NAME_LENGTH];
        double latitude;
        double longitude;
        char specialization[MAX_NAME_LENGTH];
    };

    struct Graph {
        int num_vertices;
        struct Location locations[MAX_LOCATIONS];
        double adjacency_matrix[MAX_LOCATIONS][MAX_LOCATIONS];
    };

    typedef struct info {
        char name[MAX_NAME_LENGTH];
        float rating;
        char timing[MAX];
        int fees;
        char address[MAX_NAME_LENGTH * 2];
        // New fields for reviews and ratings
        int num_reviews;
        float total_rating;
    } info;

    struct HashNode {
        struct info info;
        struct HashNode* next;
    };

    struct HashTable {
        struct HashNode* buckets[MAX_LOCATIONS];
    };

    struct Comment {
        char hospital_name[MAX_NAME_LENGTH];
        char user_name[MAX_NAME_LENGTH];
        char text[MAX_NAME_LENGTH * 2];
    };
    struct Comment comments[MAX_COMMENTS];
    int num_comments = 0;

    unsigned int hash(const char* str) {
        unsigned int hash = 5381;
        int c;

        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }

        return hash;
    }

        void insert_hash_node(struct HashTable* hash_table, struct info info) {
        unsigned int index = hash(info.name) % MAX_LOCATIONS;

        struct HashNode* new_node = malloc(sizeof(struct HashNode));
        new_node->info = info;
        new_node->info.num_reviews = 0;
        new_node->info.total_rating = 0.0;
        new_node->next = NULL;

        if (hash_table->buckets[index] == NULL) {
            hash_table->buckets[index] = new_node;
        } else {
            struct HashNode* current = hash_table->buckets[index];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }
    }


    struct HashNode* search_hash_node(struct HashTable* hash_table, const char* name) {
        unsigned int index = hash(name) % MAX_LOCATIONS;

        struct HashNode* current = hash_table->buckets[index];
        while (current != NULL) {
            if (strcmp(current->info.name, name) == 0) {
                return current;
            }
            current = current->next;
        }

        return NULL;
    }

void print_hospital_info(struct HashNode* hospital) ;
double calculate_distance(struct Location loc1, struct Location loc2);
int find_nearest_location(double* dist, int* visited, int num_vertices);
void dijkstra(struct Graph* graph, int source, double* dist, int* prev);
int find_location_index(struct Graph* graph, const char* location_name);
int find_min_distance_vertex(double* dist, bool* visited, int num_vertices);
void find_nearest_hospitals(struct Graph* graph, int source, double max_distance, const char* specialization, struct HashTable* hash_table) ;
void get_hospital_names(struct Graph* graph, struct HashTable* hash_table);
void details(struct Graph* graph, int source, double max_distance, const char* specialization, struct HashTable* hash_table); 
void save_reviews_to_file(struct HashTable* hash_table, const char* filename);
void load_reviews_from_file(struct HashTable* hash_table, const char* filename);
void review_hospitals(struct Graph* graph, struct HashTable* hash_table);
void load_comments_from_file(struct Comment* comments, int* num_comments, const char* filename);
void add_comment(struct Comment* comments, int* num_comments, const char* hospital_name, const char* user_name, const char* text);
void display_comments(struct Comment* comments, int num_comments, const char* hospital_name);
void save_comments_to_file(struct Comment* comments, int num_comments, const char* filename) ;
void display_hospitals(struct Graph* graph);
int get_hospital_index(struct Graph* graph, const char* hospital_name);
