/* Game: River Frog
 * About the author
 *	
 *	College: 		NTHU
 *	Major:			Math
 *	Double major:	CS
 *	SID:			101021120
 *	Name:			Yu Xiang, Zheng
 *	Nickname:		Slighten
 *	E-mail:			zz687@yahoo.com.tw
 *	FB: 			https://www.facebook.com/Slighten.Zheng
 *	Cellphone: 		0923576510
 *
 * Filename: frog.c
 * Status: 
 * 	Compile and execute successfully and impeccably.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/select.h>
#include <termio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#define BUFFER_LEN 100
#define OFFSET 20
#define MODULO 900000
#define REMAINDER 90000
#define MIN 50000
#define MAX 1000000
static pthread_t *threads; // [HEIGHT+3]
static long long int score = 0;
static char *boundary; // [LENGTH+1]
static int object[2];
static long long int highscore = 0;
static char *clean; // [LENGTH+2]
static char name[BUFFER_LEN];
static char **wood[2]; // [2][HEIGHT][LENGTH+2]
static pthread_cond_t hit_cv;
static pthread_mutex_t print_mutex;
static struct termios orig_termios;
static int GAP = 4;
static int WOODLEN = 4;
static int *delay; // [HEIGHT]
static time_t start_time, end_time;
static float total_time;
static char diff;
static int LENGTH = 40, HEIGHT = 15;
static int width;

void reset_terminal_mode(){
	tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode(){
	struct termios new_termios;
	/* take two copies - one for now, one for later */
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));
	/* register cleanup handler, and set the new terminal mode */
	atexit(reset_terminal_mode);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

void printFormat(){
	FILE *fh = fopen("score", "r");
	printf("-----------How to play-----------\r\n"
			"W, up arrow   : up\r\n"
			"S, down arrow : down\r\n" 
			"A, left arrow : left\r\n" 
			"D, right arrow: right\r\n" 
			"Q, ctrl+c     : quit\r\n"
			"***Godfinger***\r\n"
			"O: speed down\r\n"
			"P: speed up\r\n");
	if (fh != NULL && fscanf(fh, "%s%lld", name, &highscore) == 2){
		printf("\r\n>>>>> High Score: %lld by player %s <<<<<\r\n", highscore, name);
		fclose(fh);
	}
	return;
}

int kbhit(){
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

int getch(){
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) {
		return r;
	} 
	else {
		return c;
	}
}


void gotoXY(int x,int y){
	printf("%c[%d;%df", 0x1B, x, y);
}


void refix(){
	gotoXY(1, 1);
	printf("Score: %lld\r\n", score);
}

void cleanWood(char wood[]){
	int i;
	for (i = 0; i < LENGTH ; i++)
		wood[i] = ' ';
	wood[i] = '\0';
}

void cleanScreen(int state, int num){
	int i, j;
	total_time = (float)(end_time - start_time)/CLOCKS_PER_SEC;
	//if (num != HEIGHT + 1)
	//	pthread_mutex_lock(&print_mutex);
	for (i = 0; i < HEIGHT+1 ; i++){
		if (i != num)
			pthread_cancel(threads[i]);
	}
	//if (num != HEIGHT + 1)
	//	pthread_mutex_unlock(&print_mutex);
	pthread_cancel(threads[HEIGHT+2]);
	for (i = 1; i < HEIGHT+OFFSET; i++){
		gotoXY(i, 1);
		printf("%s\r\n", clean);
	}
		gotoXY(1, 1);
	printFormat();
	printf("\r\nResult:\r\n\t");
	if (state == 0 || state == 3)
		printf("You have quitted.\r\n");
	else if (state == 1){
		printf("You win! Well done!\r\n");
		printf("Passed time: %.3f sec\r\n", total_time);
	}
	else if (state == 2)
		printf("You lose. Challenge again!\r\n");
	printf("Your final score: %lld\r\n", score);
	FILE *fh = fopen("score", "w");
	if (score > highscore){
		printf("Highscore Update!\r\n");
		reset_terminal_mode();
		printf("Please enter your name: ");
		scanf("%50s", name);
		fprintf(fh, "%s %lld\r\n", name, score);
	}
	else{
		fprintf(fh, "%s %lld\r\n", name, highscore);
	}
	fclose(fh);
	pthread_mutex_destroy(&print_mutex);
	pthread_cond_destroy(&hit_cv);
	pthread_join(threads[num], NULL);
	// free
    free(threads);
    free(boundary);
    for (i = 0; i < 2 ; i++){
        for (j = 0; j < HEIGHT; j++){
            free(wood[i][j]);
        }
        free(wood[i]);
    }
    free(delay);
    free(clean);
	exit(EXIT_SUCCESS);
}

void printBoundary(){
	gotoXY(2, 1);
	printf("%s\r\n", boundary);
	gotoXY(HEIGHT+3, 1);
	printf("%s\r\n", boundary);
}

void changeScreen(int x, int y, char symb){
	if (x < HEIGHT && x > -1){
		wood[0][x][y] = symb;
		wood[1][x][y] = symb;
	}
	gotoXY(x+3, y+1);
	printf("%c\r\n", symb);
}

void* moveObject(){
	int ch;
	while(1){
		pthread_cond_wait(&hit_cv, &print_mutex);
		ch = getch();
here:
		if (ch == 'Q' || ch == 'q')
			cleanScreen(0, HEIGHT+1);		
		else if (ch == 0x03)
			cleanScreen(3, HEIGHT+1);
		else if (ch == 'W' || ch == 'w'){
			if (object[0] - 1 == -2)
				cleanScreen(1, HEIGHT+1);
			else if (object[0] - 1 < HEIGHT && object[0] - 1 > 0 && 
				wood[0][object[0]-1][object[1]] == ' ' && 
				wood[1][object[0]-1][object[1]] == ' ')
				cleanScreen(2, HEIGHT+1);
			else {
				if (object[0] == HEIGHT){
					start_time = clock();
					gotoXY(1, 1);
					printf("%s\r\n", clean);
					gotoXY(1, LENGTH/2);
					printf("START TIMING!\r\n");
					printBoundary();
				}
				else if (object[0] == 0){
					end_time = clock();
					gotoXY(1, 1);
					printf("%s\r\n", clean);
					gotoXY(1, LENGTH/2);
					printf("END TIMING\r\n");
					printBoundary();
					changeScreen(object[0], object[1], '=');
				}
				else 
					changeScreen(object[0], object[1], '=');
				changeScreen(object[0]-1, object[1], 'O');
				object[0]--;
			}
		}
		else if (ch == 'A' || ch == 'a'){
			if ((object[0] == HEIGHT || object[0] == -1) && object[1] == 0){}
			else if (object[1] == 0 || (object[0] < HEIGHT && object[0] > -1 && 
				wood[0][object[0]][object[1]-1] == ' ' && wood[1][object[0]][object[1]-1] == ' '))
				cleanScreen(2, HEIGHT+1);
			else{
				if (object[0] == HEIGHT || object[0] == -1)
					printBoundary();
				else
					changeScreen(object[0], object[1], '=');
				changeScreen(object[0], object[1]-1, 'O');
				object[1]--;
			}
		}
		else if (ch == 'S' || ch == 's'){
			if(object[0] == HEIGHT){}
			else{
				if (object[0] + 1 < HEIGHT && wood[0][object[0]+1][object[1]] == ' ' && 
				wood[1][object[0]+1][object[1]] == ' ')
					cleanScreen(2, HEIGHT+1);
				else {
					if (object[0] == -1)
						printBoundary();
					else 
						changeScreen(object[0], object[1], '=');
					changeScreen(object[0]+1, object[1], 'O');
					object[0]++;
				}
			}
		}
		else if (ch == 'D' || ch == 'd'){
			if ((object[0] == HEIGHT || object[0] == -1) && object[1] == LENGTH-1){}
			else if (object[1] == LENGTH-1 || (object[0] < HEIGHT && object[0] > -1 && 
				wood[0][object[0]][object[1]+1] == ' ' && wood[1][object[0]][object[1]+1] == ' '))
				cleanScreen(2, HEIGHT+1);
			else{
				if (object[0] == HEIGHT || object[0] == -1)
					printBoundary();
				else
					changeScreen(object[0], object[1], '=');
				changeScreen(object[0], object[1]+1, 'O');
				object[1]++;
			}
		}
		else if (ch == 'p' || ch == 'P'){
			int i;
			for (i = 0; i < HEIGHT; i++){
				if (delay[i] > MIN)
					delay[i] /= 1.5;
			}
		}
		else if (ch == 'o' || ch == 'O'){
			int i;
			for (i = 0; i < HEIGHT; i++){
				if (delay[i] < MAX)
					delay[i] *= 1.5;
			}
		}
		else if (ch == '\e'){
			getch();
			ch = getch();
			if (ch == 'A')
				ch = 'W';
			else if (ch == 'B')
				ch = 'S';
			else if (ch == 'C')
				ch = 'D';
			else if (ch == 'D')
				ch = 'A';
			goto here;
		}
		else{}
		refix();
		pthread_mutex_unlock(&print_mutex);
	}	
}

void* listenKeyboard(){
    int avgdelay = 0, i;
    for (i = 0; i < HEIGHT; i++)
        avgdelay += delay[i];
    avgdelay /= HEIGHT;
	while (1){
		if (kbhit()) {
			/* do some work */
			pthread_cond_signal(&hit_cv);
		}
	}
}

void* printWood(void* linenum){
	int i, flag = 0;
	long int line = (long int) linenum;
	while(1){
		for (i = 0; i < LENGTH; i++){
			if (i == LENGTH-1){
				if (line % 2)
					wood[flag][line][i] = wood[(flag+1)%2][line][0];
				else
					wood[flag][line][0] = wood[(flag+1)%2][line][i];
			}
			else{
				if (line % 2)
					wood[flag][line][i] = wood[(flag+1)%2][line][i+1];
				else
					wood[flag][line][i+1] = wood[(flag+1)%2][line][i];
			}
		}
		if (line == object[0]){
			if (line % 2)
				(object[1] == 0) ? cleanScreen(2, line) : object[1]--;
			else
				(object[1] == LENGTH-1) ? cleanScreen(2, line) : object[1]++;
		}
		cleanWood(wood[(flag+1)%2][line]);
		pthread_mutex_lock(&print_mutex);
		gotoXY(line+3, 1);
		printf("%*s\r\n", LENGTH, wood[flag][line]);
		flag = (flag == 0) ? 1 : 0;
		// fix the pointer 
		refix();
		pthread_mutex_unlock(&print_mutex);
		usleep(delay[line]);
	}
}

void *addScore(){
	int avgdelay = 0, i;
	while(1){
		for (i = 0; i < HEIGHT; i++)
			avgdelay += delay[i];
		avgdelay /= HEIGHT;
		if (object[0] < HEIGHT && object[0] > - 1 && score < INT_MAX)
			score += rand() % (((MAX-avgdelay)/(MIN/5)) + 1) + 1;
		usleep(avgdelay);
	}
}

void initialize(){
	long int i = 0, line = 0;
	for(line = 0; line < HEIGHT; line++){
		delay[line] = rand() % MODULO + REMAINDER;
		for (i = 0; i < LENGTH; i++){
			if ( (diff == 'D') ? i % (rand() % (LENGTH/2) + 1) > rand() % GAP+1 : 
					((WOODLEN+GAP) == 0 ? 1 : i % (WOODLEN+GAP) < GAP))
				wood[0][line][i] = wood[1][line][i] = ' ';
			else
				wood[0][line][i] = wood[1][line][i] = '=';
			boundary[i] = '|';
			clean[i] = ' ';
		}
		wood[0][line][i] = wood[1][line][i] = '\0';
		boundary[i] = '\0';
	}
	if (width >= LENGTH+2)
		for (i = LENGTH; i < width; i++)
			clean[i] = ' ';
	clean[i] = '\0';
	system("clear");
	gotoXY(HEIGHT+4, 1);
    printFormat();
    printBoundary();
	gotoXY(HEIGHT+3, LENGTH/2+1);
	printf("%c\r\n", 'O');
	object[0] = HEIGHT;
	object[1] = LENGTH/2;
}

int frog() {
	long int i = 0;
	pthread_mutex_init(&print_mutex, NULL);
	pthread_cond_init (&hit_cv, NULL);
	reset_terminal_mode();	
	set_conio_terminal_mode();
	initialize();
	srand(time(NULL));
	pthread_create(&threads[HEIGHT+2], NULL, addScore, NULL);
	for (i = 0; i < HEIGHT; i++)
		pthread_create(&threads[i], NULL, printWood, (void *) i);
	pthread_create(&threads[i], NULL, listenKeyboard, NULL);
	pthread_create(&threads[i+1], NULL, moveObject, NULL);
	/* 2. Wait for all threads to complete */
	for (i = 0; i < HEIGHT+2; i++)
		pthread_join(threads[i], NULL);
	/* 3. Clean up and exit */
	pthread_mutex_destroy(&print_mutex);
	pthread_cond_destroy(&hit_cv);
	pthread_exit (NULL);
	return 0;
}

int getDigit(long long int num){
	int digit = 1;
	while((digit/=10) >= 1)
		digit++;
	return digit;
}

void alloc(){
    int i, j;
    threads = (pthread_t*) malloc(sizeof(pthread_t)*(HEIGHT+3));
    boundary = (char*) malloc(sizeof(char)*(LENGTH+1));
    for (i = 0; i < 2; i++){
        wood[i] = (char**) malloc(sizeof(char*)*HEIGHT);
        for (j = 0; j < HEIGHT; j++)
            wood[i][j] = (char*) malloc(sizeof(char)*(LENGTH+2));
    }
    delay = (int*) malloc(sizeof(int)*HEIGHT);
	width = strlen(name) + 40 + getDigit(highscore);
	clean = (LENGTH+2 > width) ? (char*) malloc (sizeof(char)*(LENGTH+2)) : 
			(char*) malloc(sizeof(char)*width);
}

int main(){
	int ch;
	set_conio_terminal_mode();
	system("clear");
	printf( "River Frog                       \r\n"
			"                 made by Slighten\r\n");
	printFormat();
	printf( "\r\nPRESS ANY KEY TO START\r\n"
			"D: EXTREME MODE\r\n"
			"S: CONFIGURE PARAMETERS MANUALLY\r\n");
	while (1){
		if (kbhit()){
			ch = getch();
			if (ch == 'D' || ch == 'd'){
				diff = 'D';
			}
			else if (ch == 'S' || ch == 's'){
				reset_terminal_mode();
                printf("Set up map width(-1 for default): ");
                scanf("%d", &LENGTH);
                LENGTH = LENGTH == -1 ? 40 : LENGTH;
                printf("Set up map height(-1 for default): ");
                scanf("%d", &HEIGHT);
                HEIGHT = HEIGHT == -1 ? 15 : HEIGHT;
		      	printf("Set up wood length(-1 for default): ");
                if (scanf("%d", &WOODLEN) == 1)
                    WOODLEN = WOODLEN == -1 ? 4 : WOODLEN;
                else
                    diff = 'D';
		        printf("Set up gap length(-1 for default): ");
                if (scanf("%d", &GAP) == 1)
                    GAP = GAP == -1 ? 4 : GAP;
                else
                    diff = 'D';
				GAP = !WOODLEN ? 3 : GAP;
			}
			else if (ch == 'Q' || ch == 'q' || ch == 3)
					exit(EXIT_SUCCESS);
			break;
		}
	}
    alloc();
	frog();
	return 0;
}
