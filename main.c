#include <SDL2/SDL.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "sdl_helper/function.h"
#include "sdl_helper/text_functions.h"
#include "sdl_helper/audio_functions.h"
#include "sdl_helper/constants.h"

                                                                        //MUSIQUE
int music_on=0;
int music_bg=0;

                                                                        //DIFFERENTES PAGES
int navigation=1;
int gameLoop=0;
int menu=1;//lance le menu
int start_game = 1;
int echap=0;
int win=0;
int lose=0;
int rules=0;

                                                                         //BALLE ET REBOND
int x= 450;//position balle
int y= 700;//position balle
int v_x=2;//Inverse le sens de la balle
int v_y=2;//Inverse le sens de la balle
int nb_vie=3;//nombre de vie

int balle(){
    changeColor(60,255,234);
    drawCircle(x,y,20);
}
void rebond() {
    // Mise à jour de la position
    x += v_x;
    y += v_y;
    // Vérification collision avec les murs
    if (y > 980 && v_y > 0) { // Collision avec le bas
        v_y = -v_y;
        y = 980 - (y - 980); 
        nb_vie-=1;
    }
    if (y <= 20 && v_y < 0) { // Collision avec le haut
        v_y = -v_y;
        y = 20 + (20 - y); 
    }
    if (x >= 1180 && v_x > 0) { // Collision avec la droite
        v_x = -v_x;
        x = 1180 - (x - 1180); 
    }
    if (x <= 20 && v_x < 0) { // Collision avec la gauche
        v_x = -v_x;
        x = 20 + (20 - x); 
    }
}


                                                                        //RAQUETTE ET REBOND
int x_raquette=535;
int y_raquette=850;
int largeur_raquette=130;
bool clignote=false;//pour le bonus
int temps_clignote = 150;
int frame_clignote;
int q_press;//déplace la raquette à gauche
int d_press;//déplace la raquette à droite

void raquette(){
    changeColor(255,255,255);
    if (clignote==true){
        if (frame_clignote>=6 && frame_clignote <=22){
            drawRect(x_raquette,y_raquette,largeur_raquette,20);
        }                    
        frame_clignote++;
        if (frame_clignote==40){
            frame_clignote=0;
        }

    } else {
        drawRect(x_raquette, y_raquette, largeur_raquette, 20);
    }
}
void rebond_raquette() { // rebond raquette balle 
    if (y + 20 >= y_raquette && y + 20 <= y_raquette + 20 && x >= x_raquette && x <= x_raquette + largeur_raquette && v_y > 0) {
        // Inverser la direction verticale
        v_y = -v_y;
    }
}

void move_raquette(){
    if(q_press){
        x_raquette=x_raquette-5; // Permet de déplacer la raquette à gauche
    }
    if(d_press){
        x_raquette=x_raquette+5; // Permet de déplacer la raquette à gauche
    }
    if(x_raquette<0){
        x_raquette=0;
    }
     if(x_raquette + largeur_raquette>1200){
        x_raquette=1200-largeur_raquette;
    }
}
                                                                        //BONUS ET VIE
int vitesse_bonus=1; // Permet de faire bouger le bonus
int bonus_active=0; // État du bonus (actif ou non)
int tableau[5];
int nb_bonus;

                                                                        //TABLEAU BRIQUE
int grille[5][6];//tableau de briques
int init_x;// Remplie les colonnes de 1
int init_y;// Remplie les lignes de 1
int ligne; //Affiche les briques
int colonne;//Affiche les briques
int i; //Parcours la grille
int j;//Parcours la grille
int largeur_brique = 180;
int hauteur_brique = 80;
int x_brique;//coordonées
int y_brique;//Coordonnées
int x_brique_deux;
int y_brique_deux;
int largeur_ecran_brique=54;
int hauteur_ecran_brique=50;
int largeur_carre_balle=50;
int hauteur_carre_balle=50;
int compteur_victoire=0;
                                                                        //COMPTEUR SCORE
int score=0;//augmente quand une brique est cassée
char compteur_brique[100];// stock le texte 
void compteur(){
    char* nb_zero = "";
        if (score< 10){
            nb_zero = "000";
        } else if (score < 100) {
            nb_zero = "00";
        } else if  (score < 1000){
            nb_zero = "0";
        }
    sprintf(compteur_brique, "%s%d", nb_zero, score );
    textDrawText(compteur_brique, 895, 1045, PressStart2P_52);
}

                                                                        //BONUS
typedef struct Bonus{
    int bonus_active;
    int bonus_x;
    int bonus_y;
    int type_bonus;
    int animeframe_arrow;
    int delay_gif_test;
} Bonus;

Bonus tableau_bonus[100];
bool check_bonus= false;
int temps_bonus=0;

void draw_bonus() {
    for(nb_bonus=0;nb_bonus<=100;nb_bonus++){
        if (tableau_bonus[nb_bonus].bonus_active == 1 ) {
            switch (tableau_bonus[nb_bonus].type_bonus){
                case 0 :
                    sprite(tableau_bonus[nb_bonus].bonus_x - 25 , tableau_bonus[nb_bonus].bonus_y - 25 , "assets/Bonus/heart_bonus.bmp");
                     if ((tableau_bonus[nb_bonus].bonus_x < x_raquette + largeur_raquette) &&  
                        (tableau_bonus[nb_bonus].bonus_x + 50 > x_raquette) &&            
                        (tableau_bonus[nb_bonus].bonus_y < y_raquette + 20) &&               
                        (tableau_bonus[nb_bonus].bonus_y + 50 > y_raquette) && nb_vie<3) {               
                            tableau_bonus[nb_bonus].bonus_active = 0; // Désactive le bonus
                            if (nb_vie <= 2) {
                                audioLoadAndPlay("sdl_helper/sound/upone.wav", -1);
                                nb_vie += 1; 
                            }
                        }
                        break;
                case 1 : 
                    if(tableau_bonus[nb_bonus].animeframe_arrow>=0){//affichage et animation bonus
                         if(tableau_bonus[nb_bonus].animeframe_arrow<=0){
                                tableau_bonus[nb_bonus].animeframe_arrow=4;
                            }//parcours les images des bonus
                            char arrowFrame[100];
                            sprintf(arrowFrame, "arrow_gif/arrown%d.bmp", tableau_bonus[nb_bonus].animeframe_arrow);
                            sprite(tableau_bonus[nb_bonus].bonus_x-40, tableau_bonus[nb_bonus].bonus_y-40,arrowFrame);
                            if(tableau_bonus[nb_bonus].delay_gif_test==0){
                                tableau_bonus[nb_bonus].animeframe_arrow--;  
                            }
                            tableau_bonus[nb_bonus].delay_gif_test+=1;
                            if (tableau_bonus[nb_bonus].delay_gif_test>=60){//Image tout les 15 frames
                                tableau_bonus[nb_bonus].delay_gif_test=0;
                            }
                        }
                     if ((tableau_bonus[nb_bonus].bonus_x < x_raquette + largeur_raquette) &&
                        (tableau_bonus[nb_bonus].bonus_x + 40 > x_raquette) &&
                        (tableau_bonus[nb_bonus].bonus_y < y_raquette + 20) &&
                        (tableau_bonus[nb_bonus].bonus_y + 40 > y_raquette) ) { //collision bonus 
                            tableau_bonus[nb_bonus].bonus_active = 0; // Désactive le bonus
                            if (largeur_raquette <= 180) {
                                    audioLoadAndPlay("sdl_helper/sound/upone.wav", -1);
                                    largeur_raquette += 50;
                                    temps_bonus=800;
                                }     
                                    temps_bonus = 800; // Réinitialise le temps du bonus pour recommencer à 500
                                    clignote = false; // Arrête le clignotement
                                    frame_clignote = 0; // Réinitialise le frame de clignotement      
                        }
                    break;  
            }
            if(tableau_bonus[nb_bonus].bonus_y>1000){
                 tableau_bonus[nb_bonus].bonus_active = 0;
            }
            tableau_bonus[nb_bonus].bonus_y += 1;
        }
    }   
}
void reset_bonus_longueur(){
    if (temps_bonus>0){
        temps_bonus--;
        if (temps_bonus<temps_clignote ){
             clignote=true;
        } 
    }
    if (temps_bonus==0) {
        clignote = false;  
        largeur_raquette = 130; // Restaure la taille par défaut
    }
}


                                                                        //GRILLE DE BRIQUE
int hasard_nb;// Permet de remplir ma grille de 1 ou de 2
int level;

void initialiser_grille() {//Remplit mon tableau de 1
    if (level==1){
        for (init_x=0; init_x<5; init_x++) {
            for (init_y=0; init_y<6; init_y++) {
                grille[init_x][init_y]=1;
            }
        }
    } else if (level==2){
        for (init_x=0; init_x<5; init_x++) {
            for (init_y=0; init_y<6; init_y++) {
                hasard_nb=(rand()%2)+1;//entre 1 et 2
                grille[init_x][init_y]=hasard_nb;
            }   
        }   
    }
}
      
int affichage_briques(){// Si il y a des 1 dans mon tableau alors je calcule les positions et affiche les briques.
  for(ligne=0; ligne<=5;ligne++){
    for(colonne=0; colonne<6;colonne++){
        if(grille[ligne][colonne]==1){
            x_brique= colonne* (largeur_brique+2);//+2 pour faire un espace entre les briques
            y_brique= ligne* (hauteur_brique+2);
            sprite(x_brique+largeur_ecran_brique,y_brique+ hauteur_ecran_brique,"assets/Briques/green_brique_dark.bmp");

        } else if(grille[ligne][colonne]==2) {
            x_brique= colonne* (largeur_brique+2);//+2 pour faire un espace entre les briques
            y_brique= ligne* (hauteur_brique+2);
            sprite(x_brique + largeur_ecran_brique,y_brique + hauteur_ecran_brique,"assets/Briques/red_brique2.bmp");
        } 
    }
  } 
}


int k=0;//Scan la grille bonus 
int collision(){
   for(i=0; i<=5;i++){
      for(j=0; j<6;j++) {
        bool collision_check = false;
        if(grille[i][j]==1 || grille[i][j]==2){
        //calcule position des briques
          x_brique_deux= j* largeur_brique+largeur_ecran_brique + 2;
          y_brique_deux= i* hauteur_brique+hauteur_ecran_brique + 2;
          
         //balle arrive sur le  coté gauche
            if(v_x>0 && x+20>=x_brique_deux && x+20<=x_brique_deux+largeur_brique && y-20>=y_brique_deux && y+20<=y_brique_deux + hauteur_brique){
                collision_check=true;
                printf("gauche\n");
                v_x=-v_x;
            }
           //balle arrive d'en haut
           else if( v_y>0 && x-20>=x_brique_deux && x+20<=x_brique_deux+ largeur_brique && y+20<=y_brique_deux+hauteur_brique && y+20>=y_brique_deux ){
                collision_check=true;
                printf("haut\n");
                v_y=-v_y;
            }  
          
          //balle arrive sur le coté droit
            else  if( v_x<0 && x-20<=x_brique_deux +largeur_brique && x-20>=x_brique_deux && y-20>=y_brique_deux && y+20<=y_brique_deux + hauteur_brique){
                collision_check=true;
                printf("droit\n");
                v_x=-v_x;
            }
          
          //balle arrive d'en bas
            else  if(  v_y<0 && x-20>=x_brique_deux && x+20<=x_brique_deux + largeur_brique && y-20>=y_brique_deux && y-20<=y_brique_deux+hauteur_brique){
                printf("bas\n");
                v_y=-v_y;
                collision_check=true;
                nb_bonus++;
                
            }
            if(collision_check!=true){
                if (sqrt(pow(y - (y_brique_deux + hauteur_brique), 2) + pow(x - (x_brique_deux + largeur_brique), 2)) <= 20) {
                    // Coin bas droit
                    if (v_x < 0 && v_y < 0) { 
                        v_x = -v_x; 
                        v_y = -v_y; 
                    } else if (v_x < 0) {
                        v_x = -v_x; // Inverse horizontalement
                    } else if (v_y < 0) {
                        v_y = -v_y; // Inverse verticalement
                    }
                    collision_check = true;
                    printf("Coin bas droit\n");
                } else if (sqrt(pow(y - (y_brique_deux + hauteur_brique), 2) + pow(x - x_brique_deux, 2)) <= 20) {
                    // Coin bas gauche
                    if (v_x > 0 && v_y < 0) { 
                        v_x = -v_x; 
                        v_y = -v_y; 
                    } else if (v_x > 0) {
                        v_x = -v_x; 
                    } else if (v_y < 0) {
                        v_y = -v_y; 
                    }
                    collision_check = true;
                    printf("Coin bas gauche\n");
                } else if (sqrt(pow(y - y_brique_deux, 2) + pow(x - (x_brique_deux + largeur_brique), 2)) <= 20) {
                    // Coin haut droit
                    if (v_x < 0 && v_y > 0) { 
                        v_x = -v_x; 
                        v_y = -v_y; 
                    } else if (v_x < 0) {
                        v_x = -v_x; 
                    } else if (v_y > 0) {
                        v_y = -v_y; 
                    }
                    collision_check = true;
                    printf("Coin haut droit\n");
                } else if (sqrt(pow(y - y_brique_deux, 2) + pow(x - x_brique_deux, 2)) <= 20) {
                    // Coin haut gauche
                    if (v_x > 0 && v_y > 0) { 
                        v_x = -v_x; 
                        v_y = -v_y; 
                    } else if (v_x > 0) {
                        v_x = -v_x; 
                    } else if (v_y > 0) {
                        v_y = -v_y; 
                    }
                    collision_check = true;
                    printf("Coin haut gauche\n");
                }
        }
        
            if (collision_check==true) {
                if (grille[i][j]==1){
                int hasard_bonus=rand()%100; //entre 0 et 99
                    if(hasard_bonus<20){
                        for (int k = 0; k < 100; k++) {
                            if (tableau_bonus[k].bonus_active == 0 ) {
                                tableau_bonus[k].bonus_x = x_brique_deux + (largeur_brique / 2);
                                tableau_bonus[k].bonus_y = y_brique_deux + (hauteur_brique / 2);
                                tableau_bonus[k].bonus_active = 1;
                                tableau_bonus[k].type_bonus=rand()%2;
                                break; //sort de la boucle
                            } 
                        }  
                    }
                }
                // Diminue la résistance de la brique ou la détruit

                grille[i][j] = (grille[i][j] == 2) ? 1 : 0;
                // Augmente le score
                score++;
                // Vérifie si la brique est complètement cassée
                if (grille[i][j] == 0) {
                    compteur_victoire++;//ajoute un point au compteur
                    audioLoadAndPlay("sdl_helper/sound/break.wav",-1);
                    }
                }
                collision_check = false;
            }
        }
    }
}
                                                                        //ECRAN MENU
void ecran_menu(){ // 
    sprite(0,0,"assets/BG/image5.bmp");
    actualize();
    usleep(100000 / FPS);
}

void ecran_rules(){
    clear;
    sprite(0,0,"assets/Règle/RULES.bmp");
    actualize();
}
                                                                        //ECRAN START (choix niveaux et pilote qui parle)
int tremblement;
int tremblement_max=20;
int etat_pilote=0;
int y_pilote;
void pilote_start(){
     switch(etat_pilote){
        case 0:// Pilote qui descend
            audioLoadAndPlay("sdl_helper/sound/son_fusee.wav",2);// Son pilote qui descend
            for (y_pilote = -731; y_pilote <= -240; y_pilote += 3) { // Grand incrément pour aller vite
                clear();
                sprite(0, 0, "assets/BG/space_base.bmp");
                sprite(-40 + rand() % tremblement_max, y_pilote, "assets/Pilote/pilote_vol.bmp");
                actualize();
            }
            for (y_pilote=-240; y_pilote <= 210; y_pilote += 2) { // Petit incrément pour ralentir
                clear();
                sprite(0, 0,"assets/BG/space_base.bmp");
                for( tremblement=0;tremblement<=5;tremblement++){ // fait trembler le pilote quand il descend
                    float random=rand()%tremblement_max;
                    sprite(-40 + random, y_pilote, "assets/Pilote/pilote_vol.bmp");
                }
                if (y_pilote % 20 == 0 && tremblement_max > 1) {//tout les 20
                    tremblement_max-=1;
                }
                actualize();
            }
            etat_pilote = 1; 
                break;
        case 1: // Pilote qui parle
            if(music_on==1){
                audioLoadAndPlay("sdl_helper/sound/voix.wav",5);
                music_on=1;
            }
            clear();
            sprite(0,0,"assets/BG/space_base_text.bmp");
            sprite(0,400,"assets/Pilote/new_pilote.bmp"); 
            actualize();
            break; 
        case 2:// Texte qui change EASY
            clear();
            sprite(0,0,"assets/BG/space_base_text_easy.bmp");
            sprite(0,400,"assets/Pilote/new_pilote.bmp"); 
            actualize();
            break;
        case 3:// Texte qui change HARD
            clear();
            sprite(0,0,"assets/BG/space_base_text_hard.bmp"); 
            sprite(0,400,"assets/Pilote/new_pilote.bmp"); 
            actualize();
            break;
    }
}
                                                                        //ECRAN LOSE
int etat_pilote_lose=0;
void ecran_lose(){
    switch(etat_pilote_lose){
        case 0 :
            audioLoadAndPlay("sdl_helper/sound/game_over_voice.wav",-1);
            audioLoadAndPlay("sdl_helper/sound/defeat2.wav",-1);
            
            for (int x_pilote = 1200; x_pilote > 655; x_pilote -= 4) {//pilote déçue qui arrive
                clear();
                sprite(0,0,"assets/Défaite/defeat.bmp");
                sprite(x_pilote, 400, "assets/Pilote/lose_pilot.bmp");
                actualize();
            }
            etat_pilote_lose+=1;
            break;
        case 1:
            clear();
            sprite(0,0,"assets/Défaite/defeat.bmp");
            sprite(655, 400, "assets/Pilote/lose_pilot.bmp");
            actualize();
            break;
       case 2: // background avec menu et replay
            clear();
            sprite(0,0,"assets/Défaite/defeat1.bmp");
            sprite(655, 400, "assets/Pilote/lose_pilot.bmp");
            actualize();
            break;
        case 3: // background avec menu et replay
            clear();
            sprite(0,0,"assets/Défaite/defeat2.bmp");
            sprite(655, 400, "assets/Pilote/lose_pilot.bmp");
            actualize();
            break;
    }  
}

                                                                        //ECRAN WIN
int etat_pilote_win=0;
int etat_pilote_win_hard=0;
int flash_win_easy=0;
int flash_win_hard=0;
float delay_gif=0;
int animeframe_win=140;
int music_on_victory_easy=1;
int music_on_victory=1;
void ecran_win_easy(){
    switch(etat_pilote_win){
        case 1 :
            for (int x_pilote = 1300; x_pilote > 550; x_pilote -= 3) {//pilote content qui arrive
                clear();
                sprite(0, 0, "assets/Victoire/victoire.bmp");
                sprite(x_pilote, -80, "assets/Pilote/happy_pilot.bmp");
                actualize();
            }
            etat_pilote_win+=1;
            break;
        case 2:
            if(music_on_victory_easy==1){//boucle sinon
                audioLoadAndPlay("sdl_helper/sound/victory.wav",-1);
                audioLoadAndPlay("sdl_helper/sound/music_victory.wav",-1);
                music_on_victory_easy+=1;
            }
            clear();
            flash_win_easy++;
            if(flash_win_easy>0 && flash_win_easy<80){
                    sprite(0, 0, "assets/Victoire/victoire.bmp");
                } else  {
                    sprite(0,0,"assets/Victoire/victoire2.bmp");
                }
                if(flash_win_easy>=120){
                    flash_win_easy=0;
            }
            sprite(550, -80, "assets/Pilote/happy_pilot.bmp");
            textChangeColor(255,255,255,0);
            actualize();
            break;
    }  
}
void ecran_win_hard(){
    switch( etat_pilote_win_hard){
        case 0 :
            for (int x_pilote = 1500; x_pilote > 550; x_pilote -= 3) {//pilote content qui arrive
                clear();
                sprite(0, 0, "assets/Victoire/victoire_hard1.bmp");
                sprite(x_pilote, -80, "assets/Pilote/happy_pilot.bmp");
                actualize();
            }
            etat_pilote_win_hard+=1;
            break;
        case 1:
            if(music_on_victory==1){
                audioLoadAndPlay("sdl_helper/sound/victory.wav",-1);//voix victory
                audioLoadAndPlay("sdl_helper/sound/music_victory.wav",-1);//voix victory
                audioLoadAndPlay("sdl_helper/sound/firework.wav",-1);
                music_on_victory+=1;
            }
            clear();
            flash_win_hard++;
            if(flash_win_hard>0 && flash_win_hard<80){
                    sprite(0, 0, "assets/Victoire/victoire_hard1.bmp");
                } else  {
                    sprite(0,0,"assets/Victoire/victoire_hard2.bmp");
                }
                if(flash_win_hard>=100){
                    flash_win_hard=0;
            }
            if(animeframe_win>=0){
                char glitchFrame[200];
                sprintf(glitchFrame, "gif_victoire/win%d.bmp", animeframe_win);
                sprite(0, 0, glitchFrame);
                if(delay_gif==0){
                    animeframe_win--;
                        if(animeframe_win<=0){
                            animeframe_win=140;
                        }
                    }
            delay_gif+=1;
            if (delay_gif>=3){
                delay_gif=0;
            }
        }
            sprite(550, -80, "assets/Pilote/happy_pilot.bmp");
            actualize();
            break;
    }  
}
                                                                       //VIE 
void vie(){//Si je touche le fond, je perd une vie 
  switch (nb_vie){
    case 3:
        sprite(70,1030,"assets/Bonus/coeur.bmp");
        sprite(140,1030,"assets/Bonus/coeur.bmp");
        sprite(210,1030,"assets/Bonus/coeur.bmp");
        break;
    case 2: 
        sprite(70,1030,"assets/Bonus/coeur.bmp");
        sprite(140,1030,"assets/Bonus/coeur.bmp");
        break;
    case 1:
        sprite(70,1030,"assets/Bonus/coeur.bmp");
        break;
    case 0:
        gameLoop=0;
        navigation=1;
        lose=1;
        win=0;
        break;
  }
}
                                                                        //CONDITION VICTOIRE
int condition_victoire(){
    if(compteur_victoire==30){        
        win=1;
        etat_pilote_win=1;
        gameLoop=0;
        if (level == 1) {
        initialiser_grille(); // Prépare les briques du niveau suivant
        }
    }
}
                                                                        //INIT GAME
void init_game(){
    if(music_bg==0){
        audioLoadAndPlay("sdl_helper/sound/bg_son.wav",3);
    }
    draw_bonus();    
    initialiser_grille();
}
                                                                        //Réinitialise mes valeurs


int animeframe=8;
int sound= 1;    
int flash;

                                                                        //DRAWGAME
void vitesse_balle(){
    if(level==2){
        if (score % 10 == 0 && score > 0) { // Tous les 10 points
            if (v_x > 0) {
                v_x+=1;
            } else {
                v_x--;
                     }     
            if (v_y > 0){ 
                v_y+=1;
            } else {
                v_y--;
            }
        }   
            if (abs(v_x) > 3) {// Limite à 3
                v_x = (v_x > 0) ? 2 : -2; 
            }
            if (abs(v_y) > 3) {
                v_y = (v_y > 0) ? 2 : -2;
            }
    }      
}
void drawGame(){
       switch (level){
        case 1:
            clear();
            sprite(0,0,"assets/BG/glitch1.bmp");
            break;
        case 2:
            clear;
            if(animeframe>=0){
                int glitch_frame=0;
                char glitchFrame[34];
                sprintf(glitchFrame, "glitch_hard/test%d.bmp", animeframe);
                sprite(0, 0, glitchFrame);
                animeframe--;
                if(animeframe<=0){
                    animeframe=10;
                }
            }
        break;
    }
    move_raquette();
    compteur();
    raquette();
    vie();
    balle();
    rebond();
    rebond_raquette();
    affichage_briques();
    collision();
    condition_victoire();
    drawLine(0,1000,1200,1000);
    draw_bonus();
    reset_bonus_longueur();
    vitesse_balle();
    actualize();
    usleep(100000/FPS);
    
}
void init_balle(){//Affiche en fonction du niveau les briques, le background, initialise ma balle, ma raquette. Juste avant de commencer à jouer 
    clear();
    // Afficher l'image de fond en fonction du niveau
    if (level == 1) {
        sprite(0, 0, "assets/BG/visage_easy.bmp");
    } else if (level == 2) {
        sprite(0, 0, "assets/BG/visage.bmp");
    }
    compteur();
    x= 600;//Reset position balle
    y= 830;//Reset position balle
    x_raquette=535;
    y_raquette=850;
    affichage_briques();
    raquette();
    vie();
    balle();
    if(flash>6 && flash <80){
    textChangeColor(255,255,255,0);
    textDrawText("PRESS SPACE TO START", 250, 940, PressStart2P_36);
    }
    flash++;
    if(flash>150){
        flash=0;
    }
    actualize();
    usleep(10000/FPS);
}

void reinitialiser_bonus() {
    for (int i = 0; i < 100; i++) {
        tableau_bonus[i].bonus_active = 0;
        tableau_bonus[i].bonus_x = 0;
        tableau_bonus[i].bonus_y = 0;
        tableau_bonus[i].type_bonus = 0;
        tableau_bonus[i].animeframe_arrow = 0;
        tableau_bonus[i].delay_gif_test = 0;
    }
}

void reset_game_state() {
    navigation = 1;
    gameLoop = 0;
    menu = 1;
    start_game = 1;
    win = 0;
    lose = 0;
    rules = 0;
}
void reset_game_elements() {
    x = 600; // Position initiale de la balle
    y = 830; 
    v_x = 2; // Vitesse initiale de la balle
    v_y = 2;
    q_press=0;
    d_press=0;
    x_raquette = 535;
    y_raquette = 850;
    largeur_raquette = 130;
    nb_vie = 3;
    compteur_victoire = 0;
    score = 0;
    
    // Réinitialiser les bonus
    reinitialiser_bonus();

    // Arrêter les effets des bonus
    clignote = false;
    frame_clignote = 0;
    temps_bonus = 0;

    // Réinitialiser la grille des briques
    initialiser_grille();
}

void reset_pilot_states() {
    etat_pilote = 0;           // Réinitialise l'état du pilote dans le menu de démarrage
    etat_pilote_lose = 0;      // Réinitialise l'état du pilote lors de l'écran de défaite
    etat_pilote_win = 0;       // Réinitialise l'état du pilote lors de la victoire facile
    etat_pilote_win_hard = 0;  // Réinitialise l'état du pilote lors de la victoire difficile
    music_on_victory = 1;      // Réinitialise l'état de la musique de victoire
    music_on_victory_easy = 1; // Réinitialise l'état de la musique de victoire pour le mode facile
    y_pilote = -731;          // Réinitialise la position verticale du pilote
    tremblement_max = 20; 
    tremblement=0;
}

void reinit() {
    // Réinitialiser les états des pages
    reset_game_state();

    // Réinitialiser les éléments du jeu
    reset_game_elements();

    // Réinitialiser les pilotes

    // Réinitialiser la grille des briques
    initialiser_grille();

    // Réinitialiser les bonus
    reinitialiser_bonus();

    // Afficher l'écran de menu après réinitialisation
    menu = 1;
}


void restartGame() {
    start_game = 2; // Appuyer SPACE pour démarrer
    gameLoop = 0; // Jeu en pause
    reset_game_elements();     // Réinitialiser les éléments du jeu
    init_balle(); // Met à jour l'affichage des briques, balle, et raquette
}


void keyunpressed(SDL_KeyCode touche){
    switch(touche){
        case SDLK_q:
            q_press=0;
            break;
        case SDLK_d:
            d_press=0;
            break;
        default:
        break;
    }
}
void KeyPressed(SDL_Keycode touche){
    switch (touche) {
        case SDLK_q:
            q_press=1;
                break;
            
        case SDLK_d:
            d_press=1;
                break; 

        case SDLK_p:
            freeAndTerminate();
                break; 
        case SDLK_o: //TEST POUR REVENIR EN ARRIERE
            if(rules==1){
                rules=0;
                menu=1;
                printf("%d", rules);
            }
                break; 

        case SDLK_SPACE:
            if(start_game==2) { // Une chance sur de deux que la balle parte à gauche ou à droite
                init_balle();
                start_game=0;
                gameLoop=1;
                lose=0;
                win=0;
                d_press=0;
                q_press=0;
                int direction_balle=rand()%2; //Au début, ma balle va soit à droite, soit à gauche.
                    if(direction_balle==1) {
                    v_y=-v_y;
                } else if (direction_balle==0) {
                    v_y=-v_y;
                    v_x=-v_x;    
                } 
            } else if (win==1 && level==1){
                    win = 0; // Quitter l'écran de victoire
                    level++; // Passe au niveau suivant
                    restartGame();
            } else  if (win==1 && level==2 && etat_pilote_win_hard==1){
                reinit();
            }       
                break;   
        default:
            break;
    }
}

void gameEcran(){
    while(navigation==1 ){
//MENU
    while (menu== 1) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                            freeAndTerminate();
                        break;
                    case SDL_MOUSEBUTTONUP:
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                        // Vérifier si les coordonnées du clic sont dans la zone des règles
                        if (mouseX >= 60 && mouseX <= 350 && mouseY >= 835 && mouseY <= 1015) {//Affiche les règles
                            rules = 1;  
                            menu = 0;  
                        }
                        if (mouseX >= 458 && mouseX <= 748 && mouseY >= 835 && mouseY <= 1015) {//Lance le jeu
                            gameLoop = 1;  
                            menu = 0;   
                        }
                        if (mouseX >= 856 && mouseX <= 1146 && mouseY >= 835 && mouseY <= 1015) {//Quitte le jeu
                            freeAndTerminate();
                        }
                        break;
                    case SDL_KEYDOWN:
                        KeyPressed(event.key.keysym.sym);
                        break;
                    default:
                        break;
                }
            }
            ecran_menu();//AFffiche le menu
        }
//RULES
    while (rules== 1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                   freeAndTerminate();
                    break;
                case SDL_MOUSEBUTTONUP:
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    // Vérifier si les coordonnées du clic sont dans la zone des règles
                    if (mouseX >= 530 && mouseX <= 680 && mouseY >= 1050 && mouseY <= 1100) {
                        rules=0;
                    }      
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                default:
                    break;
            }
        }
        ecran_rules();
    }

//Choisir niveau
    while (start_game==1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    freeAndTerminate();
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break; 
                case SDL_MOUSEMOTION: //permet de passer au-dessus des niveaux pour changer leurs états
                    if (event.motion.x > 540 && event.motion.x < 790 && event.motion.y > 740 && event.motion.y < 830){
                        etat_pilote=2;
                    } else {
                        etat_pilote=1;
                    }
                    if (event.motion.x >= 895 && event.motion.x <= 1140 && event.motion.y >= 740 && event.motion.y <= 830){
                        etat_pilote=3;
                    } else {}
                    break;
                case SDL_MOUSEBUTTONUP:
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                         
                    // Vérifier si les coordonnées du clic sont dans la zone EASY or HARD
                      if (mouseX >=540  && mouseX <= 790 && mouseY >= 740 && mouseY <= 830) { //BOUTON EASY
                        level=1;
                        start_game=2;
                        initialiser_grille();
                    }
                     if (mouseX >= 895 && mouseX <= 1140 && mouseY >= 740 && mouseY <= 830) {// BOUTON HARD
                        level=2;
                        start_game=2;
                        initialiser_grille();
                    }
                    if (mouseX >= 856 && mouseX <= 1146 && mouseY >= 835 && mouseY <= 1015) {
                        freeAndTerminate();
                    }
                    break;
                default:
                    break;
            }            
        }
        pilote_start();//Choix niveau easy ou hard
    }
    while (start_game==2) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    freeAndTerminate();
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONUP:
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;
                    if (mouseX >= 856 && mouseX <= 1146 && mouseY >= 835 && mouseY <= 1015) {
                        freeAndTerminate();
                    }
                    break;
                default:
                    break;
            }            
        }
        init_balle();//Ecran de début de jeu
    }
//WIN
    while (win== 1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    freeAndTerminate();
                    break;
                case SDL_MOUSEBUTTONUP:
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                default:
                    break;
            }
        }
        if(level==1){
            ecran_win_easy();

        } else if (win==1 && level==2) {
            ecran_win_hard();
        }
    }
//LOSE
    while (lose == 1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    freeAndTerminate();
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONUP:
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    // Vérifier si les coordonnées du clic sont dans la zone des menu
                     if (mouseX >= 100 && mouseX <= 380 && mouseY >= 780 && mouseY <= 860) {//retour au menu
                        printf("Retour au menu principal\n");
                        reinit();
                    }
                    if (mouseX >= 460 && mouseX <= 740 && mouseY >= 770 && mouseY <= 870) {//Replay
                        lose=0;
                        restartGame();
                    }   
                    if (mouseX >= 856 && mouseX <= 1146 && mouseY >= 835 && mouseY <= 1015) {//quitte le jeu
                        freeAndTerminate();
                    }
                break; 
                case SDL_MOUSEMOTION: 
                    if (event.motion.x > 100 && event.motion.x < 380 && event.motion.y > 780 && event.motion.y < 860){//Retour au menu
                        etat_pilote_lose=2;
                    } else {
                        etat_pilote_lose=1;
                    }
                    if (event.motion.x > 460 && event.motion.x < 740 && event.motion.y > 780 && event.motion.y < 860){//REPLAY
                        etat_pilote_lose=3;
                    } else {}
                    break;           
                default:                                        
                    break;
            }
        }                     
        ecran_lose();
    }
//LANCEMENT JEU
    while(gameLoop==1){
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // boucle pour la gestion d'évenement
            switch (event.type) {
                case SDL_QUIT:
                    freeAndTerminate();
                    break;
                case SDL_MOUSEBUTTONUP:
                    printf("position de la souris x : %d , y : %d\n", event.motion.x, event.motion.y);
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);

                    break;
                case SDL_KEYUP:
                    keyunpressed(event.key.keysym.sym);

                    break;
                default:
                    break;
            }
        }
        reset_pilot_states();
        drawGame(); 
        }
    }
}

int main(){
    init(WINDOW_WIDTH, WINDOW_HEIGHT);
    srand(time(NULL));
    init_game();
    gameEcran();
    printf("Fin du programme\n");
    freeAndTerminate();
}