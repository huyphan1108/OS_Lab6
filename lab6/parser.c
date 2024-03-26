#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
//#include <sys/time.h>

typedef struct {
	char id[20];
	char state[20];
	char queue[20];
} Process;

//Derek
char temp_arr[20][5];
void parse_queue(char* queue) {
	//printf("%s\n", pid);
	//printf("%s\n", queue);
	char* d_token = strtok(queue, " ");

	for (int a = 0; d_token != NULL; a++) {
		//printf("%s ", d_token);
		strcpy(temp_arr[a], d_token);
		d_token = strtok(NULL, " ");
	}
}



int main()
{
	int i;
	char* rch;
	char str[200];
	char LineInFile[40][300];
	int lineP, lineQ;
	char* sch;
	char tokenizedLine[10][10];
	int blocked_count = 0;
	int user_threshold, user_cap, user_process;
	int rand_process_swap = 0;
	int overall_latency = 0;


	FILE* fp1;
	FILE* fp2;
	fp1 = fopen("inp1.txt", "r");			//open the original input file
	fp2 = fopen("inp1_parsed.txt", "w");	//output the Process ID and event to another file. 
	//You can store in variables instead of printing to file

	srand(time(NULL));  // Random num seed
	Process processes[20];

	char* printer_q = (char*)malloc(10 * sizeof(char));
	char* disk_q = (char*)malloc(10 * sizeof(char));
	char* kb_q = (char*)malloc(10 * sizeof(char));

	strcpy(disk_q, "");						// initialize memory location for queues
	strcpy(printer_q, "");
	strcpy(kb_q, "");


	printf("Started parsing...\n");

	//get the first line
	if (fgets(str, sizeof(str), fp1) != NULL) {
		//Remove the 'end' and print into write file
		char character = ' ';
		char* ptr = strrchr(str, character);
		*ptr = '\0';
		fprintf(fp2, "%s\n\n", str);
	}

	//get the initial process ID and its state from the first line
	char* token = strtok(str, " ");
	int j = 0;
	while (token != NULL && j < 20) {
		if (j % 2 == 0) {
			//processes[i / 2].id = atoi(token);
			strcpy(processes[j / 2].id, token);
		}
		else {
			strcpy(processes[j / 2].state, token);
		}
		token = strtok(NULL, " ");
		j++;
	}

	//printf("%d ", j / 2);
	for (int a = 0; a < j / 2; a++) {
		strcpy(processes[a].queue, "");
	}

	/*
	for (int z = 0; z < j / 2; z++) {
		fprintf(fp2, "Process %d: id = %s, state = %s\n", j, processes[z].id, processes[z].state);
	}
	*/
	for (int a = 0; a < j / 2; a++) {
		if (strcmp(processes[a].state, "Blocked") == 0) {
			blocked_count++;
		}
	}
	//printf("Number of blocked processes is: %d\n", blocked_count);
	printf("Choose percentage of Processes in Blocked state (80, 90 or 100): ");
	scanf("%d", &user_threshold);
	//printf("User chose: %d", user_threshold);
	while (user_threshold != 80 && user_threshold != 90 && user_threshold != 100) {
		printf("Please choose 80, 90 or 100 for percentage value: ");
		scanf("%d", &user_threshold);
	}

	printf("Choose the swapping capacity (1 or 2): ");
	scanf("%d", &user_cap);
	while (user_cap != 1 && user_cap != 2) {
		printf("Please choose 1 or 2 for swapping capacity: ");
		scanf("%d", &user_cap);
	}

	int n = j / 2; // n is total number of process
	//printf("Total number of process is: %d\n", n);

	user_process = n * user_threshold / 100;
	//printf("Number of user process is: %d\n", user_process);

	lineP = 0;
	i = 0;


	while (fgets(str, sizeof(str), fp1) != NULL) {
		fprintf(fp2, "%s", str);
		lineP = 0;
		rch = strtok(str, ":;.");					// use strtok to break up the line by : or . or ; This would separate each line into the different events
		while (rch != NULL)
		{
			strcpy(LineInFile[lineP], rch);			//copy the events into an array of strings
			lineP++;								//keep track of how many events are in that line
			rch = strtok(NULL, ":;.");				//needed for strtok to continue in the while loop
		}
		for (i = 1; i < lineP - 1; i++)
		{
			lineQ = 0;
			sch = strtok(LineInFile[i], " ");
			while (sch != NULL)
			{
				strcpy(tokenizedLine[lineQ], sch);		//use strtok to break up each line into separate words and put the words in the array of strings
				lineQ++;								//count number of valid elements
				sch = strtok(NULL, " ");
			}

			//tokenizedLine has the event separated by spaces (e.g. Time slice for P7 expires)
			if (strcmp(tokenizedLine[1], "requests") == 0)						//Process requests an I/O device
			{
				//fprintf(fp2, "%s %s \n", tokenizedLine[0], tokenizedLine[1]);
				//fprintf(fp2, "%s %s %s ", tokenizedLine[0], tokenizedLine[1], tokenizedLine[3]);
				for (int a = 0; a < n; a++) {
					if ((strcmp(tokenizedLine[0], processes[a].id) == 0) && (strcmp(tokenizedLine[3], "disk") == 0)) {
						strcpy(processes[a].state, "Blocked*");
						strcpy(processes[a].queue, "disk");
						if (strcmp(disk_q, "") == 0) {
							strcpy(disk_q, tokenizedLine[0]);
						}
						else {
							strcat(disk_q, " ");
							strcat(disk_q, tokenizedLine[0]);
						}
						//fprintf(fp2, "%s ", processes[a].state);
					}
					else if ((strcmp(tokenizedLine[0], processes[a].id) == 0) && (strcmp(tokenizedLine[3], "printer") == 0)) {
						strcpy(processes[a].state, "Blocked*");
						strcpy(processes[a].queue, "printer");
						if (strcmp(printer_q, "") == 0) {
							strcpy(printer_q, tokenizedLine[0]);

						}
						else {
							strcat(printer_q, " ");
							strcat(printer_q, tokenizedLine[0]);
						}
					}
					else if ((strcmp(tokenizedLine[0], processes[a].id) == 0) && (strcmp(tokenizedLine[3], "keyboard") == 0)) {
						strcpy(processes[a].state, "Blocked*");
						strcpy(processes[a].queue, "keyboard");
						if (strcmp(kb_q, "") == 0) {
							strcpy(kb_q, tokenizedLine[0]);

						}
						else {
							strcat(kb_q, " ");
							strcat(kb_q, tokenizedLine[0]);
						}
					}
				}
				blocked_count++;
				//printf("Number of blocked processes is: %d\n", blocked_count);
			}
			else if (strcmp(tokenizedLine[2], "dispatched") == 0) {
				for (int a = 0; a < n; a++) {
					if (strcmp(tokenizedLine[0], processes[a].id) == 0) {
						strcpy(processes[a].state, "Running*");
					}
				}
				if (strcmp(tokenizedLine[3], "disk") == 0) {

				}
			}
			/*
			else if (strcmp(tokenizedLine[3], "out") == 0) {
				//printf("%s", tokenizedLine[3]);
				for (int a = 0; a < n; a++) {
					if (strcmp(tokenizedLine[0], processes[a].id) == 0) {
						strcat(processes[a].state, "/Suspend*");
						//printf("%s \n", processes[a].state);
					}
				}
			}
			*/
			else if (strcmp(tokenizedLine[4], "expires") == 0) {
				for (int a = 0; a < n; a++) {
					if (strcmp(tokenizedLine[3], processes[a].id) == 0) {
						strcpy(processes[a].state, "Ready*");
					}
				}
			}
			/*
			else if (strcmp(tokenizedLine[3], "in") == 0) {
				for (int a = 0; a < n; a++) {
					if (strcmp(tokenizedLine[0], processes[a].id) == 0) {
						if (strcmp(processes[a].state, "Blocked/Suspend") == 0) {
							strcpy(processes[a].state, "Blocked*");
						}
						else if (strcmp(processes[a].state, "Ready/Suspend") == 0) {
							strcpy(processes[a].state, "Ready*");
						}
					}
				}
			}
			*/
			else if (strcmp(tokenizedLine[2], "terminated") == 0) {
				int index;
				for (int a = 0; a < n; a++) {
					if (strcmp(tokenizedLine[0], processes[a].id) == 0) {
						strcpy(processes[a].state, "Exit*");
						index = a;
					}
				}

				n--;
				user_process = n * user_threshold / 100;  //update the process threshold


				rand_process_swap = rand() % 20;
				while (strcmp(processes[rand_process_swap].state, "Blocked/Suspend") != 0) {
					rand_process_swap = rand() % 20;
				}
				strcpy(processes[rand_process_swap].state, "Blocked*");
				overall_latency++;
			}
			/// TODO: Interrupt
			else if (strcmp(tokenizedLine[1], "interrupt") == 0) {
				for (int a = 0; a < n; a++) {
					if (strcmp(tokenizedLine[4], processes[a].id) == 0) {
						if (strcmp(processes[a].state, "Blocked") == 0) {
							strcpy(processes[a].state, "Ready*");
							//printf("%s\n", processes[a].queue);
							if (strcmp(processes[a].queue, "disk") == 0) {
								if (strcmp(processes[a].id, disk_q) == 0) {
									disk_q[0] = '\0';
									continue;
								}
								int k = 0;
								parse_queue(disk_q);
								while (strcmp(temp_arr[k], "") != 0) {
									if (strcmp(temp_arr[k], processes[a].id) != 0) {
										size_t temp_size = strlen(disk_q);
										disk_q[temp_size - 3] = '\0';
									}
									k++;
								}
							}
							else if (strcmp(processes[a].queue, "printer") == 0) {
								if (strcmp(processes[a].id, printer_q) == 0) {
									printer_q[0] = '\0';
									continue;
								}
								int k = 0;
								parse_queue(printer_q);
								while (strcmp(temp_arr[k], "") != 0) {
									if (strcmp(temp_arr[k], processes[a].id) != 0) {
										size_t temp_size = strlen(printer_q);
										printer_q[temp_size - 3] = '\0';
									}
									k++;
								}
							}
							else if (strcmp(processes[a].queue, "keyboard") == 0) {
								//parse_queue(processes[a].id, kb_q);
								if (strcmp(processes[a].id, kb_q) == 0) {
									kb_q[0] = '\0';
									continue;
								}
								int k = 0;
								parse_queue(kb_q);
								while (strcmp(temp_arr[k], "") != 0) {
									if (strcmp(temp_arr[k], processes[a].id) != 0) {
										size_t temp_size = strlen(kb_q);
										kb_q[temp_size - 3] = '\0';
									}
									k++;
								}
							}
						}
						else if (strcmp(processes[a].state, "Blocked/Suspend") == 0) {
							strcpy(processes[a].state, "Ready/Suspend*");
							if (strcmp(processes[a].queue, "disk") == 0) {
								//parse_queue(processes[a].id, disk_q);
								if (strcmp(processes[a].id, disk_q) == 0) {
									disk_q[0] = '\0';
									continue;
								}
								int k = 0;
								parse_queue(disk_q);
								while (strcmp(temp_arr[k], "") != 0) {
									if (strcmp(temp_arr[k], processes[a].id) != 0) {
										size_t temp_size = strlen(disk_q);
										disk_q[temp_size - 3] = '\0';
									}
									k++;
								}
							}
							else if (strcmp(processes[a].queue, "printer") == 0) {
								//parse_queue(processes[a].id, printer_q);
								if (strcmp(processes[a].id, printer_q) == 0) {
									printer_q[0] = '\0';
									continue;
								}
								int k = 0;
								parse_queue(printer_q);
								while (strcmp(temp_arr[k], "") != 0) {
									if (strcmp(temp_arr[k], processes[a].id) != 0) {
										size_t temp_size = strlen(printer_q);
										printer_q[temp_size - 3] = '\0';
									}
									k++;
								}
							}
							else if (strcmp(processes[a].queue, "keyboard") == 0) {
								//parse_queue(processes[a].id, kb_q);
								if (strcmp(processes[a].id, kb_q) == 0) {
									kb_q[0] = '\0';
									continue;
								}
								int k = 0;
								parse_queue(kb_q);
								while (strcmp(temp_arr[k], "") != 0) {
									if (strcmp(temp_arr[k], processes[a].id) != 0) {
										size_t temp_size = strlen(kb_q);
										kb_q[temp_size - 3] = '\0';
									}
									k++;
								}
							}
						}
						//parse_queue(tokenizedLine[4], disk_q);
					}

				}
				blocked_count--;
				//printf("Number of blocked processes is: %d\n", blocked_count);
			}
		}

		//print
		for (int a = 0; a < n; a++) {
			fprintf(fp2, "%s %s ", processes[a].id, processes[a].state);
		}
		//Run through process state array to remove the '*'
		for (int a = 0; a < n; a++) {
			size_t temp_size = strlen(processes[a].state);
			if (processes[a].state[temp_size - 1] == '*') {
				processes[a].state[temp_size - 1] = '\0';
			}
		}
		fprintf(fp2, "\nTotal number of process: %d\n", n);
		fprintf(fp2, "Number of blocked process: %d\n", blocked_count);
		fprintf(fp2, "Process threshold: %d\n", user_process);
		fprintf(fp2, "Disk queue: %s\n", disk_q);
		fprintf(fp2, "Printer queue: %s\n", printer_q);
		fprintf(fp2, "Keyboard queue: %s\n", kb_q);

		if (blocked_count >= user_process) {
			fprintf(fp2, "\nNumber of Blocked process has reached the threshold\n");
			fprintf(fp2, "List of Blocked: ");
			for (int a = 0; a < n; a++) {
				if (strcmp(processes[a].state, "Blocked") == 0) {
					fprintf(fp2, "%s ", processes[a].id);
				}
			}
			fprintf(fp2, "\n");
			//do the swap out
			//get a random process
			rand_process_swap = rand() % 20;
			while (strcmp(processes[rand_process_swap].state, "Blocked") != 0) {
				rand_process_swap = rand() % 20; //get new random process until process is blocked
			}
			strcpy(processes[rand_process_swap].state, "Blocked/Suspended*");
			overall_latency++; //increase the latency

			//do same thing
			if (user_cap == 2) {
				rand_process_swap = rand() % 20;
				while (strcmp(processes[rand_process_swap].state, "Blocked") != 0) {
					rand_process_swap = rand() % 20;
				}
				strcpy(processes[rand_process_swap].state, "Blocked/Suspended*");
				overall_latency++; //increase the latency
			}

			for (int a = 0; a < n; a++) {
				fprintf(fp2, "%s %s ", processes[a].id, processes[a].state);
			}
			//Run through process state array to remove the '*'
			for (int a = 0; a < n; a++) {
				size_t temp_size = strlen(processes[a].state);
				if (processes[a].state[temp_size - 1] == '*') {
					processes[a].state[temp_size - 1] = '\0';
				}
			}
			fprintf(fp2, "\nTotal number of process: %d\n", n);
			fprintf(fp2, "Number of blocked process: %d\n", blocked_count);
			fprintf(fp2, "Process threshold: %d\n", user_process);
			fprintf(fp2, "Disk queue: %s\n", disk_q);
			fprintf(fp2, "Printer queue: %s\n", printer_q);
			fprintf(fp2, "Keyboard queue: %s\n", kb_q);

		}
		fprintf(fp2, "\n");
	}

	//parse_queue(disk_q);
	printf("Parsing complete\n\n");
	fclose(fp1);
	fclose(fp2);

	printf("The overall latency for this trial is %d\n", overall_latency);

	return 0;
}