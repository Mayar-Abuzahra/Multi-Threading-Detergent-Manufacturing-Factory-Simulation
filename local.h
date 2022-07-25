#ifndef __LOCAL_H_
#define __LOCAL_H_

/*
 * Common header file
 */
#include <math.h> 
#include <wait.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/types.h>
#include <linux/stat.h>


#define SHM_KEY 0x3000 
#define TOTAL_TECH 10
#define TOTAL_LINES 5
#define TRUCK_WAITING 0
#define TRUCK_LOADING 1
#define TRUCK_ON_TRIP 2
#define TRUCK_TRIP_DELAY_MIN 2
#define TRUCK_TRIP_DELAY_MAX 5
#define MIN_EMP_NUM_L1 6
#define MAX_EMP_NUM_L1 10
#define MIN_EMP_NUM_L3 5
#define MAX_EMP_NUM_L3 10
#define CAPACITY_L1 5
#define CAPACITY_L2 6
#define NUM_THREADS 5
#define LINE_PRODUCTS_THREASHOLD 2
#define TRUCK_D_CAPACITY 10
#define THRESHOLD 2

char config_file[100];
struct timespec start, finish,check;
bool endSimulation_flag = false;

//Prototyping
void *truck_A(void *);
void *truck_B(void *);
void *truck_D(void *);
void *Factory_A(void *);
void *Factory_B(void *); 
void *print_msg(void *);
void *l1_A_mission(void *);
void *l1_B_mission(void *);
void *tech_mission(void *);
void *tech_mission(void *);
void *config_parser(void *);
void *endSimulation(void *);
void *Storage_manager(void *);
int get_random_value(int, int);
int min(int, int);
void *l2_mission(void *);
void *l3_mission(void *);
void *manufacturing_lines(void *);
void* first_emp(void *);
void* last_emp(void *);

int *trucks_A_availability_arrival=0,
    *trucks_B_availability_arrival=0;

int *index_truckA_arr,
    *index_truckB_arr,
    *index_truckD_arr;
    
int emp_index[10] = {1,2,3,4,5,6,7,8,9,10},
                    line_index[5]={1,2,3,4,5},
                    products[5],
                    available_products[5],
                    steps_line_one[10],
                    steps_line_two[10],
                    steps_line_three[10],
                    steps_line_four[10],
                    steps_line_five[10];
  
float time_threshold;
unsigned int storage_employee_delay,
						 truck_a_max_capacity,
						 truck_b_max_capacity,
						 truck_trip_delay,
						 storage_area_max_A,
						 storage_area_max_B,
						 storage_area_min,
						 num_detergent_threshold,
						 storage_area_employees,
						 order_amount_A,
						 order_amount_B,
						 request_order_A,
						 request_order_B,
						 running_trucksA,
						 running_trucksB,
						 truck_A_num,
						 truck_B_num,
						 fab_A_cartoon,
						 fab_B_cartoon,
						 flag_embty_catroon_A,
						 flag_embty_catroon_B,
						 flag_lessthan_required_factA,
						 flag_lessthan_required_factB,
						 storage_Area_fabA,
						 storage_Area_fabB,
						 num_of_l1_emp_A,
						 num_of_l1_emp_B,
						 num_of_l3_emp,
						 l2_employees,
						 counter_active_l1_A,
						 counter_active_l1_B,
						 S2_final_products,
						 truck_D_num,
						 total_detergent_products,
						 total_detergent_products_threshold;

int truck_A_status = TRUCK_WAITING; 
int truck_B_status = TRUCK_WAITING;
int truck_D_status = TRUCK_WAITING;
    
unsigned int storage_A_products=0, 
	         storage_B_products=0;
	     

     

pthread_cond_t orderA_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t orderB_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t order_A_factory = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t order_B_factory = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_request_A = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t mutex_request_B = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t order_amount_check_A = PTHREAD_MUTEX_INITIALIZER;                         
pthread_mutex_t order_amount_check_B = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t mutex_trucks_A_status = PTHREAD_MUTEX_INITIALIZER;  
pthread_mutex_t mutex_trucks_B_status = PTHREAD_MUTEX_INITIALIZER; 

pthread_mutex_t storage_productA_mutex = PTHREAD_MUTEX_INITIALIZER;  
pthread_mutex_t storage_productB_mutex = PTHREAD_MUTEX_INITIALIZER;
 
pthread_mutex_t storage_Area_fabA_mutex = PTHREAD_MUTEX_INITIALIZER;  
pthread_mutex_t storage_Area_fabB_mutex = PTHREAD_MUTEX_INITIALIZER;  


pthread_mutex_t counter_l1_A_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t counter_l1_B_mutex=PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t storage_two_final_products_mutex = PTHREAD_MUTEX_INITIALIZER; 

static pthread_mutex_t lines_mutex[NUM_THREADS];

static pthread_mutex_t queues_mutex[NUM_THREADS];

pthread_mutex_t available_products_mutex = PTHREAD_MUTEX_INITIALIZER; 

pthread_mutex_t total_detergent_products_mutex = PTHREAD_MUTEX_INITIALIZER;    
      
pthread_t *thread_trucks_A ,
          *thread_trucks_B ,  
          *thread_storage_area_employees,
          *thread_manufacturing_lines,
          *l1_thread_A,
          *l1_thread_B,
          *thread_l2_employees,
          *thread_l3_mission,
          *thread_trucks_D;
           
pthread_t  thread_configFile ,
           thread_console ,
           thread_Factory_A,
           thread_Factory_B,
           thread_Storage_manager,
           thread_endSimulation;  

/*Colors*/
#define ORIGINAL "\x1B[0m"
#define RED_COLOR "\x1B[31m"
#define GREEN_COLOR "\x1B[32m"
#define YELLOW_COLOR "\x1B[33m"
#define BLUE_COLOR "\x1B[34m"
#define PURPLE_COLOR "\x1B[35m"
#define CYAN_COLOR "\x1B[36m"
#define WHITE_COLOR "\x1B[37m" 
          
 //High intensty background
#define REDHB "\e[0;101m" 
#define WHTHB "\e[0;107m"
#define WHTB "\e[47m"
#define BLKB "\e[40m"

//Bold High intensty 
#define BHBLK "\e[1;90m"
#define BHRED "\e[1;91m"
#define BHGRN "\e[1;92m"
#define BHYEL "\e[1;93m"
#define BHBLU "\e[1;94m"
#define BHMAG "\e[1;95m"
#define BHCYN "\e[1;96m"
#define BHWHT "\e[1;97m"         
                  
#endif

