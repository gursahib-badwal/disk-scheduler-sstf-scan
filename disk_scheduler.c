#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <time.h>

struct Req
{
    int disk_track;
    int initial_index;
    int final_index;
};

typedef struct Req Req;

void print_input_list(int* request, int len) {

    printf("[");
    for (int i=0 ; i < len; i++) {

        if (i != len - 1){
        printf("%d, ", request[i]);
        }
        else {
        printf("%d", request[i]);
        }
    }
    printf("]\n\n");
}
void print_return_list(Req* request, int len) {

    printf("[");
    for (int i=0 ; i < len; i++) {

        if (i != len - 1){
        printf("%d, ", request[i].disk_track);
        }
        else {
        printf("%d", request[i].disk_track);
        }
    }
    printf("]\n\n");
}

void sstf_algo(int* request, int len){

    double delay_sum = 0;
    int num_delays = 0;
    int longest_delay = 0;
    int current_index = 0;
    int tracks_traversed = 0;
    Req* sstf_array = (Req*) malloc(sizeof(Req) * len);
    bool* tt_array = (bool*) malloc(sizeof(bool) * len);

    for (int i=0 ; i < len; i++){
        tt_array[i] = false;
        sstf_array[i].final_index = i;
    }

    int k=0;
    while (k < len) {
        int minimum = 200;
        int minimum_index =0;

        for (int j=0; j < len; j++){
            if (abs(request[j] - request[current_index]) < minimum && tt_array[j] == false) {
                minimum = abs(request[j] - request[current_index]);
                minimum_index = j;
            }
        }
        tracks_traversed += abs(request[minimum_index] - request[current_index] + 1);
        sstf_array[k].disk_track = request[minimum_index];
        sstf_array[k].initial_index = minimum_index;
        current_index = minimum_index;
        tt_array[minimum_index] = true;
        k++;
    }

    for (int i=0; i < len; i++) {
        int delay_difference = sstf_array[i].final_index - sstf_array[i].initial_index;
        if (delay_difference > 0){
            delay_sum += delay_difference;
            num_delays ++;
        }
        if (delay_difference > longest_delay){
            longest_delay = delay_difference;
        }
    }
 
    printf("SSTF requests array: ");
    print_return_list(sstf_array, len);
    printf("Total number of tracks traversed: %d\n", tracks_traversed);
    printf("Longest Delay: %d\n", longest_delay);
    if (num_delays == 0){
    printf("Average Delay: 0\n");   
    }
    else {
    printf("Average Delay: %f\n", delay_sum / num_delays);
    }
    printf("<----------------------------------------------------------------------------->\n\n");
    free(sstf_array);
    free(tt_array);
}

int sort_helper(const void *first_num,const void *second_num) {
    int first = *(int*)first_num;
    int second = *(int*)second_num;
    return first - second;
}

void scan_algo(int* req, int len){
    double delay_sum = 0;
    int num_delays = 0;
    int longest_delay = 0;
    int tracks_traversed = 0;
    int* scanned_array = (int*) malloc(sizeof(int) * len);
    Req* return_array = (Req*) malloc(sizeof(Req) * len);

    for (int i=0; i< len; i++){
        scanned_array[i] = req[i];
    } 

    int orginal_head_value = scanned_array[0];
    int new_head_value = 0;

    qsort(scanned_array, len, sizeof(int), sort_helper);

    for (int j=0; j < len; j++){
        if(scanned_array[j] == orginal_head_value){
            new_head_value = j;
        }
    }

    int k=0;
    for(int i=new_head_value;i>=0;i--){
        return_array[k].disk_track = scanned_array[i];
        k++;
    }
    for(int i=new_head_value+1;i<len;i++){
        return_array[k].disk_track = scanned_array[i];
        k++;
    }

    for(int i=0;i<len;i++){
        int val = req[i];
        for(int j=0;j<len;j++){
            if(val == return_array[j].disk_track){
                return_array[j].initial_index = i;
            }
        }
    }
    for(int i=0;i<len;i++){
        return_array[i].final_index = i;
    }
    for (int i=0; i < len; i++) {
        int delay_difference = return_array[i].final_index - return_array[i].initial_index;
        if (delay_difference > 0){
            delay_sum += delay_difference;
            num_delays ++;
        }
        if (delay_difference > longest_delay){
            longest_delay = delay_difference;
        }
    }

    tracks_traversed = abs(scanned_array[new_head_value] - scanned_array[0]) + abs(scanned_array[len - 1] - scanned_array[0]);
    
    printf("SCAN array: ");
    print_return_list(return_array, len);
    printf("Total track traversed: %d\n", tracks_traversed);
    printf("Longest Delay: %d\n", longest_delay);
    printf("Average Delay: %f\n", delay_sum / num_delays);
    free(scanned_array);
    free(return_array);
}


int* parse_input(const char* arg, int* n){
    int num = 0;
    int requests_num = 0;

    for (int i=0; i < strlen(arg); i++){
        if (arg[i] == ','){
            requests_num++;
        }
    }

    int *request = (int*)malloc(sizeof(int) * (requests_num + 1));
    int index = 0;
    for (int i=0; i < strlen(arg); i++){
        if (arg[i] == ','){
            request[index] = num;
            index++;
            num = 0;
        }
        else {
            num = num*10 + (arg[i] - '0');
        }
    }
    request[index] = num;
    *n = requests_num + 1;

    for (int i=0; i < *n; i++) {
        if (request[i] >= 200) {
            printf("Invalid input requests array!");
            return NULL;
        }
    }
    return request;
}


bool check_duplicate(int* array, int array_length, int check_element){
    for (int i=0; i < array_length; i++){
        if (array[i] == check_element){
            return true;
        }
    }
    return false;
}

int* create_random()
{
    int* request = (int*) malloc(sizeof(int)*50);
    for (int i=0; i < 50; i++){
        int random_value = rand() % 200;
        if (check_duplicate(request, i + 1, random_value)){
            i--;
        }
        else {
            request[i] = random_value;
        }
    }
    return request;
}


int main(int argc, char* argv[]) {

    srand(time(0));
    int* request;
    int len;

    if (argc == 2){
        request = parse_input(argv[1], &len);
        if(request == NULL){
            return 0;
        }
    }
    else {
        len = 50;
        request = create_random();
    }

    printf("\n\nOriginal Requests Array: ");
    print_input_list(request, len);

    sstf_algo(request, len);

    printf("\n\nOriginal Requests Array: ");
    print_input_list(request, len);
    
    scan_algo(request, len);

    free(request);
    return 0;
}