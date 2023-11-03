    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <string.h>
    #include <limits.h>
    #include <float.h>
    #include <stdbool.h>
    #include "functions.h"

    int main() {
        FILE* file = fopen("colleges.txt", "r");
        FILE* file1 = fopen("hospitals.txt", "r");
        FILE* file2 = fopen("info.txt", "r");
        if (file1 == NULL) {
            printf("Failed to open the hospitals file.\n");
            return 1;
        }
        if (file == NULL) {
            printf("Failed to open the colleges file.\n");
            return 1;
        }

        struct Graph graph;
        graph.num_vertices = 0;

        char line[100];
        while (fgets(line, sizeof(line), file)) {
            char name[MAX_NAME_LENGTH];
            double lat, lon;
            sscanf(line, "%[^,],%lf,%lf", name, &lat, &lon);

            struct Location location;
            strcpy(location.name, name);
            location.latitude = lat;
            location.longitude = lon;

            graph.locations[graph.num_vertices] = location;

            for (int i = 0; i < graph.num_vertices; i++) {
                double distance = calculate_distance(graph.locations[i], location);
                graph.adjacency_matrix[i][graph.num_vertices] = distance;
                graph.adjacency_matrix[graph.num_vertices][i] = distance;
            }

            graph.num_vertices++;

            if (graph.num_vertices >= MAX_LOCATIONS) {
                break; // Reached the maximum number of locations
            }
        }

        fclose(file);

        while (fgets(line, sizeof(line), file1)) {
            char name[MAX_NAME_LENGTH];
            double lat, lon;
            char specialization[MAX_NAME_LENGTH];
            sscanf(line, "%[^;];%lf;%lf;%s", name, &lat, &lon, specialization);

            struct Location location;
            strcpy(location.name, name);
            location.latitude = lat;
            location.longitude = lon;
            strcpy(location.specialization, specialization);

            graph.locations[graph.num_vertices] = location;

            for (int i = 0; i < graph.num_vertices; i++) {
                double distance = calculate_distance(graph.locations[i], location);
                graph.adjacency_matrix[i][graph.num_vertices] = distance;
                graph.adjacency_matrix[graph.num_vertices][i] = distance;
            }

            graph.num_vertices++;

            if (graph.num_vertices >= MAX_LOCATIONS) {
                break; // Reached the maximum number of locations
            }
        }

        fclose(file1);

        struct HashTable hash_table;
        for (int i = 0; i < MAX_LOCATIONS; i++) {
            hash_table.buckets[i] = NULL;
        }

        while (fgets(line, sizeof(line), file2)) {
            char name[MAX_NAME_LENGTH];
            float rating;
            char timing[MAX_NAME_LENGTH];
            char address[MAX_NAME_LENGTH * 2];
            int fees;
            sscanf(line, "%[^;];%f;%[^;];%d;%[^\n]", name, &rating, timing, &fees, address);

            struct info hospital_info;
            strcpy(hospital_info.name, name);
            hospital_info.rating = rating;
            strcpy(hospital_info.timing, timing);
            hospital_info.fees = fees;
            strcpy(hospital_info.address, address);

            insert_hash_node(&hash_table, hospital_info);
        }

        fclose(file2);

        // Load reviews before performing any actions
        load_reviews_from_file(&hash_table, "reviews.txt");

        char location_name[MAX_NAME_LENGTH];
        double max_distance;
        char specialization[MAX_NAME_LENGTH];

        printf("\n\n");
        printf("\n\n======================================================================================================\n\n");
        printf("\t\t\t\t   HOSPITAL FINDER FOR COLLEGES");
        printf("\n\n======================================================================================================\n\n");
        printf("\n\n");

        int user_choice;

        printf("Choose an option:\n");
        printf("1. Get List and information of Hospitals\n");
        printf("2. Give a Review\n");
        printf("3. Add/Display Comment\n");
        printf("\nEnter your choice (1 or 2 or 3): ");
        scanf("%d", &user_choice);

        if (user_choice == 1) {
            // Get names of hospitals
            get_hospital_names(&graph, &hash_table);
            printf("Please choose college from the options given below: \n\n\n");

            FILE* file3 = fopen("outputFile.txt","r");
            while (fgets(line, sizeof(line), file3)) {
                char name[MAX_NAME_LENGTH];
                double lat, lon;
                printf("%s", line);

            }
            fclose(file3);

            printf("\n\n======================================================================================================\n\n");
            printf("\n\n");
            printf("Please choose specializations from the options given below: \n\n\n");
            printf("GENERAL\t\t\tMULTISPECIALITY\nORTHOPEDIC\t\tSKIN_AND_HAIR\nPEDIATRICS\t\tOPHTHALMOLOGY\nDENTAL\t\t\tAYURVED\nENDOSCOPY\t\tALLOPATHY");

            printf("\n\n\n======================================================================================================\n");

            printf("\n\nEnter the College Name: ");
            fgets(location_name, sizeof(location_name), stdin);
            location_name[strcspn(location_name, "\n")] = '\0';

            int source = find_location_index(&graph, location_name);
            if (source == -1) {
                printf("Invalid College Name.\n");
                return 1;
            }

            printf("\nEnter the maximum distance (in km): ");
            scanf("%lf", &max_distance);

            getchar(); // Consume the newline character

            printf("\nEnter the specialization: ");
            fgets(specialization, sizeof(specialization), stdin);
            specialization[strcspn(specialization, "\n")] = '\0';

            find_nearest_hospitals(&graph, source, max_distance, specialization, &hash_table);

            printf("\n");
            printf("_____________________________________________________________________________________________________\n\n");
            printf("Do you want more details of the hospitals? (YES/NO): ");
            char choice[3];
            scanf("%s",choice);
            printf("\n_____________________________________________________________________________________________________\n\n");
            if (strcmp(choice, "YES")==0){
                details(&graph, source, max_distance, specialization, &hash_table);
            }

            // Save reviews before exiting the program
            save_reviews_to_file(&hash_table, "reviews.txt");

        } else if (user_choice == 2) {
            // Give a review
            review_hospitals(&graph, &hash_table);
        }else if (user_choice == 3) {
            // Add or display comment
            printf("1. Add Comment\n");
            printf("2. Display Comments\n");
            printf("Enter your choice (1 or 2): ");
            int comment_choice;
            if (scanf("%d", &comment_choice) != 1) {
                printf("Invalid input. Please enter 1 or 2.\n");
                while (getchar() != '\n');
                    // continue;
            }
            getchar(); // Consume the newline character
            load_comments_from_file(comments, &num_comments, "comments.txt");
if (comment_choice == 1) {
    // Add Comment
    char hospital_name[MAX_NAME_LENGTH];
    char user_name[MAX_NAME_LENGTH];
    char comment_text[MAX_NAME_LENGTH * 2];
    
    display_hospitals(&graph);
    printf("Enter Hospital Name for Comment: ");
    
    // Read the hospital name with fgets and remove the newline character
    fgets(hospital_name, sizeof(hospital_name), stdin);
    hospital_name[strcspn(hospital_name, "\n")] = '\0';

    int hospital_index = get_hospital_index(&graph, hospital_name);
    if (hospital_index == -1) {
        printf("Invalid Hospital Name. The entered hospital is not in the list. Please try again.\n");
        // Handle the error or return to the menu
    } else {
        printf("Enter User Name: ");
        scanf("%s", user_name);
        printf("Enter Comment: ");
        getchar(); // Consume the newline character
        fgets(comment_text, sizeof(comment_text), stdin);
        comment_text[strcspn(comment_text, "\n")] = '\0';

        add_comment(comments, &num_comments, hospital_name, user_name, comment_text);
    }
} else if (comment_choice == 2) {
    // Display Comments
    char hospital_name[MAX_NAME_LENGTH];
    
    display_hospitals(&graph);
    printf("Enter Hospital Name to Display Comments: ");
    
    // Read the hospital name with fgets and remove the newline character
    fgets(hospital_name, sizeof(hospital_name), stdin);
    hospital_name[strcspn(hospital_name, "\n")] = '\0';

    int hospital_index = get_hospital_index(&graph, hospital_name);
    if (hospital_index == -1) {
        printf("Invalid Hospital Name. The entered hospital is not in the list. Please try again.\n");
        // Handle the error or return to the menu
    } else {
        // Display comments if the hospital name is valid
        display_comments(comments, num_comments, hospital_name);
    }
} else {
    printf("Invalid choice for comments.\n");
}





// Save reviews and comments before exiting the program
save_reviews_to_file(&hash_table, "reviews.txt");
save_comments_to_file(comments, num_comments, "comments.txt");

return 0;

    }

    }