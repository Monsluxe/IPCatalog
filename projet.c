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
int InitDb()
{
printf("Opening db \n");
// pointer to SQLite database connection object
int rc = sqlite3_open("IPCatalog.db", &db);
if (rc != SQLITE_OK) {
  printf("Failed to open database: %s\n", sqlite3_errmsg(db));
  sqlite3_close(db);
  exit(1);
  
} else {
  printf("Successfully opened database.\n");
}
}
////////////////////////////////////---------FONCTION QUI ENVOIE LES IPS FAITES PAR L'USER DANS LA DB ---------------/////
	void EnvoiIp(int ip[5])
	{
	printf("Adding address to the db");
	sprintf(sql, "INSERT INTO ip_addresses (address, mask) VALUES ('%d.%d.%d.%d', %d);", ip[0], ip[1], ip[2], ip[3], ip[4]);
	int rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
	if (rc != SQLITE_OK) 
	{
    printf("Failed to insert IP address into database: %s\n", sqlite3_errmsg(db));
	}
	else
	{
		printf("ip added to the db :)");
	}
	//sqlite3_finalize(db);
	}

//////////////////////////////////-----------------------------------------------------------------------------////////////
// Cette fonction ne renvoie rien, elle modifie directement le tableau ip et la variable masque.
void genereIp(int a, int b, int c, int d, int masque){
	int rep;
    printf("Une adresse IP est composée de 4 nombres chacun entre 0 et 255.\n");
    printf("Le premier nombre de votre IP sera: ");
    do {
        scanf("%d", &a);
        fflush(stdout);
        if (a < MIN_IP_VALUE || a > MAX_IP_VALUE) {
            printf("Nombre invalide, veuillez refaire s'il-vous-plait: ");
        }
    } while (a < MIN_IP_VALUE || a > MAX_IP_VALUE);
    ip[0] = a;
    printf("Le deuxiéme nombre de votre IP sera: ");
    do {
        scanf("%d", &b);
        fflush(stdout);
        if (b < MIN_IP_VALUE || b > MAX_IP_VALUE) {
            printf("Nombre invalide, veuillez refaire s'il-vous-plait: ");
        }
    } while (b < MIN_IP_VALUE || b > MAX_IP_VALUE);
    ip[1] = b;
    printf("Le troisiéme nombre de votre IP sera: ");
    do {
        scanf("%d", &c);
        fflush(stdout);
        if (c < MIN_IP_VALUE || c > MAX_IP_VALUE) {
            printf("Nombre invalide, veuillez refaire s'il-vous-plait: ");
        }
    } while (c < MIN_IP_VALUE || c > MAX_IP_VALUE);
    ip[2] = c;
    printf("Le dernier nombre de votre IP sera: ");
    do {
        scanf("%d", &d);
        fflush(stdout);
        if (d < MIN_IP_VALUE || d > MAX_IP_VALUE) {
            printf("Nombre invalide, veuillez refaire s'il-vous-plait: ");
        }
    } while (d < MIN_IP_VALUE || d > MAX_IP_VALUE);
    ip[3] = d;
    if (a <= 223) {
        printf("Voulez-vous définir vous-même un masque? (1 pour oui le reste pour non) ");
        scanf("%d", &rep);
        fflush(stdout);
        if (rep == 1) {
            if (a <= 127) {
                do {
                    printf("Veuillez entrer votre masque(entre 8 et 15): ");
                    scanf("%d", &masque);
                    fflush(stdout);
                } while (masque < MIN_MASK_VALUE || masque > 15);
            } else if (a > 127 || a <= 191) {
                do {
                    printf("Veuillez entrer votre masque(entre 16 et 23): ");
                    scanf("%d", &masque);
                    fflush(stdout);
                } while (masque < 16 || masque > 23);
            } else {
                do {
              		printf("Veuillez entrer votre masque de sous-réseau (entre 16 et 23) : ");
		            scanf("%d", &masque);
        	}
        	 while (masque < 16 || masque > 23);
		}
	}
	else
	{
		if(a<=127)
		{
			masque=8;
		}
		
		else if(a>127 || a<=191)
		{
			masque=16;
		}
		
		else if(a>191||a<=223)
		{
			masque=24;
		}
	}
}
	else
	{
		printf("Impossible d'avoir un masque pour ces adresses ip\n");
		masque=0;
	}
	
	ip[4]=masque;
	EnvoiIp(ip);
}



//////////////////////////////////-----------------------------------------------------------------------------////////////
void binaire(int ipBin[4][8]) {
    for (int j = 0; j < 4; j++) {
        int n = ip[j];
        for (int i = 7; i >= 0; i--) {
            ipBin[j][7-i] = (n >> i) & 1;
        }
    }
}
//////////////////////////////////-----------------------------------------------------------------------------////////////
void hexadecimal(char ipHexa[9]) {
    char hexa[3];
    for (int i = 0; i < 4; i++) {
        sprintf(hexa, "%02x", ip[i]);
        strcat(ipHexa, hexa);
    }
}
//////////////////////////////////-----------------------------------------------------------------------------////////////
void afficherAdresse(GtkWidget *widget, gpointer data) {
	int a,b,c,d,masque;
    int choix2 = 0;
    int ipBin[4][8] = {{0}}; // Initialise les valeurs à 0
    char ipHexa[9] = {0}; // Initialise les valeurs à 0
    GtkWidget *dialog;
	const gchar *message;
    dialog = gtk_dialog_new_with_buttons("Afficher une adresse IP", NULL, GTK_DIALOG_MODAL,
                                         "Sous sa forme binaire", 1, "Sous sa forme décimale", 2,
                                         "Sous sa forme hexadécimale", 3,NULL);
    gtk_widget_show_all(dialog);
    switch(gtk_dialog_run(GTK_DIALOG(dialog))) {
        case 1:
            binaire(ipBin);
            message = "L'adresse IP en binaire est : ";
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 8; j++) {
                    message = g_strdup_printf("%s%d", message, ipBin[i][j]);
                }
                message = g_strdup_printf("%s.", message);
            }
            break;

        case 2:
            message = g_strdup_printf("L'adresse IP est la suivante : %d.%d.%d.%d de masque %d", ip[0], ip[1], ip[2], ip[3], ip[4]);
            break;

        case 3:
		hexadecimal(ipHexa);
            message = g_strdup_printf("L'adresse IP en hexadécimal est : %s", ipHexa);
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

void on_button2_clicked(GtkWidget *widget, gpointer data) {
    afficherAdresse(widget, NULL);
}

void on_button1_clicked(GtkWidget *widget, gpointer data) {
    int a, b, c, d, masque;
    genereIp(a, b, c, d, masque);
}

////////-----------------------------------PREMIERE BRIQUE DE FILTRAGE PAR MASQUE---------------------/////
void on_button3_clicked(GtkWidget *widget, gpointer data)
	{
		GtkWidget *dialog, *label, *content_area;
///////////----------------VAR POUR STOCKER LE MASK CHOISI INIT A 0 -----------------////////////////////
		int mask = 0;
    
		printf("choose a mask\n");
	
		scanf("%d", &mask);
		
			if (mask < 8 || mask > 32)
			 
				{
	
					sprintf(sql, "SELECT address FROM ip_addresses WHERE mask = %d",mask);

				}
				
		int rc = sqlite3_exec(db, sql, NULL, 0, NULL);

		if (rc != SQLITE_OK) 
		{
			printf("Failed to execute query: %s\n", sqlite3_errmsg(db));
			sqlite3_free(NULL);
		} 
		else 
		{
			printf("Query executed successfully.\n");
			printf("Mask: %d\n",mask);
		}
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
	gtk_window_set_title(GTK_WINDOW(window), "Menu");

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
	image = gtk_image_new_from_file("network.jpg");

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
