/* Including standard libraries */
#include <stdio.h>
#include <stdlib.h>

/* Definition of constants */
#define INPUT_FILE "input.txt"
#define ARRAY_LENGTH 50

void clear_buffer();
int get_valid_int(char *field_name);

int main() {

	/* Defining some constant values */
	const char *products_ids[ARRAY_LENGTH] = {"MZXK", "CQA8", "VMMW", "TGK3", "9WYR", "SZ5I", "EWG4", "VJMJ", "KMU7", "O3EV", "CSUF", "KJD2", "6VF2", "JZ3E", "O81V", "FIBN", "1X2G", "P4UN", "GOA5", "3SMH", "H0WY", "DQUY", "CQIC", "U4VQ", "CUF4", "IJLM", "PZXQ", "58WE", "8WPD", "FZM6", "BCWZ", "EZV0", "88NX", "J12I", "U4G1", "NPUT", "01UX", "6KMP", "LCM5", "G9JN", "E1B0", "M386", "NJ1S", "BPUQ", "FLSZ", "5VWJ", "ENDV", "OEWD", "BRVP", "8O1D"};
	const char *names[ARRAY_LENGTH] = {"Screw", "Bolt", "Stud", "Nut", "Washer", "Rivet", "Insert", "Standoff", "Thread_insert", "Pin", "Locking_pin", "Clevis_pin", "Shim", "Spacer", "Hose_clamp", "Fixing_clip", "Cable_tie", "Toggle_clamp", "Spring_plunger", "Locating_pin", "Ball_plunger", "HingesHinge", "Lid_stay", "Lock", "Draw_latche", "Latche", "Strike_plate", "Locking_insert", "Cylinder_lock", "Locking_device", "SlidesSlide", "HandlesHandle", "Clamping_lever", "Knob", "Lever", "Handwheel", "Crank_handle", "PlugsPlug", "Cap", "SpringsSpring", "Air_spring", "Gas_spring", "Damper", "Shock_damper", "Stop", "Bumper", "Thrust_pad", "Rotary_damper", "Other_damper", "Wheel"};
	const char *pieces_ids[ARRAY_LENGTH] = {"FP8K", "07HA", "ZWDI", "XUNF", "FEH4", "7RM1", "CRUX", "8DJC", "TDNR", "OWP9", "OKJ8", "69NI", "C2Q0", "31R1", "P57B", "79Y9", "ZU78", "EN1E", "CWTD", "4VOS", "1A97", "SMWR", "W3PF", "ZGO3", "VMU7", "OS34", "6EKY", "8F5F", "Z97I", "PP78", "4KKD", "1LV6", "OYKH", "9P1V", "B8H0", "V4CQ", "MFBV", "FS9O", "COFX", "HIMV", "RHAK", "DUBL", "3OD6", "03GQ", "MDBV", "49VM", "JW06", "AGPB", "G1DJ", "L637"};
	const char *times_entry[ARRAY_LENGTH] = {"17:23:05","21:16:56","15:06:49","14:25:11","00:05:15","02:44:29","20:06:48","08:12:56","00:49:27","20:15:39","06:11:57","00:15:44","03:32:07","05:39:18","01:14:21","21:07:49","13:34:09","01:47:35","02:17:06","01:51:00","13:13:48","21:46:30","16:26:28","05:50:57","00:23:23","02:12:03","00:32:44","13:23:11","06:02:36","19:46:17","16:59:08","11:26:24","18:03:26","01:55:09","16:33:28","07:23:21","21:08:15","03:14:06","22:14:34","17:17:41","13:45:10","23:38:31","09:06:38","04:10:19","12:30:05","23:23:16","11:35:10","03:38:13","12:55:01","18:35:16"};
	const char *times_exit[ARRAY_LENGTH] = {"18:14:24","22:48:55","16:18:57","15:32:32","01:13:16","17:12:56","20:24:35","08:44:10","01:54:08","21:55:43","06:41:28","01:19:12","15:08:36","06:26:36","06:54:04","23:29:03","20:42:53","06:54:04","05:11:44","02:23:52","15:01:50","22:12:20","21:49:39","09:25:50","05:11:44","10:52:56","00:32:49","13:55:21","07:16:15","20:14:13","18:45:35","12:44:01","19:44:34","02:22:50","17:05:43","08:49:21","23:13:51","05:45:55","23:11:57","17:42:58","15:11:01","23:49:00","11:12:15","05:26:05","15:52:40","23:59:59","13:30:00","05:39:13","15:38:11","19:20:12"};

	int i = 0;

	int max;
	printf("\nInsert number of articles to generate (0-50): ");
	max = get_valid_int("Num articles to generate");

	FILE *f = fopen(INPUT_FILE, "w");
	if (f != NULL)
	{
		/* Loop until max value */
		for (i = 0; i < max; i++) {
			fprintf(f, "%s %s %s %s %s\n", products_ids[i], names[i], pieces_ids[i], times_entry[i], times_exit[i]); /* Writing to file */
		}
		fclose(f);
	} else {
		printf("Error opening file!\n");
		return 1;
	}

	printf("\n%d articles generated!\n\n", max);

	return 0;
}

void clear_buffer()
{
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int get_valid_int(char *field_name)
{
	int is_valid = 0;
	int value;
	char term;
	while (!is_valid) {
		if (scanf("%d%c", &value, &term) != 2 || term != '\n') {
			clear_buffer();
			printf("%s must be an integer, try again: ", field_name);
		} else {
			if (value < 0 || value > ARRAY_LENGTH) {
				printf("%s must be between 0 and 50, try again: ", field_name);
			} else {
				is_valid = 1;
			}
		}
	}

	return value;
}