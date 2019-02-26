#include <stdio.h>
#include <fcntl.h>

#define REPORT_FILE "report.txt"
#define TOTAL_TAGS 24

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./dmesg-analizer.o logfile.txt\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

int isTag(char dict[24][500], char *word, int size){

	int i;

	for (i = 0; i < size; i++){
		if(strcmp(dict[i], word) == 0){
			return i;
		}
	}

	return -1;
}

void analizeLog(char *logFile, char *report) {

    	printf("Generating Report from: [%s] log file\n", logFile);

    	int input_file;
	int report_file;

	const char tag_dictionary[24][500] = {
			"Command",
			"KERNEL",
			"x86/fpu",
			"e820",
			"BIOS-e820",
			"NX",
			"DMI",
			"Hypervisor",
			"MTRR",
			"x86/PAT",
			"RAMDISK",
			"ACPI",
			"kvm-clock",
			"clocksource",
			"Zone",
			"Reserved",
			"On",
			"IOAPIC",
			"smpboot",
			"PM",
			"random",
			"setup_percpu",
			"percpu",
			"pcpu-alloc"
	};

	char line[65535];
	char times[500][500];
	char details[500][500];
	char bufTemp[1000];
	char toWrite[1000];
	int tagNo[500];

	int i = 0;
	int j = 0;
	int c;
	int countTimes = 0;
	int countTags = 0;
	int countDetails = 0;
	int bufCount = 0;
	int inTime = 1;
	int inTag = 0;
	int inDetail = 0;

	input_file = open(logFile, O_RDONLY);
	report_file = open(report, O_WRONLY);

	read(input_file, line, 65535);

	close(input_file);

	for(i = 0; i < (sizeof(line)/sizeof(char)); i++){
		if(inTime == 1 && inTag == 0 && inDetail == 0){
			bufTemp[bufCount] = line[i];
			bufCount++;
			if(line[i] == ']'){
				bufTemp[bufCount + 1] = '\0';
				strncpy(times[countTimes], bufTemp, 15);
				bufCount = 0;
				countTimes++;
				memset(bufTemp, 0, 1000);
				inTime = 0;
				i++;
			}
		}else if(inTime == 0 && inTag == 0 && inDetail == 0){
			bufTemp[bufCount] = line[i];
			if(line[i+1] == ' ' || line[i+1] == ':'){
				int tagNumber = isTag(tag_dictionary, bufTemp, 24);
				if(tagNumber != -1){
					tagNo[countTags] = tagNumber;
					countTags++;
					if(line[i+1] != ':'){
						inTag = 1;
					} else {
						bufCount = 0;
						inTag = 0;
						inDetail = 1;
						memset(bufTemp, 0, 1000);
						i++;
					}
				} else {
					inTag = 0;
					inDetail = 1;
				}
			}
			bufCount++;
		} else if(inTime == 0 && inTag == 1 && inDetail == 0){
			bufTemp[bufCount] = line[i];
			if(line[i+1] == ':'){
				inTag = 0;
				inDetail = 1;
				bufCount = 0;
				memset(bufTemp, 0, 1000);
				i++;
			}
		}else if (inTime == 0 && inTag == 0 && inDetail == 1){
			bufTemp[bufCount] = line[i];
			if(line[i] == '\n'){
				strncpy(details[countDetails], bufTemp, sizeof(bufTemp)/sizeof(char));
				inTime = 1;
				inDetail = 0;
				memset(bufTemp, 0, 1000);
				bufCount = 0;
				countDetails++;
				bufCount = 0;
			}
			bufCount++;
		}
	}

	countTags = 1;

	for(i = 0; i < TOTAL_TAGS; i++){
		sprintf(toWrite, "%s:\n", tag_dictionary[countTags]);
		write(report_file, toWrite, strlen(toWrite));
		for(j = 0; j < 500; j++){
			if(tagNo[j] == countTags){
				sprintf(toWrite, "\t%s %s", times[j], details[j]);
				write(report_file, toWrite, strlen(toWrite));
			}
		}
		countTags++;
	}

	sprintf(toWrite, "%s:\n", "General");

	for(j = 0; j < 500; j++){
		if(tagNo[j] == 0){
			sprintf(toWrite, "\t%s %s", times[j], details[j]);
			write(report_file, toWrite, strlen(toWrite));
		}
	}

	close(report_file);
  	printf("Report is generated at: [%s]\n", report);
}