// Santino Nardolillo
// Quang Vu
// Joey Daermann
#define _CRT_SECURE_NO_DEPRECATE
//parses the input file into Process and event
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	int i;
	char* rch;
	char str[200];
	char LineInFile[40][300];
	char ProcessState[40][20];
	char PrintState[40][20];
	int lineP, lineQ;
	char* sch;
	char tokenizedLine[10][10];

	FILE* fp1;
	FILE* fp2;
	fp1 = fopen("inp1.txt", "r");			//open the original input file
	fp2 = fopen("inp1parsed.txt", "w");	//output the Process ID and event to another file. 
	//You can store in variables instead of printing to file

	lineP = 0;
	i = 0;

	// creating character arrays
	char DiskArr[20][10];
	char PrinterArr[20][10];
	char KeyboardArr[20][10];

	for (int j = 0; j < 20; j++)
	{
		strcpy(DiskArr[j], "0");
		strcpy(PrinterArr[j], "0");
		strcpy(KeyboardArr[j], "0");
	}
	for (int j = 0; j < 40; j++)
	{
		memset(ProcessState[j], NULL, 20);
		memset(PrintState[j], NULL, 20);
	}

	printf("Started parsing...\n");

	//copy first line of the original file to the new filefile
	if (fgets(str, sizeof(str), fp1) != NULL)
		fprintf(fp2, "%s", str);

	int j = 0;
	rch = strtok(str, " ");
	while (strcmp(rch, "end\n") != 0)
	{
		strcpy(ProcessState[j], rch);
		j++;
		rch = strtok(NULL, " ");
	}
	int tempi = 0;
	printf("Initial States: ");
	for (; ProcessState[tempi][1] != NULL && tempi < 40;)
	{
		printf("%s ", ProcessState[tempi]);
		tempi++;
	}
	printf("\n\n");

	//parse each remaining line into Process event
	//while loop with fgets reads each line
	while (fgets(str, sizeof(str), fp1) != NULL)
	{
		printf("% s\n", str);
		for (int j = 0; j < 40; j++)
			strcpy(PrintState[j], ProcessState[j]);
		lineP = 0;
		rch = strtok(str, ":;.");					// use strtok to break up the line by : or . or ; This would separate each line into the different events
		while (rch != NULL)
		{
			strcpy(LineInFile[lineP], rch);			//copy the events into an array of strings
			lineP++;								//keep track of how many events are in that line
			rch = strtok(NULL, ":;.");				//needed for strtok to continue in the while loop
		}


		//for each event (e.g. Time slice for P7 expires) pull out process number and event
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

				//fprintf(fp2, "%s %s ", tokenizedLine[0], tokenizedLine[1]);
				fprintf(fp2, "%s %s %s ", tokenizedLine[0], tokenizedLine[1], tokenizedLine[3]);

				for (int j = 0; j < sizeof(ProcessState) / 20; j++)
				{
					if (strcmp(ProcessState[j], tokenizedLine[0]) == 0)
					{
						strcpy(ProcessState[j + 1], "Blocked");
						strcpy(PrintState[j + 1], "Blocked*");
						if (strcmp(tokenizedLine[3], "disk") == 0)
						{
							for (int j = 0; j < 20; j++)
								if (strcmp(DiskArr[j], "0") == 0)
								{
									strcpy(DiskArr[j], tokenizedLine[0]);
									break;
								}
						}
						else if (strcmp(tokenizedLine[3], "printer") == 0)
						{
							for (int j = 0; j < 20; j++)
								if (strcmp(PrinterArr[j], "0") == 0)
								{
									strcpy(PrinterArr[j], tokenizedLine[0]);
									break;
								}
						}
						else if (strcmp(tokenizedLine[3], "keyboard") == 0)
						{
							for (int j = 0; j < 20; j++)
								if (strcmp(KeyboardArr[j], "0") == 0)
								{
									strcpy(KeyboardArr[j], tokenizedLine[0]);
									break;
								}
						}
					}
				}
			}
			else if ((strcmp(tokenizedLine[2], "dispatched") == 0))				//Process is dispatched
			{
				fprintf(fp2, "%s %s ", tokenizedLine[0], tokenizedLine[2]);
				for (int j = 0; j < sizeof(ProcessState) / 20; j++)
				{
					if (strcmp(ProcessState[j], tokenizedLine[0]) == 0)
					{
						strcpy(ProcessState[j + 1], "Running");
						strcpy(PrintState[j + 1], "Running*");
						break;
					}
				}
			}

			else if (strcmp(tokenizedLine[0], "Time") == 0)						//Process has timed off
			{
				fprintf(fp2, "%s %s ", tokenizedLine[3], tokenizedLine[4]);
				for (int j = 0; j < sizeof(ProcessState) / 20; j++)
				{
					if (strcmp(ProcessState[j], tokenizedLine[3]) == 0)
					{
						strcpy(ProcessState[j + 1], "Ready");
						strcpy(PrintState[j + 1], "Ready*");
						break;
					}
				}
			}
			else if (strcmp(tokenizedLine[3], "out") == 0)						//Process is swapped out
			{
				fprintf(fp2, "%s %s ", tokenizedLine[0], tokenizedLine[3]);
				for (int j = 0; j < sizeof(ProcessState) / 20; j++)
				{
					if (strcmp(ProcessState[j], tokenizedLine[0]) == 0)
					{
						strcat(ProcessState[j + 1], " Suspend");
						strcat(PrintState[j + 1], " Suspend*");
						break;
					}
				}
			}
			else if (strcmp(tokenizedLine[3], "in") == 0)						//Process is swapped in
			{
				fprintf(fp2, "%s %s ", tokenizedLine[0], tokenizedLine[3]);
				for (int j = 0; j < sizeof(ProcessState) / 20; j++)
				{
					if (strcmp(ProcessState[j], tokenizedLine[0]) == 0)
					{

						rch = strtok(ProcessState[j + 1], " ");
						strcpy(PrintState[j + 1], ProcessState[j + 1]);
						strcat(PrintState[j + 1], "*");
						break;
					}
				}
			}
			else if (strcmp(tokenizedLine[1], "interrupt") == 0)				//An interrupt has occured
			{
				fprintf(fp2, "%s %s ", tokenizedLine[4], tokenizedLine[1]);
				for (int j = 0; j < sizeof(ProcessState) / 20; j++)
				{
					if (strcmp(ProcessState[j], tokenizedLine[4]) == 0)
					{

						for (int k = 0; k < 19; k++)
						{
							if (strcmp(ProcessState[j], DiskArr[k]) == 0)
							{
								for (; k < 19; k++)
								{
									strcpy(DiskArr[k], DiskArr[k + 1]);
									strcpy(DiskArr[k + 1], "0");
								}
							}
							if (strcmp(ProcessState[j], KeyboardArr[k]) == 0)
							{
								for (; k < 19; k++)
								{
									strcpy(KeyboardArr[k], KeyboardArr[k + 1]);
									strcpy(KeyboardArr[k + 1], "0");
								}
							}
							if (strcmp(ProcessState[j], PrinterArr[k]) == 0)
							{
								for (; k < 19; k++)
								{
									strcpy(PrinterArr[k], PrinterArr[k + 1]);
									strcpy(PrinterArr[k + 1], "0");
								}
							}
						}
						if (strcmp(ProcessState[j + 1], "Blocked") == 0)
						{
							strcpy(ProcessState[j + 1], "Ready");
							strcpy(PrintState[j + 1], "Ready*");
						}
						else if (strcmp(ProcessState[j + 1], "Blocked Suspend") == 0)
						{
							strcpy(ProcessState[j + 1], "Ready Suspend");
							strcpy(PrintState[j + 1], "Ready Suspend*");
						}

					}
				}
			}
			else																//Process has been terminated
			{
				fprintf(fp2, "%s %s ", tokenizedLine[0], tokenizedLine[2]);
				for (int j = 0; j < sizeof(ProcessState) / 20; j++)
				{
					if (strcmp(ProcessState[j], tokenizedLine[0]) == 0)
					{
						strcpy(ProcessState[j + 1], "Exit");
						strcpy(PrintState[j + 1], "Exit*");
					}
				}
			}
			for (int j = 0; j < 40; j++)
			{
				if (strcmp(ProcessState[j], "New") == 0)
				{
					strcpy(ProcessState[j], "Ready");
					strcpy(PrintState[j], "Ready*");
				}
			}

		}
		fprintf(fp2, "\n");


		tempi = 0;
		while (PrintState[tempi][1] != NULL && tempi < 40)
		{
			printf("%s ", PrintState[tempi]);
			tempi++;
		}
		tempi = 0;
		printf("\nDisk Queue : ");
		while (strcmp(DiskArr[tempi], "0") != 0 && tempi < 20)
		{
			printf(" %s", DiskArr[tempi]);
			tempi++;
		}
		printf("\nPrinter Queue : ");
		tempi = 0;
		while (strcmp(PrinterArr[tempi], "0") != 0 && tempi < 20)
		{
			printf("%s", PrinterArr[tempi]);
			tempi++;
		}
		tempi = 0;
		printf("\nKeyboard Queue : ");
		while (strcmp(KeyboardArr[tempi], "0") != 0 && tempi < 20)
		{
			printf("%s", KeyboardArr[tempi]);
			tempi++;
		}
		printf("\n\n");
	}

	printf("Parsing complete\n\n");

	fclose(fp1);
	fclose(fp2);

	return 0;
}