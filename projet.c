#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <gtk/gtk.h>
#include <gtk/gtkeventbox.h>
#include <glib.h>

// Il est généralement recommandé d'utiliser des constantes pour les valeurs limites de l'adresse IP et du masque.
// Cela permet de modifier facilement ces limites en un seul endroit si besoin.
#define MIN_IP_VALUE 0
#define MAX_IP_VALUE 255
#define MIN_MASK_VALUE 8
#define MAX_MASK_VALUE 30
//////////////////////////////////------------------GLOBAL VARIABLE----------------------------////////////


int ip[5];
char sql[100];


//////////////////////////////////------------------INITIALISATION DE LA DB--------------------////////////
sqlite3 *db;

int InitDb(){
	printf("Opening db \n");
	int rc = sqlite3_open("IPCatalog.db", &db);
	if (rc != SQLITE_OK) {
  		printf("Failed to open database: %s\n", sqlite3_errmsg(db));
  		sqlite3_close(db);
  		exit(1);
	} 
	else {
  		printf("Successfully opened database.\n");
	}
}
////////////////////////////////////---------FONCTION QUI ENVOIE LES IPS FAITES PAR L'USER DANS LA DB ---------------/////
void EnvoiIp(int ip[5], char type[10]) {
    	int id = -1;

    	// Recherche d'un ID libre dans la table
    	sqlite3_stmt *stmt;
    	int rc = sqlite3_prepare_v2(db, "SELECT MIN(t1.id + 1) FROM ip_addresses AS t1 LEFT JOIN ip_addresses AS t2 ON t1.id + 1 = t2.id WHERE t2.id IS NULL AND t1.id < (SELECT MAX(id) FROM ip_addresses);", -1, &stmt, 0);
    	if (rc != SQLITE_OK){
        	fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    	}
    	rc = sqlite3_step(stmt);
    	if (rc == SQLITE_ROW){
        	id = sqlite3_column_int(stmt, 0);
    	}
    	sqlite3_finalize(stmt);
    // Si aucun ID libre n'a été trouvé, on utilise le MAX(id) + 1
    	if (id == -1||id==0){
        	rc = sqlite3_prepare_v2(db, "SELECT MAX(id) FROM ip_addresses;", -1, &stmt, 0);
        	if (rc != SQLITE_OK) {
            		fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        	}
        	rc = sqlite3_step(stmt);
        	int last_id = rc == SQLITE_ROW ? sqlite3_column_int(stmt, 0) : 0;
        	sqlite3_finalize(stmt);
        	id = last_id + 1;
    	}

    	printf("New ID: %d\n", id);

    	// Ajout de l'adresse IP dans la table
    	char sql[256];
    	sprintf(sql, "INSERT INTO ip_addresses (id, address, mask, type) VALUES (%d, '%d.%d.%d.%d', %d, '%s');", id, ip[0], ip[1], ip[2], ip[3], ip[4], type);
    	rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    	if (rc != SQLITE_OK) {
        	fprintf(stderr, "Failed to insert IP address into database: %s\n", sqlite3_errmsg(db));
    	}
 	else {
        	printf("IP address added to the database :)\n");
    	}
}


//////////////////////////////////-----------------------------------------------------------------------------////////////
// Cette fonction ne renvoie rien, elle modifie directement le tableau ip et la variable masque.
void genereIp(int a, int b, int c, int d, int masque){
	int rep,verifscanf,tentatives=0;
	char type[10];
    	printf("Une adresse IP est composée de 4 nombres chacun entre 0 et 255.\n");
    	do {
		printf("Le premier nombre de votre IP sera: ");
        	verifscanf=scanf("%d", &a);
        	fflush(stdout);
        	while(getchar()!='\n');
            	tentatives++;
		if(tentatives==3){
			printf("Trop de tentatives invalides, veuillez réessayer plus tard.\n");
        		return;
		}
    	}while((a<MIN_IP_VALUE||a>MAX_IP_VALUE||verifscanf!=1)&&tentatives<3);
    	tentatives=0;
	ip[0] = a;
        do {
		printf("Le deuxième nombre de votre IP sera: ");
        	verifscanf=scanf("%d", &b);
        	fflush(stdout);
       		while(getchar()!='\n');
		tentatives++;
        	if(tentatives==3){
			printf("Trop de tentatives invalides, veuillez réessayer plus tard.\n");
		}
    	}while((b<MIN_IP_VALUE||b>MAX_IP_VALUE||verifscanf!=1)&&tentatives<3);
    	tentatives=0;
	ip[1] = b;
        do {
                printf("Le troisième nombre de votre IP sera: ");
                verifscanf=scanf("%d", &c);
                fflush(stdout);
                while(getchar()!='\n');
                tentatives++;
                if(tentatives==3){
                        printf("Trop de tentatives invalides, veuillez réessayer plus tard.\n");
                }
        }while((c<MIN_IP_VALUE||c>MAX_IP_VALUE||verifscanf!=1)&&tentatives<3);
        tentatives=0;
    	ip[2] = c;
        do {
        	printf("Le troisième nombre de votre IP sera: ");
                verifscanf=scanf("%d", &c);
                fflush(stdout);
                while(getchar()!='\n');
                tentatives++;
                if(tentatives==3){
                        printf("Trop de tentatives invalides, veuillez réessayer plus tard.\n");
                }
        }while((d<MIN_IP_VALUE||d>MAX_IP_VALUE||verifscanf!=1)&&tentatives<3);
        tentatives=0;
    	ip[3] = d;
    	if (a <= 223) {
        	printf("Voulez-vous définir vous-même un masque? (1 pour oui le reste pour non) ");
        	while(scanf("%d", &rep)!=1){
        		printf("Caractère invalides. Veuillez refaire s'il-vous-plait: ");
			while(getchar()!='\n');
		}
        if (rep == 1){
            	if (a <= 127){
                	do{
                    		printf("Veuillez entrer votre masque(entre 8 et 15): ");
                    		verifscanf=scanf("%d", &masque);
                    		fflush(stdout);
				while(getchar()!='\n');
				tentatives++;
				if(tentatives==3){
					printf("Trop de tentatives invalides. Veuilez réessayer plus tard.\n");
					return;
				}
                	}while((masque<MIN_MASK_VALUE||masque>15||verifscanf!=1)&&tentatives<3);
			tentatives=0;
            	}
                else if (a > 127 || a <= 191){
                	do{
                    		printf("Veuillez entrer votre masque(entre 16 et 23): ");
                    		verifscanf=scanf("%d", &masque);
                    		fflush(stdout);
				while(getchar()!='\n');
				tentatives++;
				if(tentatives==3){
					printf("Trop de tentatives invalides. Veuillez réessayer plus tard.\n");
					return;
				}
                	}while((masque<16||masque>23||verifscanf!=1)&&tentatives<3);
            		tentatives=0;
		}
            	else{
                	do{
              			printf("Veuillez entrer votre masque de sous-réseau (entre 16 et 23) : ");
		            	verifscanf=scanf("%d", &masque);
				fflush(stdout);
				while(getchar()!='\n');
				tentatives++;
				if(tentatives==3){
					printf("Trop de tentatives invalides. Veuillez réessayer plus tard.\n");
					return;
				}
			}while((masque<24||masque>30||verifscanf!=1)&&tentatives<3);
			tentatives=0;
		}
	}
	else{
		if(a<=127){
			masque=8;
		}
		else if(a>127 || a<=191){
			masque=16;
		}
		else if(a>191||a<=223){
			masque=24;
		}
	}
	}
	else{
		printf("Impossible d'avoir un masque pour ces adresses ip\n");
		masque=0;
	}
	if(a==127&&b==0&&c==0&&d==1){
		sprintf(type,"spéciale",NULL);
	}
	else if((a==10)||(a==172&&(b>=16||b<=31))||(a==192&&b==168)){
		sprintf(type,"privée",NULL);
	}
	else{
		sprintf(type,"publique",NULL);
	}
	ip[4]=masque;
	EnvoiIp(ip,type);
}



//////////////////////////////////-----------------------------------------------------------------------------////////////
void binaire(int ipBin[4][8]) {
    	for (int j = 0; j < 4; j++){
        	int n = ip[j];
        	for (int i = 7; i >= 0; i--){
            		ipBin[j][7-i] = (n >> i) & 1;
        	}
    	}
}
//////////////////////////////////-----------------------------------------------------------------------------////////////
void hexadecimal(char ipHexa[9]){
    	char hexa[3];
    	for (int i = 0; i < 4; i++){
        	sprintf(hexa, "%02x", ip[i]);
        	strcat(ipHexa, hexa);
    	}
}

void filtrage(){
///////////----------------VAR POUR STOCKER LE MASK CHOISI INIT A 0 -----------------////////////////////
	int masque = 0;
	int verifscanf;
	char sqlQuery[1000];

	do{
		printf("Choisissez un masque \n");
		verifscanf=scanf("%d", &masque);
		sprintf(sqlQuery, "SELECT address FROM ip_addresses WHERE mask = %d",masque);
		if (masque<8||masque>30||verifscanf!=1){
			printf("Masque incorrect\n");
			while(getchar()!='\n');
		}

	}
	while (masque < 8 || masque > 30);
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL);
	if (rc != SQLITE_OK){
		printf("Impossible d'executer la requete %s\n", sqlite3_errmsg(db));
		sqlite3_free(NULL);
	}
	else{
		printf("Adresse IP pour le masque: %d\n",masque);
//-------------PRINTF DE LOUTPUT DE LA DB DANSLE TERMINAL EN ATTENDANT DE SETUP GTK------------//////
		while (sqlite3_step(stmt) == SQLITE_ROW){
			printf("%s\n", sqlite3_column_text(stmt, 0));
        	}
	}
}
//////////////////////////////////--------------------------POPUP POUR ASK L'ID A CHERCHER DANS LA DB -----------------------------////////////

int IdDialogBox(char *user_input_str){
 // Create a new dialog box
	GtkWidget *dialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (dialog), "Entrer un ID");
	// Add a message label to the dialog box
	GtkWidget *label = gtk_label_new ("ENTREZ UN ID");
	GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	gtk_box_pack_start (GTK_BOX (content_area), label, FALSE, FALSE, 0);
	// Add a text entry widget to the dialog box
	GtkWidget *entry = gtk_entry_new ();
	gtk_box_pack_start (GTK_BOX (content_area), entry, FALSE, FALSE, 0);
	// Add an "Enter" button to the dialog box
	GtkWidget *enter_button = gtk_dialog_add_button (GTK_DIALOG (dialog), "Enter", GTK_RESPONSE_ACCEPT);
	gtk_widget_set_can_default (enter_button, TRUE);
	gtk_widget_grab_default (enter_button);
	// Show the dialog box and wait for a response
	gtk_widget_show_all (dialog);
	int response = gtk_dialog_run (GTK_DIALOG (dialog));
	// Check if the user clicked the "Enter" button
	if (response == GTK_RESPONSE_ACCEPT){
	    	// Get the user input from the text entry widget
		user_input_str = gtk_entry_get_text (GTK_ENTRY (entry));
     		int user_input = atoi(user_input_str);
		gtk_widget_destroy(dialog);
		return user_input;
   	}
}

//////////////////---------------------------------------------------------/////////////////////////////////

int RequeteId(char *user_input_str){
	int user_input;
	user_input = IdDialogBox(user_input_str);
	char Sql_Query[100];
	printf("Obtention de l'adresse ayant l'ID %d", user_input);
	sprintf(Sql_Query, "SELECT address, mask FROM ip_addresses WHERE id = %d;",user_input);
	sqlite3_stmt* stmt = NULL;
	int rc = sqlite3_prepare_v2(db, Sql_Query, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
        	printf("Impossible d'executer la requete %s\n", sqlite3_errmsg(db));
        	sqlite3_free(NULL);
    	}
    	else{
        	printf("Voici l'IP qui a l'ID %d:\n", user_input);
        	while (sqlite3_step(stmt) == SQLITE_ROW){
            		printf("IP: %s, Masque: %d\n", sqlite3_column_text(stmt, 0), sqlite3_column_int(stmt, 1));
        	}
    	}
    	sqlite3_finalize(stmt);

	//sqlite3_finalize(db);
	return user_input;
}
//////////////////-----------------BOUTON "AFFICHER UNE ADRESSE"--------/////////////////////////////////

void afficherAdresse(GtkWidget *widget, gpointer data,char *user_input_str) 
{
	int user_input;
	int a,b,c,d,masque;
    	int choix2 = 0;
    	int ipBin[4][8] = {{0}}; // Initialise les valeurs a 0
    	char ipHexa[9] = {0}; // Initialise les valeurs a 0
    	GtkWidget *dialog;
	const gchar *message;
    	dialog = gtk_dialog_new_with_buttons("Afficher une adresse IP", NULL, GTK_DIALOG_MODAL,
                                         "Sous sa forme binaire", 1, "Sous sa forme decimale", 2,
                                         "Sous sa forme hexadecimale", 3,NULL);
    	gtk_widget_show_all(dialog);
    	switch(gtk_dialog_run(GTK_DIALOG(dialog))){
        	case 1:
       //VERIF A METTRE ICI SI USER INPUT NEST PAS NUL, ALORS CONTINUE, SINON REPROMPT LA FENETRE POUR CHOISIR L'ID A FILTRER
 
        //SI L'ID NEST PAS NUL, ET SI LA DB CONTIENT L'ID DE L'INPUT, ALORS ENVOIE DE LA REQUETE SQL :
        //POPUP POUR CHOISIR L'ID A CHERCHER - RequeteId lance dabord la fonction IdDialogBox avant d'operer
			user_input = RequeteId(user_input_str);
			binaire(ipBin);
            		message = "L'adresse IP en binaire est : ";
        		for (int i = 0; i < 4; i++){
                		for (int j = 0; j < 8; j++) {
                    			message = g_strdup_printf("%s%d", message, ipBin[i][j]);
                		}
                	message = g_strdup_printf("%s.", message);
            		}
        		break;

		case 2:
                //POPUP POUR CHOISIR L'ID A CHERCHER VOIR IdDialogBox()
        		user_input = RequeteId(user_input_str);
          		message = g_strdup_printf("L'adresse IP est la suivante : %d.%d.%d.%d de masque %d", ip[0], ip[1], ip[2], ip[3], ip[4]);
        		break;

        	case 3:
                //POPUP POUR CHOISIR L'ID A CHERCHER 
        		user_input = RequeteId(user_input_str);
        		hexadecimal(ipHexa);
            		message = g_strdup_printf("L'adresse IP en hexadecimal est : %s", ipHexa[9]);
            		break;
		default:
			break;
    	}
     	GtkWidget *dialog2 = gtk_message_dialog_new(GTK_WINDOW(dialog), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);
    	gtk_window_set_title(GTK_WINDOW(dialog2), "Adresse IP");
    	gtk_dialog_run(GTK_DIALOG(dialog2));
    	gtk_widget_destroy(dialog2);
    	gtk_widget_destroy(dialog);
}

void on_button1_clicked(GtkWidget *widget, gpointer data) {
    	int a, b, c, d, masque;
    	genereIp(a, b, c, d, masque);
}


void on_button2_clicked(GtkWidget *widget, gpointer data, char *user_input_str){
	afficherAdresse(widget,NULL,user_input_str);
}

void on_button3_clicked(GtkWidget *widget, gpointer data){
	GtkWidget *dialog, *label, *content_area;
	filtrage();
}

void on_button4_clicked(GtkWidget *widget, gpointer data) {
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Au revoir");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}


void menu() {
	GtkWidget *window;
	GtkWidget *vbox, *hbox;
	GtkWidget *button1, *button2, *button3, *button4;
	GtkWidget *label;
	GtkWidget *image;

	gtk_init(NULL, NULL);
	

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Catalogue d'Adresse IP");

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	button1 = gtk_button_new_with_label("Générer une adresse IP");
	g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);

	button2 = gtk_button_new_with_label("Afficher une adresse IP");
	g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);

	button3 = gtk_button_new_with_label("Filtrer une adresse IP via le masque");
	g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), NULL);
	
	button4 = gtk_button_new_with_label("Quitter");
	g_signal_connect(button4, "clicked", G_CALLBACK(on_button4_clicked), NULL);

	gtk_box_pack_start(GTK_BOX(hbox), button1, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button2, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), button3, TRUE, TRUE, 0);

	label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), "<span font_desc=\"24.0\">Menu principal</span>");
	image = gtk_image_new_from_file("network.jpeg");
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), image, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(window), vbox);

	gtk_widget_show_all(window);

	gtk_main();
}
//////////////////////////////////-----------------------------------------------------------------------------////////////
//INITIALIZATION DE LA DB EN MEME TEMPS QUE LE MENU POUR LA PERFORMANCE DE L'APP (DB DEJA PRETE QUAND NEEDED)
int main(int argc, char *argv[]) {
	InitDb();
	menu();
	return EXIT_SUCCESS;
}
