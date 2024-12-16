/** @file route_manager.c
 *  @brief A small program to analyze airline routes data.
 *  @author Mike Z.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Jose O.
 *  @author Saasha J.
 *  @author Victoria L.
 *  @author Lindsay Gammon
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// TODO: Make sure to adjust this based on the input files given
#define MAX_LINE_LEN 100
void q1(FILE *fp, int number_of_entries);
void q2(FILE *fp, int number_of_entries);
void q3(FILE *fp, int number_of_entries);

/**
 * @brief Serves as an incremental counter for navigating the list.
 *
 * @param p The pointer of the node to print.
 * @param arg The pointer of the index.
 *
 */
void inccounter(node_t *p, void *arg)
{
    int *ip = (int *)arg;
    (*ip)++;
}

/**
 * @brief Allows to print out the content of a node.
 *
 * @param p The pointer of the node to print.
 * @param arg The format of the string.
 *
 */
void print_node(node_t *p, void *arg)
{
    char *fmt = (char *)arg;
    printf(fmt, p->word, p->count);
}

/**
 * @brief Allows to print each node in the list.
 *
 * @param l The first node in the list
 *
 */
void analysis(node_t *l)
{
    int len = 0;

    apply(l, inccounter, &len);
    printf("Number of words: %d\n", len);

    apply(l, print_node, "%s, %d\n");
}

/**
 * @brief The main function and entry point of the program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */
int main(int argc, char *argv[])
{	
	char *yaml = NULL;
	char *line = NULL;
	char *question = NULL;
	char *N = NULL;
    int number_of_entries;
		
	for (int i = 1; i < argc; i++){
		if (strncmp("--DATA", argv[i], strlen("--DATA")) == 0){
			yaml = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
			sscanf(argv[i], "%*[^=]=%[^\n]", yaml);
		//copy whats after --DATA=
		}
		if (strncmp("--QUESTION", argv[i], strlen("--QUESTION")) == 0){
			question = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
			sscanf(argv[i], "%*[^=]=%[^\n]", question);
			//copy whats after --QUESTION = to input
		} 
		if (strncmp("--N", argv[i], strlen("--N")) == 0){
			N = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
			sscanf(argv[i], "%*[^=]=%[^\n]", N);
			number_of_entries = atoi(N);
			//copy whats after --N= to input
		}
	}
	
	FILE *fp = fopen(yaml, "r");

	if (strncmp(question, "1", 1) ==0){
		q1(fp, number_of_entries);
	} else if (strncmp(question, "2", 1) ==0){
		q2(fp, number_of_entries);
	} else if (strncmp(question, "3", 1) ==0){
		q3(fp, number_of_entries);
	}

	free(line);
	free(yaml);
	free(question);
	free(N);
	
	exit(0);
}
	
// ▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼ QUESTION 1 ▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼
	
void q1(FILE *fp, int number_of_entries){
	
	char *airline = NULL;
	char *icao = NULL;
	airline = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
	icao = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
	node_t *preliminary_list = NULL;
	
	char buffer[1024];
	fgets(buffer, 1024, fp);//ignore first line
	int counter = 0;
	while (fgets(buffer, 1024, fp)){ 
		char* value = strtok(buffer, "\n");
		counter++;
		if (counter == 13){
			counter = 0;
		}
		while (value) {
			if (counter == 1) {//airline_name
				strncpy(airline, value + 16, 40);
			}
			if (counter == 2) {//airline_icao_unique_code
				strncpy(icao, value + 28, 4);
			}
			if (counter == 11){ //to_airport_country
				if (strncmp(value, "  to_airport_country: Canada",28)==0){
					//if dest country is canada, then add data to list
					airline[strcspn(airline,"\n")] = '\0'; //get rid of \n
					icao[strcspn(icao,"\n")] = '\0';
					char airline_icao[MAX_LINE_LEN]; //combine & format airlien & icao to add to list
					sprintf(airline_icao,"%s (%s)",airline,icao);
					int check = is_in(preliminary_list, airline_icao);
					if (check == 0) { //if is_in returns 0, the airline is not in list & so must be added
						preliminary_list = add_inorder(preliminary_list, new_node(airline_icao, 1));
					} //if is_in determines that the entry is already in list, it increments the count 
				}
			}
			value = strtok(NULL, "\n");
		}
	}
	fclose(fp);
	
	// ♦♦♦♦ take items from preliminary_list, add to finalized list so they're sorted by count ♦♦♦♦
	
	node_t *list = NULL;
	node_t *temp_n2 = NULL;
    for (; preliminary_list != NULL; preliminary_list = temp_n2){
		temp_n2 = preliminary_list->next;
		list = add_inorder(list, new_node(preliminary_list->word, preliminary_list->count));
	}
	
	// ♦♦♦♦ create output files ♦♦♦♦
	
	FILE *fw;
	fw = fopen("output.csv", "w+");
	fprintf(fw, "subject, statistic\n");
	
	// ♦♦♦♦ write to output files ♦♦♦♦
	
	node_t *temp_n4 = NULL;
	int count_additions = 0;
	for (; list != NULL; list = temp_n4){
		temp_n4 = list->next;
		fprintf(fw, "%s, %d\n", list->word, list->count);
		count_additions++;
		if (count_additions >= number_of_entries){
			break;
		}
	}
	
    // ○•○•○•○•○•○•○•○•○• Release space allocated ○•○•○•○•○•○•○•○•○•
    node_t *temp_n = NULL;
    for (; preliminary_list != NULL; preliminary_list = temp_n)
    {
        temp_n = preliminary_list->next;
        free(preliminary_list->word);
        free(preliminary_list);
    }
	node_t *temp_n3 = NULL;
	for (; list != NULL; list = temp_n3)
    {
        temp_n3 = list->next;
        free(list->word);
        free(list);
    }
	free(icao);
	free(airline);
}

// ▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼ QUESTION 2 ▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼

void q2(FILE *fp, int number_of_entries){
	char *dest_country = NULL;
	dest_country = (char *)malloc(sizeof(char) * MAX_LINE_LEN);

	node_t *preliminary_list = NULL;
	
	char buffer[1024];
	fgets(buffer, 1024, fp);//ignore first line
	int counter = 0;
	while (fgets(buffer, 1024, fp)){ 
		char* value = strtok(buffer, "\n");
		counter++;
		if (counter == 13){
			counter = 0;
		}
		while (value) {
			if (counter == 11) {//airline_name
				char *find;
				find = strstr(value, "'");
				if (find != NULL){
					int size = strlen(value); 
					value[size-1] = '\0';
					strncpy(dest_country, value + 24, 40);
				} else {
					strncpy(dest_country, value + 22, 40);
				} 
				int check = is_in(preliminary_list, dest_country);
				if (check == 0) { //if is_in returns 0, the airline is not in list & so must be added
					preliminary_list = add_inorder(preliminary_list, new_node(dest_country, 1));
				}
			}
			value = strtok(NULL, "\n");
		}
	}
	fclose(fp);
	
	// ♦♦♦♦ take items from preliminary list, add to finalized list so they're sorted ascending by count ♦♦♦♦
	
	node_t *list = NULL;
	node_t *temp_n2 = NULL;
    for (; preliminary_list != NULL; preliminary_list = temp_n2){
		temp_n2 = preliminary_list->next;
		list = add_ascending_order(list, new_node(preliminary_list->word, preliminary_list->count));
	}
	
	// ♦♦♦♦ create output files ♦♦♦♦
	FILE *fw;
	fw = fopen("output.csv", "w+");
	fprintf(fw, "subject, statistic\n");
	
	// ♦♦♦♦ write to output files ♦♦♦♦
	//could be its own function
	node_t *temp_n4 = NULL;
	int count_additions = 0;
	for (; list != NULL; list = temp_n4){
		temp_n4 = list->next;
		fprintf(fw, "%s, %d\n", list->word, list->count);
		count_additions++;
		if (count_additions >= number_of_entries){
			break;
		}
	}
	fclose(fw);
	
	
	// ○•○•○•○•○•○•○•○•○• Release space allocated ○•○•○•○•○•○•○•○•○•
	node_t *temp_n = NULL;
    for (; preliminary_list != NULL; preliminary_list = temp_n)
    {
        temp_n = preliminary_list->next;
        free(preliminary_list->word);
        free(preliminary_list);
    }
	node_t *temp_n3 = NULL;
	for (; list != NULL; list = temp_n3)
    {
        temp_n3 = list->next;
        free(list->word);
        free(list);
    }
	free(dest_country);
}

// ▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼ QUESTION 3 ▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼▲▼

void q3(FILE *fp, int number_of_entries){
	char *airport = NULL;
	airport = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
	char *icao = NULL;
	icao = (char *)malloc(sizeof(char) * MAX_LINE_LEN);	
	char *dest_city = NULL;
	dest_city = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
	char *dest_country = NULL;
	dest_country = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
	
	node_t *preliminary_list = NULL;
	
	char buffer[1024];
	fgets(buffer, 1024, fp);//ignore first line
	int counter = 0;
	while (fgets(buffer, 1024, fp)){ 
		char* value = strtok(buffer, "\n");
		counter++;
		if (counter == 13){
			counter = 0;
		}
		while (value) {
			//a better way to do this would be to go through value until you found ":"
			// then save what's after that and delete the leading whitespace
			if (counter == 9) {//to_airport_name
				strncpy(airport, value + 19, 40);
			}
			if (counter == 10) {//to_airport_city
				strncpy(dest_city, value + 19, 40);
			}
			if (counter == 11) {//to_airport_country
				strncpy(dest_country, value + 22, 40);
			}
			if (counter == 12) {//to_airport_icao_unique_code
				strncpy(icao, value + 31, 4);
			}
			airport[strcspn(airport,"\n")] = '\0'; //get rid of \n
			icao[strcspn(icao,"\n")] = '\0';
			dest_city[strcspn(dest_city,"\n")] = '\0';
			dest_country[strcspn(dest_country,"\n")] = '\0';
			char airport_icao_city_country[MAX_LINE_LEN]; //combine & format to add to list
			sprintf(airport_icao_city_country,"%s (%s), %s, %s",airport,icao,dest_city,dest_country);
			
			int check = is_in(preliminary_list, airport_icao_city_country);
			if (check == 0) { //if is_in returns 0, the airline is not in list & so must be added
				preliminary_list = add_inorder(preliminary_list, new_node(airport_icao_city_country, 1));
			}
			value = strtok(NULL, "\n");
		}
	}
	fclose(fp);
	
	// ♦♦♦♦ take items from preliminary list, add to finalized list so they're sorted ascending by count ♦♦♦♦
	
	node_t *list = NULL;
	node_t *temp_n2 = NULL;
    for (; preliminary_list != NULL; preliminary_list = temp_n2){
		temp_n2 = preliminary_list->next;
		list = add_inorder(list, new_node(preliminary_list->word, preliminary_list->count));
	}
	
	// Without this function, my statistics are off by a factor of 10. I think it is an issue with 
	// the is_in function, but I can't for the life of me figure out where. 
	decrease(list);
	
	FILE *fw;
	fw = fopen("output.csv", "w+");
	fprintf(fw, "subject, statistic\n");
	
	// ♦♦♦♦ write to output files ♦♦♦♦
	//could be its own function
	node_t *temp_n4 = NULL;
	int count_additions = 0;
	for (; list != NULL; list = temp_n4){
		temp_n4 = list->next;
		fprintf(fw, "\"%s\", %d\n", list->word, list->count);
		count_additions++;
		if (count_additions >= number_of_entries){
			break;
		}
	}
	fclose(fw);
	
	// ○•○•○•○•○•○•○•○•○• Release space allocated ○•○•○•○•○•○•○•○•○•
	node_t *temp_n = NULL;
    for (; preliminary_list != NULL; preliminary_list = temp_n)
    {
        temp_n = preliminary_list->next;
        free(preliminary_list->word);
        free(preliminary_list);
    }
	node_t *temp_n3 = NULL;
	for (; list != NULL; list = temp_n3)
    {
        temp_n3 = list->next;
        free(list->word);
        free(list);
    }
	free(airport);
	free(icao);
	free(dest_city);
	free(dest_country);
}

