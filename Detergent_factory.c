#include "local.h"

int main(int argc, char **argv)
{
    unsigned int i=0,j=0;
    srand(time(0));
    if (argc != 2)
    {
        fprintf(stderr, "Input argument error !\n");
        exit(-1);
    }
    memset(config_file, '\0', 100);
    memcpy(config_file, argv[1], 100);

    /*Prepare the environment with all needed communication protocols */

    /* Prepare number of L1 employees */
    num_of_l1_emp_A=get_random_value(MIN_EMP_NUM_L1, MAX_EMP_NUM_L1);
    num_of_l1_emp_B=get_random_value(MIN_EMP_NUM_L1, MAX_EMP_NUM_L1);
    num_of_l3_emp=get_random_value(MIN_EMP_NUM_L3, MAX_EMP_NUM_L3);

    thread_configFile = (pthread_t)malloc(1* sizeof(pthread_t));
    /* Create threads for reading from config_file user-defined variables */
    if ((pthread_create(&thread_configFile, NULL, config_parser, NULL)) < 0)
    {
        fprintf(stderr, "Thread creation error !!!\n");
        exit(-1);
    }
    usleep(4000000);

    // assign values to dynamic array
    index_truckA_arr=(int *) malloc((truck_A_num)* sizeof(int));
    for (i=0 ; i < truck_A_num; i++)
    {
        index_truckA_arr[i]=i; // to give a number for each truck from Factory A
    }

    // assign values to dynamic array
    index_truckB_arr=(int *) malloc((truck_B_num)* sizeof(int));
    for (i=0; i < truck_B_num; i++)
    {
        index_truckB_arr[i]=i;
    }

    // assign values to dynamic array
    index_truckD_arr=(int *) malloc((truck_D_num)* sizeof(int));
    for (i=0; i < truck_D_num; i++)
    {
        index_truckB_arr[i]=i;
    }

    /*initialize mutexes arrays*/
    for ( i = 0; i < NUM_THREADS; i++)
        pthread_mutex_init(&lines_mutex[i], NULL);

    for ( i = 0; i < NUM_THREADS; i++)
        pthread_mutex_init(&queues_mutex[i], NULL);
        
    /* allocate 2D array that check the availability and arrival for trucks A */
    pthread_mutex_lock(&mutex_trucks_A_status);
    trucks_A_availability_arrival=(int*)malloc((truck_A_num*2)*sizeof(int));
    for(i=0; i<truck_A_num; i++)
    {
        for (j = 0; j < 2; j++)
        {
            trucks_A_availability_arrival[i*2+j]=0;
        }
    }
    pthread_mutex_unlock(&mutex_trucks_A_status);

    /* allocate 2D array that check the availability and arrival for trucks B */
    pthread_mutex_lock(&mutex_trucks_B_status);
    trucks_B_availability_arrival=(int*)malloc((truck_B_num*2)*sizeof(int));
    for(i=0; i<truck_B_num; i++)
    {
        for (j = 0; j < 2; j++)
        {
            trucks_B_availability_arrival[i*2+j]=0;
        }
    }
    pthread_mutex_unlock(&mutex_trucks_B_status);    

    /* Create the Threads needed to simulate the Working Factory */
    /* And assign them to their functions*/

    thread_manufacturing_lines=(pthread_t*)malloc(TOTAL_LINES * sizeof(pthread_t));
    /* Create threads for manufacturing_lines > 6 lines */
    for (i = 0; i < TOTAL_LINES ; i++)
    {
        if ((pthread_create(&thread_manufacturing_lines[i], NULL, manufacturing_lines, (void*)&line_index[i])) < 0)
        {
            fprintf(stderr, "Thread creation error !!!\n");
            exit(-1);
        }
    }

    thread_Factory_A = (pthread_t)malloc(1 * sizeof(pthread_t));
    /* Create thread for Factory A */
    if ((pthread_create(&thread_Factory_A, NULL, Factory_A, NULL)) < 0)
    {
        fprintf(stderr, "Thread creation error !!!\n");
        exit(-1);
    }

    thread_Factory_B = (pthread_t)malloc(1 * sizeof(pthread_t));
    /* Create thread for Factory B */
    if ((pthread_create(&thread_Factory_B, NULL, Factory_B, NULL)) < 0)
    {
        fprintf(stderr, "Thread creation error !!!\n");
        exit(-1);
    }

    thread_Storage_manager = (pthread_t)malloc(1 * sizeof(pthread_t));
    /* Create thread for Thread for Storage Manager */
    if ((pthread_create(&thread_Storage_manager, NULL, Storage_manager, NULL)) < 0)
    {
        fprintf(stderr, "Thread creation error !!!\n");
        exit(-1);
    }

    thread_trucks_A = (pthread_t*)malloc((truck_A_num) * sizeof(pthread_t));
    /* Create threads for trucks A*/
    for(i=0; i < truck_A_num; i++)
    {
        if((pthread_create(&thread_trucks_A[i], NULL,truck_A, (void*)&index_truckA_arr[i])) < 0)
        {
            fprintf(stderr, "Thread creation error !!!\n");
            exit(-1);
        }
    }

    thread_trucks_B = (pthread_t*)malloc((truck_B_num)* sizeof(pthread_t));
    /* Create threads for trucks B*/
    for(i=0; i < truck_B_num; i++)
    {
        if((pthread_create(&thread_trucks_B[i], NULL,truck_B, (void*)&index_truckB_arr[i])) < 0)
        {
            fprintf(stderr, "Thread creation error !!!\n");
            exit(-1);
        }
    }

    thread_trucks_D = (pthread_t*)malloc((truck_D_num)* sizeof(pthread_t));
    /* Create threads for trucks B*/
    for(i=0; i < truck_D_num; i++)
    {
        if((pthread_create(&thread_trucks_D[i], NULL,truck_D, (void*)&index_truckD_arr[i])) < 0)
        {
            fprintf(stderr, "Thread creation error !!!\n");
            exit(-1);
        }
    }


    thread_l2_employees = (pthread_t*)malloc((l2_employees)* sizeof(pthread_t));
    /* Create threads for L2 employees */
    for(i=0; i<l2_employees; i++)
    {
        if((pthread_create(&thread_l2_employees[i], NULL,l2_mission, NULL)) < 0)
        {
            fprintf(stderr, "Thread creation error !!!\n");
            exit(-1);
        }
    }

    /*Create L1 employees threads to get raw products from Factory A*/
    l1_thread_A=(pthread_t*)malloc(num_of_l1_emp_A * sizeof(pthread_t));
    /* Create threads for L1 A employees */
    for (i = 0; i < num_of_l1_emp_A  ; i++)
    {
        if ((pthread_create(&l1_thread_A[i], NULL, l1_A_mission, NULL)) < 0)
        {
            fprintf(stderr, "Thread creation error !!!\n");
            exit(-1);
        }
    }

    /* Create threads for L1 employees to get raw products from Factory B*/
    l1_thread_B=(pthread_t*)malloc(num_of_l1_emp_B * sizeof(pthread_t));
    /* Create threads for L1 B employees */
    for (i = 0; i < num_of_l1_emp_B  ; i++)
    {
        if ((pthread_create(&l1_thread_B[i], NULL, l1_B_mission, NULL)) < 0)
        {
            fprintf(stderr, "Thread creation error !!!\n");
            exit(-1);
        }
    }

    thread_console = (pthread_t)malloc(1 * sizeof(pthread_t));
    /* Create threads for Print on Console*/
    if((pthread_create(&thread_console, NULL, print_msg, NULL)) < 0)
    {
        fprintf(stderr, "Thread creation error !!!\n");
        exit(-1);
    }

    thread_l3_mission = (pthread_t*)malloc(num_of_l3_emp * sizeof(pthread_t));
    /* Create threads for ending the Simulation*/
    for (i = 0; i < num_of_l3_emp ; i++)
    {
		if((pthread_create(&thread_l3_mission[i], NULL, l3_mission, NULL)) < 0)
		{
		    fprintf(stderr, "Thread creation error !!!\n");
		    exit(-1);
		}
	}

    thread_endSimulation = (pthread_t)malloc(1 * sizeof(pthread_t));
    /* Create threads for ending the Simulation*/
    if((pthread_create(&thread_endSimulation, NULL, endSimulation, NULL)) < 0)
    {
        fprintf(stderr, "Thread creation error !!!\n");
        exit(-1);
    }
        clock_gettime(CLOCK_MONOTONIC, &start);
        


    /* _____________ Join all threads _____________ */
    for(i=0; i<TOTAL_LINES; i++)
    {
        pthread_join(thread_manufacturing_lines[i],NULL);
    }

    for(i=0; i<truck_A_num; i++)
    {
        pthread_join(thread_trucks_A[i],NULL);
    }

    for(i=0; i<truck_B_num; i++)
    {
        pthread_join(thread_trucks_B[i],NULL);
    }

    for(i=0; i<truck_D_num; i++)
    {
        pthread_join(thread_trucks_D[i],NULL);
    }
    
    pthread_join(thread_console,NULL);
    pthread_join(thread_Factory_A,NULL);
    pthread_join(thread_Factory_B,NULL);
    pthread_join(thread_configFile,NULL);
    pthread_join(thread_endSimulation,NULL);
    pthread_join(thread_Storage_manager,NULL);

    for(i=0; i<l2_employees; i++)
    {
        pthread_join(thread_l2_employees[i],NULL);
    }

    for(i=0; i<num_of_l1_emp_A; i++)
    {
        pthread_join(l1_thread_A[i],NULL);
    }

    for(i=0; i<num_of_l1_emp_B; i++)
    {
        pthread_join(l1_thread_B[i],NULL);
    }

	for(i=0; i<num_of_l3_emp; i++)
	{
		pthread_join(thread_l3_mission[i],NULL);
	}
    return 0;
}

//_____________________________________________________________
/*
  Function name : tech_mission
  Args : None
  Returns : None

  Description : A thread routine that simulates the tech_mission
*/
void *tech_mission(void *data)

{
    int empNum,index_of_line,order,time;

    int *arr = (int *)data;
    empNum=arr[0];
    index_of_line=arr[1];
    int * local_pass_array;

    if(index_of_line==1)
    {
        local_pass_array=steps_line_one;
    }
    else if (index_of_line==2)
    {
        local_pass_array=steps_line_two;
    }
    else if (index_of_line==3)
    {
        local_pass_array=steps_line_three;
    }
    else if (index_of_line==4)
    {
        local_pass_array=steps_line_four;
    }
    else if (index_of_line==5)
    {
        local_pass_array=steps_line_five;
    }
    while (true)
    {
        pthread_mutex_lock(&queues_mutex[index_of_line-1]);
        order=local_pass_array[empNum-2];// wait for a signal from the first emloyee to start passing
        pthread_mutex_unlock(&queues_mutex[index_of_line-1]);

        if(order > 0)
        {
            pthread_mutex_lock(&queues_mutex[index_of_line-1]);
            local_pass_array[empNum-2]--;
            pthread_mutex_unlock(&queues_mutex[index_of_line-1]);

            time=ceil((rand()%8)+2); // to indicate time work
            usleep(time*1000000);

            pthread_mutex_lock(&queues_mutex[index_of_line-1]);
            local_pass_array[empNum-1]++;
            pthread_mutex_unlock(&queues_mutex[index_of_line-1]);
        }
        
        sleep(3);
    }
}

//_____________________________________________________________
/*
  Function name : manufacturing_lines
  Args : None
  Returns : None

  Description : A thread routine that simulates the manufacturing lines. Each line creates 10 threads
  representing the employees from 1 to 10.
*/

void *manufacturing_lines(void *data)
{
    unsigned int index_of_line=*((int*)data);
    unsigned int p_thread = 0,i;
    pthread_t threadid[10];
    int passing_args[2];
    while (true)
    {
        /* Create threads for the 10 technical employees */
        p_thread = pthread_create(&threadid[0], NULL, first_emp,(void*)&index_of_line);
        if (p_thread < 0)
        {
            fprintf(stderr, "Thread creation error !\n");
            exit(-1);
        }
        for (i = 1; i < TOTAL_TECH-1; i++)
        {
            passing_args[0]=emp_index[i];
            passing_args[1]=index_of_line;
            p_thread = pthread_create(&threadid[i], NULL, tech_mission,(void*)passing_args);
            if (p_thread < 0)
            {
                fprintf(stderr, "Thread creation error !\n");
                exit(-1);
            }
            p_thread = pthread_create(&threadid[9], NULL, last_emp, (void*)&index_of_line);
            if (p_thread < 0)
            {
                fprintf(stderr, "Thread creation error !\n");
                exit(-1);
            }
            usleep(500000);
        }

        for (i = 0; i < TOTAL_TECH; i++)
        {
            pthread_join(threadid[i], NULL);
        }
        usleep(1000000);
        fflush(stdout);
    }
}

//_____________________________________________________________
/*
  Function name : Factory_A
  Args : None
  Returns : None

  Description : A thread routine that simulates the factory A
*/

void *Factory_A(void *data)
{
    unsigned int available_trucks_A=truck_A_num;
    unsigned int A_request,u,A_trucks_counter;

    pthread_mutex_lock(&storage_Area_fabA_mutex);
    /*Store initially products to fab A */
    storage_Area_fabA=fab_A_cartoon;
    pthread_mutex_unlock(&storage_Area_fabA_mutex);

    while (true)
    {
        pthread_mutex_lock(&mutex_request_A);
        A_request=request_order_A;
        pthread_mutex_unlock(&mutex_request_A);

        if(A_request == 1)
        {
            /* KEEP WORKING UNLESS THERE IS NO REQUESTS */
            pthread_mutex_lock(&order_amount_check_A);
            pthread_mutex_lock(&storage_Area_fabA_mutex);
            if(order_amount_A > storage_Area_fabA)
            {
                order_amount_A=storage_Area_fabA;
                if(storage_Area_fabA==0)
                {
                    flag_embty_catroon_A=1;
                }
                else
                {
                    flag_lessthan_required_factA=1;
                }
            }
            pthread_mutex_unlock(&storage_Area_fabA_mutex);


            if(order_amount_A > truck_a_max_capacity)
            {
                running_trucksA= ceil(order_amount_A/truck_a_max_capacity);
                // wait for available trucks, wait(travelTime) then recheck
                if( running_trucksA > available_trucks_A)
                {
                    running_trucksA= available_trucks_A;
                }
            }
            else
            {
                running_trucksA=1;
            }
            pthread_mutex_unlock(&order_amount_check_A);

            pthread_mutex_lock(&mutex_trucks_A_status);
            /* loop on truck A num (runningtrucks) times */
            for(u = 0; u < truck_A_num ; u++)
            {
                if(A_trucks_counter < running_trucksA )
                {
                    if(trucks_A_availability_arrival[2*u]==0)
                    {
                        trucks_A_availability_arrival[2*u]=1;
                    }
                    A_trucks_counter++;
                }
                else
                {
                    break;
                    A_trucks_counter=0;

                }
            }
            pthread_mutex_unlock(&mutex_trucks_A_status);

            pthread_mutex_lock(&mutex_request_A);
            request_order_A=2; // to prevent exeuting same order
            pthread_mutex_unlock(&mutex_request_A);

            usleep(1000000);
        }

        /*Reset the flags*/
        flag_embty_catroon_A=0;
        flag_lessthan_required_factA=0;

        /* Produce Process */
        pthread_mutex_lock(&storage_Area_fabA_mutex);
        storage_Area_fabA++;
        pthread_mutex_unlock(&storage_Area_fabA_mutex);
        usleep(4000000);
    }
}

//_____________________________________________________________
/*
  Function name : Factory_B
  Args : None
  Returns : None

  Description : A thread routine that simulates the factory B
*/

void *Factory_B(void *data)
{
    unsigned int available_trucks_B=truck_B_num;
    unsigned int B_request, u,B_trucks_counter;

    pthread_mutex_lock(&storage_Area_fabB_mutex);
    /* Store initially products to fab B */
    storage_Area_fabB=fab_B_cartoon;
    pthread_mutex_unlock(&storage_Area_fabB_mutex);

    while (true)
    {
        pthread_mutex_lock(&mutex_request_B);
        B_request=request_order_B;
        pthread_mutex_unlock(&mutex_request_B);

        if(B_request == 1 )
        {
            /* KEEP WORKING UNLESS THERE IS NO REQUESTS */
            pthread_mutex_lock(&order_amount_check_B);
            pthread_mutex_lock(&storage_Area_fabB_mutex);
            if(order_amount_B > storage_Area_fabB)
            {
                order_amount_B=storage_Area_fabB;
                if(storage_Area_fabB==0)
                {
                    flag_embty_catroon_B=1;
                }
                else
                {
                    flag_lessthan_required_factB=1;
                }
            }
            pthread_mutex_unlock(&storage_Area_fabB_mutex);

            if(order_amount_B > truck_b_max_capacity)
            {
                running_trucksB= ceil(order_amount_B/truck_b_max_capacity);
                // wait for available trucks, wait(travelTime) then recheck
                if( running_trucksB > available_trucks_B)
                {
                    running_trucksB= available_trucks_B;
                }
            }
            else
            {
                running_trucksB = 1;
            }
            pthread_mutex_unlock(&order_amount_check_B);
            pthread_mutex_lock(&mutex_trucks_B_status);

            /* loop on truck B num (runningtrucks) times */
            for(u = 0; u < truck_B_num ; u++)
            {
                if(B_trucks_counter < running_trucksB )
                {
                    if(trucks_B_availability_arrival[2*u]==0)
                    {
                        trucks_B_availability_arrival[2*u]=1;
                    }
                    B_trucks_counter++;
                }
                else
                {
                    break;
                    B_trucks_counter=0;

                }
            }
            pthread_mutex_unlock(&mutex_trucks_B_status);

            pthread_mutex_lock(&mutex_request_B);
            request_order_B = 2; // to prevent exeuting same order
            pthread_mutex_unlock(&mutex_request_B);


            usleep(1000000);

        }

        /*Reset the flags*/
        flag_embty_catroon_B=0;
        flag_lessthan_required_factB=0;

        pthread_mutex_lock(&storage_Area_fabB_mutex);
        storage_Area_fabB++;
        pthread_mutex_unlock(&storage_Area_fabB_mutex);
        usleep(4000000);
    }
}

//_____________________________________________________________
/*
  Function name : Storage_manager
  Args : None
  Returns : None
  Description : A thread routine that simulates the Storage_manager
*/

void *Storage_manager(void *data)
{
    unsigned int request_a,
             request_b,
             store_storage_A_products,
             store_storage_B_products;

    while (true)
    {
        pthread_mutex_lock(&mutex_request_A);
        request_a=request_order_A;
        pthread_mutex_unlock(&mutex_request_A);

        pthread_mutex_lock(&storage_productA_mutex);
        store_storage_A_products=storage_A_products;
        pthread_mutex_unlock(&storage_productA_mutex);

        if((store_storage_A_products <= storage_area_min) && (request_a==0))
        {
            pthread_mutex_lock(&storage_productB_mutex);
            store_storage_B_products=storage_B_products;
            pthread_mutex_unlock(&storage_productB_mutex);


            if(store_storage_B_products > (2*store_storage_A_products))
            {
                pthread_mutex_lock(&order_amount_check_A);
                order_amount_A = 2*storage_B_products;

                /* load the remaining amount */
                if(order_amount_A > storage_area_max_A)
                {
                    order_amount_A=storage_area_max_A;
                }
                pthread_mutex_unlock(&order_amount_check_A);
            }
            else
            {
                pthread_mutex_lock(&order_amount_check_A);
                order_amount_A = storage_area_max_A;
                pthread_mutex_unlock(&order_amount_check_A);
            }

            pthread_mutex_lock(&mutex_request_A);
            request_order_A=1;
            pthread_mutex_unlock(&mutex_request_A);
        }

        pthread_mutex_lock(&mutex_request_B);
        request_b=request_order_B;
        pthread_mutex_unlock(&mutex_request_B);

        pthread_mutex_lock(&storage_productB_mutex);
        store_storage_B_products=storage_B_products;
        pthread_mutex_unlock(&storage_productB_mutex);

        if((store_storage_B_products <= storage_area_min) && (request_b==0))
        {
            pthread_mutex_lock(&order_amount_check_B);
            order_amount_B = storage_area_max_B;
            pthread_mutex_unlock(&order_amount_check_B);

            pthread_mutex_lock(&mutex_request_B);
            request_order_B=1;
            pthread_mutex_unlock(&mutex_request_B);
        }
        usleep(1000000);
    }
}

//_____________________________________________________________
/*
  Function name : truck_A
  Args : None
  Returns : None

  Description : A thread routine that simulates the truck_A
*/

void *truck_A(void *data)
{
    int i = *((int*)data);   /* thread identifying number */
    int status;

    while (true)
    {
        pthread_mutex_lock(&mutex_trucks_A_status);
        status=trucks_A_availability_arrival[i*2];
        pthread_mutex_unlock(&mutex_trucks_A_status);

        if(status==1)
        {
            truck_A_status = TRUCK_WAITING;
            usleep(5000000);

            pthread_mutex_lock(&storage_Area_fabA_mutex);
            storage_Area_fabA-=truck_a_max_capacity;
            pthread_mutex_unlock(&storage_Area_fabA_mutex);

            truck_A_status = TRUCK_LOADING;	// Truck is in loading state.
            usleep(4000000);
            truck_A_status = TRUCK_ON_TRIP; // Truck is in trip state

            //for going and returning
            usleep((get_random_value(TRUCK_TRIP_DELAY_MIN,TRUCK_TRIP_DELAY_MAX))*1000000);
            pthread_mutex_lock(&mutex_trucks_A_status);
            //ensure the arrival of truck A
            trucks_A_availability_arrival[i*2]=0;
            trucks_A_availability_arrival[i*2+1]=1;
            pthread_mutex_unlock(&mutex_trucks_A_status);
            truck_A_status = TRUCK_WAITING;
        }

        usleep(500000);

    }
}

//_____________________________________________________________
/*
  Function name : truck_B
  Args : None
  Returns : None

  Description : A thread routine that simulates the truck_B
*/

void *truck_B(void *data)
{
    int i = *((int*)data);   /* thread identifying number */
    int status;
    while (true)
    {
        pthread_mutex_lock(&mutex_trucks_B_status);
        status=trucks_B_availability_arrival[i*2];
        pthread_mutex_unlock(&mutex_trucks_B_status);

        if(status==1)
        {

            truck_B_status = TRUCK_WAITING;
            usleep(5000000);

            pthread_mutex_lock(&storage_Area_fabB_mutex);
            storage_Area_fabB-=truck_b_max_capacity;
            pthread_mutex_unlock(&storage_Area_fabB_mutex);

            truck_B_status = TRUCK_LOADING;	// Truck is in loading state.
            usleep(4000000);
            truck_B_status = TRUCK_ON_TRIP; // Truck is in trip state

            //for going and returning
            usleep((get_random_value(TRUCK_TRIP_DELAY_MIN,TRUCK_TRIP_DELAY_MAX))*1000000);
            pthread_mutex_lock(&mutex_trucks_B_status);
            //ensure the arrival of truck B
            trucks_B_availability_arrival[i*2]=0;
            trucks_B_availability_arrival[i*2+1]=1;
            pthread_mutex_unlock(&mutex_trucks_B_status);
            truck_B_status = TRUCK_WAITING;
        }
        usleep(500000);
    }
}

//_____________________________________________________________
/*
  Function name : l2_mission
  Args : None
  Returns : None
  Description : A thread routine that simulates the l2_mission that checks the capacity of S2 and if it is full stopped active lines via putting "available->0" and decide when to make lines start producing
*/

void *l2_mission(void *data)
{

    unsigned int S2_capacity,i,num_activation_lines,pA,pB,thresh1,thresh2,time,line_numA,line_numB;
    while (true)
    {
        /*pthread_mutex_lock(&storage_productA_mutex);
        storage_A_products;
        pthread_mutex_unlock(&storage_productA_mutex);
        */
        pthread_mutex_lock(&storage_two_final_products_mutex);
        S2_capacity=S2_final_products;
        pthread_mutex_unlock(&storage_two_final_products_mutex);

        if(S2_capacity>num_detergent_threshold)
        {
            pthread_mutex_lock(&available_products_mutex);
            for(i=0; i<TOTAL_LINES; i--)
            {
                available_products[i]=0;// force activation lines from producing new products
            }
            pthread_mutex_unlock(&available_products_mutex);

        }
        else
        {
            /*Check the current amount of both products A and B*/
            pthread_mutex_lock(&storage_productA_mutex);
            pA=storage_A_products;
            pthread_mutex_unlock(&storage_productA_mutex);

            pthread_mutex_lock(&storage_productB_mutex);
            pB=storage_B_products;
            pthread_mutex_unlock(&storage_productB_mutex);

            if(pA>=pB)
            {
                pA/=2;
                line_numA=(int)(ceil(pA/TOTAL_LINES));
                line_numB=(int)(ceil(pB/TOTAL_LINES));
                thresh1= floor(line_numA/TOTAL_LINES);
                thresh2= floor(line_numB/TOTAL_LINES);

                num_activation_lines=min(thresh1,thresh2);
                
                time=ceil((rand()%storage_employee_delay)+1); // to indicate time for loading products into the lines
                usleep(time*1000000);

                pthread_mutex_lock(&available_products_mutex);
                for(i=0; i<num_activation_lines; i++)
                {
                    available_products[i]=1;//activate lines to start work on the final products
                }
                pthread_mutex_unlock(&available_products_mutex);


            }

        }
       usleep(4000000);
    }

}
//_____________________________________________________________
/*
  Function name : l3_mission
  Args : None
  Returns : None
  Description : A thread routine that simulates the l3_mission that checks the number of products for each line and compare it with  certain threashold then adding them to S2
*/

void *l3_mission(void *data)
{
    unsigned int i,product_num,time;
    while (true)
    {
        for(i=0; i<TOTAL_LINES; i++)
        {
            pthread_mutex_lock(&lines_mutex[i]);
            product_num=products[i];

            pthread_mutex_unlock(&lines_mutex[i]);

            if(product_num>=LINE_PRODUCTS_THREASHOLD)
            {

                pthread_mutex_lock(&lines_mutex[i]);
                products[i]-=product_num;
                pthread_mutex_unlock(&lines_mutex[i]);

                //to indicate time for uploading the products from lines to the Storage2 room
                time=ceil((rand()%storage_employee_delay)+1);
                usleep(time*1000000);

                pthread_mutex_lock(&storage_two_final_products_mutex);
                S2_final_products+=product_num;
                pthread_mutex_unlock(&storage_two_final_products_mutex);

                pthread_mutex_lock(&total_detergent_products_mutex);
                total_detergent_products+=product_num;
                pthread_mutex_unlock(&total_detergent_products_mutex);

            }

        }
        usleep(4000000);
    }
}

//_____________________________________________________________
/*
  Function name : l1_A_mission
  Args : None
  Returns : None

  Description : A thread routine that simulates the mission of the L1 employee for factory A
*/
void*  l1_A_mission(void *data)
{
    unsigned int status_arrival, status_availability, i, num_active_l1,counter_l1_A;

    while(true)
    {
        for(i=0; i < truck_A_num; i++)
        {
            pthread_mutex_lock(&mutex_trucks_A_status);
            status_availability=trucks_A_availability_arrival[i*2];
            status_arrival=trucks_A_availability_arrival[2*i+1];
            pthread_mutex_unlock(&mutex_trucks_A_status);

            if((status_availability==0) && (status_arrival==1))
            {
                pthread_mutex_lock(&order_amount_check_A);
                num_active_l1=ceil((order_amount_A/ CAPACITY_L1) % num_of_l1_emp_A);
                pthread_mutex_unlock(&order_amount_check_A);

                pthread_mutex_lock(&counter_l1_A_mutex);
                counter_active_l1_A++;
                pthread_mutex_unlock(&counter_l1_A_mutex);

                pthread_mutex_lock(&counter_l1_A_mutex);
                counter_l1_A=counter_active_l1_A;
                pthread_mutex_unlock(&counter_l1_A_mutex);

                if(counter_l1_A == num_active_l1)
                {
                    pthread_mutex_lock(&storage_productA_mutex);//storage_A_products
                    // deliver the whole order to storage
                    storage_A_products+=truck_a_max_capacity;
                    pthread_mutex_unlock(&storage_productA_mutex);//storage_A_products

                    pthread_mutex_lock(&mutex_request_A);
                    request_order_A=0; // acknowlagement
                    pthread_mutex_unlock(&mutex_request_A);

                    pthread_mutex_lock(&mutex_trucks_A_status);
                    trucks_A_availability_arrival[2*i+1]=0;
                    pthread_mutex_unlock(&mutex_trucks_A_status);

                    pthread_mutex_lock(&counter_l1_A_mutex);
                    counter_active_l1_A=0;
                    pthread_mutex_unlock(&counter_l1_A_mutex);

                    usleep(3000000);
                }

            }
        }
        usleep(5000000);
    }
}

//_____________________________________________________________
/*
  Function name : l1_B_mission
  Args : None
  Returns : None

  Description : A thread routine that simulates the mission of the L1 employee for factory B
*/
void*  l1_B_mission(void *data)
{
    unsigned int status_arrival, status_availability, i, num_active_l1,counter_l1_B;

    while(true)
    {
        for(i=0; i < truck_B_num; i++)
        {
            pthread_mutex_lock(&mutex_trucks_B_status);
            status_availability=trucks_B_availability_arrival[i*2];
            status_arrival=trucks_B_availability_arrival[2*i+1];
            pthread_mutex_unlock(&mutex_trucks_B_status);

            if((status_availability==0) && (status_arrival==1))
            {
                pthread_mutex_lock(&order_amount_check_B);
                num_active_l1=ceil((order_amount_B/ CAPACITY_L1) % num_of_l1_emp_B);
                pthread_mutex_unlock(&order_amount_check_B);

                pthread_mutex_lock(&counter_l1_B_mutex);
                counter_active_l1_B++;
                pthread_mutex_unlock(&counter_l1_B_mutex);

                pthread_mutex_lock(&counter_l1_B_mutex);
                counter_l1_B = counter_active_l1_B;
                pthread_mutex_unlock(&counter_l1_B_mutex);
                
                if(counter_l1_B == num_active_l1)
                {
                    pthread_mutex_lock(&storage_productB_mutex);//storage_B_products
                    // deliver truck b max capacity to storage
                    storage_B_products+=truck_b_max_capacity;
                    pthread_mutex_unlock(&storage_productB_mutex);//storage_B_products

                    pthread_mutex_lock(&mutex_request_B);
                    request_order_B=0; // acknowledgement
                    pthread_mutex_unlock(&mutex_request_B);

                    pthread_mutex_lock(&mutex_trucks_B_status);
                    trucks_B_availability_arrival[2*i+1]=0;
                    pthread_mutex_unlock(&mutex_trucks_B_status);

                    pthread_mutex_lock(&counter_l1_B_mutex);
                    counter_active_l1_B=0;
                    pthread_mutex_unlock(&counter_l1_B_mutex);

                    usleep(3000000);

                }

            }
        }
        usleep(5000000);
    }
}

//_____________________________________________________________
/*
  Function name : get_random_value
  Args : int, int
  Return value : int

  Description : Function to generate a random integer between a lower and upper value. This function is used
  to introduce delay during manufacturing steps.
*/

int get_random_value(int lower, int upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}


//_____________________________________________________________
/*
  Function name : get_two_values
  Args : int, int
  Return value : min value between two numbers

  Description : Function to return the min value between two numbers
*/

int min(int first, int second)
{
    unsigned int result;
    if(first<second)
    {
        result=first;
    }
    else
    {
        result=second;
    }
    return result;
}

//_____________________________________________________________
/*
  Function name : print_msg
  Args : None
  Returns : None
  Description : It is a thread routine that prints the information to the user.
*/

void *print_msg(void *data)
{
	unsigned int i;
    while (true)
    {
        system("clear");
        pthread_mutex_lock(&console_mutex);
        fprintf(stdout, "\n👋🏼 %s Welcome to Detergent_factory Simulation 🧫 🧪\n",BHBLU);
        

 		// Print Product 
		printf(" _______\n");
		for(int i=0; i<5; i++)
		{
		    if(i==0)
		    {
		        printf("|");
		        printf("\t|\t");
		        printf("%s\t ⚠️ WARNING \n",BHRED );
		        printf("%s", ORIGINAL);
                	        
		    }
		    else if(i==1)
		    {
		        printf("|");
		        printf("\t|\n");
		    }
		    else if(i==2)
		    {
		        printf("|  🇵🇸");
		        printf("\t|\t");
		        printf("%s 🚷☣️ LEAD STORAGE AREA\n",BHRED );
		        printf("%s", ORIGINAL);
		    }
		    else if(i==3)
		    {
		        printf("|");
		        printf("\t|\t");
		        printf("%s 🏴‍☠️   LEAD IS TOXIC\n",BHRED);
		        printf("%s", ORIGINAL);
		    }
		    else if(i==4)
		    {
		        printf("|");
		        printf("_______|\t");
		        printf("%s ❌🍔🍕🍟🌭 NO EATING\n",BHRED);
		        printf("%s", ORIGINAL);
		        printf("|");
		        printf("%s", GREEN_COLOR);
		        printf("Cleano |\t");
		        printf("%s🚱🚭 NO DRINKING OR SMOKING \n",BHRED);	
		        printf("%s", ORIGINAL);
		        printf("|_______|\t");
		        printf("%s🧼🧻WASH HANDS AFTER HANDELING\n",BHGRN);
		        printf("%s", ORIGINAL);
		        printf("|\t|\n");

		        printf("|__📛___|\n");
		    }
		}
		printf("___________________________________________________________________________\n");
    
        pthread_mutex_lock(&storage_Area_fabA_mutex);
        if( storage_Area_fabA > THRESHOLD)
        {
        fprintf(stdout, "\n 🟢 Factory A is Producing\n");
        }
        else
        {
        fprintf(stdout, "\n 🟡 Factory A is Selling\n");
        }
        pthread_mutex_unlock(&storage_Area_fabA_mutex);


        pthread_mutex_lock(&storage_Area_fabB_mutex);
		if( storage_Area_fabB > THRESHOLD)
	    {
		    fprintf(stdout, "\n 🟢 Factory B is Producing\n");
	    }
	    else
	    {
		    fprintf(stdout, "\n 🟡 Factory B is Selling\n");
	    }
        pthread_mutex_unlock(&storage_Area_fabB_mutex);



        pthread_mutex_lock(&storage_productA_mutex);
        fprintf(stdout, "\n 🕵🏼💰📈 Number of Products A in our factory : %d 📦\n",  storage_A_products);
        pthread_mutex_unlock(&storage_productA_mutex);



        pthread_mutex_lock(&storage_productB_mutex);
        fprintf(stdout, "\n 🕵🏼💰📈 Number of Products B in our factory : %d 📦\n",  storage_B_products);
        pthread_mutex_unlock(&storage_productB_mutex);




        if (truck_A_status == TRUCK_WAITING)
        {
            fprintf(stdout, "\n🚚 TRUCKS_A STATUS : WAITING 🕜⌛️ !!!\n");
        }
        else if (truck_A_status == TRUCK_LOADING)
        {
            fprintf(stdout, "\n🚚 TRUCKS_A STATUS : LOADING RAW PRODUCTS 🟡 !!!\n");
        }
        else
        {
            fprintf(stdout, "\n🚚 TRUCKS_A STATUS : ON TRIP 🚚🚚 !!!\n");
        }

        if (truck_B_status == TRUCK_WAITING)
        {
            fprintf(stdout, "\n🚛 TRUCKS_B STATUS : WAITING 🕜⌛️!!!\n");
        }
        else if (truck_B_status == TRUCK_LOADING)
        {
            fprintf(stdout, "\n🚛 TRUCKS_B STATUS : LOADING RAW PRODUCTS 🟡 !!!\n");
        }
        else
        {
            fprintf(stdout, "\n🚛 TRUCKS_B STATUS : ON TRIP 🚛🚛 !!!\n");
        }

        pthread_mutex_lock(&mutex_request_A);
        if (request_order_A==1)
        {
            fflush(stdout);
            fprintf(stdout, "\n🔴⏳ WE NEED RAW MATERIAL FROM FACTORY A !!!\n");
        }
        pthread_mutex_unlock(&mutex_request_A);

        pthread_mutex_lock(&mutex_request_B);
        if (request_order_B==1)
        {
            fprintf(stdout, "\n🔴⏳ WE NEED RAW MATERIAL FROM FACTORY B !!!\n");
        }
        pthread_mutex_unlock(&mutex_request_B);

        pthread_mutex_lock(&mutex_request_A);
        if(request_order_A ==0)
        {
            fprintf(stdout, "\n 📢 POSSIBLE REQUEST ORDER FROM FACTORY A \n");
        }
        else if(request_order_A ==1)
        {
            fprintf(stdout, "\n 📢 NO REQUEST ORDER FROM FACTORY A\n");
        }
        else
        {
            fprintf(stdout, "\n 📢 WAITING FOR ACKNOWLEDGEMENT \n");
        }
        pthread_mutex_unlock(&mutex_request_A);

        pthread_mutex_lock(&mutex_request_B);
        if(request_order_B ==0)
        {
            fprintf(stdout, "\n 📢 POSSIBLE REQUEST ORDER FROM FACTORY B \n");
        }
        else if(request_order_B ==1)
        {
            fprintf(stdout, "\n 📢 NO REQUEST ORDER FROM FACTORY B \n");
        }
        else
        {
            fprintf(stdout, "\n 📢 WAITING FOR ACKNOWLEDGEMENT \n");
        }
        pthread_mutex_unlock(&mutex_request_B);
        
       
        /*pthread_mutex_lock(&storage_two_final_products_mutex);
        fprintf(stdout, "\n 👨🏼‍🔧 L3 EMPLOYEES ADDED PRODUCTS TO S2 , ITS CAPACITY BECOME >> \t %d\n",S2_final_products);
        pthread_mutex_unlock(&storage_two_final_products_mutex);*/
        
		if (truck_D_status == TRUCK_LOADING)
        {
            fprintf(stdout, "\n 🚐 TRUCKS_D STATUS : LOADING DETERGENT FACTORY PRODUCTS 🟢 !!!\n");
        }
        else if(truck_D_status == TRUCK_ON_TRIP)
        {
            fprintf(stdout, "\n 🚐 TRUCKS_D STATUS : ON TRIP  !!!\n");
        }
        else
        {
            fprintf(stdout, "\n 🚐 TRUCKS_D STATUS : WAITING  !!!\n");
        }

        pthread_mutex_lock(&total_detergent_products_mutex);
        fprintf(stdout, "\n 📦 TOTAL DETERGENT PRODUCTS >> \t %d\n",total_detergent_products);
   		pthread_mutex_unlock(&total_detergent_products_mutex);
   		

        if(flag_embty_catroon_A)
        {
            fprintf(stdout,"\n Sorry at the moment there is no available products in our Factory_A \n");
        }

        if(flag_embty_catroon_B)
        {
            fprintf(stdout,"\n Sorry at the moment there is no available products in our Factory_B \n");
        }

        if(flag_lessthan_required_factA)
        {
            fprintf(stdout,"\n Sorry we have provided you the available products in our Factory_A \n");
        }

        if(flag_lessthan_required_factB)
        {
            fprintf(stdout,"\n Sorry we have provided you the available products in our Factory_B\n");
        }        

        pthread_mutex_unlock(&console_mutex);

//***________________________________________________________***
		if (endSimulation_flag) 
		{
					fprintf(stdout, "\n\t\t\t 🔴🔴😔 SIMULATION ENDS 😔🔴🔴 \n");

					for(i=0; i<TOTAL_LINES; i++)
					{
		        if (pthread_cancel(thread_manufacturing_lines[i]) != 0)
		        {
		            perror("Failed to cancel manufacturing lines thread\n");
		        }
					}

	    		for(i=0; i<truck_A_num; i++)
					{
		        if (pthread_cancel(thread_trucks_A[i]) != 0)
						{
		            perror("Failed to cancel Factory A trucks thread\n");
		        }
					}

	    		for(i=0; i<truck_B_num; i++)
					{
		        if (pthread_cancel(thread_trucks_B[i]) != 0)
						{
		            perror("Failed to cancel Factory B trucks thread\n");
		        }
					}

					for(i=0; i<truck_D_num; i++)
					{
		        if (pthread_cancel(thread_trucks_D[i]) != 0)
						{
		            perror("Failed to cancel Factory D trucks thread\n");
						}
					}

					for(i=0; i<l2_employees; i++)
					{
			      if (pthread_cancel(thread_l2_employees[i]) != 0)
						{
			          perror("Failed to cancel l2 mission thread\n");
						}
					}

		   		for(i=0; i<num_of_l3_emp; i++)
		  	 	{
						if(pthread_cancel(thread_l3_mission[i]) != 0)
						{
			    		perror("Failed to cancel l3 mission thread\n");
						}
			 		}

    			for(i=0; i<num_of_l1_emp_A; i++)
					{
	      		if (pthread_cancel(l1_thread_A[i]) != 0)
						{
		    			perror("Failed to cancel l1 mission truck A thread\n");
						}
					}

    			for(i=0; i<num_of_l1_emp_B; i++)
					{
	      		if (pthread_cancel(l1_thread_B[i]) != 0)
						{
		    			perror("Failed to cancel l1 mission truck B thread\n");
						}
					}

      		if (pthread_cancel(thread_console) != 0)
					{
	    			perror("Failed to cancel Console thread\n");
					}

				  if (pthread_cancel(thread_Factory_A) != 0)
					{
						perror("Failed to cancel Factory A thread\n");
					}

				  if (pthread_cancel(thread_Factory_B) != 0)
					{
						perror("Failed to cancel Factory B thread\n");
					}

				  if (pthread_cancel(thread_configFile) != 0)
					{
						perror("Failed to cancel config File thread\n");
					}

				  if (pthread_cancel(thread_endSimulation) != 0)
					{
						perror("Failed to cancel end Simulation thread\n");
					}

				  if (pthread_cancel(thread_Storage_manager) != 0)
					{
						perror("Failed to cancel Storage manager thread\n");
					}

					pthread_mutex_destroy(&console_mutex); 
					pthread_mutex_destroy(&order_A_factory);
					pthread_mutex_destroy(&order_B_factory);
					pthread_mutex_destroy(&mutex_request_A); 
					pthread_mutex_destroy(&mutex_request_B);
					pthread_mutex_destroy(&order_amount_check_A);                         
					pthread_mutex_destroy(&order_amount_check_B );
					pthread_mutex_destroy(&mutex_trucks_A_status);  
					pthread_mutex_destroy(&mutex_trucks_B_status); 
					pthread_mutex_destroy(&storage_productA_mutex);  
					pthread_mutex_destroy(&storage_productB_mutex);
					pthread_mutex_destroy(&storage_Area_fabA_mutex);  
					pthread_mutex_destroy(&storage_Area_fabB_mutex); 
					pthread_mutex_destroy(&counter_l1_A_mutex);
					pthread_mutex_destroy(&counter_l1_B_mutex);
					pthread_mutex_destroy(&storage_two_final_products_mutex); 
					pthread_mutex_destroy(&available_products_mutex);
					pthread_mutex_destroy(&total_detergent_products_mutex); 

					for(i = 0 ; i < NUM_THREADS; i++)
					{
					pthread_mutex_destroy(&lines_mutex[i]);
					pthread_mutex_destroy(&queues_mutex[i]);
					}        
					exit(0);
			}
			
        usleep(1500000);

    }
}
//_____________________________________________________________
/*
  Function name : first_emp
  Args : None
  Returns : None
  Description :
*/
void* first_emp(void *data)
{
    unsigned int flag,time,empNum,index_of_line;
    empNum=1;
    index_of_line=*((int*)data);
    int * local_pass_array;

    if(index_of_line==1)
    {
        local_pass_array=steps_line_one;
    }
    else if (index_of_line==2)
    {
        local_pass_array=steps_line_two;
    }
    else if (index_of_line==3)
    {
        local_pass_array=steps_line_three;
    }
    else if (index_of_line==4)
    {
        local_pass_array=steps_line_four;
    }
    else if (index_of_line==5)
    {
        local_pass_array=steps_line_five;
    }

    while(true)
    {
        pthread_mutex_lock(&available_products_mutex);
        flag=available_products[index_of_line-1];// wait for a signal from L2 emloyee to start producing
        pthread_mutex_unlock(&available_products_mutex);
        if(flag)
        {
            pthread_mutex_lock(&storage_productA_mutex);
            storage_A_products-=2;
            pthread_mutex_unlock(&storage_productA_mutex);

            pthread_mutex_lock(&storage_productB_mutex);
            storage_B_products-=1;
            pthread_mutex_unlock(&storage_productB_mutex);


            pthread_mutex_lock(&available_products_mutex);
            available_products[index_of_line-1]=0;      //set available to 0 to let first employee responds to new orders
            pthread_mutex_unlock(&available_products_mutex);

            time=ceil((rand()%10)+2); // to indicate time work
            usleep(time*1000000);

            pthread_mutex_lock(&queues_mutex[index_of_line-1]);
            local_pass_array[empNum-1]=1;
            pthread_mutex_unlock(&queues_mutex[index_of_line-1]);
        }
    }
}


//_____________________________________________________________
/*
  Function name : last_emp
  Args : None
  Returns : None
  Description :
*/

void* last_emp(void *data)
{
    int empNum,index_of_line,order,time;
    empNum=10;
    index_of_line=*((int*)data);
    int * local_pass_array;

    if(index_of_line==1)
    {
        local_pass_array=steps_line_one;
    }
    else if (index_of_line==2)
    {
        local_pass_array=steps_line_two;
    }
    else if (index_of_line==3)
    {
        local_pass_array=steps_line_three;
    }
    else if (index_of_line==4)
    {
        local_pass_array=steps_line_four;
    }
    else if (index_of_line==5)
    {
        local_pass_array=steps_line_five;
    }

    while(true)
    {
        pthread_mutex_lock(&queues_mutex[index_of_line-1]);
        order=local_pass_array[empNum-2];
        pthread_mutex_unlock(&queues_mutex[index_of_line-1]);
        if (order>0)
        {
            pthread_mutex_lock(&queues_mutex[index_of_line-1]);
            local_pass_array[empNum-2]--;
            pthread_mutex_unlock(&queues_mutex[index_of_line-1]);

            time=ceil((rand()%12)+2); // to indicate time work for packaging the product
            usleep(time*1000000);

            /* new product is generated */
            pthread_mutex_lock(&lines_mutex[index_of_line-1]);
            products[index_of_line-1]++;
            pthread_mutex_unlock(&lines_mutex[index_of_line-1]);

        }
    }
}

//_____________________________________________________________
/*
  Function name : truck_D
  Args:None
  Returns:None
  Description : A thread routine that simulates the truck_D that exports the final products to different supermarkets
*/

void *truck_D(void *data)
{
    unsigned int index,S2_capacity;
    index=*((int*)data);
    while(true)
    {
        pthread_mutex_lock(&storage_two_final_products_mutex);
        S2_capacity=S2_final_products;
        pthread_mutex_unlock(&storage_two_final_products_mutex);
        
        if(S2_capacity>=num_detergent_threshold)
        {
            truck_D_status = TRUCK_LOADING; // Truck is in trip state
            sleep(2);
            pthread_mutex_lock(&storage_two_final_products_mutex);
            S2_final_products-=TRUCK_D_CAPACITY;
            pthread_mutex_unlock(&storage_two_final_products_mutex);

            /*Trip delay time of distributing produced products to shops */
            usleep((truck_trip_delay)*1000000);
            truck_D_status = TRUCK_ON_TRIP; // Truck is in trip state 
        }
       truck_D_status = TRUCK_WAITING; // Truck is in trip state
    }
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Termination >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//_____________________________________________________________
/*
  Function name : endSimulation
  Args : None
  Returns : None
  Description : It is a thread routine that ends the Simulation.
  >> if The factory has manufactured a number of detergent products that exceeds a threshold.
  >> The simulation has been running for more than a user-defined number of minutes.
*/

void *endSimulation(void *data)
{
	double elapsed,finish_minutes,start_minutes;
	unsigned int i;
    while (true)
    {
      pthread_mutex_lock(&total_detergent_products_mutex);
      if(total_detergent_products == total_detergent_products_threshold)
			{
				endSimulation_flag = true; // Checks if manufactured number of detergent products exceeds a threshold
			}
	pthread_mutex_unlock(&total_detergent_products_mutex);
	clock_gettime(CLOCK_MONOTONIC, &finish);

      elapsed = (finish.tv_sec - start.tv_sec) ;
      elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	  if((elapsed/60) >= time_threshold)
	  {
		endSimulation_flag = true; // Checks if the simulation has been running for more than a user-defined number of minutes.
	 }
    }
}

//_____________________________________________________________
/*
  Function name : config_parser
  Args : file name as pointer
  Returns : None

  Description : This is a thread routine to read the config file and update the corresponding parameters.The
  thread reads the file for every 5 seconds and updated the parameter. Hence, even if the value is changed in
  the file during runtime it gets updated.
*/

void *config_parser(void *data)
{
    while (true)
    {
        FILE *fp;
        fp = fopen(config_file, "r");

        char *numOf_truck_A="TRUCK_A_NUM=",
             *numOf_truck_B="TRUCK_B_NUM=",
             *A_cartoon="FAB_A_CARTOON=",
             *B_cartoon="FAB_B_CARTOON=",
             *T_threshold="MINUTES_TO_END_SIMULATION=",
             *truck_delay="TRUCK_TRIP_DELAY=",
             *storage_min="MIN_THRESHOLD_STORAGE_ROOM_CAPACITY=",
             *storage_max_A="STORAGE_AREA_MAX_A=",
             *storage_max_B="STORAGE_AREA_MAX_B=",
             *truck_A_max_cap="TRUCK_A_MAX_CAPACITY=",
             *truck_B_max_cap="TRUCK_B_MAX_CAPACITY=",
             *l2_emp="L2_EMPLOYEES=",
             *storage_emp_delay="STORAGE_EMPLOYEE_ABSENCE=",
             *det_threshold="NUM_DETERGENT_THRESHOLD=",
             *truck_detergent_num="TRUCK_D_NUM=",
             *total_detergent_products_ths="TOTAL_DETERGENT_PRODUCTS_THRESHOLD=";

        char buf[100];

        while((fgets(buf, 100, fp)!=NULL))
        {
            if(strstr(buf,truck_A_max_cap)!=NULL)
            {
                sscanf(buf + strlen(truck_A_max_cap),"%d", &truck_a_max_capacity);
            }
            else if(strstr(buf, truck_B_max_cap)!=NULL)
            {
                sscanf(buf + strlen(truck_B_max_cap), "%d", &truck_b_max_capacity);
            }
            else if(strstr(buf,truck_delay)!=NULL)
            {
                sscanf(buf + strlen(truck_delay), "%d", &truck_trip_delay);
            }
            else if(strstr(buf,T_threshold)!=NULL)
            {
                sscanf(buf + strlen(T_threshold), "%f", &time_threshold);
            }
            else if(strstr(buf,storage_max_A)!=NULL)
            {
                sscanf(buf + strlen(storage_max_A), "%d", &storage_area_max_A);
            }
            else if(strstr(buf,storage_max_B)!=NULL)
            {
                sscanf(buf + strlen(storage_max_B), "%d", &storage_area_max_B);
            }
            else if(strstr(buf,storage_min)!=NULL)
            {
                sscanf(buf + strlen(storage_min), "%d", &storage_area_min);
            }
            else if(strstr(buf,storage_emp_delay)!=NULL)
            {
                sscanf(buf + strlen(storage_emp_delay), "%d", &storage_employee_delay);
            }

            else if(strstr(buf,det_threshold)!=NULL)
            {
                sscanf(buf + strlen(det_threshold), "%d", &num_detergent_threshold);
            }
            else if(strstr(buf,numOf_truck_A)!=NULL)
            {
                sscanf(buf + strlen(numOf_truck_A), "%d", &truck_A_num);
            }
            else if(strstr(buf,numOf_truck_B)!=NULL)
            {
                sscanf(buf + strlen(numOf_truck_B), "%d", &truck_B_num);
            }
            else if(strstr(buf,l2_emp)!=NULL)
            {
                sscanf(buf + strlen(l2_emp),"%d",&l2_employees);
            }
            else if(strstr(buf,A_cartoon)!=NULL)
            {
                sscanf(buf + strlen(A_cartoon),"%d", &fab_A_cartoon);
            }
            else if(strstr(buf,B_cartoon)!=NULL)
            {
                sscanf(buf + strlen(B_cartoon), "%d", &fab_B_cartoon);
            }
            else if(strstr(buf,truck_detergent_num)!=NULL)
            {
                sscanf(buf + strlen(truck_detergent_num), "%d", &truck_D_num);
            }
            else if(strstr(buf,total_detergent_products_ths)!=NULL)
            {
                sscanf(buf + strlen(total_detergent_products_ths), "%d", &total_detergent_products_threshold);
            }

        }
        fclose(fp);
        usleep(8000000);
    }
}

